#include "habits.h"

void clearScreen();
void clearInputBuffer();
void dashboard();

Habit habits[MAX_HABITS];
HabitLog logs[MAX_LOGS];
int habitCount = 0, logCount = 0;

void getString(char *s, int size){
    fgets(s, size, stdin);
    s[strcspn(s, "\n")] = '\0';
}

int validTime(char *t){
    int h, m;
    return (strlen(t)==5 && t[2]==':') && sscanf(t,"%2d:%2d",&h,&m)==2 && (h>=0 && h<=23 && m>=0 && m<=59);
}

void getToday(char *out){
    time_t now = time(NULL);
    strftime(out, 11, "%Y-%m-%d", localtime(&now));
}

void loadHabits(){
    FILE *fp = fopen("habits.txt", "r");
    if (!fp) return;
    while(habitCount < MAX_HABITS && fscanf(fp, "%d|%59[^|]|%5[^\n]\n",
           &habits[habitCount].id, habits[habitCount].name, habits[habitCount].time) == 3)

        habitCount++;
    fclose(fp);
}

void saveHabits(){
    FILE *fp = fopen("habits.txt", "w");
    if (!fp) { printf("Could not save habits.\n"); return; }

    for (int i = 0; i < habitCount; i++)
        fprintf(fp, "%d|%s|%s\n", habits[i].id, habits[i].name, habits[i].time);
         
    fclose(fp);
}

void loadLogs(){
    FILE *fp = fopen("habit_log.txt", "r");
    if (!fp) return;

    while(logCount < MAX_LOGS && fscanf(fp,"%10[^|]|%d|%d\n",logs[logCount].date,
                                    &logs[logCount].habitId,&logs[logCount].completed)==3)

        logCount++;

    fclose(fp);
}

void saveLogs(){
    FILE *fp = fopen("habit_log.txt", "w");
    if (!fp) { printf("Could not save logs.\n"); return; }

    for(int i = 0; i < logCount; i++)
        fprintf(fp, "%s|%d|%d\n", logs[i].date, logs[i].habitId, logs[i].completed);

    fclose(fp);
}

int findHabit(int id){
    for (int i = 0; i < habitCount; i++)
        if (habits[i].id == id) return i;

    return -1;
}

int findLog(char *date, int habitId){
    for (int i = 0; i < logCount; i++)
        if (logs[i].habitId == habitId && strcmp(logs[i].date, date) == 0) return i;

    return -1;
}

int isCompleted(char *date, int habitId){
    int i = findLog(date, habitId);
    return i != -1 && logs[i].completed;
}

void sortHabitsByTime(){
    for (int i = 0; i < habitCount - 1; i++)
        for (int j = 0; j < habitCount - i - 1; j++)
            if (strcmp(habits[j].time, habits[j + 1].time) > 0){
                Habit tmp = habits[j];
                habits[j] = habits[j + 1];
                habits[j + 1] = tmp;
            }
}

void addHabit(){
    if (habitCount >= MAX_HABITS) {
        printf("\nMaximum %d habits reached.\n", MAX_HABITS);
        return;
    }

    Habit h;  h.id = 1;
    for (int i = 0; i < habitCount; i++)
        if (habits[i].id >= h.id) h.id = habits[i].id + 1;

    printf("\nHabit name: ");
    getString(h.name, sizeof(h.name));

    if (strlen(h.name) == 0) { printf("Name cannot be empty.\n"); return; }

    do{
        printf("Time (HH:MM): ");
        getString(h.time, sizeof(h.time));
    }while (!validTime(h.time) && printf("Invalid time, e.g. 07:30\n"));

    habits[habitCount++] = h;
    sortHabitsByTime();
    saveHabits();
    printf("Added.\n");
}

void deleteHabit(int id){
    int idx = findHabit(id);
    if (idx == -1) { printf("Habit not found.\n"); return; }

    for (int i = idx; i < habitCount - 1; i++)
        habits[i] = habits[i + 1];
    habitCount--;
    saveHabits();
    printf("Deleted.\n");
}

void showToday(){
    clearScreen();
    char today[11];   getToday(today);
    printf("\n===== %s =====\n", today);

    if (habitCount == 0) { printf("No habits yet.\n"); return; }

    for (int i = 0; i < habitCount; i++)
        printf("%d. [%c] %s - %s\n", habits[i].id, isCompleted(today, habits[i].id) ? 'X' : ' ',
                                     habits[i].time, habits[i].name);

    printf("====================\n");
}

void toggleHabit(int id){
    char today[11];
    getToday(today);

    if (findHabit(id) == -1) { printf("Habit not found.\n"); return; }

    int idx = findLog(today, id);
    if (idx == -1) {
        if (logCount >= MAX_LOGS) { printf("Log storage full.\n"); return; }

        strcpy(logs[logCount].date, today);
        logs[logCount].habitId = id;
        logs[logCount].completed = 1;
        logCount++;
    } else {
        logs[idx].completed = !logs[idx].completed;
    }

    saveLogs();
    printf("Toggled.\n");
}

time_t dateToTime(char *date){
    int y, m, d;
    struct tm t = {0};
    sscanf(date, "%d-%d-%d", &y, &m, &d);
    t.tm_year = y - 1900;
    t.tm_mon = m - 1;
    t.tm_mday = d;
    t.tm_hour = 12;
    return mktime(&t);
}

void timeToDate(time_t v, char *out){
    strftime(out, 11, "%Y-%m-%d", localtime(&v));
}

int dayQualifies(char *date){
    if (habitCount == 0) return 0;
    int completed = 0;

    for (int i = 0; i < habitCount; i++)
        if (isCompleted(date, habits[i].id)) completed++;

    return completed * 100 >= habitCount * 80;
}

int currentStreak(){
    char today[11];
    getToday(today);
    time_t t = dateToTime(today);
    int streak = 0;
    char date[11];
    timeToDate(t, date);
    while (dayQualifies(date)) {
        streak++;
        t -= 86400;
        timeToDate(t, date);
    }
    return streak;
}

int todayPerformance(){
    if (habitCount == 0) return 0;
    char today[11];
    getToday(today);
    int completed = 0;
    for (int i = 0; i < habitCount; i++)
        if (isCompleted(today, habits[i].id)) completed++;
    return (completed * 100) / habitCount;
}

int habitCompletionRate(int habitId, int days){
    char today[11];
    getToday(today);
    time_t t = dateToTime(today);
    int completed = 0;
    for (int i = 0; i < days; i++){
        char date[11];
        timeToDate(t - (time_t)i * 86400, date);
        if (isCompleted(date, habitId)) completed++;
    }
    return (completed * 100) / days;
}

void showPerformance(){
    clearScreen();
    printf("\n===== PERFORMANCE (last %d days) =====\n", TREND_DAYS);
    if (habitCount == 0) { printf("No habits yet.\n"); return; }

    for (int i = 0; i < habitCount; i++){
        int rate = habitCompletionRate(habits[i].id, TREND_DAYS);
        printf("%-20s %3d%%  %s\n", habits[i].name, rate, rate < IGNORED_THRESHOLD ? "<- ignoring" : "");
    }

    printf("\nNeglected habits:\n");
    int any = 0;
    for (int i = 0; i < habitCount; i++) {
        if (habitCompletionRate(habits[i].id, TREND_DAYS) < IGNORED_THRESHOLD) {
            printf("  - %s\n", habits[i].name);
            any = 1;
        }
    }
    if (!any) printf("  None, nice work.\n");
    printf("=======================================\n");
    printf("\n====== go back  (press 5): ");
    while (1) {
        int choice;
        scanf("%d", &choice);
        switch (choice){
            case 5: habitboard(); break;
            default: printf("\nInvalid choice! Please press '5' again: "); break;
        }
    }
}

void habitboard(){
    int choice, id;
    clearScreen();
    while (1) {
        printf("\n============================\n");
        printf("      DAILY HABITS\n");
        printf("============================\n");
        printf("Today: %d%%   Streak: %d day(s)\n",
               todayPerformance(), currentStreak());
        printf("----------------------------\n");
        printf("1. Show Today's Habits\n");
        printf("2. Add Habit\n");
        printf("3. Mark Habit Done\n");
        printf("4. Delete Habit\n");
        printf("5. Show Performance\n");
        printf("6. Back to Home page\n");
        printf("----------------------------\n");
        printf("Choice: ");

        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Invalid choice.\n");
            continue;
        }
        clearInputBuffer();

        switch(choice){
            case 1: showToday(); break;
            case 2: addHabit(); break;
            case 3:
                showToday();
                printf("Habit ID to toggle: ");
                if (scanf("%d", &id) == 1) { clearInputBuffer(); toggleHabit(id); }
                else clearInputBuffer();
                break;
            case 4:
                showToday();
                printf("Habit ID to delete: ");
                if (scanf("%d", &id) == 1) { clearInputBuffer(); deleteHabit(id); }
                else clearInputBuffer();
                break;
            case 5: showPerformance(); break;
            case 6: home(); break;
            default: printf("Invalid choice. Try again\n");
        }
    }
}