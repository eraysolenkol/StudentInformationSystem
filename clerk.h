#ifndef CLERK_H_
#define CLERK_H_

typedef struct {
    char *firstName;
    char *lastName;
    char *nickname;
    char *password;
} Clerk;

int clerkLoginCheck(Clerk clerk,char *nickname, char *password);
Clerk createClerkAccount(char *firstName, char* lastName, char *nickname, char *password);
void clerkMenu(Clerk clerk);
void clerkLogin();
int countTotalLines(char *filename);
void addLectureNameToDatabase(char lessonName[], char instructorNickname[]);
void showLectures();
int updateSemester(char *studentNumber);
int passTheLecture(char *lectureName, char *studentNumber);
void updateEcts(char *studentNumber, int choice);
void studentGraduation(char *studentNumber);
void changePasswordClerk(char *nickname, char *newPassword);
void deleteStudent(char *tempStudentNumber);
void deleteStudentLectures(char *tempStudentNumber);


#endif // CLERK_H_