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

int main(){
    unsigned int numberOfStudents;
    scanf ("%d", &numberOfStudents);

    StudentDetails students[numberOfStudents];

    for (int index = 0; index < numberOfStudents; index++)
    {
        getStudentDetails(students, index);
    }

    return 0;
}