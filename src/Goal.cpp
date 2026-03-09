#include "Goal.h"

Goal::Goal(const std::string& name, double targetAmount, int targetMonths)
    : m_name(name), m_targetAmount(targetAmount), m_targetMonths(targetMonths) {}

const std::string& Goal::getName() const {
    return m_name;
}

double Goal::getTargetAmount() const {
    return m_targetAmount;
}

int Goal::getTargetMonths() const {
    return m_targetMonths;
}

void Goal::setCurrentSaved(double amount) {
    m_currentSaved = amount >= 0.0 ? amount : 0.0;
}

double Goal::getCurrentSaved() const {
    return m_currentSaved;
}

double Goal::getProgressRatio() const {
    if (m_targetAmount <= 0.0) {
        return 0.0;
    }
    return m_currentSaved / m_targetAmount;
}

