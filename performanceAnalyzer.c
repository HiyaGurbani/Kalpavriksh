#include<stdio.h>
#include<string.h>

#define NAME_SIZE 100
#define STAR_SIZE 6
#define MIN_ROLL_NUMBER 1

#define TOTAL_SUBJECTS 3
#define MAX_MARKS 100
#define MIN_MARKS 0
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
    char performance[STAR_SIZE];
} StudentDetails;

void getStudentDetails(StudentDetails *student){
    while (1) {
        scanf("%u", &student->rollNumber);
        scanf("%s", student->name);
        scanf("%u %u %u", &student->marksSubject1, &student->marksSubject2, &student->marksSubject3);

        if (student->rollNumber < MIN_ROLL_NUMBER) {
            printf("Entered Roll Number is invalid. Please enter again.\n");
            continue;
        }

        if (student->marksSubject1 > MAX_MARKS || student->marksSubject2 > MAX_MARKS || student->marksSubject3 > MAX_MARKS ||
            student->marksSubject1 < MIN_MARKS || student->marksSubject2 < MIN_MARKS || student->marksSubject3 < MIN_MARKS) {
            printf("Entered marks are incorrect. Please enter again in a range of 1-100.\n");
            continue;
        }

        break;
    }
}

void calculateTotalMarks(StudentDetails *student){
    student->totalMarks = student->marksSubject1 + student->marksSubject2 + student->marksSubject3;
}

void calculateAverageMarks(StudentDetails *student){
    student->averageMarks = (float) student->totalMarks / TOTAL_SUBJECTS;
}

void calculateGrade(StudentDetails *student){
    if (student->averageMarks < MIN_MARKS || student->averageMarks > MAX_MARKS) {
        printf("Warning: Invalid average %.2f detected for Roll No %u\n",
            student->averageMarks, student->rollNumber);
        student->grade = 'X';
        strcpy(student->performance, "");
        return;
    }

    if (student->averageMarks >= GRADE_A && student->averageMarks <= MAX_MARKS)
    {
        student->grade = 'A';
        strcpy (student->performance, "*****");
    } 
    else if (student->averageMarks >= GRADE_B)
    {
        student->grade = 'B';
        strcpy (student->performance, "****");
    }
    else if (student->averageMarks >= GRADE_C)
    {
        student->grade = 'C';
        strcpy (student->performance, "***");
    }
    else if (student->averageMarks >= GRADE_D)
    {
        student->grade = 'D';
        strcpy (student->performance, "**");
    }
    else if (student->averageMarks < GRADE_D && student->averageMarks >= MIN_MARKS )
    {
        student->grade = 'F';
        strcpy (student->performance, "");
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

        printf("Performance: %s\n", students[index].performance);
        
    }
}

void displayRollNumbers(StudentDetails students[], int numberOfStudents){
    if (numberOfStudents == 0)
    {
        return;
    }

    displayRollNumbers(students, numberOfStudents-1);
    printf("%u ", students[numberOfStudents-1].rollNumber);
}

int main(){
    unsigned int numberOfStudents;
    scanf ("%u", &numberOfStudents);

    StudentDetails students[numberOfStudents];

    for (int index = 0; index < numberOfStudents; index++)
    {
        getStudentDetails(&students[index]);
        
        calculateTotalMarks(&students[index]);

        calculateAverageMarks(&students[index]);

        calculateGrade(&students[index]);
    }

    displayStudentDetails(students, numberOfStudents);

    printf("\nList of Roll Numbers: ");
    displayRollNumbers(students, numberOfStudents);
    printf("\n");
    
    return 0;
}