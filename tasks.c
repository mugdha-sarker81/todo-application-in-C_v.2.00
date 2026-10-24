#include "tasks.h"

void clearScreen();
void clearInputBuffer();
void dashboard();

struct Task *tasks = NULL;
int taskCount = 0;
int taskCapacity = 0;
int nextId = 1;

long dateToKey(const char *date){
    int d, m, y;
    sscanf(date, "%2d-%2d-%4d", &d, &m, &y);
    return (long)y * 10000L + (long)m * 100L + d;
}

int cmpByDate(const void *a, const void *b){
    const struct Task *ta = (const struct Task *)a;
    const struct Task *tb = (const struct Task *)b;
    long ka = dateToKey(ta->date);
    long kb = dateToKey(tb->date);
    if (ka != kb) return (ka < kb) ? -1 : 1;
    return ta->id - tb->id;
}

int validDate(char *date){
    if (strlen(date) != 10) return 0;
    if (date[2] != '-' || date[5] != '-') return 0;

    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) continue;
        if (!isdigit((unsigned char)date[i])) return 0;
    }

    int d, m, y;
    sscanf(date, "%2d-%2d-%4d", &d, &m, &y);
    if (m < 1 || m > 12) return 0;
    if (d < 1 || d > 31) return 0;
    return 1;
}

void loadTasks() {
    taskCapacity = INITIAL_CAPACITY;
    tasks = (struct Task *)malloc(taskCapacity * sizeof(struct Task));
    if (!tasks) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    taskCount = 0;
    nextId = 1;

    FILE *fp = fopen("tasks.txt", "r");
    if (!fp) return;

    struct Task tmp;
    while (fscanf(fp, "%d|%19[^|]|%99[^|]|%9[^|]|%14[^\n]",
                  &tmp.id, tmp.date, tmp.title, tmp.priority, tmp.status) == 5)
    {
        int c = fgetc(fp);
        if (c != '\n' && c != EOF) ungetc(c, fp);

        if (taskCount >= taskCapacity) {
            taskCapacity *= 2;
            struct Task *newTasks = (struct Task *)realloc(tasks, taskCapacity * sizeof(struct Task));
            if (!newTasks) {
                printf("Memory reallocation failed!\n");
                free(tasks);
                exit(1);
            }
            tasks = newTasks;
        }

        if (tmp.id >= nextId) nextId = tmp.id + 1;
        tasks[taskCount++] = tmp;
    }
    fclose(fp);
    qsort(tasks, taskCount, sizeof(struct Task), cmpByDate);
}

void saveTasks(){
    FILE *fp = fopen("tasks.txt", "w");
    if (!fp) {
        printf("Could not save tasks.\n");
        return;
    }

    qsort(tasks, taskCount, sizeof(struct Task), cmpByDate);
    for (int i = 0; i < taskCount; i++){
        fprintf(fp, "%d|%s|%s|%s|%s\n",
                tasks[i].id, tasks[i].date, tasks[i].title,
                tasks[i].priority, tasks[i].status);
    }
    fclose(fp);
}

void addTask() {
    clearScreen();

    if (taskCount >= taskCapacity) {
        taskCapacity = (taskCapacity == 0) ? INITIAL_CAPACITY : taskCapacity * 2;

        struct Task *newTasks = (struct Task *)realloc(tasks, taskCapacity * sizeof(struct Task));
        
        if (!newTasks) {
            printf("Memory allocation failed! Cannot add more tasks.\n");
            return;
        }
        tasks = newTasks;
    }

    struct Task tmp;
    tmp.id = nextId++;

    do {
        printf("Enter date (DD-MM-YYYY): ");
        scanf("%19s", tmp.date);
        if (!validDate(tmp.date))
            printf("Invalid date. Please use DD-MM-YYYY.\n");
    } while (!validDate(tmp.date));

    clearInputBuffer();

    printf("Enter task title: ");
    fgets(tmp.title, sizeof(tmp.title), stdin);
    tmp.title[strcspn(tmp.title, "\n")] = 0;

    do {
        printf("Enter priority (High/Medium/Low): ");
        scanf("%9s", tmp.priority);
        if (strcmp(tmp.priority, "High") && strcmp(tmp.priority, "Medium") && strcmp(tmp.priority, "Low"))
            printf("Invalid priority.\n");
    } while (strcmp(tmp.priority, "High") && strcmp(tmp.priority, "Medium") && strcmp(tmp.priority, "Low"));

    strcpy(tmp.status, "Pending");
    tasks[taskCount++] = tmp;
    saveTasks();
    printf("\nTask added successfully!\n");
}

void viewTasks() {
    clearScreen();
    int found = 0;
    for (int i = 0; i < taskCount; i++) {
        if (!strcmp(tasks[i].status, "Pending")) {
            printf("\n%d | %s | %s | Priority: %s | Status: %s\n",
                   tasks[i].id, tasks[i].date, tasks[i].title,
                   tasks[i].priority, tasks[i].status);
            found = 1;
        }
    }
    if (!found) printf("\nNo unfinished tasks.\n");
    askuser();
}

void searchByDate() {
    char date[20];
    int found = 0;

    printf("Enter date (DD-MM-YYYY): ");
    scanf("%19s", date);
    if (!validDate(date)) {
        printf("Invalid date format.\n");
        return;
    }

    clearScreen();
    printf("\n=======Tasks for %s:=========\n\n", date);
    for (int i = 0; i < taskCount; i++) {
        if (!strcmp(tasks[i].date, date)) {
            printf("\n%d | %s | Priority: %s | Status: %s\n",
                   tasks[i].id, tasks[i].title,
                   tasks[i].priority, tasks[i].status);
            found = 1;
        }
    }
    if (!found) printf("[]No task found for this date.\n");
    askuser();
}

void completeTask() {
    int id;
    printf("Enter task ID: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid input.\n");
        clearInputBuffer();
        return;
    }

    for (int i = 0; i < taskCount; i++) {
        if (tasks[i].id == id) {
            strcpy(tasks[i].status, "Done");
            saveTasks();
            printf("Task marked as completed.\n");
            return;
        }
    }
    printf("Task not found.\n");
}

void deleteTask() {
    int id;
    printf("\nEnter task ID: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid input.\n");
        clearInputBuffer();
        return;
    }

    for (int i = 0; i < taskCount; i++) {
        if (tasks[i].id == id) {
            for (int j = i; j < taskCount - 1; j++)
                tasks[j] = tasks[j + 1];
            taskCount--;
            saveTasks();
            printf("Task deleted.\n");
            return;
        }
    }
    printf("Task not found.\n");
}

void askuser(){
    int choice;
    while (1){
        printf("\n=====================================\n");
        printf("            want to customize ?\n");
        printf("=====================================\n");
        printf("1. Add Task\n");
        printf("2. Mark a Task done\n");
        printf("3. Delete Task\n");
        printf("4. back to dashboard\n");
        printf("=====================================\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            clearInputBuffer();
            continue;
        }
        switch (choice) {
            case 1: addTask(); break;
            case 2: completeTask(); break;
            case 3: deleteTask(); break;
            case 4: return;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}