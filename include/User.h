#pragma once

#include <string>

// Represents a banking system user with basic profile
// and authentication details.
class User {
public:
    User() = default;
    User(int id, const std::string& name, const std::string& password);

    int getId() const;
    const std::string& getName() const;
    void setName(const std::string& name);

    // For a real system you would store a salted hash.
    const std::string& getPassword() const;
    void setPassword(const std::string& password);

private:
    int m_id{0};
    std::string m_name;
    std::string m_password;
};

