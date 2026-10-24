
#include <stdio.h>
#include <string.h>
#include "team_auth.h"

#define USER_SIZE 30
#define PASS_SIZE 100

int register_user(char username[], int size)
{
    char password[PASS_SIZE];
    char saved_user[USER_SIZE];
    char saved_pass[PASS_SIZE];
    
    printf("Enter new username: ");
    if (fgets(username, size, stdin) == NULL)  return 0;
    username[strcspn(username, "\n")] = '\0';

    FILE *file = fopen("team_users.txt", "r");

    if (file != NULL) {
        while (fscanf(file, "%29[^|]|%99[^\n]\n", saved_user, saved_pass) == 2) {
            if (strcmp(username, saved_user) == 0) {
                printf("Username already exists.\n");
                fclose(file);  return 0;
            }
        }
        fclose(file);
    }

    printf("Enter password: ");
    if (fgets(password, sizeof(password), stdin) == NULL) return 0;
    password[strcspn(password, "\n")] = '\0';

    file = fopen("team_users.txt", "a");

    if (file == NULL){
        printf("Could not save user.\n");
        return 0;
    }

    fprintf(file, "%s|%s\n", username, password);

    printf("Registration successful!\n");
    fclose(file);  return 1;
}

int login_user(char username[], int size)
{
    char password[PASS_SIZE];
    char saved_user[USER_SIZE];
    char saved_pass[PASS_SIZE];

    printf("Enter username: ");
    if (fgets(username, size, stdin) == NULL)  return 0;
    username[strcspn(username, "\n")] = '\0';

    printf("Enter password: ");
    if (fgets(password, sizeof(password), stdin) == NULL) return 0;
    password[strcspn(password, "\n")] = '\0';

    FILE *file = fopen("team_users.txt", "r");

    if (file == NULL){
        printf("No registered users found.\n");
        return 0;
    }

    while (fscanf(file, "%29[^|]|%99[^\n]\n",saved_user, saved_pass) == 2){
        if (strcmp(username, saved_user) == 0 && strcmp(password, saved_pass) == 0){
            printf("Login successful!\n");
            fclose(file); return 1;
        }
    }

    printf("Invalid username or password.\n");
    fclose(file);  return 0;
}
