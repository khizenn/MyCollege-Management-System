#ifndef STUDENTPORTAL_H
#define STUDENTPORTAL_H

#include "student.h"
#include <vector>

using namespace std;

class StudentPortal
{
private:
    vector<Student> students;
    int nextId = 1;

public:
    void registerStudent();
    void registerStudent(int assignedId);
    void viewAllStudents();
    void searchStudent();
    void updateProfile(int id);
    void saveStudents();
    void loadStudents();
};

#endif
