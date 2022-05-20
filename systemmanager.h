#ifndef SYSTEMMANAGER_H_
#define SYSTEMMANAGER_H_

typedef struct {
    char *firstName;
    char *lastName;
    char *nickname;
    char *password;
} SystemManager;


int systemManagerLoginCheck(SystemManager systemManager,char *nickname, char *password);
SystemManager createSystemManagerAccount(char *firstName, char* lastName, char *nickname, char *password);
void systemManagerMenu(SystemManager systemManager);
void systemManagerLogin();
void mainMenu();
void changePasswordSystemManager(char *nickname, char *newPassword);
void start();


#endif /* SYSTEMMANAGER_H_ */