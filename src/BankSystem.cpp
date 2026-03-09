#include "BankSystem.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <random>
#include <sstream>

namespace {
std::tm currentLocalTime() {
  auto now = std::chrono::system_clock::now();
  std::time_t t = std::chrono::system_clock::to_time_t(now);
  std::tm result{};
#if defined(_WIN32)
  localtime_s(&result, &t);
#else
  localtime_r(&t, &result);
#endif
  return result;
}

std::string generateAccountNumber(int seed) {
  std::ostringstream oss;
  oss << "AC" << std::setw(6) << std::setfill('0') << seed;
  return oss.str();
}
} // namespace

BankSystem::BankSystem() = default;

int BankSystem::registerUser(const std::string &name,
                             const std::string &password) {
  int id = m_nextUserId++;
  m_users.emplace_back(id, name, password);
  // Create a default account for the user.
  createAccount(id);
  return id;
}

const User *BankSystem::findUserByName(const std::string &name) const {
  for (const auto &user : m_users) {
    if (user.getName() == name) {
      return &user;
    }
  }
  return nullptr;
}

int BankSystem::login(const std::string &name,
                      const std::string &password) const {
  for (const auto &user : m_users) {
    if (user.getName() == name && user.getPassword() == password) {
      return user.getId();
    }
  }
  return -1;
}

int BankSystem::loginByAccount(const std::string &accountNumber,
                               const std::string &password,
                               int &outAccountId) const {
  for (const auto &pair : m_accounts) {
    const Account &acc = pair.second;
    if (acc.getAccountNumber() == accountNumber) {
      const User *user = getUser(acc.getUserId());
      if (user && user->getPassword() == password) {
        outAccountId = acc.getId();
        return acc.getUserId();
      }
      return -1; // Account found but wrong password
    }
  }
  outAccountId = -1;
  return -1;
}

bool BankSystem::adminLogin(const std::string &username,
                            const std::string &password) const {
  return username == "admin" && password == "admin123";
}

int BankSystem::createAccount(int userId) {
  int accountId = m_nextAccountId++;
  std::string accountNumber = generateAccountNumber(accountId);
  Account account(accountId, userId, accountNumber);
  m_accounts.emplace(accountId, account);
  m_txByAccount[accountId] = {};
  return accountId;
}

Account *BankSystem::getAccount(int accountId) {
  auto it = m_accounts.find(accountId);
  if (it != m_accounts.end()) {
    return &it->second;
  }
  return nullptr;
}

const Account *BankSystem::getAccount(int accountId) const {
  auto it = m_accounts.find(accountId);
  if (it != m_accounts.end()) {
    return &it->second;
  }
  return nullptr;
}

const User *BankSystem::getUser(int userId) const {
  for (const auto &user : m_users) {
    if (user.getId() == userId) {
      return &user;
    }
  }
  return nullptr;
}

bool BankSystem::deposit(int accountId, double amount,
                         SpendingCategory category,
                         const std::string &description) {
  Account *account = getAccount(accountId);
  if (!account || amount <= 0.0) {
    return false;
  }
  account->deposit(amount);

  int txId = m_nextTransactionId++;
  std::tm ts = currentLocalTime();
  Transaction tx(txId, accountId, TransactionType::Deposit, amount, category,
                 description, ts);
  m_txByAccount[accountId].push_back(tx);

  // Deposits do not add to budget spending (only withdrawals do).

  return true;
}

bool BankSystem::withdraw(int accountId, double amount,
                          SpendingCategory category,
                          const std::string &description) {
  Account *account = getAccount(accountId);
  if (!account || amount <= 0.0) {
    return false;
  }
  if (!account->withdraw(amount)) {
    return false;
  }

  int txId = m_nextTransactionId++;
  std::tm ts = currentLocalTime();
  Transaction tx(txId, accountId, TransactionType::Withdrawal, amount, category,
                 description, ts);
  m_txByAccount[accountId].push_back(tx);

  int userId = account->getUserId();
  m_budgets[userId].addSpending(category, amount);

  return true;
}

const std::vector<Transaction> &
BankSystem::getTransactionsForAccount(int accountId) const {
  static const std::vector<Transaction> empty;
  auto it = m_txByAccount.find(accountId);
  if (it != m_txByAccount.end()) {
    return it->second;
  }
  return empty;
}

Budget &BankSystem::getBudgetForUser(int userId) { return m_budgets[userId]; }

Goal &BankSystem::getGoalForUser(int userId) { return m_goals[userId]; }

SmartAnalyzer::AnalysisResult BankSystem::runAnalysis(int userId) const {
  // Aggregate all accounts for this user.
  std::vector<Transaction> allTx;
  for (const auto &pair : m_accounts) {
    const Account &acc = pair.second;
    if (acc.getUserId() == userId) {
      auto it = m_txByAccount.find(acc.getId());
      if (it != m_txByAccount.end()) {
        allTx.insert(allTx.end(), it->second.begin(), it->second.end());
      }
    }
  }

  auto budgetIt = m_budgets.find(userId);
  auto goalIt = m_goals.find(userId);

  Budget budget;
  if (budgetIt != m_budgets.end()) {
    budget = budgetIt->second;
  }

  Goal goal;
  if (goalIt != m_goals.end()) {
    goal = goalIt->second;
  }

  // Estimate current saved towards goal by summing Savings category deposits.
  double saved = 0.0;
  for (const auto &tx : allTx) {
    if (tx.getType() == TransactionType::Deposit &&
        tx.getCategory() == SpendingCategory::Savings) {
      saved += tx.getAmount();
    }
  }
  goal.setCurrentSaved(saved);

  return m_analyzer.analyze(allTx, budget, goal);
}

const std::vector<User> &BankSystem::getAllUsers() const { return m_users; }

const std::map<int, Account> &BankSystem::getAllAccounts() const {
  return m_accounts;
}

std::vector<std::pair<int, Transaction>>
BankSystem::getAllTransactions() const {
  std::vector<std::pair<int, Transaction>> result;
  for (const auto &pair : m_txByAccount) {
    int accountId = pair.first;
    for (const auto &tx : pair.second) {
      result.emplace_back(accountId, tx);
    }
  }
  return result;
}
