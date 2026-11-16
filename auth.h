#ifndef AUTH_H
#define AUTH_H

#include <string>
#include <vector>

struct User {
    int id;
    std::string fullName;
    std::string email;
    std::string password;
    std::string role;
    int collegeId;
    std::string department;
};

class AuthManager {
private:
    std::vector<User> users;
    int nextId;
    void saveUsers() const;
public:
    AuthManager();
    void loadUsers();
    bool registerUser();
    bool loginUser(User &loggedInUser);
    const std::vector<User>& getUsers() const;
    bool emailExists(const std::string& email) const;
};

#endif
