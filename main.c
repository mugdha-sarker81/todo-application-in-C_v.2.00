#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tasks.h"
#include "habits.h"
#include "calendar.h"
#include "export.h"
#include "team.h"

void clearScreen();
void clearInputBuffer();
void dashboard();
void home();
void printTutorial();

int main(int argc, char *argv[]) {

    if (argc > 1) {
        if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {

            printf("\n============================================================\n");
            printf("       TASK MANAGER, HABIT TRACKER & TEAM WORK\n");
            printf("============================================================\n\n");
            printf("Usage:  ");
            printf("  ./todo [option]\n\n");

            printf("Commands:\n");
            printf("  -h, --help              Show this help message\n");
            printf("  --clear                 Clear all saved tasks and habits\n");
            printf("  --date DD-MM-YYYY       View tasks for a specific date\n");
            printf("  --habit                 Open the Daily Habits dashboard\n");
            printf("  --team                  Open the Team Work dashboard\n");
            printf("  --report                Generate and open the HTML task report\n");
            printf("  --data                  Show where your data is saved\n");
            printf("  --tut                   Open the quick tutorial\n\n");

            printf("Tip: Run './todo --tut' if you're using the app for the first time.\n");
            printf("============================================================\n\n");

            return 0;
        }
        else if (strcmp(argv[1], "--tut") == 0) {
            printTutorial();
            return 0;
        }
        else if (strcmp(argv[1], "--data") == 0) {
            printf("------------------------------------------------------------\n");
            printf("             YOUR DATA\n");
            printf("------------------------------------------------------------\n");
            printf(" Your progress is automatically saved in:\n");
            printf(" # tasks.txt → Your tasks\n");
            printf(" # habits.txt → Your habits\n");
            printf(" # habit_log.txt → Your habit history\n\n");
            return 0;
        }
        else if (strcmp(argv[1], "--report") == 0) {
            if (exportTasksToHTML() != 0) {
                fprintf(stderr, "Could not generate the task report.\n");
                return 1;
            }

            #ifdef _WIN32
                        const char *openCommand = "start \"\" \"report.html\"";
            #elif defined(__APPLE__)
                        const char *openCommand = "open \"report.html\"";
            #else
                        const char *openCommand = "xdg-open \"report.html\"";
            #endif
            
            if (system(openCommand) != 0) {
                fprintf(stderr, "Report created, but could not open it. "
                        "Open report.html manually.\n");
                return 1;
            }

            printf("Report created and opened: report.html\n");
            return 0;
        }
        else if (strcmp(argv[1], "--clear") == 0) {
            remove("tasks.txt");
            remove("habits.txt");
            remove("habit_log.txt");
            printf("All data cleared successfully.\n");
            return 0;
        }
        else if (strcmp(argv[1], "--date") == 0) {
            if (argc < 3) {
                printf("Please provide a date. Example: ./todo --date 26-09-2026\n");
                return 1;
            }
            loadTasks();
            char *date = argv[2];
            if (!validDate(date)) {
                printf("Invalid date format. Use DD-MM-YYYY\n");
                free(tasks);
                return 1;
            }
            printf("\n======= Tasks for %s =======\n\n", date);
            int found = 0;
            for (int i = 0; i < taskCount; i++) {
                if (!strcmp(tasks[i].date, date)) {
                    printf("%d | %s | Priority: %s | Status: %s\n",
                           tasks[i].id, tasks[i].title,
                           tasks[i].priority, tasks[i].status);
                    found = 1;
                }
            }
            if (!found) printf("No tasks found for this date.\n");
            free(tasks);
            return 0;
        }
        else if (strcmp(argv[1], "--habit") == 0) {
            loadHabits();
            loadLogs();
            habitboard();
            return 0;
        }
        else if (strcmp(argv[1], "--team") == 0) {
            teamDashboard();
            return 0;
        }
        else {
            printf("Unknown option: %s\n", argv[1]);
            printf("Use --help for available options.\n");
            return 1;
        }
    }

    loadTasks();
    loadHabits();
    loadLogs();
    home();

    free(tasks);
    return 0;
}


void home() {
    int choice;

    while (1) {
        clearScreen();
        printf("\n=====================================\n");
        printf("          PRODUCTIVITY HOME\n");
        printf("=====================================\n");
        printf("1. Task Manager\n");
        printf("2. Habit Tracker\n");
        printf("3. Team Work\n");
        printf("4. Exit\n");
        printf("=====================================\n");
        printf("Choose a feature: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            clearInputBuffer();
            continue;
        }

        switch (choice) {
            case 1: dashboard(); break;
            case 2: habitboard(); break;
            case 3:
                clearInputBuffer();
                teamDashboard();
                break;
            case 4: return;
            default: printf("Invalid choice! Please try again.\n");
        }
    }
}

void dashboard() {
    clearScreen();
    int choice;
    while (1){
        printf("=====================================\n");
        printf("            TASK MANAGER\n");
        printf("=====================================\n");
        printf("1. Add Task\n");
        printf("2. View All pending Tasks\n");
        printf("3. Search Task by Date\n");
        printf("4. Mark a Task done\n");
        printf("5. Delete Task\n");
        printf("6. Show Calendar\n");
        printf("7. Export to HTML report\n");
        printf("8. Back to Home\n");
        printf("=====================================\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            clearInputBuffer();
            continue;
        }

        switch (choice) {
            case 1: addTask(); break;
            case 2: viewTasks(); break;
            case 3: searchByDate(); break;
            case 4: viewTasks(); completeTask(); break;
            case 5: viewTasks(); deleteTask(); break;
            case 6: {
                int year, month;
                printf("Enter month and year (e.g. 9 2026): ");
                scanf("%d %d", &month, &year);

                if (month < 1 || month > 12) {
                    printf("Invalid month!\n");
                } else {
                    clearScreen();
                    showCalendar(year, month);
                    printf("\n\nPress Enter to go back to dashboard...");
                    clearInputBuffer();
                    getchar();
                    clearScreen();
                }
                break;
            }
            case 7: {
                if (exportTasksToHTML() != 0) {
                    fprintf(stderr, "Could not generate the task report.\n");
                    break;
                }

#ifdef _WIN32
                const char *openCommand = "start \"\" \"report.html\"";
#elif defined(__APPLE__)
                const char *openCommand = "open \"report.html\"";
#else
                const char *openCommand = "xdg-open \"report.html\"";
#endif

                if (system(openCommand) != 0) {
                    fprintf(stderr, "Report created, but could not open it. "
                            "Open report.html manually.\n");
                    break;
                }

                printf("Report created and opened: report.html\n");
                break;
            }
            case 8:
                return;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}

void clearInputBuffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void clearScreen(){
    system("cls");
}

void printTutorial() {
    printf("\n");
    printf("============================================================\n"); 
    printf(" TASK MANAGER, HABITS & TEAM WORK - GUIDE\n"); 
    printf("============================================================\n\n"); 
    printf("Welcome! Choose an area from the home menu:\n\n");
    printf(" [1] Task Manager - create, find, complete, and delete tasks.\n");
    printf(" [2] Habit Tracker - manage habits and review your streak.\n");
    printf(" [3] Team Work - follow project progress and manage collaborators.\n");
    printf(" [4] Exit\n\n");
    printf("------------------------------------------------------------\n"); 
    printf("  TASK MANAGER\n"); 
    printf("------------------------------------------------------------\n"); 
    printf(" [1] Add Task\n"); 
    printf(" \tCreate a new task with a date, title, and priority.\n"); 
    printf(" [2] View Tasks\n"); 
    printf(" \tSee all your unfinished tasks in one place.\n"); 
    printf(" [3] Find Tasks by Date\n"); 
    printf(" \tEnter a date to see what you need to do that day.\n"); 
    printf(" [4] Complete a Task\n"); 
    printf(" \tFinished something? Mark it as DONE using its ID.\n"); 
    printf(" [5] Delete a Task\n"); 
    printf(" \tRemove a task you no longer need using its ID.\n"); 
    printf(" [6] Show Calendar\n"); 
    printf(" \tPick a month and year to see your task calendar.\n"); 
    printf(" [7] Export Report\n"); 
    printf(" \tCreate a report.html file to view your tasks in a browser.\n"); 
    printf(" [8] Back to Home\n\n");
    printf(" Tip: Choose a different area from the home menu at any time.\n"); 
    printf("============================================================\n");
}