#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "systemmanager.h"
#include "clerk.h"
#include "instructor.h"
#include "student.h"


// Returns 1 if login is correct.
int systemManagerLoginCheck(SystemManager systemManager,char *nickname, char *password) {
    return (strcmp(systemManager.password,password) == 0) && (strcmp(systemManager.nickname,nickname) == 0);
}


// Constructor for SystemManager Struct.
SystemManager createSystemManagerAccount(char *firstName, char* lastName, char *nickname, char *password) {
    SystemManager systemManager;
    systemManager.firstName = firstName;
    systemManager.lastName = lastName;
    systemManager.nickname = nickname;
    systemManager.password = password;
    return systemManager;
}


// systemManager Menu will work on successful systemManager login.
void systemManagerMenu(SystemManager systemManager) {
    int choice;
    while (1) {
        printf("\n0.Exit\n1.Create an instructor account\n2.Create a clerk account\n3.Change password\n");
        printf("Please select your process : ");
        scanf("%d",&choice);
        
        if (choice == 0) {
            printf("\nLogged out.\n\n");
            break;
        }
        
        else if (choice == 1) {
            int instructorCount = countTotalLines("instructors.txt") - 1;
            if (instructorCount >= 4) {
                printf("\nThere is already %d instructors. You can't add more.\n\n",instructorCount);
                continue;
            }

            char firstName[50], lastName[50], nickname[50], password[50];
            printf("Enter the first name of new instructor : ");
            scanf(" %s",firstName);
            printf("Enter the last name of new instructor : ");
            scanf(" %s",lastName);
            printf("Enter the nickname of new instructor : ");
            scanf(" %s",nickname);
            printf("Enter the password of new instructor : ");
            scanf(" %s",password);
            Instructor instructor = createInstructorAccount(firstName,lastName,nickname,password);
            FILE *file;
            file = fopen("instructors.txt","a+");
            fprintf(file,"%s %s %s %s\n",instructor.firstName,instructor.lastName,instructor.nickname,instructor.password);
            fclose(file);
        }
        else if (choice == 2) {
            int clerkCount = countTotalLines("clerks.txt") - 1;
            if (clerkCount >= 1) {
                printf("\nThere is already %d clerk. You can't add more.\n\n",clerkCount);
                continue;
            }

            char firstName[50], lastName[50], nickname[50], password[50];
            printf("Enter the first name of new clerk : ");
            scanf(" %s",firstName);
            printf("Enter the last name of new clerk : ");
            scanf(" %s",lastName);
            printf("Enter the nickname of new clerk : ");
            scanf(" %s",nickname);
            printf("Enter the password of new clerk : ");
            scanf(" %s",password);
            Clerk clerk = createClerkAccount(firstName,lastName,nickname,password);
            FILE *file;
            file = fopen("clerks.txt","a+");
            fprintf(file,"%s %s %s %s\n",clerk.firstName,clerk.lastName,clerk.nickname,clerk.password);
            fclose(file);
        } else if (choice == 3) {
            char oldPassword[50], newPassword[50];
            printf("Please enter your current password : ");
            scanf(" %s",oldPassword);
            if (strcmp(systemManager.password,oldPassword) == 0) {
                printf("\nPlease enter your new password : ");
                scanf(" %s",newPassword);
                changePasswordSystemManager(systemManager.nickname, newPassword);
                strcpy(systemManager.password,newPassword);
                printf("\n\nYour password succesfully changed!\n");
            } else {
                printf("\nYour password is wrong.\n");
            }
        } else {
            printf("\nInvalid choice\n");
        }
        
    }
}


// Authenticator for systemManager accounts.
void systemManagerLogin() {
    char nickname[50], password[50];
    printf("Please enter your nickname :");
    scanf(" %s",nickname);
    printf("Please enter your password :");
    scanf(" %s",password);
    char * lines[4];
    FILE *file;
    file = fopen("systemmanager.txt","r");
	for (int i =0; i < 4; i++) {
        lines[i] = malloc (50); 
        fscanf (file, "%49s", lines[i]);
    }
    fclose(file);
    SystemManager systemManager;
    systemManager = createSystemManagerAccount(lines[0],lines[1],lines[2],lines[3]);

    if (strcmp(lines[2],nickname) != 0) {
        printf("\nCouldn't find an account with %s name\n\n",nickname);
        return;
    }        
    if (systemManagerLoginCheck(systemManager,nickname,password) == 1) {
        printf("\nWelcome %s %s,\n",lines[0],lines[1]);
        systemManagerMenu(systemManager);
    } else {
        printf("\nWrong password. Please try again.\n\n");
    }
   
    for (int i =0; i < 4; i++) {
        free (lines[i]);
    }
}


// Changes password of system manager.
void changePasswordSystemManager(char *nickname, char *newPassword) {
    FILE *file = fopen("systemmanager.txt","r");
    FILE *tempfile = fopen("tempfile.txt","w");
    char systemManagerFirstName[50], systemManagerLastName[50], systemManagerNickname[50], systemManagerPassword[50];
    while (fscanf(file,"%s %s %s %s", systemManagerFirstName, systemManagerLastName, systemManagerNickname, systemManagerPassword) != EOF) {
        if (strcmp(nickname,systemManagerNickname) == 0) {
            strcpy(systemManagerPassword,newPassword);
        }
        fprintf(tempfile,"%s %s %s %s\n", systemManagerFirstName, systemManagerLastName, systemManagerNickname, systemManagerPassword);
    }
    fclose(file);
    fclose(tempfile);
    remove("systemmanager.txt");
    rename("tempfile.txt","systemmanager.txt");
}


// Main menu for main.c
void mainMenu() {
    int choice;
    while(1) {
        /* if there is no systemmanager file, create all necessary files and
        create a new system manager account */
        FILE *file;
        if ((file = fopen("systemmanager.txt","r"))!=NULL) {
            fclose(file);
        } else {
            printf("\nPlease create a system manager account\n\n");
            start();
        }

        printf("0.Exit\n1.System Manager Login\n2.Clerk Login\n3.Instructor Login\n4.Student Login\n");
        printf("Please select one : ");
        scanf("%d",&choice);
        if (choice == 0) {
            printf("Program has succesfully ended.\n");
            break;
        } else if (choice == 1) {
            systemManagerLogin();
        } else if (choice == 2) {
            clerkLogin();
        } else if (choice == 3) {
            instructorLogin();
        } else if (choice == 4) {
            studentLogin();
        } else {
            printf("\nInvalid choice\n\n");
        }
    }
}


// Starter function for new systems
void start() {
    char firstName[50], lastName[50], nickname[50], password[50];

    printf("Enter the first name of new system manager : ");
    scanf(" %s",firstName);
    printf("Enter the last name of new system manager : ");
    scanf(" %s",lastName);
    printf("Enter the nickname of new system manager : ");
    scanf(" %s",nickname);
    printf("Enter the password of new system manager : ");
    scanf(" %s",password);
    SystemManager systemManager = createSystemManagerAccount(firstName,lastName,nickname,password);
    FILE *file = fopen("systemmanager.txt","w");
    fprintf(file, "%s %s %s %s\n", systemManager.firstName, systemManager.lastName, systemManager.nickname, systemManager.password);
    fclose(file);

    FILE *students = fopen("students.txt","w");
    fclose(students);
    FILE *instructors = fopen("instructors.txt","w");
    fclose(instructors);
    FILE *clerks = fopen("clerks.txt","w");
    fclose(clerks);
    FILE *lessons = fopen("lessons.txt","w");
    fclose(lessons);
} 