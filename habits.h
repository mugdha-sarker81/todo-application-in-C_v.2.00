#ifndef HABITS_H
#define HABITS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_HABITS 10
#define MAX_LOGS 5000
#define TREND_DAYS 7
#define IGNORED_THRESHOLD 40

typedef struct {
    int id;
    char name[60];
    char time[6];   
} Habit;

typedef struct {
    char date[11];  
    int habitId;
    int completed;
} HabitLog;

extern Habit habits[MAX_HABITS];
extern HabitLog logs[MAX_LOGS];
extern int habitCount, logCount;

void loadHabits();
void saveHabits();
void loadLogs();
void saveLogs();
void habitboard();
int todayPerformance();
int currentStreak();

#endif