#pragma once

#include <string>

// Represents a user savings goal such as
// "Save $5000 for emergency fund in 12 months".
class Goal {
public:
    Goal() = default;
    Goal(const std::string& name, double targetAmount, int targetMonths);

    const std::string& getName() const;
    double getTargetAmount() const;
    int getTargetMonths() const;

    void setCurrentSaved(double amount);
    double getCurrentSaved() const;

    double getProgressRatio() const; // 0..1 based on target amount.

private:
    std::string m_name;
    double m_targetAmount{0.0};
    int m_targetMonths{0};
    double m_currentSaved{0.0};
};

