#ifndef INSTRUCTOR_H_
#define INSTRUCTOR_H_

typedef struct {
    char *firstName;
    char *lastName;
    char *nickname;
    char *password;
} Instructor;  

int instructorLoginCheck(Instructor instructor,char *nickname, char *password);
Instructor createInstructorAccount(char *firstName, char* lastName, char *nickname, char *password);
void instructorMenu(Instructor instructor);
void instructorLogin();
int checkInstructor(char *nickname);
Instructor getInstructorByNickname(char *nickname);
void createLecture(char *lectureName, Instructor instructor);
int calculateSuccess(const int midterm, const int finale);
int updateGradesOfStudent(char number[], char lessonName[], int midterm, int finale);
int checkInstructorOfLecture(char *lessonName, Instructor instructor);
void changePasswordInstructor(char *nickname, char *newPassword);

#endif  // INSTRUCTOR_H_