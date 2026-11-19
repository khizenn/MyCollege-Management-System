#include "auth.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <limits>
using namespace std;

static string lowerText(string s)
{
    transform(s.begin(), s.end(), s.begin(), [](unsigned char c)
              { return static_cast<char>(tolower(c)); });
    return s;
}

// Removes leading and trailing whitespace (spaces, tabs, '\r', etc.)
static string trimText(const string &s)
{
    size_t start = 0;
    while (start < s.size() && isspace(static_cast<unsigned char>(s[start])))
        ++start;
    size_t end = s.size();
    while (end > start && isspace(static_cast<unsigned char>(s[end - 1])))
        --end;
    return s.substr(start, end - start);
}

static bool isValidEmail(const string &email)
{
    size_t at = email.find('@');
    if (at == string::npos || at == 0 || email.find('@', at + 1) != string::npos)
        return false;
    if (email.find(' ') != string::npos)
        return false;
    size_t dot = email.find('.', at);
    // need at least one character between '@' and '.', and one after '.'
    return dot != string::npos && dot > at + 1 && dot + 1 < email.size();
}

static string readRequired(const string &prompt)
{
    string value;
    do
    {
        cout << prompt;
        if (!getline(cin >> ws, value))
        {
            // Input stream closed (EOF) - avoid an infinite loop
            cout << "\nInput ended unexpectedly. Exiting.\n";
            exit(1);
        }
        value = trimText(value);
        if (value.empty())
            cout << "This field cannot be empty.\n";
    } while (value.empty());
    return value;
}

AuthManager::AuthManager() : nextId(1) { loadUsers(); }

bool AuthManager::emailExists(const string &email) const
{
    const string target = lowerText(email);
    for (const User &u : users)
        if (lowerText(u.email) == target)
            return true;
    return false;
}

void AuthManager::loadUsers()
{
    ifstream file("users.txt");
    if (!file)
        return;
    users.clear();
    nextId = 1;
    while (true)
    {
        User u{};
        if (!(file >> u.id))
            break;
        file.ignore(numeric_limits<streamsize>::max(), '\n');
        if (!getline(file, u.fullName) ||
            !getline(file, u.email) ||
            !getline(file, u.password) ||
            !getline(file, u.role))
            break;
        if (!(file >> u.collegeId))
            break;
        file.ignore(numeric_limits<streamsize>::max(), '\n');
        if (!getline(file, u.department))
            break;
        users.push_back(u);
        if (u.id >= nextId)
            nextId = u.id + 1;
    }
}

void AuthManager::saveUsers() const
{
    ofstream file("users.txt");
    if (!file)
    {
        cout << "Warning: could not save user data.\n";
        return;
    }
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
        if (!isValidEmail(u.email))
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
    while (!(cin >> u.collegeId) || u.collegeId <= 0)
    {
        if (cin.eof())
        {
            cout << "\nInput ended unexpectedly. Exiting.\n";
            exit(1);
        }
        cout << "Enter a valid numeric college ID: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
        const string emailKey = lowerText(email);
        for (const User &u : users)
        {
            if (lowerText(u.email) == emailKey && u.password == password)
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
