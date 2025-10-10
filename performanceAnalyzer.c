#include<stdio.h>
#include<string.h>

#define NAME_SIZE 100

#define GRADE_A 85
#define GRADE_B 70
#define GRADE_C 50
#define GRADE_D 35

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
    if (students[index].averageMarks >= GRADE_A)
    {
        students[index].grade = 'A';
    } 
    else if (students[index].averageMarks >= GRADE_B)
    {
        students[index].grade = 'B';
    }
    else if (students[index].averageMarks >= GRADE_C)
    {
        students[index].grade = 'C';
    }
    else if (students[index].averageMarks >= GRADE_D)
    {
        students[index].grade = 'D';
    }
    else 
    {
        students[index].grade = 'F';
    }
}

void displayStudentDetails(StudentDetails students[], const int numberOfStudents){
    for (int index = 0; index < numberOfStudents; index++)
    {
        printf("\nRoll: %u\n", students[index].rollNumber);
        printf("Name: %s\n", students[index].name);
        printf("Total: %u\n", students[index].totalMarks);
        printf("Average: %.2f\n", students[index].averageMarks);
        printf("Grade: %c\n", students[index].grade);

        if (students[index].grade == 'F')
        {
            continue;
        }

        int numberOfStars;
        switch (students[index].grade) 
        {
            case 'A':
                numberOfStars = 5;
                break;
            case 'B':
                numberOfStars = 4;
                break;
            case 'C':
                numberOfStars = 3;
                break;
            case 'D':
                numberOfStars = 2;
                break;
            default:
                numberOfStars = -1;  
                printf("Invalid grade for student %d\n", index);
        }

        printf("Performance: ");
        for (int starNumber = 0; starNumber < numberOfStars ; starNumber++)
        {
            printf("*");
        }
        printf("\n");
    }
}

void displayRollNumbers(StudentDetails students[], int numberOfStudents){
    if (numberOfStudents == 0)
    {
        return;
    }

    displayRollNumbers(students, numberOfStudents-1);
    printf("%d ", students[numberOfStudents-1].rollNumber);
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

    displayStudentDetails(students, numberOfStudents);

    printf("\nList of Roll Numbers: ");
    displayRollNumbers(students, numberOfStudents);
    return 0;
}