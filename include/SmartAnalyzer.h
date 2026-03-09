#pragma once

#include <map>
#include <string>
#include <vector>

#include "Budget.h"
#include "Goal.h"
#include "Transaction.h"


// Encapsulates rule-based smart analysis on top of
// transactions, budgets, and goals.
class SmartAnalyzer {
public:
  struct AnalysisResult {
    double savingsRatio{0.0};         // percent of income saved.
    double financialHealthScore{0.0}; // 0..100
    bool isOverspending{false};       // discretionary > 35%
    bool isBudgetOver80{false};       // budget usage > 80%
    bool isHighRisk{false};           // savings ratio < 15%

    // Percentage of expenses by category.
    std::map<SpendingCategory, double> categoryExpenseShare;

    std::vector<std::string> warnings;
    std::vector<std::string> recommendations;
  };

  AnalysisResult analyze(const std::vector<Transaction> &transactions,
                         const Budget &budget, const Goal &goal) const;

private:
  double
  calculateSavingsRatio(const std::vector<Transaction> &transactions) const;
  std::map<SpendingCategory, double>
  calculateCategoryShares(const std::vector<Transaction> &transactions) const;
  bool detectOverspending(
      const std::map<SpendingCategory, double> &categoryShares) const;
  bool detectBudgetWarning(const Budget &budget) const;
  bool detectRisk(double savingsRatio) const;
  double computeHealthScore(double savingsRatio, bool overspending,
                            bool budgetWarning, bool highRisk,
                            const Goal &goal) const;

  void buildRecommendations(const AnalysisResult &result, const Goal &goal,
                            std::vector<std::string> &out) const;
};
