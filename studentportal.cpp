#include "studentportal.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void StudentPortal::registerStudent()
{
    registerStudent(nextId);
}

void StudentPortal::registerStudent(int assignedId)
{
    Student s;
    s.id = assignedId;
    if (assignedId >= nextId) nextId = assignedId + 1;
    cout <<endl<< "Enter student name: ";
    getline(cin >> ws, s.name); 

    cout << "Enter bio: ";
    getline(cin, s.bio);

    cout << "Enter branch: ";
    getline(cin, s.branch);

    cout << "Enter passing year: ";
    cin >> s.passingYear;

    int n;

    cout << "Enter number of skills (maximum 5): ";
    cin >> n;

    while (n < 0 || n > 5)
    {
        cout << "Enter a number from 0 to 5: ";
        cin >> n;
    }

    cin.ignore();

    for (int i = 0; i < n; i++)
    {
        string skill;

        cout << "Enter skill " << i + 1 << ": ";
        getline(cin, skill);

        s.skills.push_back(skill);
    }

    cout << "Enter CGPA: ";
    cin >> s.cgpa;

    cin.ignore();

    cout << "Enter contact details: ";
    getline(cin, s.contact);

    int a;

    cout << "Enter number of achievements: ";
    cin >> a;

    while (a < 0 || a > 10)
    {
        cout << "Enter a number from 0 to 10: ";
        cin >> a;
    }

    cin.ignore();

    for (int i = 0; i < a; i++)
    {
        string achievement;

        cout << "Enter achievement " << i + 1 << ": ";
        getline(cin, achievement);

        s.achievements.push_back(achievement);
    }

    students.push_back(s);

    cout << "\nStudent registered successfully!\n";
    s.displayProfile();

    saveStudents();
}
                                         

                    
void StudentPortal::viewAllStudents()
{
    if (students.empty())
    {
        cout << "\nNo students registered yet.\n";
        return;
    }

    for (int i = 0; i < students.size(); i++)
    {
        students[i].displayProfile();
    }
}

void StudentPortal::searchStudent()
{
    string name;
    bool found = false;

    cout << "\nEnter student name to search: ";
    getline(cin >> ws, name);

    for (int i = 0; i < students.size(); i++)
    {
        if (students[i].name == name)
        {
            students[i].displayProfile();
            found = true;
        }
    }

    if (found == false)
    {
        cout << endl<<"Student not found"<<endl;
    }
}

void StudentPortal::updateProfile(int id)
{
    for (int i = 0; i < students.size(); i++)
    {
        if (students[i].id == id)
        {
            int choice;

            cout << "\n----- UPDATE PROFILE -----\n";
            cout << "1. Update Name\n";
            cout << "2. Update Bio\n";
            cout << "3. Update Branch\n";
            cout << "4. Update Passing Year\n";
            cout << "5. Update Skills\n";
            cout << "6. Update CGPA\n";
            cout << "7. Update Contact\n";
            cout << "8. Update Achievements\n";
            cout << "Enter choice: ";

            cin >> choice;
            cin.ignore();

            if (choice == 1)
            {
                cout << "Enter new name: ";
                getline(cin, students[i].name);
            }
            else if (choice == 2)
            {
                cout << "Enter new bio: ";
                getline(cin, students[i].bio);
            }
            else if (choice == 3)
            {
                cout << "Enter new branch: ";
                getline(cin, students[i].branch);
            }
            else if (choice == 4)
            {
                cout << "Enter new passing year: ";
                cin >> students[i].passingYear;
            }
            else if (choice == 5)
            {
                int n;

                cout << "Enter number of skills (0-5): ";
                cin >> n;

                if (n < 0 || n > 5)
                {
                    cout << "Invalid number of skills.\n";
                    return;
                }

                cin.ignore();
                students[i].skills.clear();

                for (int j = 0; j < n; j++)
                {
                    string skill;

                    cout << "Enter skill " << j + 1 << ": ";
                    getline(cin, skill);

                    students[i].skills.push_back(skill);
                }
            }
            else if (choice == 6)
            {
                cout << "Enter new CGPA: ";
                cin >> students[i].cgpa;
            }
            else if (choice == 7)
            {
                cout << "Enter new contact: ";
                getline(cin, students[i].contact);
            }
            else if (choice == 8)
            {
                int n;

                cout << "Enter number of achievements (0-10): ";
                cin >> n;

                if (n < 0 || n > 10)
                {
                    cout << "Invalid number of achievements.\n";
                    return;
                }

                cin.ignore();
                students[i].achievements.clear();

                for (int j = 0; j < n; j++)
                {
                    string achievement;

                    cout << "Enter achievement " << j + 1 << ": ";
                    getline(cin, achievement);

                    students[i].achievements.push_back(achievement);
                }
            }
            else
            {
                cout << "Invalid choice.\n";
                return;
            }

            saveStudents();

            cout << "\nProfile updated successfully!\n";
            students[i].displayProfile();

            return;
        }
    }

    cout << "\nStudent ID not found.\n";
}



void StudentPortal::saveStudents()
{
    ofstream file("students.txt");

    for (int i = 0; i < students.size(); i++)
    {
        file << students[i].id << endl;
        file << students[i].name << endl;
        file << students[i].bio << endl;
        file << students[i].branch << endl;
        file << students[i].passingYear << endl;

        file << students[i].skills.size() << endl;

        for (int j = 0; j < students[i].skills.size(); j++)
        {
            file << students[i].skills[j] << endl;
        }

        file << students[i].cgpa << endl;
        file << students[i].contact << endl;

        file << students[i].achievements.size() << endl;

        for (int j = 0; j < students[i].achievements.size(); j++)
        {
            file << students[i].achievements[j] << endl;
        }
    }

    file.close();
    cout << "Student data saved successfully!\n";
}



void StudentPortal::loadStudents()
{
    ifstream file("students.txt");

    if (!file)
    {
        return;
    }

    students.clear();
    nextId = 1;

    Student s;

    while (file >> s.id)
    {
        file.ignore();

        getline(file, s.name);
        getline(file, s.bio);
        getline(file, s.branch);

        file >> s.passingYear;

        int n;
        file >> n;
        file.ignore();

        s.skills.clear();

        for (int i = 0; i < n; i++)
        {
            string skill;
            getline(file, skill);
            s.skills.push_back(skill);
        }

        file >> s.cgpa;
        file.ignore();

        getline(file, s.contact);

        int a;
        file >> a;
        file.ignore();

        s.achievements.clear();

        for (int i = 0; i < a; i++)
        {
            string achievement;
            getline(file, achievement);
            s.achievements.push_back(achievement);
        }

        students.push_back(s);

        if (s.id >= nextId)
        {
            nextId = s.id + 1;
        }
    }

    file.close();
}
