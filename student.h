#ifndef STUDENT_H_
#define STUDENT_H_

typedef struct {
    int semester;
    int totalEcts;
    int semesterEcts;
    int currentEcts;
    int internship;
} StudentInfo;

typedef struct {
    char *firstName;
    char *lastName;
    char *number;
    char *password;
    char *majorName;
    StudentInfo studentInfo;
} Student;

int studentLoginCheck(Student student,char *number, char *password);
Student createStudentAccount(char *firstName, char *lastName, char *number, char *password, char *majorName, int semester, int totalEcts, int semesterEcts, int currentEcts, int internship);
void studentMenu(Student student);
void studentLogin();
int checkStudentByNumber(char *nickname);
int addNewLectureToSchedule(Student student, char *lectureName);
Student getStudentByNumber(char *number);
int checkHaveLectureAlready(Student student, char *lectureName);
void changePasswordStudent(char *studentNumber, char *newPassword);
void createStudentCertificate(Student student);
void createStudentGraduationCertificate(Student student);
void showGrades(char *tempStudentNumber);

#endif //  STUDENT_H_