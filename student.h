
#ifndef STUDENT_H
#define STUDENT_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Student
{
public:
    int id;
    string name;
    string bio;
    string branch;
    int passingYear;
    vector<string> skills;

    vector<string> achievements;
    float cgpa;
    string contact;

    void displayProfile();
};

#endif
