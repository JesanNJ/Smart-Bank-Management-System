#include "Account.h"

Account::Account(int id, int userId, const std::string& accountNumber)
    : m_id(id), m_userId(userId), m_accountNumber(accountNumber) {}

int Account::getId() const {
    return m_id;
}

int Account::getUserId() const {
    return m_userId;
}

const std::string& Account::getAccountNumber() const {
    return m_accountNumber;
}

double Account::getBalance() const {
    return m_balance;
}

void Account::deposit(double amount) {
    if (amount > 0.0) {
        m_balance += amount;
    }
}

bool Account::withdraw(double amount) {
    if (amount <= 0.0) {
        return false;
    }
    if (amount > m_balance) {
        return false;
    }
    m_balance -= amount;
    return true;
}

