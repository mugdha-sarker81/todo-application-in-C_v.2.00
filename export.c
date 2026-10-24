#include <stdio.h>
#include <string.h>
#include "export.h"

struct date{
    int day;
    int month;
    int year;
};
typedef struct date Date;

struct task{
    Date date;
    char title[100];
    char update[100];
};
typedef struct task Task;

char months[][10] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};


const char *html_template_head = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>To Do</title>
</head>
<body style="font-family: Arial, sans-serif; background-color: #f4f6f8; margin: 0; padding: 40px 20px; color: #333;">

    <div style="max-width: 500px; margin: 0 auto; background: #ffffff; padding: 30px; border-radius: 12px; box-shadow: 0 4px 15px rgba(0, 0, 0, 0.08);">
            
        <!-- Header -->
        <h1 style="margin-top: 0; font-size: 24px; color: #2c3e50; text-align: center; border-bottom: 2px solid #eee; padding-bottom: 15px;">
            To Do List Tasks
        </h1>
)rawliteral";



const char *html_template_chart = R"rawliteral(
        <div style="display: flex; align-items: center; justify-content: space-around; background-color: #f9fbfd; border: 1px solid #e1e8ed; padding: 15px; border-radius: 8px; margin-bottom: 25px;">
            
            <!-- Pie Chart -->
            <div style="width: 80px; height: 80px; border-radius: 50%%; background: conic-gradient(#2ecc71 0%% %.1f%%, #e74c3c %.1f%% 100%%); flex-shrink: 0;"></div>
            
            <!-- Chart Legend -->
            <div style="font-size: 14px;">
                <div style="display: flex; align-items: center; gap: 8px; margin-bottom: 6px;">
                    <span style="display: inline-block; width: 12px; height: 12px; background-color: #e74c3c; border-radius: 3px;"></span>
                    <span><strong>Undone:</strong> %d tasks</span>
                </div>
                <div style="display: flex; align-items: center; gap: 8px; margin-bottom: 6px;">
                    <span style="display: inline-block; width: 12px; height: 12px; background-color: #2ecc71; border-radius: 3px;"></span>
                    <span><strong>Done:</strong> %d task</span>
                </div>
                <div style="font-size: 12px; color: #7f8c8d; margin-top: 4px;">
                    Total Progress: <strong>%.1f%%</strong>
                </div>
            </div>
        </div>

    <!-- Task List Display -->
        <div style="display: flex; flex-direction: column; gap: 10px;">
    )rawliteral";



const char *html_template_task_pending = R"rawliteral(
    <div style="display: flex; align-items: center; justify-content: space-between; padding: 12px 15px; background-color: #fafafa; border: 1px solid #e0e0e0; border-radius: 6px;">
                <div style="display: flex; align-items: center; gap: 10px;">
                    <span style="display: inline-block; width: 10px; height: 10px; border-radius: 50%%; background-color: #e74c3c;"></span>
                    <span style="font-size: 15px;">%s</span>
                </div>
                <span style="font-size: 12px; color: #888; background-color: #eef2f5; padding: 4px 8px; border-radius: 4px; white-space: nowrap; margin-left: 10px;">
                    Due: %s %d, %d
                </span>
            </div>
    )rawliteral";



const char *html_template_task_completed = R"rawliteral(
    <div style="display: flex; align-items: center; justify-content: space-between; padding: 12px 15px; background-color: #f0f0f0; border: 1px solid #e0e0e0; border-radius: 6px; opacity: 0.75;">
                <div style="display: flex; align-items: center; gap: 10px;">
                    <span style="display: inline-block; width: 10px; height: 10px; border-radius: 50%%; background-color: #2ecc71;"></span>
                    <span style="font-size: 15px; text-decoration: line-through; color: #777;">%s</span>
                </div>
                <span style="font-size: 12px; color: #888; background-color: #e0e0e0; padding: 4px 8px; border-radius: 4px; white-space: nowrap; margin-left: 10px;">
                    Done: %s %d, %d
                </span>
            </div>
    )rawliteral";



const char *html_template_ending= R"rawliteral(
        </div>
        </div>

    </body>
</html>)rawliteral";



int exportTasksToHTML(){
    Task tasklist[1000];
    
    int id, day, month, year, taskCount=0, taskDone=0;
    char title[100], priority[100], update[100];

    FILE *taskFile = fopen("tasks.txt", "r");

    if (taskFile!=NULL){
        while(fscanf(taskFile, " %d|%d-%d-%d|%99[^|]|%9[^|]|%9[^\n]\n", &id, &day, &month, &year, title, priority, update)==7){
            Date dat;
            dat.month = month;
            dat.day = day;
            dat.year = year;

            tasklist[taskCount].date = dat;
            strcpy(tasklist[taskCount].update, update);
            strcpy(tasklist[taskCount].title, title);
            if (strcmp(update, "Done")==0) taskDone++;
            taskCount++;
        }
        fclose(taskFile);
    }
    else{
        printf("Can't access task file");
        return -1;
    }
    

    if (taskCount==0) {
        printf("Your tasks file is empty.\n");
        return -1;
    }

    else{
        FILE *exportFile = fopen("report.html", "w");
        if (exportFile == NULL) {
            perror("Could not create report.html");
            return -1;
        }
        float percent = ((float)taskDone/taskCount)*100.0;
        fprintf(exportFile, "%s\n", html_template_head);
        fprintf(exportFile, html_template_chart, percent, percent, taskCount-taskDone, taskDone);
        for (int i=0; i<taskCount; i++){
            if (strcmp(tasklist[i].update, "Done")==0) fprintf(exportFile,html_template_task_completed, tasklist[i].title, months[tasklist[i].date.month-1], tasklist[i].date.day, tasklist[i].date.year);
            else fprintf(exportFile,html_template_task_pending, tasklist[i].title, months[tasklist[i].date.month-1], tasklist[i].date.day, tasklist[i].date.year);
        }
        fprintf(exportFile,"%s", html_template_ending);
        if (fclose(exportFile) != 0) {
            perror("Could not finish writing report.html");
            return -1;
        }
    }
    return 0;
}