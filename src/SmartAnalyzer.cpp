#include "SmartAnalyzer.h"

#include <algorithm>
#include <numeric>

namespace {
bool isDiscretionary(SpendingCategory category) {
  switch (category) {
  case SpendingCategory::Entertainment:
  case SpendingCategory::Dining:
  case SpendingCategory::Education:
  case SpendingCategory::Other:
    return true;
  default:
    return false;
  }
}
} // namespace

SmartAnalyzer::AnalysisResult
SmartAnalyzer::analyze(const std::vector<Transaction> &transactions,
                       const Budget &budget, const Goal &goal) const {
  AnalysisResult result;
  result.savingsRatio = calculateSavingsRatio(transactions);
  result.categoryExpenseShare = calculateCategoryShares(transactions);

  result.isOverspending = detectOverspending(result.categoryExpenseShare);
  result.isBudgetOver80 = detectBudgetWarning(budget);
  result.isHighRisk = detectRisk(result.savingsRatio);

  result.financialHealthScore =
      computeHealthScore(result.savingsRatio, result.isOverspending,
                         result.isBudgetOver80, result.isHighRisk, goal);

  if (result.isOverspending) {
    result.warnings.push_back("You are overspending in discretionary "
                              "categories (over 35% of expenses).");
  }
  if (result.isBudgetOver80) {
    result.warnings.push_back(
        "Your monthly budget usage is above 80%. Slow down spending.");
  }
  if (result.isHighRisk) {
    result.warnings.push_back("Your savings ratio is below 15%. This is "
                              "considered financially risky.");
  }

  buildRecommendations(result, goal, result.recommendations);

  return result;
}

double SmartAnalyzer::calculateSavingsRatio(
    const std::vector<Transaction> &transactions) const {
  double income = 0.0;
  double savings = 0.0;

  for (const auto &tx : transactions) {
    if (tx.getType() == TransactionType::Deposit) {
      if (tx.getCategory() == SpendingCategory::Income) {
        income += tx.getAmount();
      } else if (tx.getCategory() == SpendingCategory::Savings) {
        savings += tx.getAmount();
      }
    }
  }

  if (income <= 0.0) {
    return 0.0;
  }

  return (savings / income) * 100.0;
}

std::map<SpendingCategory, double> SmartAnalyzer::calculateCategoryShares(
    const std::vector<Transaction> &transactions) const {
  std::map<SpendingCategory, double> totals;
  double totalExpenses = 0.0;

  for (const auto &tx : transactions) {
    if (tx.getType() == TransactionType::Withdrawal) {
      totals[tx.getCategory()] += tx.getAmount();
      totalExpenses += tx.getAmount();
    }
  }

  if (totalExpenses <= 0.0) {
    return {};
  }

  std::map<SpendingCategory, double> shares;
  for (const auto &entry : totals) {
    shares[entry.first] = (entry.second / totalExpenses) * 100.0;
  }
  return shares;
}

bool SmartAnalyzer::detectOverspending(
    const std::map<SpendingCategory, double> &categoryShares) const {
  double discretionaryShare = 0.0;
  for (const auto &entry : categoryShares) {
    if (isDiscretionary(entry.first)) {
      discretionaryShare += entry.second;
    }
  }
  return discretionaryShare > 35.0;
}

bool SmartAnalyzer::detectBudgetWarning(const Budget &budget) const {
  return budget.getUsageRatio() > 0.8;
}

bool SmartAnalyzer::detectRisk(double savingsRatio) const {
  return savingsRatio < 15.0;
}

double SmartAnalyzer::computeHealthScore(double savingsRatio, bool overspending,
                                         bool budgetWarning, bool highRisk,
                                         const Goal &goal) const {
  // Start from a neutral score.
  double score = 50.0;

  // Savings ratio impact.
  if (savingsRatio >= 20.0) {
    score += 20.0;
  } else if (savingsRatio >= 15.0) {
    score += 10.0;
  } else if (savingsRatio >= 10.0) {
    score += 0.0;
  } else {
    score -= 10.0;
  }

  // Goal progress.
  double goalProgress = goal.getProgressRatio() * 100.0;
  if (goalProgress >= 75.0) {
    score += 15.0;
  } else if (goalProgress >= 40.0) {
    score += 5.0;
  } else if (goalProgress < 10.0 && goal.getTargetAmount() > 0.0) {
    score -= 5.0;
  }

  // Penalties.
  if (overspending) {
    score -= 10.0;
  }
  if (budgetWarning) {
    score -= 10.0;
  }
  if (highRisk) {
    score -= 10.0;
  }

  if (score < 0.0)
    score = 0.0;
  if (score > 100.0)
    score = 100.0;
  return score;
}

void SmartAnalyzer::buildRecommendations(const AnalysisResult &result,
                                         const Goal &goal,
                                         std::vector<std::string> &out) const {
  if (result.savingsRatio < 15.0) {
    out.push_back(
        "Try to increase your monthly savings to at least 15% of your income.");
  } else if (result.savingsRatio < 20.0) {
    out.push_back(
        "You are close to a healthy savings level. Aim for 20% of income.");
  } else {
    out.push_back("Great job! Maintain your savings ratio above 20% to build "
                  "strong reserves.");
  }

  if (result.isOverspending) {
    out.push_back("Reduce discretionary expenses such as dining out and "
                  "entertainment by 10–20%.");
  }

  if (result.isBudgetOver80) {
    out.push_back("Pause non-essential purchases for the rest of the month to "
                  "stay within budget.");
  }

  if (goal.getTargetAmount() > 0.0) {
    double remaining = goal.getTargetAmount() - goal.getCurrentSaved();
    if (remaining > 0.0 && goal.getTargetMonths() > 0) {
      double perMonth = remaining / goal.getTargetMonths();
      out.push_back("To reach your goal \"" + goal.getName() +
                    "\" save approximately " +
                    std::to_string(static_cast<int>(perMonth)) + " per month.");
    } else if (remaining <= 0.0) {
      out.push_back("Congratulations! You have reached your savings goal \"" +
                    goal.getName() + "\".");
    }
  }

  if (result.financialHealthScore < 40.0) {
    out.push_back("Your financial health score is low. Consider building an "
                  "emergency fund covering 3–6 months of expenses.");
  } else if (result.financialHealthScore > 75.0) {
    out.push_back("Your financial health looks strong. You can explore "
                  "long-term investments aligned with your risk profile.");
  }
}
