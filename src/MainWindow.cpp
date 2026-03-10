#include "MainWindow.h"

#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

namespace {

int readInt(const std::string &prompt) {
  int value;
  while (true) {
    std::cout << prompt;
    if (std::cin >> value) {
      return value;
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Invalid number. Please try again.\n";
  }
}

double readDouble(const std::string &prompt) {
  double value;
  while (true) {
    std::cout << prompt;
    if (std::cin >> value) {
      return value;
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Invalid number. Please try again.\n";
  }
}

std::string readLine(const std::string &prompt) {
  std::cout << prompt;
  std::string line;
  std::getline(std::cin >> std::ws, line);
  return line;
}

SpendingCategory selectCategory() {
  std::cout << "\nSelect spending category:\n";
  std::cout << " 1) Income\n";
  std::cout << " 2) Housing\n";
  std::cout << " 3) Utilities\n";
  std::cout << " 4) Groceries\n";
  std::cout << " 5) Dining\n";
  std::cout << " 6) Transport\n";
  std::cout << " 7) Entertainment\n";
  std::cout << " 8) Healthcare\n";
  std::cout << " 9) Education\n";
  std::cout << "10) Savings\n";
  std::cout << "11) Other\n";

  int choice = readInt("Enter category number: ");
  switch (choice) {
  case 1:
    return SpendingCategory::Income;
  case 2:
    return SpendingCategory::Housing;
  case 3:
    return SpendingCategory::Utilities;
  case 4:
    return SpendingCategory::Groceries;
  case 5:
    return SpendingCategory::Dining;
  case 6:
    return SpendingCategory::Transport;
  case 7:
    return SpendingCategory::Entertainment;
  case 8:
    return SpendingCategory::Healthcare;
  case 9:
    return SpendingCategory::Education;
  case 10:
    return SpendingCategory::Savings;
  default:
    return SpendingCategory::Other;
  }
}

std::string categoryToString(SpendingCategory category) {
  switch (category) {
  case SpendingCategory::Income:
    return "Income";
  case SpendingCategory::Housing:
    return "Housing";
  case SpendingCategory::Utilities:
    return "Utilities";
  case SpendingCategory::Groceries:
    return "Groceries";
  case SpendingCategory::Dining:
    return "Dining";
  case SpendingCategory::Transport:
    return "Transport";
  case SpendingCategory::Entertainment:
    return "Entertainment";
  case SpendingCategory::Healthcare:
    return "Healthcare";
  case SpendingCategory::Education:
    return "Education";
  case SpendingCategory::Savings:
    return "Savings";
  case SpendingCategory::Other:
    return "Other";
  }
  return "Unknown";
}

std::string formatDate(const std::tm &tm) {
  char buffer[32];
  std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", &tm);
  return buffer;
}

} // namespace

MainWindow::MainWindow() { populateDummyData(); }

void MainWindow::populateDummyData() {
  // ---------------------------------------------------------
  // User 1: John (Healthy Financials - High savings, balanced)
  // ---------------------------------------------------------
  int jId = m_bankSystem.registerUser("john", "password");
  int jAccId = -1;
  m_bankSystem.loginByAccount(m_bankSystem.getAccount(jId)->getAccountNumber(),
                              "password", jAccId);

  m_bankSystem.deposit(jAccId, 6000.0, SpendingCategory::Income, "Salary");
  m_bankSystem.deposit(jAccId, 500.0, SpendingCategory::Income, "Side Hustle");
  m_bankSystem.withdraw(jAccId, 1500.0, SpendingCategory::Housing, "Rent");
  m_bankSystem.withdraw(jAccId, 120.0, SpendingCategory::Utilities,
                        "Electricity");
  m_bankSystem.withdraw(jAccId, 80.0, SpendingCategory::Utilities, "Water");
  m_bankSystem.withdraw(jAccId, 150.0, SpendingCategory::Groceries,
                        "Whole Foods");
  m_bankSystem.withdraw(jAccId, 200.0, SpendingCategory::Groceries, "Costco");
  m_bankSystem.withdraw(jAccId, 150.0, SpendingCategory::Dining, "Steakhouse");
  m_bankSystem.withdraw(jAccId, 45.0, SpendingCategory::Dining, "Coffee Shop");
  m_bankSystem.withdraw(jAccId, 100.0, SpendingCategory::Transport, "Gas");
  m_bankSystem.withdraw(jAccId, 30.0, SpendingCategory::Transport, "Uber");
  m_bankSystem.withdraw(jAccId, 150.0, SpendingCategory::Entertainment,
                        "Concert");
  m_bankSystem.withdraw(jAccId, 50.0, SpendingCategory::Healthcare, "Pharmacy");
  m_bankSystem.withdraw(jAccId, 100.0, SpendingCategory::Education,
                        "Online Course");
  m_bankSystem.deposit(jAccId, 1500.0, SpendingCategory::Savings,
                       "Auto Transfer to Savings"); // >20% savings

  Budget &jBudget = m_bankSystem.getBudgetForUser(jId);
  jBudget.setMonthlyLimit(3500.0);
  jBudget.setCategoryLimit(SpendingCategory::Dining, 250.0);

  Goal &jGoal = m_bankSystem.getGoalForUser(jId);
  jGoal = Goal("Vacation", 5000.0, 6);

  // ---------------------------------------------------------
  // User 2: Alice (Overspending on discretionary, low savings)
  // ---------------------------------------------------------
  int aId = m_bankSystem.registerUser("alice", "1234");
  int aAccId = -1;
  m_bankSystem.loginByAccount(m_bankSystem.getAccount(aId)->getAccountNumber(),
                              "1234", aAccId);

  m_bankSystem.deposit(aAccId, 4000.0, SpendingCategory::Income,
                       "Freelance IT");
  m_bankSystem.withdraw(aAccId, 1200.0, SpendingCategory::Housing, "Apartment");
  m_bankSystem.withdraw(aAccId, 100.0, SpendingCategory::Utilities, "Internet");
  m_bankSystem.withdraw(aAccId, 300.0, SpendingCategory::Entertainment,
                        "Music Festival");
  m_bankSystem.withdraw(aAccId, 250.0, SpendingCategory::Entertainment,
                        "Gaming Console");
  m_bankSystem.withdraw(aAccId, 150.0, SpendingCategory::Entertainment,
                        "Movie night");
  m_bankSystem.withdraw(aAccId, 400.0, SpendingCategory::Dining,
                        "Fancy Dinner");
  m_bankSystem.withdraw(aAccId, 350.0, SpendingCategory::Dining, "Clubs");
  m_bankSystem.withdraw(aAccId, 150.0, SpendingCategory::Dining, "Cafe");
  m_bankSystem.withdraw(aAccId, 400.0, SpendingCategory::Transport,
                        "Car Repair");
  m_bankSystem.withdraw(aAccId, 50.0, SpendingCategory::Transport, "Tolls");
  m_bankSystem.withdraw(aAccId, 150.0, SpendingCategory::Groceries, "Snacks");
  m_bankSystem.withdraw(aAccId, 150.0, SpendingCategory::Other,
                        "Random Shopping");
  m_bankSystem.withdraw(aAccId, 200.0, SpendingCategory::Other, "Clothes");
  m_bankSystem.deposit(aAccId, 100.0, SpendingCategory::Savings,
                       "Leftovers"); // Low savings

  Budget &aBudget = m_bankSystem.getBudgetForUser(aId);
  aBudget.setMonthlyLimit(3000.0); // Over budget

  Goal &aGoal = m_bankSystem.getGoalForUser(aId);
  aGoal = Goal("New Laptop", 1500.0, 3);

  // ---------------------------------------------------------
  // User 3: Bob (High risk - living paycheck to paycheck, 0 savings)
  // ---------------------------------------------------------
  int bId = m_bankSystem.registerUser("bob", "pass");
  int bAccId = -1;
  m_bankSystem.loginByAccount(m_bankSystem.getAccount(bId)->getAccountNumber(),
                              "pass", bAccId);

  m_bankSystem.deposit(bAccId, 2200.0, SpendingCategory::Income, "Retail Job");
  m_bankSystem.withdraw(bAccId, 900.0, SpendingCategory::Housing, "Rent");
  m_bankSystem.withdraw(bAccId, 150.0, SpendingCategory::Utilities, "Bills");
  m_bankSystem.withdraw(bAccId, 300.0, SpendingCategory::Groceries,
                        "Supermarket");
  m_bankSystem.withdraw(bAccId, 50.0, SpendingCategory::Transport, "Bus Pass");
  m_bankSystem.withdraw(bAccId, 100.0, SpendingCategory::Dining, "Fast food");
  m_bankSystem.withdraw(bAccId, 80.0, SpendingCategory::Dining, "Takeout");
  m_bankSystem.withdraw(bAccId, 70.0, SpendingCategory::Dining, "Takeout");
  m_bankSystem.withdraw(bAccId, 60.0, SpendingCategory::Entertainment,
                        "Subscription");
  m_bankSystem.withdraw(bAccId, 45.0, SpendingCategory::Entertainment, "Games");
  m_bankSystem.withdraw(bAccId, 150.0, SpendingCategory::Healthcare, "Dentist");
  m_bankSystem.withdraw(bAccId, 100.0, SpendingCategory::Other,
                        "Miscellaneous");
  m_bankSystem.withdraw(bAccId, 120.0, SpendingCategory::Other, "Gadget");
  m_bankSystem.withdraw(bAccId, 50.0, SpendingCategory::Other, "Gifts");
  // No savings deposits

  Budget &bBudget = m_bankSystem.getBudgetForUser(bId);
  bBudget.setMonthlyLimit(2000.0); // Continually near or over 100% usage

  Goal &bGoal = m_bankSystem.getGoalForUser(bId);
  bGoal = Goal("Emergency Fund", 1000.0, 12); // Way behind

  // ---------------------------------------------------------
  // User 4: Clara (Strict Budgeter - excellent balance, perfect)
  // ---------------------------------------------------------
  int cId = m_bankSystem.registerUser("clara", "admin123");
  int cAccId = -1;
  m_bankSystem.loginByAccount(m_bankSystem.getAccount(cId)->getAccountNumber(),
                              "admin123", cAccId);

  m_bankSystem.deposit(cAccId, 4500.0, SpendingCategory::Income,
                       "Full Time Salary");
  m_bankSystem.withdraw(cAccId, 1200.0, SpendingCategory::Housing, "Mortgage");
  m_bankSystem.withdraw(cAccId, 130.0, SpendingCategory::Utilities,
                        "Power/Water");
  m_bankSystem.withdraw(cAccId, 50.0, SpendingCategory::Utilities, "Internet");
  m_bankSystem.withdraw(cAccId, 80.0, SpendingCategory::Transport, "Gas");
  m_bankSystem.withdraw(cAccId, 40.0, SpendingCategory::Transport, "Parking");
  m_bankSystem.withdraw(cAccId, 300.0, SpendingCategory::Groceries,
                        "Groceries");
  m_bankSystem.withdraw(cAccId, 50.0, SpendingCategory::Groceries, "Market");
  m_bankSystem.withdraw(cAccId, 100.0, SpendingCategory::Dining, "Dinner out");
  m_bankSystem.withdraw(cAccId, 50.0, SpendingCategory::Dining, "Lunch");
  m_bankSystem.withdraw(cAccId, 120.0, SpendingCategory::Entertainment,
                        "Theater");
  m_bankSystem.withdraw(cAccId, 60.0, SpendingCategory::Education, "Books");
  m_bankSystem.withdraw(cAccId, 40.0, SpendingCategory::Other, "Haircut");
  m_bankSystem.deposit(cAccId, 900.0, SpendingCategory::Savings,
                       "Index Funds"); // 20% savings

  Budget &cBudget = m_bankSystem.getBudgetForUser(cId);
  cBudget.setMonthlyLimit(2500.0); // Exact usage and perfect budget

  Goal &cGoal = m_bankSystem.getGoalForUser(cId);
  cGoal = Goal("House Renovation", 10000.0, 12);

  // ---------------------------------------------------------
  // User 5: David (Wealthy - excessive income, extremely high savings)
  // ---------------------------------------------------------
  int dId = m_bankSystem.registerUser("david", "rich");
  int dAccId = -1;
  m_bankSystem.loginByAccount(m_bankSystem.getAccount(dId)->getAccountNumber(),
                              "rich", dAccId);

  m_bankSystem.deposit(dAccId, 15000.0, SpendingCategory::Income,
                       "Executive Salary");
  m_bankSystem.deposit(dAccId, 2500.0, SpendingCategory::Income, "Dividends");
  m_bankSystem.withdraw(dAccId, 3500.0, SpendingCategory::Housing,
                        "Luxury Condo");
  m_bankSystem.withdraw(dAccId, 300.0, SpendingCategory::Utilities,
                        "Utilities");
  m_bankSystem.withdraw(dAccId, 500.0, SpendingCategory::Groceries,
                        "Premium Groceries");
  m_bankSystem.withdraw(dAccId, 800.0, SpendingCategory::Dining,
                        "Michelin Star Restaurants");
  m_bankSystem.withdraw(dAccId, 400.0, SpendingCategory::Dining,
                        "Business Lunches");
  m_bankSystem.withdraw(dAccId, 1500.0, SpendingCategory::Entertainment,
                        "Yacht Club");
  m_bankSystem.withdraw(dAccId, 700.0, SpendingCategory::Entertainment, "Golf");
  m_bankSystem.withdraw(dAccId, 600.0, SpendingCategory::Transport,
                        "Sports Car Lease");
  m_bankSystem.withdraw(dAccId, 150.0, SpendingCategory::Transport,
                        "Premium Gas");
  m_bankSystem.withdraw(dAccId, 1200.0, SpendingCategory::Other,
                        "Designer Clothes");
  m_bankSystem.withdraw(dAccId, 500.0, SpendingCategory::Other, "Charity");
  m_bankSystem.deposit(dAccId, 7000.0, SpendingCategory::Savings,
                       "Investment Portfolio"); // Massive savings

  Budget &dBudget = m_bankSystem.getBudgetForUser(dId);
  dBudget.setMonthlyLimit(12000.0);

  Goal &dGoal = m_bankSystem.getGoalForUser(dId);
  dGoal = Goal("New Boat", 50000.0, 10);
}

void MainWindow::run() {
  bool running = true;
  while (running) {
    if (m_loggedInUserId < 0 && !m_isAdmin) {
      showWelcomeMenu();
      int choice = readInt("Select option: ");
      if (choice == 0) {
        running = false;
      } else {
        handleWelcomeChoice(choice);
      }
    } else if (m_isAdmin) {
      showAdminMenu();
      int choice = readInt("Select option: ");
      if (choice == 0) {
        std::cout << "Logging out...\n";
        m_isAdmin = false;
      } else {
        handleAdminChoice(choice);
      }
    } else {
      showMainMenu();
      int choice = readInt("Select option: ");
      if (choice == 0) {
        std::cout << "Logging out...\n";
        m_loggedInUserId = -1;
        m_loggedInAccountId = -1;
      } else {
        handleMainChoice(choice);
      }
    }
  }

  std::cout << "Thank you for using AI-Driven Bank Management System.\n";
}

void MainWindow::showWelcomeMenu() {
  std::cout << "\n============================================\n";
  std::cout << "   AI-Driven Bank Management System\n";
  std::cout << "============================================\n";
  std::cout << " 1) Register\n";
  std::cout << " 2) User Login (Account No + Password)\n";
  std::cout << " 3) Admin Login\n";
  std::cout << " 0) Exit\n";
}

void MainWindow::handleWelcomeChoice(int choice) {
  switch (choice) {
  case 1:
    doRegister();
    break;
  case 2:
    doLogin();
    break;
  case 3:
    doAdminLogin();
    break;
  default:
    std::cout << "Unknown option.\n";
    break;
  }
}

void MainWindow::showMainMenu() {
  const User *user = m_bankSystem.getUser(m_loggedInUserId);
  const Account *acc = m_bankSystem.getAccount(m_loggedInAccountId);
  std::string name = user ? user->getName() : "Customer";
  double balance = acc ? acc->getBalance() : 0.0;

  std::cout << "\n============================================\n";
  std::cout << " Hey " << name << ", your available balance is " << std::fixed
            << std::setprecision(2) << balance << "\n";
  std::cout << "============================================\n";
  std::cout << " 1) Deposit Money\n";
  std::cout << " 2) Withdraw Money\n";
  std::cout << " 3) View Transactions\n";
  std::cout << " 4) Set Monthly Budget\n";
  std::cout << " 5) Set Savings Goal\n";
  std::cout << " 6) Monthly Report Generator\n";
  std::cout << " 7) Spending Trend Comparison\n";
  std::cout << " 8) Risk Alerts & AI Analysis\n";
  std::cout << " 9) Financial Insights Chatbot\n";
  std::cout << " 0) Logout\n";
}

void MainWindow::handleMainChoice(int choice) {
  switch (choice) {
  case 1:
    doDeposit();
    break;
  case 2:
    doWithdraw();
    break;
  case 3:
    doViewTransactions();
    break;
  case 4:
    doSetBudget();
    break;
  case 5:
    doSetGoal();
    break;
  case 6:
    doMonthlyReport();
    break;
  case 7:
    doSpendingTrendComparison();
    break;
  case 8:
    doRiskAndAIAnalysis();
    break;
  case 9:
    doFinancialChatbot();
    break;
  default:
    std::cout << "Unknown option.\n";
    break;
  }
}

void MainWindow::showAdminMenu() {
  std::cout << "\n============================================\n";
  std::cout << "   Admin Dashboard\n";
  std::cout << "============================================\n";
  std::cout << " 1) View All Users\n";
  std::cout << " 2) View All Accounts\n";
  std::cout << " 3) View All Transactions\n";
  std::cout << " 0) Logout\n";
}

void MainWindow::handleAdminChoice(int choice) {
  switch (choice) {
  case 1:
    doAdminViewAllUsers();
    break;
  case 2:
    doAdminViewAllAccounts();
    break;
  case 3:
    doAdminViewAllTransactions();
    break;
  default:
    std::cout << "Unknown option.\n";
    break;
  }
}

void MainWindow::doAdminLogin() {
  std::string username = readLine("Admin username: ");
  std::string password = readLine("Admin password: ");

  if (m_bankSystem.adminLogin(username, password)) {
    m_isAdmin = true;
    std::cout << "Admin login successful.\n";
  } else {
    std::cout << "Invalid admin credentials.\n";
  }
}

void MainWindow::doAdminViewAllUsers() {
  const auto &users = m_bankSystem.getAllUsers();
  if (users.empty()) {
    std::cout << "No users registered.\n";
    return;
  }
  std::cout << "\n--- All Users ---\n";
  for (const auto &u : users) {
    std::cout << "ID: " << u.getId() << "  Name: " << u.getName() << "\n";
  }
}

void MainWindow::doAdminViewAllAccounts() {
  const auto &accounts = m_bankSystem.getAllAccounts();
  if (accounts.empty()) {
    std::cout << "No accounts.\n";
    return;
  }
  std::cout << "\n--- All Accounts ---\n";
  for (const auto &pair : accounts) {
    const Account &acc = pair.second;
    const User *user = m_bankSystem.getUser(acc.getUserId());
    std::cout << "Acct: " << acc.getAccountNumber()
              << "  User: " << (user ? user->getName() : "?")
              << "  Balance: " << std::fixed << std::setprecision(2)
              << acc.getBalance() << "\n";
  }
}

void MainWindow::doAdminViewAllTransactions() {
  auto txs = m_bankSystem.getAllTransactions();
  if (txs.empty()) {
    std::cout << "No transactions.\n";
    return;
  }
  std::cout << "\n--- All Transactions ---\n";
  for (const auto &p : txs) {
    int accountId = p.first;
    const Transaction &tx = p.second;
    const Account *acc = m_bankSystem.getAccount(accountId);
    std::string acctNo = acc ? acc->getAccountNumber() : "?";
    std::cout << "[" << formatDate(tx.getTimestamp()) << "] Acct " << acctNo
              << " "
              << (tx.getType() == TransactionType::Deposit ? "DEP" : "WDR")
              << " " << std::setw(10) << tx.getAmount() << " " << std::setw(12)
              << categoryToString(tx.getCategory()) << " "
              << tx.getDescription() << "\n";
  }
}

void MainWindow::doRegister() {
  std::string name = readLine("Enter your name: ");
  std::string password = readLine("Create a password: ");
  int userId = m_bankSystem.registerUser(name, password);

  m_loggedInUserId = userId;
  // registerUser already creates an account; find it.
  m_loggedInAccountId = -1;
  for (const auto &pair : m_bankSystem.getAllAccounts()) {
    if (pair.second.getUserId() == userId) {
      m_loggedInAccountId = pair.second.getId();
      break;
    }
  }

  std::cout << "Registration successful. You are now logged in.\n";
  const Account *acc = m_bankSystem.getAccount(m_loggedInAccountId);
  if (acc) {
    std::cout << "Your account number: " << acc->getAccountNumber()
              << " (use this to login)\n";
  }
}

void MainWindow::doLogin() {
  std::string accountNumber = readLine("Account number (e.g. AC000001): ");
  std::string password = readLine("Password: ");

  int accountId = -1;
  int userId = m_bankSystem.loginByAccount(accountNumber, password, accountId);
  if (userId < 0 || accountId < 0) {
    std::cout << "Invalid account number or password.\n";
    return;
  }

  m_loggedInUserId = userId;
  m_loggedInAccountId = accountId;
  std::cout << "Login successful.\n";
}

void MainWindow::ensureLoggedIn() {
  if (m_loggedInUserId < 0 || m_loggedInAccountId < 0) {
    std::cout << "Please login first.\n";
  }
}

void MainWindow::doDeposit() {
  ensureLoggedIn();
  if (m_loggedInUserId < 0)
    return;

  double amount = readDouble("Enter deposit amount: ");
  std::string desc = readLine("Short description (optional): ");

  // Deposits default to Income category for AI analysis; no need to ask user.
  if (m_bankSystem.deposit(m_loggedInAccountId, amount,
                           SpendingCategory::Income, desc)) {
    const Account *acc = m_bankSystem.getAccount(m_loggedInAccountId);
    std::cout << "Deposit successful. New balance: "
              << (acc ? acc->getBalance() : 0.0) << "\n";
  } else {
    std::cout << "Deposit failed.\n";
  }
}

void MainWindow::doWithdraw() {
  ensureLoggedIn();
  if (m_loggedInUserId < 0)
    return;

  double amount = readDouble("Enter withdrawal amount: ");
  SpendingCategory cat = selectCategory();
  std::string desc = readLine("Short description: ");

  if (m_bankSystem.withdraw(m_loggedInAccountId, amount, cat, desc)) {
    const Account *acc = m_bankSystem.getAccount(m_loggedInAccountId);
    std::cout << "Withdrawal successful. New balance: "
              << (acc ? acc->getBalance() : 0.0) << "\n";
  } else {
    std::cout << "Withdrawal failed (insufficient funds or invalid amount).\n";
  }
}

void MainWindow::doViewTransactions() {
  ensureLoggedIn();
  if (m_loggedInUserId < 0)
    return;

  const auto &txs = m_bankSystem.getTransactionsForAccount(m_loggedInAccountId);
  if (txs.empty()) {
    std::cout << "No transactions recorded yet.\n";
    return;
  }

  std::cout << "\n--- Transactions ---\n";
  for (const auto &tx : txs) {
    std::cout << "[" << formatDate(tx.getTimestamp()) << "] "
              << (tx.getType() == TransactionType::Deposit ? "DEP" : "WDR")
              << "  " << std::setw(10) << tx.getAmount() << "  "
              << std::setw(12) << categoryToString(tx.getCategory()) << "  "
              << tx.getDescription() << "\n";
  }
}

void MainWindow::doSetBudget() {
  ensureLoggedIn();
  if (m_loggedInUserId < 0)
    return;

  Budget &budget = m_bankSystem.getBudgetForUser(m_loggedInUserId);
  double total = readDouble("Set total monthly budget: ");
  budget.setMonthlyLimit(total);

  std::cout << "Optionally set category-specific limits (0 to skip).\n";
  for (int i = 2; i <= 11; ++i) {
    SpendingCategory cat;
    switch (i) {
    case 2:
      cat = SpendingCategory::Housing;
      break;
    case 3:
      cat = SpendingCategory::Utilities;
      break;
    case 4:
      cat = SpendingCategory::Groceries;
      break;
    case 5:
      cat = SpendingCategory::Dining;
      break;
    case 6:
      cat = SpendingCategory::Transport;
      break;
    case 7:
      cat = SpendingCategory::Entertainment;
      break;
    case 8:
      cat = SpendingCategory::Healthcare;
      break;
    case 9:
      cat = SpendingCategory::Education;
      break;
    case 10:
      cat = SpendingCategory::Savings;
      break;
    default:
      cat = SpendingCategory::Other;
      break;
    }
    double limit = readDouble("Limit for " + categoryToString(cat) + ": ");
    if (limit > 0.0) {
      budget.setCategoryLimit(cat, limit);
    }
  }

  std::cout << "Budget updated.\n";
}

void MainWindow::doSetGoal() {
  ensureLoggedIn();
  if (m_loggedInUserId < 0)
    return;

  std::string name = readLine("Goal name (e.g., Emergency Fund): ");
  double amount = readDouble("Target amount to save: ");
  int months = readInt("Target months to achieve this goal: ");

  Goal &goal = m_bankSystem.getGoalForUser(m_loggedInUserId);
  goal = Goal(name, amount, months);

  std::cout << "Savings goal updated.\n";
}

void MainWindow::doMonthlyReport() {
  ensureLoggedIn();
  if (m_loggedInUserId < 0)
    return;

  const auto &txs = m_bankSystem.getTransactionsForAccount(m_loggedInAccountId);
  double totalDeposits = 0.0;
  double totalWithdrawals = 0.0;
  for (const auto &tx : txs) {
    if (tx.getType() == TransactionType::Deposit) {
      totalDeposits += tx.getAmount();
    } else {
      totalWithdrawals += tx.getAmount();
    }
  }

  const Account *acc = m_bankSystem.getAccount(m_loggedInAccountId);
  std::cout << "\n--- Monthly Report (Simple View) ---\n";
  std::cout << "Total deposits   : " << totalDeposits << "\n";
  std::cout << "Total withdrawals: " << totalWithdrawals << "\n";
  std::cout << "Net cash flow    : " << (totalDeposits - totalWithdrawals)
            << "\n";
  std::cout << "Current balance  : " << (acc ? acc->getBalance() : 0.0) << "\n";
}

void MainWindow::doSpendingTrendComparison() {
  ensureLoggedIn();
  if (m_loggedInUserId < 0)
    return;

  const auto &txs = m_bankSystem.getTransactionsForAccount(m_loggedInAccountId);
  std::map<SpendingCategory, double> totals;
  for (const auto &tx : txs) {
    if (tx.getType() == TransactionType::Withdrawal) {
      totals[tx.getCategory()] += tx.getAmount();
    }
  }

  if (totals.empty()) {
    std::cout << "No spending data to analyze.\n";
    return;
  }

  double total = 0.0;
  for (const auto &entry : totals) {
    total += entry.second;
  }

  std::cout << "\n--- Spending Trend (by Category) ---\n";
  for (const auto &entry : totals) {
    double share = (entry.second / total) * 100.0;
    std::cout << std::setw(15) << categoryToString(entry.first) << ": "
              << std::setw(10) << entry.second << " (" << std::fixed
              << std::setprecision(1) << share << "%)\n";
  }
}

void MainWindow::doRiskAndAIAnalysis() {
  ensureLoggedIn();
  if (m_loggedInUserId < 0)
    return;

  auto result = m_bankSystem.runAnalysis(m_loggedInUserId);

  std::cout << "\n--- AI-Based Spending Analysis ---\n";
  std::cout << "Savings ratio           : " << std::fixed
            << std::setprecision(1) << result.savingsRatio << "%\n";
  std::cout << "Financial health score  : " << std::fixed
            << std::setprecision(1) << result.financialHealthScore
            << " / 100\n";

  std::cout << "\nCategory-wise expense share:\n";
  for (const auto &entry : result.categoryExpenseShare) {
    std::cout << " - " << std::setw(15) << categoryToString(entry.first) << ": "
              << std::fixed << std::setprecision(1) << entry.second << "%\n";
  }

  if (!result.warnings.empty()) {
    std::cout << "\nRisk alerts:\n";
    for (const auto &w : result.warnings) {
      std::cout << " * " << w << "\n";
    }
  } else {
    std::cout << "\nNo major risk alerts detected.\n";
  }

  std::cout << "\nPersonalized recommendations:\n";
  for (const auto &r : result.recommendations) {
    std::cout << " - " << r << "\n";
  }
}

void MainWindow::doFinancialChatbot() {
  ensureLoggedIn();
  if (m_loggedInUserId < 0)
    return;

  auto result = m_bankSystem.runAnalysis(m_loggedInUserId);
  const Account *acc = m_bankSystem.getAccount(m_loggedInAccountId);
  const Goal &goal = m_bankSystem.getGoalForUser(m_loggedInUserId);
  double balance = acc ? acc->getBalance() : 0.0;

  m_chatbot.setContext(result, balance, goal);

  std::cout << "\n";
  std::cout << "+----------------------------------------------------------+\n";
  std::cout << "|           Financial Insights Chatbot (Gemini 3)          |\n";
  std::cout << "| Ask anything about your finances. Replies from Gemini 3. |\n";
  std::cout << "| Set GEMINI_API_KEY in your environment for API access.   |\n";
  std::cout << "| Type 'exit' or 'quit' to return to the main menu.        |\n";
  std::cout << "+----------------------------------------------------------+\n";
  std::cout << "\n";

  while (true) {
    std::string input = readLine("You: ");
    if (input.empty())
      continue;

    std::string lower = input;
    for (auto &c : lower)
      c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));

    if (lower == "exit" || lower == "quit" || lower == "bye") {
      std::cout
          << "\nAssistant: Goodbye. Return anytime for financial insights.\n\n";
      break;
    }

    std::string reply = m_chatbot.respond(input);
    std::cout << "\nAssistant: " << reply << "\n\n";
  }
}
