// Student Record / Transcript Generator.
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

// ==============================
// COURSE STRUCTURE
// ==============================
struct Course {
    string code;
    string name;
    int creditHours;
    double score;
    string grade;
    double gradePoint;
};

// ==============================
// STUDENT STRUCTURE
// ==============================
struct Student {
    string studentID;
    string name;
    string program;
    string department;
    int level;
    string academicYear;
    string semester;

    vector<Course> courses;
};

// ==============================
// FUNCTION TO DETERMINE GRADE
// ==============================
void calculateGrade(Course &course) {

    if (course.score >= 80 && course.score <= 100) {
        course.grade = "A";
        course.gradePoint = 4.0;
    }
    else if (course.score >= 75) {
        course.grade = "B+";
        course.gradePoint = 3.5;
    }
    else if (course.score >= 70) {
        course.grade = "B";
        course.gradePoint = 3.0;
    }
    else if (course.score >= 65) {
        course.grade = "C+";
        course.gradePoint = 2.5;
    }
    else if (course.score >= 60) {
        course.grade = "C";
        course.gradePoint = 2.0;
    }
    else if (course.score >= 55) {
        course.grade = "D+";
        course.gradePoint = 1.5;
    }
    else if (course.score >= 50) {
        course.grade = "D";
        course.gradePoint = 1.0;
    }
    else {
        course.grade = "F";
        course.gradePoint = 0.0;
    }
}

// ==============================
// CALCULATE GPA
// ==============================
double calculateGPA(const Student &student) {

    double totalQualityPoints = 0;
    int totalCredits = 0;

    for (const Course &course : student.courses) {

        totalQualityPoints +=
            course.creditHours * course.gradePoint;

        totalCredits += course.creditHours;
    }

    if (totalCredits == 0)
        return 0.0;

    return totalQualityPoints / totalCredits;
}

// ==============================
// CALCULATE TOTAL CREDITS
// ==============================
int calculateTotalCredits(const Student &student) {

    int totalCredits = 0;

    for (const Course &course : student.courses) {
        totalCredits += course.creditHours;
    }

    return totalCredits;
}

// ==============================
// DETERMINE ACADEMIC STANDING
// ==============================
string getAcademicStanding(double gpa) {

    if (gpa >= 3.50)
        return "EXCELLENT";

    else if (gpa >= 3.00)
        return "GOOD";

    else if (gpa >= 2.00)
        return "SATISFACTORY";

    else if (gpa >= 1.00)
        return "PROBATION";

    else
        return "ACADEMIC WARNING";
}

// ==============================
// DISPLAY STUDENT INFORMATION
// ==============================
void displayStudentInformation(const Student &student) {

    cout << "\n=============================================\n";
    cout << "             STUDENT INFORMATION\n";
    cout << "=============================================\n";

    cout << "Student ID     : " << student.studentID << endl;
    cout << "Name           : " << student.name << endl;
    cout << "Program        : " << student.program << endl;
    cout << "Department     : " << student.department << endl;
    cout << "Level          : " << student.level << endl;
    cout << "Academic Year  : " << student.academicYear << endl;
    cout << "Semester       : " << student.semester << endl;
}

// ==============================
// DISPLAY TRANSCRIPT
// ==============================
void displayTranscript(const Student &student) {

    double gpa = calculateGPA(student);
    int totalCredits = calculateTotalCredits(student);

    cout << "\n\n";
    cout << "=============================================\n";
    cout << "             STUDENT TRANSCRIPT\n";
    cout << "=============================================\n";

    cout << "\nStudent ID : " << student.studentID << endl;
    cout << "Name       : " << student.name << endl;
    cout << "Program    : " << student.program << endl;

    cout << "\nSemester   : "
         << student.semester << " "
         << student.academicYear << endl;

    cout << "\n---------------------------------------------\n";

    cout << left
         << setw(12) << "Course"
         << setw(25) << "Course Name"
         << setw(10) << "Credit"
         << setw(10) << "Score"
         << setw(10) << "Grade"
         << setw(10) << "Point"
         << endl;

    cout << "---------------------------------------------\n";

    for (const Course &course : student.courses) {

        cout << left
             << setw(12) << course.code
             << setw(25) << course.name
             << setw(10) << course.creditHours
             << setw(10) << fixed << setprecision(0)
             << course.score
             << setw(10) << course.grade
             << setw(10) << fixed << setprecision(1)
             << course.gradePoint
             << endl;
    }

    cout << "---------------------------------------------\n";

    cout << "\nTotal Credits      : "
         << totalCredits << endl;

    cout << "GPA                : "
         << fixed << setprecision(2)
         << gpa << endl;

    cout << "Academic Standing  : "
         << getAcademicStanding(gpa)
         << endl;

    cout << "\n=============================================\n";
}

// ==============================
// SAVE TRANSCRIPT TO FILE
// ==============================
void saveTranscriptToFile(const Student &student) {

    string filename =
        "transcript_" + student.studentID + ".txt";

    ofstream file(filename);

    if (!file) {
        cout << "\nError creating transcript file.\n";
        return;
    }

    double gpa = calculateGPA(student);
    int totalCredits = calculateTotalCredits(student);

    file << "=============================================\n";
    file << "             STUDENT TRANSCRIPT\n";
    file << "=============================================\n\n";

    file << "Student ID     : "
         << student.studentID << endl;

    file << "Name           : "
         << student.name << endl;

    file << "Program        : "
         << student.program << endl;

    file << "Department     : "
         << student.department << endl;

    file << "Level          : "
         << student.level << endl;

    file << "Academic Year  : "
         << student.academicYear << endl;

    file << "Semester       : "
         << student.semester << endl;

    file << "\n---------------------------------------------\n";

    file << left
         << setw(12) << "Course"
         << setw(25) << "Course Name"
         << setw(10) << "Credit"
         << setw(10) << "Score"
         << setw(10) << "Grade"
         << setw(10) << "Point"
         << endl;

    file << "---------------------------------------------\n";

    for (const Course &course : student.courses) {

        file << left
             << setw(12) << course.code
             << setw(25) << course.name
             << setw(10) << course.creditHours
             << setw(10) << fixed << setprecision(0)
             << course.score
             << setw(10) << course.grade
             << setw(10) << fixed << setprecision(1)
             << course.gradePoint
             << endl;
    }

    file << "---------------------------------------------\n";

    file << "\nTotal Credits     : "
         << totalCredits << endl;

    file << "GPA               : "
         << fixed << setprecision(2)
         << gpa << endl;

    file << "Academic Standing : "
         << getAcademicStanding(gpa)
         << endl;

    file << "\n=============================================\n";

    file.close();

    cout << "\nTranscript successfully saved as: "
         << filename << endl;
}

// ==============================
// ADD COURSE
// ==============================
void addCourse(Student &student) {

    Course course;

    cout << "\n=============================================\n";
    cout << "                 ADD COURSE\n";
    cout << "=============================================\n";

    cout << "Course Code: ";
    cin >> course.code;

    cin.ignore();

    cout << "Course Name: ";
    getline(cin, course.name);

    cout << "Credit Hours: ";
    cin >> course.creditHours;

    cout << "Score: ";
    cin >> course.score;

    // Validate score
    while (course.score < 0 || course.score > 100) {

        cout << "Invalid score. Enter a score between 0 and 100: ";
        cin >> course.score;
    }

    calculateGrade(course);

    student.courses.push_back(course);

    cout << "\nCourse added successfully!\n";

    cout << "Grade       : "
         << course.grade << endl;

    cout << "Grade Point : "
         << fixed << setprecision(1)
         << course.gradePoint << endl;
}

// ==============================
// CREATE STUDENT
// ==============================
Student createStudent() {

    Student student;

    cout << "\n=============================================\n";
    cout << "             CREATE STUDENT RECORD\n";
    cout << "=============================================\n";

    cout << "Student ID: ";
    cin >> student.studentID;

    cin.ignore();

    cout << "Name: ";
    getline(cin, student.name);

    cout << "Program: ";
    getline(cin, student.program);

    cout << "Department: ";
    getline(cin, student.department);

    cout << "Level: ";
    cin >> student.level;

    cin.ignore();

    cout << "Academic Year: ";
    getline(cin, student.academicYear);

    cout << "Semester: ";
    getline(cin, student.semester);

    cout << "\nStudent record created successfully!\n";

    return student;
}

// ==============================
// MAIN FUNCTION
// ==============================
int main() {

    Student student;

    int choice;

    cout << "=============================================\n";
    cout << "       STUDENT RECORD MANAGEMENT SYSTEM\n";
    cout << "=============================================\n";

    student = createStudent();

    do {

        cout << "\n\n=============================================\n";
        cout << "                  MAIN MENU\n";
        cout << "=============================================\n";

        cout << "1. Add Course\n";
        cout << "2. View Student Information\n";
        cout << "3. View Transcript\n";
        cout << "4. Save Transcript\n";
        cout << "5. Exit\n";

        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {

            case 1:
                addCourse(student);
                break;

            case 2:
                displayStudentInformation(student);
                break;

            case 3:

                if (student.courses.empty()) {
                    cout << "\nNo courses have been added yet.\n";
                }
                else {
                    displayTranscript(student);
                }

                break;

            case 4:

                if (student.courses.empty()) {
                    cout << "\nAdd courses before saving the transcript.\n";
                }
                else {
                    saveTranscriptToFile(student);
                }

                break;

            case 5:
                cout << "\nThank you for using the Student Record Management System.\n";
                break;

            default:
                cout << "\nInvalid choice. Please try again.\n";
        }

    } while (choice != 5);

    return 0;
}