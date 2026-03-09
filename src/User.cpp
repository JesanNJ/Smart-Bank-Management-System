#include "User.h"

User::User(int id, const std::string& name, const std::string& password)
    : m_id(id), m_name(name), m_password(password) {}

int User::getId() const {
    return m_id;
}

const std::string& User::getName() const {
    return m_name;
}

void User::setName(const std::string& name) {
    m_name = name;
}

const std::string& User::getPassword() const {
    return m_password;
}

void User::setPassword(const std::string& password) {
    m_password = password;
}

