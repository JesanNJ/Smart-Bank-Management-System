#pragma once

#include "Account.h"

#include <QMap> // REQUIRED
#include <QString>
#include <QStringList>


class AIAnalyzer {
public:
  static double calculateSavingsRatio(double totalIncome, double totalSavings);

  static QMap<QString, double> getCategoryExpenses(const Account &account,
                                                   int days = 30);

  static QStringList
  checkOverspending(const Account &account,
                    const QMap<QString, double> &categoryExpenses);

  static QStringList
  checkBudgetUsage(const Account &account,
                   const QMap<QString, double> &categoryExpenses);

  static QStringList detectRisk(const Account &account, double totalIncome);

  static int calculateFinancialHealthScore(const Account &account,
                                           double totalIncome);

  static QStringList generateRecommendations(const Account &account,
                                             double totalIncome);

  static double calculateTotalIncome(const Account &account, int days = 30);

  static double calculateTotalExpense(const Account &account, int days = 30);
};