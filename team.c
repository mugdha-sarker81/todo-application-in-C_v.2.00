#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "team.h"

void clearScreen(void);

#define MAX_PROJECTS 15
#define MAX_COMMITS 500
#define NAME_SIZE 50
#define USER_SIZE 30
#define MESSAGE_SIZE 100

typedef struct {
    char id[5];
    char name[NAME_SIZE];
    char members[150];
} Project;

typedef struct {
    char project_id[5];
    char message[MESSAGE_SIZE];
    char user[USER_SIZE];
    char date[15];
    char time[10];
} Commit;

Project projects[MAX_PROJECTS];
Commit commits[MAX_COMMITS];
int project_count = 0, commit_count = 0;

void ClearInputBuffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void load_projects(void) {
    FILE *file = fopen("team_project_name.txt", "r");
    char line[250];
    if (file == NULL) {
        printf("Could not open team_project_name.txt\n");
        return;
    }
    while (project_count < MAX_PROJECTS && fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = 0;
        if (sscanf(line, "%4[^|]|%49[^|]|%149[^\n]", projects[project_count].id,projects[project_count].name,
                                         projects[project_count].members) == 3)
            project_count++;
        else
            printf("Skipping invalid project record.\n");
    }
    fclose(file);
}

void load_commits(void) {
    FILE *file = fopen("commit_msg.txt", "r");
    char line[350];
    if (file == NULL) return;
    while (commit_count < MAX_COMMITS && fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = 0;
        if (sscanf(line, "%4[^|]|%99[^|]|%29[^|]|%9[^|]|%14[^\n]",
                   commits[commit_count].project_id, commits[commit_count].message,
                   commits[commit_count].user, commits[commit_count].time,
                   commits[commit_count].date) == 5)
            commit_count++;
        else
            printf("Skipping invalid commit record.\n");
    }
    fclose(file);
}

int is_member(char members[], char username[]) {
    char member_list[150], *member;
    strcpy(member_list, members);
    for (member = strtok(member_list, ","); member != NULL; member = strtok(NULL, ","))
        if (strcmp(member, username) == 0) return 1;
    return 0;
}

void show_projects(char username[], int available[], int *available_count) {
    clearScreen();
    *available_count = 0;
    printf("\nProjects you are working on:\n");

    for (int i = 0; i < project_count; i++){
        if (is_member(projects[i].members, username)) {
            available[(*available_count)++] = i;
            printf("%d. %s\n", *available_count, projects[i].name);
        }
    }
    if (*available_count == 0) printf("No project assigned to this username.\n");
}

void save_projects(void) {
    FILE *file = fopen("team_project_name.txt", "w");
    if (file == NULL) {
        printf("Could not save project.\n");
        return;
    }
    for (int i = 0; i < project_count; i++)
        fprintf(file, "%s|%s|%s\n", projects[i].id, projects[i].name, projects[i].members);
    fclose(file);
}

int create_project(char username[]) {
    Project *p = &projects[project_count];

    if (project_count >= MAX_PROJECTS) {
        printf("Project list is full.\n");
        return -2;
    }
    printf("Enter new project name: ");
    if (fgets(p->name, NAME_SIZE, stdin) == NULL) return -1;
    p->name[strcspn(p->name, "\n")] = 0;

    if (p->name[0] == '\0' || strchr(p->name, '|') != NULL) {
        printf("Name cannot be empty or contain '|'.\n");
        return -2;
    }
    
    strcpy(p->members, username);  project_count++;
    save_projects();
    printf("Project created!\n");
    return project_count - 1;
}

int choose_project(char username[]) {
    int available[MAX_PROJECTS], count, choice;
    
    show_projects(username, available, &count);
    printf("0. Create new project\nChoose a project: ");
    if (scanf("%d", &choice) != 1) return -1;
    ClearInputBuffer();
    if (choice == 0) return create_project(username);
    if (choice < 1 || choice > count) {
        printf("Invalid project number.\n");
        return -1;
    }
    return available[choice - 1];
}

void save_commits(void) {
    FILE *file = fopen("commit_msg.txt", "w");
    if (file == NULL) {
        printf("Could not save commit history.\n");
        return;
    }
    for (int i = 0; i < commit_count; i++)
        fprintf(file, "%s|%s|%s|%s|%s\n", commits[i].project_id,
                commits[i].message, commits[i].user, commits[i].time, commits[i].date);
    fclose(file);
}

void show_history(int project_index) {
    clearScreen();
    int number = 0;
    printf("\nCommit history for %s:\n", projects[project_index].name);
    for (int i = 0; i < commit_count; i++)
        if (strcmp(commits[i].project_id, projects[project_index].id) == 0)
            printf("%d. %s\n   By: %s   Date: %s  Time: %s\n",
                   ++number, commits[i].message, commits[i].user,
                   commits[i].date, commits[i].time);
    if (number == 0) printf("No progress messages yet.\n");
}

void add_commit(int project_index, char username[]) {
    time_t now = time(NULL);
    Commit new_commit;
    if (commit_count >= MAX_COMMITS) {
        printf("Commit history is full.\n");
        return;
    }
    printf("Enter your new progress message: ");
    if (fgets(new_commit.message, MESSAGE_SIZE, stdin) == NULL) return;
    new_commit.message[strcspn(new_commit.message, "\n")] = 0;

    if (new_commit.message[0] == '\0' || strchr(new_commit.message, '|') != NULL) {
        printf("Message cannot be empty or contain '|'.\n");
        return;
    }
    strcpy(new_commit.project_id, projects[project_index].id);
    strcpy(new_commit.user, username);

    strftime(new_commit.date, sizeof(new_commit.date), "%d-%m-%Y", localtime(&now));
    strftime(new_commit.time, sizeof(new_commit.time), "%H:%M", localtime(&now));

    commits[commit_count++] = new_commit;
    save_commits();
    printf("\nProgress message saved!\n");
}

void delete_commit(int project_index) {
    int number = 0, selected, actual_index[MAX_COMMITS];
    for (int i = 0; i < commit_count; i++)
        if (strcmp(commits[i].project_id, projects[project_index].id) == 0) {
            actual_index[number++] = i;
            printf("%d. %s | %s | %s %s\n", number, commits[i].message,
                   commits[i].user, commits[i].date, commits[i].time);
        }
    if (number == 0) {
        printf("No messages to delete.\n");
        return;
    }
    printf("Enter message number to delete: ");
    if (scanf("%d", &selected) != 1) return;
    ClearInputBuffer();
    if (selected < 1 || selected > number) {
        printf("Invalid message number.\n");
        return;
    }
    selected = actual_index[selected - 1];

    for (int i = selected; i < commit_count - 1; i++)
        commits[i] = commits[i + 1];

    commit_count--;
    save_commits();
    printf("Message deleted.\n");
}

void add_collaborator(int project_index) {
    char name[USER_SIZE];
    char line[100];
    Project *p = &projects[project_index];
    printf("Enter collaborator username to add: ");
    if (fgets(line, sizeof(line), stdin) == NULL) return;
    if (strchr(line, '\n') == NULL) ClearInputBuffer();
    line[strcspn(line, "\n")] = 0;
    if (line[0] == '\0' || strlen(line) >= USER_SIZE) {
        printf("Invalid name. Try again.\n");
        return;
    }
    strcpy(name, line);
    if (is_member(p->members, name)) {
        printf("%s is already a collaborator.\n", name);
        return;
    }
    if (strlen(p->members) + strlen(name) + 2 > sizeof(p->members)) {
        printf("Collaborator list is full.\n");
        return;
    }
    strcat(p->members, ","); strcat(p->members, name);
    save_projects();
    printf("Collaborator added.\n");
}

void teamDashboard() {

    char username[USER_SIZE];
    int project_index, choice;

    project_count = 0;
    commit_count = 0;
    load_projects();
    load_commits();

    printf("Team Project Progress Tracker\nEnter username: ");
    if (fgets(username, sizeof(username), stdin) == NULL) return;
    username[strcspn(username, "\n")] = 0;
    if (username[0] == '\0') {
        printf("Username cannot be empty.\n");
        return;
    }
    clearScreen();
    while ((project_index = choose_project(username)) != -1)
        while (project_index >= 0) {
            show_history(project_index);
            printf("\n");
            printf("1. Add new progress message\n");
            printf("2. Delete a previous message\n");
            printf("3. Add collaborator\n");
            printf("4. Switch project\n");
            printf("5. Back to Home\n\n");
            printf("Choose option: ");

            if(scanf("%d", &choice) != 1) {
                ClearInputBuffer();
                printf("Invalid option.\n");
                continue;
            }
            ClearInputBuffer();

            if (choice == 1) add_commit(project_index, username);
            else if (choice == 2) delete_commit(project_index);
            else if (choice == 3) add_collaborator(project_index);
            else if (choice == 4) break;
            else if (choice == 5) return;
            else printf("Invalid option.\n");
        }
}
