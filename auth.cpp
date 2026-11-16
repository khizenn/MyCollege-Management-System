#include "auth.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
using namespace std;

static string lowerText(string s)
{
    transform(s.begin(), s.end(), s.begin(), [](unsigned char c)
              { return static_cast<char>(tolower(c)); });
    return s;
}

static string readRequired(const string &prompt)
{
    string value;
    do
    {
        cout << prompt;
        getline(cin >> ws, value);
        if (value.empty())
            cout << "This field cannot be empty.\n";
    } while (value.empty());
    return value;
}

AuthManager::AuthManager() : nextId(1) { loadUsers(); }

bool AuthManager::emailExists(const string &email) const
{
    for (const User &u : users)
        if (lowerText(u.email) == lowerText(email))
            return true;
    return false;
}

void AuthManager::loadUsers()
{
    ifstream file("users.txt");
    if (!file)
        return;
    users.clear();
    User u;
    while (file >> u.id)
    {
        file.ignore();
        getline(file, u.fullName);
        getline(file, u.email);
        getline(file, u.password);
        getline(file, u.role);
        file >> u.collegeId;
        file.ignore();
        getline(file, u.department);
        users.push_back(u);
        if (u.id >= nextId)
            nextId = u.id + 1;
    }
}

void AuthManager::saveUsers() const
{
    ofstream file("users.txt");
    for (const User &u : users)
    {
        file << u.id << '\n'
             << u.fullName << '\n'
             << u.email << '\n'
             << u.password << '\n'
             << u.role << '\n'
             << u.collegeId << '\n'
             << u.department << '\n';
    }
}

bool AuthManager::registerUser()
{
    cout << "\n--- MyCollege Registration ---\n";
    User u{};
    u.fullName = readRequired("Enter full name: ");
    while (true)
    {
        u.email = readRequired("Enter email: ");
        if (u.email.find('@') == string::npos || u.email.find('.', u.email.find('@')) == string::npos)
            cout << "Please enter a valid email address.\n";
        else if (emailExists(u.email))
            cout << "Email already registered.\n";
        else
            break;
    }
    while (true)
    {
        u.password = readRequired("Enter password (minimum 6 characters): ");
        if (u.password.size() >= 6)
            break;
        cout << "Password must contain at least 6 characters.\n";
    }
    while (true)
    {
        u.role = lowerText(readRequired("Select role (student/faculty): "));
        if (u.role == "student" || u.role == "faculty")
            break;
        cout << "Role must be student or faculty.\n";
    }
    cout << "Enter college ID: ";
    while (!(cin >> u.collegeId))
    {
        cout << "Enter a valid numeric college ID: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }
    cin.ignore(10000, '\n');
    for (const User &existing : users)
    {
        if (existing.collegeId == u.collegeId)
        {
            cout << "This college ID is already registered.\n";
            return false;
        }
    }
    u.department = readRequired("Enter department: ");
    u.id = nextId++;
    users.push_back(u);
    saveUsers();
    cout << "\nRegistration successful! Your user ID is " << u.id << ".\n";
    return true;
}

bool AuthManager::loginUser(User &loggedInUser)
{
    cout << "\n--- MyCollege Login ---\n";
    for (int attempt = 1; attempt <= 3; ++attempt)
    {
        string email = readRequired("Enter email: ");
        string password = readRequired("Enter password: ");
        for (const User &u : users)
        {
            if (lowerText(u.email) == lowerText(email) && u.password == password)
            {
                loggedInUser = u;
                cout << "Login successful. Welcome, " << u.fullName << "!\n";
                return true;
            }
        }
        cout << "Incorrect email or password. Attempts left: " << 3 - attempt << '\n';
    }
    cout << "Too many failed attempts.\n";
    return false;
}

const vector<User> &AuthManager::getUsers() const { return users; }
