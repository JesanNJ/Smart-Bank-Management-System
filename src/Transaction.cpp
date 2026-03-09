#include "Transaction.h"

Transaction::Transaction(int id,
                         int accountId,
                         TransactionType type,
                         double amount,
                         SpendingCategory category,
                         const std::string& description,
                         const std::tm& timestamp)
    : m_id(id),
      m_accountId(accountId),
      m_type(type),
      m_amount(amount),
      m_category(category),
      m_description(description),
      m_timestamp(timestamp) {}

int Transaction::getId() const {
    return m_id;
}

int Transaction::getAccountId() const {
    return m_accountId;
}

TransactionType Transaction::getType() const {
    return m_type;
}

double Transaction::getAmount() const {
    return m_amount;
}

SpendingCategory Transaction::getCategory() const {
    return m_category;
}

const std::string& Transaction::getDescription() const {
    return m_description;
}

const std::tm& Transaction::getTimestamp() const {
    return m_timestamp;
}

