#pragma once

#include <string>
#include <ctime>

// Basic transaction types.
enum class TransactionType {
    Deposit,
    Withdrawal
};

// High-level spending categories to enable AI analysis.
enum class SpendingCategory {
    Income,
    Housing,
    Utilities,
    Groceries,
    Dining,
    Transport,
    Entertainment,
    Healthcare,
    Education,
    Savings,
    Other
};

// Represents a single money movement on an account.
class Transaction {
public:
    Transaction() = default;
    Transaction(int id,
                int accountId,
                TransactionType type,
                double amount,
                SpendingCategory category,
                const std::string& description,
                const std::tm& timestamp);

    int getId() const;
    int getAccountId() const;
    TransactionType getType() const;
    double getAmount() const;
    SpendingCategory getCategory() const;
    const std::string& getDescription() const;
    const std::tm& getTimestamp() const;

private:
    int m_id{0};
    int m_accountId{0};
    TransactionType m_type{TransactionType::Deposit};
    double m_amount{0.0};
    SpendingCategory m_category{SpendingCategory::Other};
    std::string m_description;
    std::tm m_timestamp{};
};

