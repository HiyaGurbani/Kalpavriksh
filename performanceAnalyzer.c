#include<stdio.h>

#define NAME_SIZE 100

typedef struct {
    unsigned int rollNumber;
    char name[NAME_SIZE];
    unsigned int marksSubject1;
    unsigned int marksSubject2;
    unsigned int marksSubject3;
    unsigned int totalMarks;
    float averageMarks;
    unsigned char grade;
} StudentDetails;

void getStudentDetails(StudentDetails students[], const int index){
    scanf ("%u", &students[index].rollNumber);
    scanf ("%s", &students[index].name);
    scanf ("%u", &students[index].marksSubject1);
    scanf ("%u", &students[index].marksSubject2);
    scanf ("%u", &students[index].marksSubject3);
}

void calculateTotalAndAverageMarks(StudentDetails students[], const int index){
    students[index].totalMarks = students[index].marksSubject1 + students[index].marksSubject2 + students[index].marksSubject3;
    students[index].averageMarks = students[index].totalMarks / 3.0;
}

void calculateGrade(StudentDetails students[], const int index){
    if (students[index].averageMarks >= 85)
    {
        students[index].grade = 'A';
    } 
    else if (students[index].averageMarks >= 70)
    {
        students[index].grade = 'B';
    }
    else if (students[index].averageMarks >= 50)
    {
        students[index].grade = 'C';
    }
    else if (students[index].averageMarks >= 35)
    {
        students[index].grade = 'D';
    }
    else 
    {
        students[index].grade = 'F';
    }
}

int main(){
    unsigned int numberOfStudents;
    scanf ("%d", &numberOfStudents);

    StudentDetails students[numberOfStudents];

    for (int index = 0; index < numberOfStudents; index++)
    {
        getStudentDetails(students, index);
        
        calculateTotalAndAverageMarks(students, index);

        calculateGrade(students, index);
    }

    return 0;
}