#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "clerk.h"
#include "student.h"
#include "instructor.h"

#define SENG "Software Engineering"
#define CENG "Computer Engineering"
#define MECH "Mechanical Engineering"


// Returns 1 if login is correct.
int clerkLoginCheck(Clerk clerk,char *nickname, char *password) {
    return (strcmp(clerk.password,password) == 0) && (strcmp(clerk.nickname,nickname) == 0);
}


// Conctructor for Clerk Struct.
Clerk createClerkAccount(char *firstName, char* lastName, char *nickname, char *password) {
    Clerk clerk;
    clerk.firstName = firstName;
    clerk.lastName = lastName;
    clerk.nickname = nickname;
    clerk.password = password;
    return clerk;
}


// Clerk Menu will work on successful clerk login.
void clerkMenu(Clerk clerk) {
    int choice;
    while (1) {
        printf("\n0.Exit\n1.Create a student account\n2.Create a new lecture\n3.Update semester of a student\n4.Pass a lecture of a student\n5.Graduate a student\n6.Change password\n7.Delete a student account\n");
        printf("Please select your process : ");
        scanf("%d",&choice);
        
        if (choice == 0) {
            printf("\nLogged out.\n\n");
            break;
        }
        
        else if (choice == 1) {
            int studentCount = countTotalLines("students.txt") - 1;
            if (studentCount >= 100) {
                printf("\nThere is already %d students. You can't add more.\n\n",studentCount);
                continue;
            }

            char firstName[50], lastName[50], number[50], password[50] , majorName[50];
            int choice;
            printf("Enter the first name of new student : ");
            scanf(" %s",firstName);
            printf("Enter the last name of new student : ");
            scanf(" %s",lastName);
            printf("Enter the number of new student : ");
            scanf(" %s",number);
            printf("Enter the password of new student : ");
            scanf(" %s",password);
            printf("Enter the major of new student\n1.Software Engineering\n2.Computer Engineering\n3.Mechanical Engineering\n :  ");
            scanf("%d",&choice);
            if (choice == 1) {
                strcpy(majorName,SENG);
            } else if (choice == 2) {
                strcpy(majorName,CENG);
            } else if (choice == 3) {
                strcpy(majorName,MECH);
            } else {
                printf("\nYou entered invalid input. Student will not be saved.\n");
                continue;
            }
            Student tempStudent = getStudentByNumber(number);
            if (strcmp(tempStudent.firstName,"0") != 0) {
                printf("\nThere is a student with %s number already! Student will not be saved.\n\n");
                continue;
            }
            Student student = createStudentAccount(firstName,lastName,number,password,majorName,1,0,0,0,0);
            FILE *file;
            file = fopen("students.txt","a+");
            fprintf(file,"%s %s %s %s %s %d %d %d %d %d\n",student.firstName,student.lastName,student.number,student.password,student.majorName, student.studentInfo.semester, student.studentInfo.totalEcts, student.studentInfo.semesterEcts, student.studentInfo.currentEcts, student.studentInfo.internship);
            fclose(file);
            printf("\nStudent added succesfully!\n\n");
        } else if (choice == 2) {
            char lectureName[50] , instructorNickname[50];
            printf("Please enter the lecture name: ");
            scanf(" %s",lectureName);
            printf("Please enter the instructor nickname: ");
            scanf(" %s",instructorNickname);
            if (checkInstructor(instructorNickname) == 1) {
                Instructor instructor = getInstructorByNickname(instructorNickname);
                addLectureNameToDatabase(lectureName,instructor.nickname);
                createLecture(lectureName,instructor);
            } else {
                printf("\nCouldn't find a instructor with %s nickname.\n",instructorNickname);
            }

        } else if (choice == 3) {
            char studentNumber[50];
            printf("Please enter the number of student : ");
            scanf(" %s",studentNumber);
            int result = updateSemester(studentNumber);
            if (result == 1) {
                printf("\nSemester of student with number %s has updated!\n",studentNumber);
            } else if (result == 2) {
                char kick;
                printf("\nSemester of student with number %s is already in 6. semester.\n",studentNumber);
                printf("Do you want to kick the student from the university? [y/n] :");
                scanf(" %c", &kick);
                if (kick == 'Y' || kick == 'y') {
                    deleteStudent(studentNumber);
                    printf("\nStudent deleted!\n\n");
                } else {
                    printf("\nCancelled!\n\n");
                }
            } else if (result == 0) {
                printf("\nCouldn't find a student with number %s\n",studentNumber);
            }
        } else if (choice == 4) {
            char studentNumber[50], lectureName[50];
            printf("Please enter the name of lecture : ");
            scanf(" %s",lectureName);
            printf("Please enter the number of student : ");
            scanf(" %s",studentNumber);
            int result = passTheLecture(lectureName, studentNumber);
            if (result == 1) {
                updateEcts(studentNumber, 1);
            }
        } else if (choice == 5) {
            char studentNumber[50];
            printf("Please enter the number of student : ");
            scanf(" %s",studentNumber);
            studentGraduation(studentNumber);
        } else if (choice == 6) {
            char oldPassword[50], newPassword[50];
            printf("Please enter your current password : ");
            scanf(" %s",oldPassword);
            if (strcmp(clerk.password,oldPassword) == 0) {
                printf("\nPlease enter your new password : ");
                scanf(" %s",newPassword);
                changePasswordClerk(clerk.nickname, newPassword);
                strcpy(clerk.password,newPassword);
                printf("\n\nYour password succesfully changed!\n");
            } else {
                printf("\nYour password is wrong.\n");
            }
        } else if (choice == 7) {
            char studentNumber[50];
            char choice;
            printf("Please enter the student number : ");
            scanf(" %s",studentNumber);
            printf("You will delete the student with number %s\n",studentNumber);
            printf("Do you want to continue? [y/n]");
            scanf(" %c",&choice);
            if (choice == 'Y' || choice == 'y') {
                deleteStudent(studentNumber);
                printf("\n%s successfully deleted!\n\n",studentNumber);
            } else {
                printf("\nCancelled!\n\n");
            }

        } else {
            printf("\nInvalid choice\n");
        }
        
    }
}


// Authenticator for clerk accounts.
void clerkLogin() {
    char nickname[50], password[50];
    printf("Please enter your nickname :");
    scanf(" %s",nickname);
    printf("Please enter your password :");
    scanf(" %s",password);
    char * lines[4];
    FILE *file;
    file = fopen("clerks.txt","r");
	for (int i =0; i < 4; i++) {
        lines[i] = malloc (50); 
        fscanf (file, "%49s", lines[i]);
    }
    fclose(file);
    Clerk clerk;
    clerk = createClerkAccount(lines[0],lines[1],lines[2],lines[3]);        
    if (clerkLoginCheck(clerk,nickname,password) == 1) {
        printf("\nWelcome %s %s,\n",lines[0],lines[1]);
        clerkMenu(clerk);
    } else {
        printf("\nWrong nickname or password. Please try again.\n\n");
    }
   
    for (int i =0; i < 4; i++) {
        free (lines[i]);
    }
}


// Counts total line count in a file.
int countTotalLines(char *filename) {
    FILE* file = fopen(filename, "r");
    int c, lineCount = 0;
    do {
        c = fgetc(file);
        if(c == '\n') {
            lineCount++;
        }       
    } while (c != EOF);
    if(c != '\n' && lineCount != 0) {
        lineCount++;
    }
    fclose(file);
    return lineCount;
}


// Adds lesson to lessons database file.
void addLectureNameToDatabase(char lessonName[], char instructorNickname[]) {
    FILE* file = fopen("lessons.txt", "a+"); 
    fprintf(file,"%s %s\n",lessonName, instructorNickname);
    fclose(file);
}


// Shows all lectures in the university.
void showLectures() {
    char lectureName[50], instructorName[50];
    FILE* file = fopen("lessons.txt", "r");

    /* Lecture count will be 1 more missing of total lines because when we add a new lecture
    we print a newline for the next lecture. */
    int lectureCount = countTotalLines("lessons.txt") - 1;
    /* if the file is empty, there will be no newline and count lines -1 will be equal to -1.
    so we make the lecture count 0*/
    if (lectureCount == -1) {
        lectureCount = 0;
    }
    printf("\nThere is %d lessons you can take:\n",lectureCount);
    while (fscanf(file, "%s %s", lectureName, instructorName) != EOF) {
        printf("%s (%s)\n", lectureName, instructorName);
    }
    fclose(file);
}


// Updates semester of a student.
int updateSemester(char *studentNumber) {
    Student student = getStudentByNumber(studentNumber);
    if (strcmp(student.firstName,"0") == 0) {
        return 0;
    }
    int flag = 0;
    char majorNameFirst[50], majorNameSecond[50];
    FILE *file = fopen("students.txt","r");
    FILE *tempfile = fopen("tempfile.txt","w");
    while (fscanf(file, "%s %s %s %s %s %s %d %d %d %d %d", student.firstName,student.lastName,student.number,student.password, majorNameFirst ,majorNameSecond, &student.studentInfo.semester, &student.studentInfo.totalEcts, &student.studentInfo.semesterEcts, &student.studentInfo.currentEcts, &student.studentInfo.internship) != EOF) {
        if ((strcmp(studentNumber,student.number) == 0)) {
            if (student.studentInfo.semester < 6) {
                if (student.studentInfo.currentEcts != 0) {
                    student.studentInfo.semesterEcts = 0;
                }
                student.studentInfo.semester++;
                student.studentInfo.currentEcts = 0;
                flag = 1;
            } else {
                flag = 2;
            }
        }
        fprintf(tempfile,"%s %s %s %s %s %s %d %d %d %d %d\n", student.firstName,student.lastName,student.number,student.password, majorNameFirst, majorNameSecond, student.studentInfo.semester, student.studentInfo.totalEcts, student.studentInfo.semesterEcts, student.studentInfo.currentEcts, student.studentInfo.internship);
    }
    fclose(file);
    fclose(tempfile);
    remove("students.txt");
    rename("tempfile.txt","students.txt");
    
    return flag;
}


// Passes a lecture of student.
int passTheLecture(char *lectureName, char *studentNumber) {
    int flag = 0;
    Student student = getStudentByNumber(studentNumber);
    char fileName[50], instructorName[50], tempLectureName[50];
    int midterm, finale, average;
    strcpy(fileName,lectureName);
    strcat(fileName,".txt");
    FILE *file = fopen(fileName,"r");
    FILE *tempfile = fopen("tempfile.txt","w");
    fscanf(file,"%s %s",tempLectureName,instructorName);
    fprintf(tempfile,"%s %s\n",tempLectureName,instructorName);

    while (fscanf(file,"%s %s %s %d %d %d", student.firstName, student.lastName, student.number, &midterm, &finale, &average) != EOF) {
            if ((strcmp(studentNumber,student.number) == 0)) {
                if (average >= 50) {
                    printf("\n%s is passed %s!\n", studentNumber, lectureName);
                    flag = 1;
                } else {
                    printf("\n%s 's average is %d and it is not enough to pass %s!\n", studentNumber, average, lectureName);
                    fprintf(tempfile,"%s %s %s %d %d %d\n", student.firstName, student.lastName, student.number, midterm, finale, average);
                }
            } else {
                fprintf(tempfile,"%s %s %s %d %d %d\n", student.firstName, student.lastName, student.number, midterm, finale, average);
            }
    }
    fclose(file);
    fclose(tempfile);
    remove(fileName);
    rename("tempfile.txt",fileName);

    return flag;
}


// Updates ects score of student.
void updateEcts(char *studentNumber ,int choice) {
    FILE *file = fopen("students.txt","r");
    FILE *tempfile = fopen("tempfile.txt","w");
    Student student = getStudentByNumber(studentNumber);
    char majorNameFirst[50], majorNameSecond[50];
    while (fscanf(file,"%s %s %s %s %s %s %d %d %d %d %d", student.firstName,student.lastName,student.number,student.password, majorNameFirst, majorNameSecond, &student.studentInfo.semester, &student.studentInfo.totalEcts, &student.studentInfo.semesterEcts, &student.studentInfo.currentEcts, &student.studentInfo.internship) != EOF) {
        if (strcmp(studentNumber,student.number) == 0) {
            if (choice == 1) {
                student.studentInfo.currentEcts +=5;
                student.studentInfo.totalEcts += 5;
            } else if (choice == 2) {
                student.studentInfo.semesterEcts +=5;
            }
        }
        fprintf(tempfile,"%s %s %s %s %s %s %d %d %d %d %d\n", student.firstName,student.lastName,student.number,student.password, majorNameFirst, majorNameSecond, student.studentInfo.semester, student.studentInfo.totalEcts, student.studentInfo.semesterEcts, student.studentInfo.currentEcts, student.studentInfo.internship);
    }
    fclose(file);
    fclose(tempfile);
    remove("students.txt");
    rename("tempfile.txt","students.txt");
}


// Graduates the student.
void studentGraduation(char *studentNumber) {
    Student student = getStudentByNumber(studentNumber);
    if (strcmp(student.firstName,"0") == 0 ) {
        printf("\nStudent not found!\n");
        return;
    }

    FILE *file = fopen("students.txt","r");
    FILE *tempfile = fopen("tempfile.txt","w");
    char majorNameFirst[50], majorNameSecond[50];
    int flag = 0;
    while (fscanf(file,"%s %s %s %s %s %s %d %d %d %d %d", student.firstName,student.lastName,student.number,student.password, majorNameFirst, majorNameSecond, &student.studentInfo.semester, &student.studentInfo.totalEcts, &student.studentInfo.semesterEcts, &student.studentInfo.currentEcts, &student.studentInfo.internship) != EOF) {
        if (strcmp(studentNumber,student.number) == 0) {
            if (student.studentInfo.internship == 1) {
                printf("\n%s has graduated!\n",student.number);
                createStudentGraduationCertificate(student);
                printf("\nHere is your graduation certificate!\n");
                flag = 1;
            } else if (student.studentInfo.totalEcts < 20) {
                printf("\n%s can't graduate due to ects(%d) not enough.\n",student.number,student.studentInfo.totalEcts);
                flag = 0;
            } else {
                printf("\n%s can't graduate due to intership not done.\n",student.number);
                flag = 0;
            }       
        } else {
            flag = 0;
        }
        if (flag == 0) {
            fprintf(tempfile,"%s %s %s %s %s %s %d %d %d %d %d\n", student.firstName,student.lastName,student.number,student.password, majorNameFirst, majorNameSecond, student.studentInfo.semester, student.studentInfo.totalEcts, student.studentInfo.semesterEcts, student.studentInfo.currentEcts, student.studentInfo.internship);
        }
    }
    fclose(file);
    fclose(tempfile);
    remove("students.txt");
    rename("tempfile.txt","students.txt");
}


// Changes password of clerk.
void changePasswordClerk(char *nickname, char *newPassword) {
    FILE *file = fopen("clerks.txt","r");
    FILE *tempfile = fopen("tempfile.txt","w");
    char clerkFirstName[50], clerkLastName[50], clerkNickname[50], clerkPassword[50];
    while (fscanf(file,"%s %s %s %s", clerkFirstName, clerkLastName, clerkNickname, clerkPassword) != EOF) {
        if (strcmp(nickname,clerkNickname) == 0) {
            strcpy(clerkPassword,newPassword);
        }
        fprintf(tempfile,"%s %s %s %s\n", clerkFirstName, clerkLastName, clerkNickname, clerkPassword);
    }
    fclose(file);
    fclose(tempfile);
    remove("clerks.txt");
    rename("tempfile.txt","clerks.txt");
}


// Deletes student from database file.
void deleteStudent(char *studentNumber) {
    FILE *file = fopen("students.txt","r");
    FILE *tempfile = fopen("tempfile.txt","w");
    Student student = getStudentByNumber(studentNumber);
    char majorNameFirst[50], majorNameSecond[50];
    while (fscanf(file,"%s %s %s %s %s %s %d %d %d %d %d", student.firstName,student.lastName,student.number,student.password, majorNameFirst, majorNameSecond, &student.studentInfo.semester, &student.studentInfo.totalEcts, &student.studentInfo.semesterEcts, &student.studentInfo.currentEcts, &student.studentInfo.internship) != EOF) {
        if (strcmp(student.number, studentNumber) == 0) {
            printf("\nStudent deleted!\n\n");
        } else {
            fprintf(tempfile,"%s %s %s %s %s %s %d %d %d %d %d\n", student.firstName,student.lastName,student.number,student.password, majorNameFirst, majorNameSecond, student.studentInfo.semester, student.studentInfo.totalEcts, student.studentInfo.semesterEcts, student.studentInfo.currentEcts, student.studentInfo.internship);
        }
    }
    fclose(file);
    fclose(tempfile);
    remove("students.txt");
    rename("tempfile.txt","students.txt");
    deleteStudentLectures(studentNumber);
}


void deleteStudentLectures(char *tempStudentNumber) {
    char lectureName[50], instructorName[50];
    char studentFirstName[50], studentLastName[50], studentNumber[50];
    int midterm, finale, average;
    char fileName[50];
    FILE *file = fopen("lessons.txt","r");
    
    while (fscanf(file,"%s %s", lectureName, instructorName) != EOF) {
        strcpy(fileName, lectureName);
        strcat(fileName,".txt");
        FILE *lecture = fopen(fileName,"r");
        FILE *temp = fopen("tempfile.txt","w");
        fscanf(lecture, "%s %s", lectureName, instructorName);
        fprintf(temp, "%s %s\n", lectureName, instructorName);
        while (fscanf(lecture, "%s %s %s %d %d %d", studentFirstName, studentLastName, studentNumber, &midterm, &finale, &average) != EOF) {
            if (strcmp(tempStudentNumber,studentNumber) != 0) {
                fprintf(temp, "%s %s %s %d %d %d\n", studentFirstName, studentLastName, studentNumber, midterm, finale, average);
            }
        }
        fclose(lecture);
        fclose(temp);
        remove(fileName);
        rename("tempfile.txt",fileName);
    }
    fclose(file);
}