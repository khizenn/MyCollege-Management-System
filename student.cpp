#include "student.h"

void Student::displayProfile()
{
    cout << "\n---- STUDENT PROFILE ----\n";
    cout << "ID: " << id << endl;
    cout << "Name: " << name << endl;
    cout << "Bio: " << bio << endl;
    cout << "Branch: " << branch << endl;
    cout << "Passing Year: " << passingYear << endl;

    cout << "Skills: ";
    for (int i = 0; i < skills.size(); i++)
    {
        cout << skills[i] << " ";
    }

    cout << endl<<"Achievements::";
    if (achievements.empty())
    {
        cout << "None";
    }

    for (int i = 0; i < achievements.size(); i++)
    {
        cout << achievements[i] << ", ";
    }

    cout << "\nCGPA::" << cgpa << endl;
    cout << "Contact:" << contact << endl;
}
