#include <iostream>
#include <limits>
#include <string>
#include "auth.h"
#include "connection.h"
#include "studentportal.h"
#include "feedmanager.h"
using namespace std;

static int readChoice(const string &prompt)
{
    int choice;
    cout << prompt;
    while (!(cin >> choice))
    {
        cout << "Please enter a valid number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return choice;
}

static void searchAndConnect(const User &me, const AuthManager &auth, ConnectionManager &connections)
{
    string name;
    cout << "Enter name to search: ";
    getline(cin, name);
    bool found = false;
    for (const User &u : auth.getUsers())
    {
        if (u.fullName.find(name) != string::npos)
        {
            found = true;
            cout << "\nID: " << u.id << "\nName: " << u.fullName
                 << "\nRole: " << u.role << "\nDepartment: " << u.department << '\n';
            if (u.id != me.id)
            {
                cout << "Send connection request? (1=Yes, 0=No): ";
                int answer;
                if (cin >> answer && answer == 1)
                    connections.sendRequest(me.id, u.id);
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
    }
    if (!found)
        cout << "No matching user found.\n";
}

static void studentMenu(const User &me, AuthManager &auth, StudentPortal &portal,
                        FeedManager &feed, ConnectionManager &connections)
{
    int choice;

    cout << "\n---- STUDENT PORTAL: " << me.fullName << " ----\n"
         << "1. View all student profiles\n2. Search student profile\n"
         << "3. Update my student profile\n4. Create post\n5. Create blog\n"
         << "6. View feed\n7. Search users / Send connection request\n"
         << "8. View connection requests\n9. View connections\n10. View account profile\n11. Logout\n";
    do
    {
        choice = readChoice("Enter choice: ");
        switch (choice)
        {
        case 1:
            portal.viewAllStudents();
            break;
        case 2:
            portal.searchStudent();
            break;
        case 3:
            portal.updateProfile(me.id);
            break;
        case 4:
            feed.createPost(me.id);
            break;
        case 5:
            feed.createBlog(me.id);
            break;
        case 6:
            feed.viewFeed();
            break;
        case 7:
            searchAndConnect(me, auth, connections);
            break;
        case 8:
            connections.viewRequests(me.id, auth.getUsers());
            break;
        case 9:
            connections.viewConnections(me.id, auth.getUsers());
            break;
        case 10:
            cout << "\nUser ID: " << me.id << "\nName: " << me.fullName << "\nEmail: " << me.email
                 << "\nRole: " << me.role << "\nCollege ID: " << me.collegeId
                 << "\nDepartment: " << me.department << '\n';
            break;
        case 11:
            cout << "Logged out successfully.\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 11);
}

static void facultyMenu(const User &me, AuthManager &auth, StudentPortal &portal,
                        FeedManager &feed, ConnectionManager &connections)
{
    int choice;

    cout << "\n----- FACULTY PORTAL: " << me.fullName << " -----\n"
         << "1. View all student profiles\n2. Search student profile\n"
         << "3. Create post\n4. Create blog\n5. View feed\n"
         << "6. Search users / Send connection request\n7. View connection requests\n"
         << "8. View connections\n9. View account profile\n10. Logout\n";
    do
    {
        choice = readChoice("Enter choice: ");
        switch (choice)
        {
        case 1:
            portal.viewAllStudents();
            break;
        case 2:
            portal.searchStudent();
            break;
        case 3:
            feed.createPost(me.id);
            break;
        case 4:
            feed.createBlog(me.id);
            break;
        case 5:
            feed.viewFeed();
            break;
        case 6:
            searchAndConnect(me, auth, connections);
            break;
        case 7:
            connections.viewRequests(me.id, auth.getUsers());
            break;
        case 8:
            connections.viewConnections(me.id, auth.getUsers());
            break;
        case 9:
            cout << "\nUser ID: " << me.id << "\nName: " << me.fullName << "\nEmail: " << me.email
                 << "\nRole: " << me.role << "\nCollege ID: " << me.collegeId
                 << "\nDepartment: " << me.department << '\n';
            break;
        case 10:
            cout << "Logged out successfully.\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 10);
}

int main()
{
    AuthManager auth;
    StudentPortal portal;
    FeedManager feed;
    ConnectionManager connections;
    portal.loadStudents();
    feed.loadFeed();

    int choice;

    cout << "\n-------- MYCOLLEGE STUDENT PORTAL --------\n"
         << "1. Register\n2. Login\n3. Exit\n";
    do
    {
        choice = readChoice("Enter choice: ");
        if (choice == 1)
        {
            if (auth.registerUser())
            {
                const User &u = auth.getUsers().back();
                if (u.role == "student")
                {
                    cout << "\nComplete your student profile:\n";
                    portal.registerStudent(u.id);
                }
            }
        }
        else if (choice == 2)
        {
            User loggedIn{};
            if (auth.loginUser(loggedIn))
            {
                if (loggedIn.role == "student")
                    studentMenu(loggedIn, auth, portal, feed, connections);
                else
                    facultyMenu(loggedIn, auth, portal, feed, connections);
            }
        }
        else if (choice == 3)
        {
            cout << "Thank you for using MyCollege. Goodbye!\n";
        }
        else
            cout << "Invalid choice.\n";
    } while (choice != 3);
    return 0;
}
