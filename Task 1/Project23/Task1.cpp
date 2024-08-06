#include <iostream>
#include <vector>
#include <iomanip> // For setting decimal precision
#include <unordered_map> // For mapping letter grades to GPA values

using namespace std;

struct Course {
    string courseName;
    int creditHours;
    string letterGrade;
    float gradePoint;
};

// Function to convert letter grades to GPA values
float convertGradeToPoint(const string& letterGrade) {
    unordered_map<string, float> gradeMap = {
        {"A", 4.0}, {"A-", 3.7}, {"B+", 3.3}, {"B", 3.0},
        {"B-", 2.7}, {"C+", 2.3}, {"C", 2.0}, {"C-", 1.7},
        {"D+", 1.3}, {"D", 1.0}, {"F", 0.0}
    };
    return gradeMap[letterGrade];
}

float calculateGPA(const vector<Course>& courses) {
    float totalGradePoints = 0;
    int totalCredits = 0;

    for (const auto& course : courses) {
        totalGradePoints += course.gradePoint * course.creditHours;
        totalCredits += course.creditHours;
    }

    return totalGradePoints / totalCredits;
}

int main() {
    int numCourses;
    vector<Course> courses;

    cout << "Enter the number of courses: ";
    cin >> numCourses;

    for (int i = 0; i < numCourses; ++i) {
        Course course;
        cout << "Enter the name of course " << i + 1 << ": ";
        cin >> course.courseName;
        cout << "Enter the credit hours for " << course.courseName << ": ";
        cin >> course.creditHours;
        cout << "Enter the letter grade for " << course.courseName << ": ";
        cin >> course.letterGrade;

        course.gradePoint = convertGradeToPoint(course.letterGrade);
        courses.push_back(course);
    }

    float gpa = calculateGPA(courses);

    cout << fixed << setprecision(2);
    cout << "\nCourse Details:\n";
    for (const auto& course : courses) {
        cout <<" -->Course: " << course.courseName
            << "  -->Credit Hours: " << course.creditHours
            << "  -->Letter Grade: " << course.letterGrade
            << "  -->Grade Point: " << course.gradePoint << "\n";
    }

    cout << "\nTotal GPA: " << gpa << "\n";

    return 0;
}
