#include "Budget.h"

void Budget::setMonthlyLimit(double amount) {
    m_monthlyLimit = amount > 0.0 ? amount : 0.0;
}

double Budget::getMonthlyLimit() const {
    return m_monthlyLimit;
}

void Budget::setCategoryLimit(SpendingCategory category, double amount) {
    if (amount < 0.0) {
        amount = 0.0;
    }
    m_categoryLimits[category] = amount;
}

double Budget::getCategoryLimit(SpendingCategory category) const {
    auto it = m_categoryLimits.find(category);
    if (it != m_categoryLimits.end()) {
        return it->second;
    }
    return 0.0;
}

void Budget::addSpending(SpendingCategory category, double amount) {
    if (amount <= 0.0) {
        return;
    }
    m_categorySpent[category] += amount;
}

double Budget::getTotalSpent() const {
    double total = 0.0;
    for (const auto& entry : m_categorySpent) {
        total += entry.second;
    }
    return total;
}

double Budget::getSpentInCategory(SpendingCategory category) const {
    auto it = m_categorySpent.find(category);
    if (it != m_categorySpent.end()) {
        return it->second;
    }
    return 0.0;
}

double Budget::getUsageRatio() const {
    if (m_monthlyLimit <= 0.0) {
        return 0.0;
    }
    return getTotalSpent() / m_monthlyLimit;
}

