#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "student.h" 
#include "clerk.h"

// Returns 1 if login is correct.
int studentLoginCheck(Student student,char *number, char *password) {
    return (strcmp(student.password,password) == 0) && (strcmp(student.number,number) == 0);
}
 

// Constructor for Student Struct.
Student createStudentAccount(char *firstName, char *lastName, char *number, char *password, char *majorName, int semester, int totalEcts, int semesterEcts, int currentEcts, int internship) {
    Student student;
    student.firstName = firstName;
    student.lastName = lastName;
    student.number = number;
    student.password = password;
    student.majorName = majorName;
    student.studentInfo.semester = semester;
    student.studentInfo.totalEcts = totalEcts;
    student.studentInfo.semesterEcts = semesterEcts;
    student.studentInfo.currentEcts = currentEcts;
    student.studentInfo.internship = internship;
    return student;
}


// Student Menu will work on successful student login.
void studentMenu(Student student) { 
    int choice;
    while (1) {
        printf("\n0.Exit\n1.Show my grades\n2.Take a new lecture\n3.Change password\n4.Create student certificate\n");
        printf("Please select your process : ");
        scanf("%d",&choice);
        
        if (choice == 0) {
            printf("\nLogged out.\n\n");
            break;
        } else if (choice == 1) {
            showGrades(student.number);
        } else if (choice == 2) {
            char lectureName[50];
            showLectures();
            printf("\nEnter the lesson name that you want to take: ");
            scanf(" %s",lectureName);
            if (checkHaveLectureAlready(student,lectureName) == 0) {
                if (student.studentInfo.semesterEcts >= 5) {
                    printf("\nYour ects is already %d, you can't take more lectures.\n\n",student.studentInfo.semesterEcts);
                    continue;
                }
                int result = addNewLectureToSchedule(student,lectureName);
                if (result == 1) {
                    updateEcts(student.number, 2);
                }
            } else {
                printf("\nYou already have %s lesson!\n",lectureName);
                continue;
            }
        } else if (choice == 3) {
            char oldPassword[50], newPassword[50];
            printf("Please enter your current password : ");
            scanf(" %s",oldPassword);
            if (strcmp(student.password,oldPassword) == 0) {
                printf("\nPlease enter your new password : ");
                scanf(" %s",newPassword);
                changePasswordStudent(student.number, newPassword);
                strcpy(student.password,newPassword);
                printf("\n\nYour password succesfully changed!\n");
            } else {
                printf("\nYour password is wrong.\n");
            }
        } else if (choice == 4) {
            createStudentCertificate(student);
            printf("\nYour certificate has created!\n");
            printf("\nHere is your student certificate!\n");
        } else {
            printf("\nInvalid choice\n");
        }
        
    }
}


// Authenticator for student accounts.
void studentLogin() {
    char number[50], password[50];
    printf("Please enter your number :");
    scanf(" %s",number);
    printf("Please enter your password :");
    scanf(" %s",password);
    char * data[1100];
    FILE *file;
    file = fopen("students.txt","r");
    // Memory allocation for the array
	for (int i =0; i < 1100; i++) {
        data[i] = malloc (50); 
        fscanf (file, "%49s", data[i]);
    }
    fclose(file);
    Student student;

    // Select returns -1 if can't find the student with number.
    int select = -1;
    for (int i = 0; i < 1100; i+=11) {
        if (strcmp(number,data[i+2]) == 0) {
            select = i;
            break;
        }
    }
    if (select == -1) {
        printf("\nCouldn't find an account with number %s.\n\n",number);
        return;
    }
    strcat(data[select+4]," ");
    strcat(data[select+4],data[select+5]);
    student = createStudentAccount(data[select],data[select+1],data[select+2],data[select+3],data[select+4], atoi(data[select+6]), atoi(data[select+7]), atoi(data[select+8]), atoi(data[select+9]), atoi(data[select+10]));        
    if (studentLoginCheck(student,number,password) == 1) {
        printf("\nWelcome %s %s,\n",data[select],data[select+1]);
        studentMenu(student);
    } else {
        printf("\nWrong student number or password. Please try again.\n\n");
    }
   
    // Freeing memory 
    for (int i =0; i < 1100; i++) {
        free (data[i]);
    }
}


// Returns 1 if there is a student with that number.
int checkStudentByNumber(char *number) {
    char *data[1100];
    FILE *file;
    file = fopen("students.txt","r");
    // Memory allocation for the array
	for (int i = 0; i < 1100; i++) {
        data[i] = malloc (50); 
        fscanf (file, "%49s", data[i]);
    }
    fclose(file);

    // Select returns -1 if can't find the student with number.
    int select = -1;
    for (int i = 0; i < 1100; i+=11) {
        if (strcmp(number,data[i+2]) == 0) {
            return 1;
        }
    }
    return 0;
}


// Adds student to lecture database file.
int addNewLectureToSchedule(Student student, char *lectureName) {
    char holdLectureName[50];
    strcpy(holdLectureName, lectureName);
    strcat(lectureName,".txt");
    int flag = 1;

    FILE *lecture;
    if ((lecture = fopen(lectureName,"r"))!=NULL) {
        fclose(lecture);
    } else {
        printf("\nLecture not found!\n\n");
        return 0;
    }

    FILE *file = fopen(lectureName,"a+");
    fprintf(file,"%s %s %s %d %d %d\n",student.firstName,student.lastName,student.number,0,0,0);
    printf("\nNew lecture %s added to the schedule!\n",holdLectureName);
    fclose(file);

    return flag;
}


// Returns student by number.
Student getStudentByNumber(char *number) {
    Student student;
    char * data[1100];
    FILE *file;
    file = fopen("students.txt","r");
	for (int i =0; i < 1100; i++) {
        data[i] = malloc (50); 
        fscanf (file, "%49s", data[i]);
    }
    fclose(file);

    int select = -1;
    for (int i = 0; i < 1100; i+=11) {
        if (strcmp(number,data[i+2]) == 0) {
            select = i;
            break;
        }
    }
    if (select == -1) {
        student = createStudentAccount("0","0","0","0","0",0,0,0,0,0);
        return student;
    }
    strcat(data[select+4]," ");
    strcat(data[select+4],data[select+5]);
    student = createStudentAccount(data[select],data[select+1],data[select+2],data[select+3],data[select+4], atoi(data[select+6]), atoi(data[select+7]), atoi(data[select+8]), atoi(data[select+9]), atoi(data[select+10]));
    
    return student;
}


// If student takes that lecture already return 1 else 0.
int checkHaveLectureAlready(Student student, char *lectureName) {
    int lectureCount = countTotalLines("lessons.txt") - 1;
    char data[lectureCount*2][50];
    FILE *file = fopen("lessons.txt","r");
	for (int i =0; i < lectureCount*2; i++) {
        fscanf (file, "%49s", data[i]);
    }
    fclose(file);

    // Select returns -1 if can't find the lecture.
    int select = -1;
    for (int i = 0; i < lectureCount*2; i+=2) {
        if (strcmp(lectureName,data[i]) == 0) {
            select = i;
            break;
        }
    }
    // If lecture found, it checks if lecture file contains the student.
    if (select != -1) {
        char fileName[50];
        strcpy(fileName,lectureName);
        strcat(fileName,".txt");
        FILE *file = fopen(fileName,"r");
        int studentCount = countTotalLines(fileName) - 2;
        char students[studentCount*6][50];
        char temp[50];
        fscanf (file, "%49s", temp);
        fscanf (file, "%49s", temp);
        for (int i =0; i < (studentCount*6); i++) {
            fscanf (file, "%49s", students[i]);
            printf("%s\n",students[i]);
        }
        fclose(file);

        for (int i = 0; i < (studentCount*6); i+=6) {
            if (strcmp(student.number,students[i+2]) == 0) {
                return 1;
            }
        }
    } 
    return 0;
}


// Changes password of student.
void changePasswordStudent(char *studentNumber, char *newPassword) {
    FILE *file = fopen("students.txt","r");
    FILE *tempfile = fopen("tempfile.txt","w");
    Student student = getStudentByNumber(studentNumber);
    char majorNameFirst[50], majorNameSecond[50];
    while (fscanf(file,"%s %s %s %s %s %s %d %d %d %d %d", student.firstName,student.lastName,student.number,student.password, majorNameFirst, majorNameSecond, &student.studentInfo.semester, &student.studentInfo.totalEcts, &student.studentInfo.semesterEcts, &student.studentInfo.currentEcts, &student.studentInfo.internship) != EOF) {
        if (strcmp(studentNumber,student.number) == 0) {
            strcpy(student.password,newPassword);
        }
        fprintf(tempfile,"%s %s %s %s %s %s %d %d %d %d %d\n", student.firstName,student.lastName,student.number,student.password, majorNameFirst, majorNameSecond, student.studentInfo.semester, student.studentInfo.totalEcts, student.studentInfo.semesterEcts, student.studentInfo.currentEcts, student.studentInfo.internship);
    }
    fclose(file);
    fclose(tempfile);
    remove("students.txt");
    rename("tempfile.txt","students.txt");
}


// Creates student certificate.
void createStudentCertificate(Student student) {
    char fileName[50];
    strcpy(fileName,student.number);
    strcat(fileName,".txt");
    FILE *file = fopen(fileName,"w");
    fprintf(file,"\t\t\t\t\tYUKSEKOGRETIM KURULU BASKANLIGI\n\t\t\t\t\t\tSTUDENT CERTIFACICATE\n");
    fprintf(file,"First Name: %s\nLast Name: %s\nStudent Number: %s\nMajor: %s\nCurrent Semester: %d", student.firstName, student.lastName, student.number, student.majorName ,student.studentInfo.semester);
    fclose(file);
}


// Creates student graduation certificate.
void createStudentGraduationCertificate(Student student) {
    char fileName[50];
    strcpy(fileName,student.number);
    strcat(fileName,"_graduation");
    strcat(fileName,".txt");
    FILE *file = fopen(fileName,"w");
    fprintf(file,"\t\t\t\t\tYUKSEKOGRETIM KURULU BASKANLIGI\n\t\t\t\t\t\tGRADUATION CERTIFACICATE\n\n");
    fprintf(file,"This certificate is presented to %s %s\nFor having successfully graduated at %s\n\n", student.firstName, student.lastName, student.majorName);
    fprintf(file,"Congratulations on a job well done!\n\nEray Solenkol (Chairman)");
    fclose(file);
}


void showGrades(char *tempStudentNumber) {
    char lectureName[50], instructorName[50];
    char studentFirstName[50], studentLastName[50], studentNumber[50];
    int midterm, finale, average;
    char fileName[50];
    FILE *file = fopen("lessons.txt","r");
    
    while (fscanf(file,"%s %s", lectureName, instructorName) != EOF) {
        strcpy(fileName, lectureName);
        strcat(fileName,".txt");
        FILE *lecture = fopen(fileName,"r");
        fscanf(lecture, "%s %s", lectureName, instructorName);
        while (fscanf(lecture, "%s %s %s %d %d %d", studentFirstName, studentLastName, studentNumber, &midterm, &finale, &average) != EOF) {
            if (strcmp(tempStudentNumber,studentNumber) == 0) {
                printf("*************************\n");
                printf("Lesson: %s\nMidterm: %d\nFinale: %d\nAverage: %d\n",lectureName, midterm, finale, average);
                printf("*************************\n\n");
            }
        }
        fclose(lecture);
    }
    fclose(file);
}