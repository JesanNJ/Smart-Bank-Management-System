#pragma once

#include <map>
#include <string>

#include "Transaction.h"

// Tracks monthly budget both overall and per spending category.
class Budget {
public:
    Budget() = default;

    void setMonthlyLimit(double amount);
    double getMonthlyLimit() const;

    void setCategoryLimit(SpendingCategory category, double amount);
    double getCategoryLimit(SpendingCategory category) const;

    void addSpending(SpendingCategory category, double amount);
    double getTotalSpent() const;
    double getSpentInCategory(SpendingCategory category) const;

    double getUsageRatio() const; // 0..1 based on total limit.

private:
    double m_monthlyLimit{0.0};
    std::map<SpendingCategory, double> m_categoryLimits;
    std::map<SpendingCategory, double> m_categorySpent;
};

