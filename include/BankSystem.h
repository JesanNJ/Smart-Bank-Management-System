#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Account.h"
#include "Budget.h"
#include "Goal.h"
#include "SmartAnalyzer.h"
#include "Transaction.h"
#include "User.h"


// High-level façade that coordinates users, accounts,
// transactions, budgeting, goals and AI analysis.
class BankSystem {
public:
  BankSystem();

  // Registration & authentication.
  int registerUser(const std::string &name, const std::string &password);
  int login(const std::string &name, const std::string &password) const;

  // Login by account number + password. Returns userId, or -1 on failure.
  int loginByAccount(const std::string &accountNumber,
                     const std::string &password, int &outAccountId) const;

  // Admin login. Returns true if credentials valid.
  bool adminLogin(const std::string &username,
                  const std::string &password) const;

  // Accounts.
  int createAccount(int userId);
  Account *getAccount(int accountId);
  const Account *getAccount(int accountId) const;

  // User lookup (for display).
  const User *getUser(int userId) const;

  // Money operations automatically record transactions.
  bool deposit(int accountId, double amount, SpendingCategory category,
               const std::string &description);
  bool withdraw(int accountId, double amount, SpendingCategory category,
                const std::string &description);

  // Transactions.
  const std::vector<Transaction> &
  getTransactionsForAccount(int accountId) const;

  // Budget & goals – per user for simplicity.
  Budget &getBudgetForUser(int userId);
  Goal &getGoalForUser(int userId);

  SmartAnalyzer::AnalysisResult runAnalysis(int userId) const;

  // Admin: view all data.
  const std::vector<User> &getAllUsers() const;
  const std::map<int, Account> &getAllAccounts() const;
  std::vector<std::pair<int, Transaction>> getAllTransactions() const;

private:
  int m_nextUserId{1};
  int m_nextAccountId{1};
  int m_nextTransactionId{1};

  std::vector<User> m_users;
  std::map<int, Account> m_accounts; // accountId -> Account
  std::map<int, std::vector<Transaction>>
      m_txByAccount;               // accountId -> transactions
  std::map<int, Budget> m_budgets; // userId -> Budget
  std::map<int, Goal> m_goals;     // userId -> Goal

  SmartAnalyzer m_analyzer;

  const User *findUserByName(const std::string &name) const;
};
