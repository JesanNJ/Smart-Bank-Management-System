#pragma once

#include "BankSystem.h"
#include "FinancialChatbot.h"

// Simple console "UI" class that presents menus
// and delegates work to BankSystem. This keeps main.cpp
// clean and focused on startup/shutdown.
class MainWindow {
public:
  MainWindow();
  void run();

private:
  BankSystem m_bankSystem;
  FinancialChatbot m_chatbot;
  void populateDummyData();

  int m_loggedInUserId{-1};
  int m_loggedInAccountId{-1};
  bool m_isAdmin{false};

  void showWelcomeMenu();
  void handleWelcomeChoice(int choice);

  void showMainMenu();
  void handleMainChoice(int choice);

  void showAdminMenu();
  void handleAdminChoice(int choice);

  void doRegister();
  void doLogin();
  void doAdminLogin();

  void ensureLoggedIn();

  void doDeposit();
  void doWithdraw();
  void doViewTransactions();
  void doSetBudget();
  void doSetGoal();
  void doMonthlyReport();
  void doSpendingTrendComparison();
  void doRiskAndAIAnalysis();
  void doFinancialChatbot();

  void doAdminViewAllUsers();
  void doAdminViewAllAccounts();
  void doAdminViewAllTransactions();
};
