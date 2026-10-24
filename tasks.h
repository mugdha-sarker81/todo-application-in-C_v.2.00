#ifndef TASKS_H
#define TASKS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Task {
    int id;
    char date[20];
    char title[100];
    char priority[10];
    char status[15];
};

extern struct Task *tasks;
extern int taskCount;
extern int taskCapacity;
extern int nextId;

#define INITIAL_CAPACITY 20

void loadTasks();
void saveTasks();
void addTask();
void viewTasks();
void searchByDate();
void completeTask();
void deleteTask();
int validDate(char *date);
void askuser();

#endif