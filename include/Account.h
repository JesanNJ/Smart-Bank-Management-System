#pragma once

#include <string>
#include <vector>

class Transaction;

// Represents a bank account owned by a single user.
class Account {
public:
    Account() = default;
    Account(int id, int userId, const std::string& accountNumber);

    int getId() const;
    int getUserId() const;
    const std::string& getAccountNumber() const;

    double getBalance() const;
    void deposit(double amount);
    bool withdraw(double amount);

private:
    int m_id{0};
    int m_userId{0};
    std::string m_accountNumber;
    double m_balance{0.0};
};

