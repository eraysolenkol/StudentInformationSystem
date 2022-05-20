#include <stdio.h> 
#include <string.h>
#include <stdlib.h>

#include "instructor.h"


// Returns 1 if login is correct.
int instructorLoginCheck(Instructor instructor,char *nickname, char *password) {
    return (strcmp(instructor.password,password) == 0) && (strcmp(instructor.nickname,nickname) == 0);
}


// Constructor for Instructor Struct.
Instructor createInstructorAccount(char *firstName, char* lastName, char *nickname, char *password) {
    Instructor instructor;
    instructor.firstName = firstName;
    instructor.lastName = lastName;
    instructor.nickname = nickname;
    instructor.password = password;
    return instructor;
}


// Instructor Menu will work on successful instructor login.
void instructorMenu(Instructor instructor) {
    int choice;
    while (1) {
        printf("\n0.Exit\n1.Entry grades of a student\n2.Change password\n");
        printf("Please select your process : ");
        scanf("%d",&choice);
        
        if (choice == 0) {
            printf("\nLogged out.\n\n");
            break;
        } else if (choice == 1) {
            char lectureName[50], studentNumber[50];
            int midterm, finale;
            printf("Enter the lecture name : ");
            scanf(" %s",lectureName);
            if (checkInstructorOfLecture(lectureName, instructor) == 0) {
                printf("\nYou are not allowed change the grades of students in this lesson!\n");
                continue;
            } else if (checkInstructorOfLecture(lectureName, instructor) == 2) {
                printf("\nCouldn't find a lesson with %s name\n",lectureName);
                continue;
            }
            printf("Enter the number of student : ");
            scanf(" %s",studentNumber);
            printf("Enter the midterm grade : ");
            scanf("%d",&midterm);
            printf("Enter the finale grade : ");
            scanf("%d",&finale);
            int update = updateGradesOfStudent(studentNumber,lectureName,midterm,finale);
            if (update == 0) {
                printf("\nNo student has found with number %s\n",studentNumber);
            } else {
                printf("\nGrades of student with %s number has updated!\n",studentNumber);
            }
        } else if (choice == 2) {
            char oldPassword[50], newPassword[50];
            printf("Please enter your current password : ");
            scanf(" %s",oldPassword);
            if (strcmp(instructor.password,oldPassword) == 0) {
                printf("\nPlease enter your new password : ");
                scanf(" %s",newPassword);
                changePasswordInstructor(instructor.nickname, newPassword);
                strcpy(instructor.password,newPassword);
                printf("\n\nYour password succesfully changed!\n");
            } else {
                printf("\nYour password is wrong.\n");
            }
        } else {
            printf("\nInvalid choice\n");
        }
        
    }
}


// Authenticator for instructor accounts.
void instructorLogin() {
    char nickname[50], password[50];
    printf("Please enter your nickname :");
    scanf(" %s",nickname);
    printf("Please enter your password :");
    scanf(" %s",password);
    char * lines[16];
    FILE *file;
    file = fopen("instructors.txt","r");
	for (int i =0; i < 16; i++) {
        lines[i] = malloc (50); 
        fscanf (file, "%49s", lines[i]);
    }
    fclose(file);
    Instructor instructor;

    int select = -1;
    for (int i = 0; i < 16; i+=4) {
        if (strcmp(nickname,lines[i+2]) == 0) {
            select = i;
            break;
        }
    }
    if (select == -1) {
        printf("\nCouldn't find an account with %s name\n\n",nickname);
        return;
    }
    instructor = createInstructorAccount(lines[select],lines[select+1],lines[select+2],lines[select+3]);    
    if (instructorLoginCheck(instructor,nickname,password) == 1) {
        printf("\nWelcome %s %s,\n",lines[select],lines[select+1]);
        instructorMenu(instructor);
    } else {
        printf("\nWrong password. Please try again.\n\n");
    }
   
    for (int i =0; i < 16; i++) {
        free (lines[i]);
    }
}


// Returns 1 if there is a instructor with that nickname.
int checkInstructor(char *nickname) {
    char * lines[16];
    FILE *file;
    file = fopen("instructors.txt","r");
	for (int i =0; i < 16; i++) {
        lines[i] = malloc (50); 
        fscanf (file, "%49s", lines[i]);
    }
    fclose(file);

    int select = -1;
    for (int i = 0; i < 16; i+=4) {
        if (strcmp(nickname,lines[i+2]) == 0) {
            return 1;
        }
    }
    return 0;
}


// Returns instructor by nickname.
Instructor getInstructorByNickname(char *nickname) {
    Instructor instructor;
    char * lines[16];
    FILE *file;
    file = fopen("instructors.txt","r");
	for (int i =0; i < 16; i++) {
        lines[i] = malloc (50); 
        fscanf (file, "%49s", lines[i]);
    }
    fclose(file);

    int select = -1;
    for (int i = 0; i < 16; i+=4) {
        if (strcmp(nickname,lines[i+2]) == 0) {
            select = i;
            break;
        }
    }
    instructor = createInstructorAccount(lines[select],lines[select+1],lines[select+2],lines[select+3]);
    
    return instructor;
}


// Creates new lecture for an instructor
void createLecture(char *lectureName, Instructor instructor) {
    FILE *file;
    char holdLectureName[50];
    strcpy(holdLectureName,lectureName);
    strcat(lectureName,".txt");
    file = fopen(lectureName, "w");
    fprintf(file,"%s %s\n",holdLectureName,instructor.nickname);
    fclose(file);
    printf("\nCreated a new lecture : %s\n",holdLectureName);
    printf("%s %s will be with us in %s lessons.\n\n",instructor.firstName, instructor.lastName, holdLectureName);
}


// Returns average of the student.
int calculateSuccess(const int midterm, const int finale) {
    return ((midterm*4 + finale*6) / 10);
}


// Updates grades of the student.
int updateGradesOfStudent(char number[], char lessonName[], int midterm, int finale) {
    char lessonFile[50], studentFirstName[50], studentLastName[50], studentNumber[50], instructorNickname[50];
    int average ,tempMidterm ,tempFinale;
    int flag = 0;
    strcpy(lessonFile,lessonName);
    strcat(lessonFile,".txt");
    FILE *file = fopen(lessonFile,"r");
    FILE *tempfile = fopen("tempfile.txt","a+");
    
    fscanf(file,"%s %s",lessonName, instructorNickname);
    fprintf(tempfile,"%s %s\n",lessonName,instructorNickname);
    while (fscanf(file, "%s %s %s %d %d %d", studentFirstName, studentLastName, studentNumber, &tempMidterm, &tempFinale, &average) != EOF) {
        if (strcmp(number,studentNumber) == 0) {
            average = calculateSuccess(midterm,finale);
            tempMidterm = midterm;
            tempFinale = finale;
            flag = 1;
        }
        fprintf(tempfile,"%s %s %s %d %d %d\n", studentFirstName, studentLastName, studentNumber, tempMidterm, tempFinale, average);
    }
    fclose(file);
    fclose(tempfile);
    remove(lessonFile);
    rename("tempfile.txt",lessonFile);
    
    return flag;
}


// Return 1 if Instructor have this lesson else 0.
int checkInstructorOfLecture(char *lessonName, Instructor instructor) {
    FILE *file = fopen("lessons.txt","r");
    char fileLessonName[50], fileIntructorNickname[50];
    int flag = 2;
    while (fscanf(file, "%s %s", fileLessonName, fileIntructorNickname) != EOF) {
        if ((strcmp(instructor.nickname,fileIntructorNickname) == 0) && (strcmp(lessonName, fileLessonName) == 0)) {
            return 1;
        }
        if ((strcmp(lessonName, fileLessonName) == 0)) {
            flag = 0;
        }
    }
    return flag;
}


// Changes password of instructor.
void changePasswordInstructor(char *nickname, char *newPassword) {
    FILE *file = fopen("instructors.txt","r");
    FILE *tempfile = fopen("tempfile.txt","w");
    Instructor instructor = getInstructorByNickname(nickname);
    while (fscanf(file,"%s %s %s %s", instructor.firstName, instructor.lastName, instructor.nickname, instructor.password) != EOF) {
        if (strcmp(nickname,instructor.nickname) == 0) {
            strcpy(instructor.password,newPassword);
        }
        fprintf(tempfile,"%s %s %s %s\n", instructor.firstName, instructor.lastName, instructor.nickname, instructor.password);
    }
    fclose(file);
    fclose(tempfile);
    remove("instructors.txt");
    rename("tempfile.txt","instructors.txt");
}