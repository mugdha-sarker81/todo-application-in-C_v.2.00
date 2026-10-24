#include <stdio.h>
#include <string.h>
#include "calendar.h"

#define YELLOW "\x1b[33m"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"


struct pair{
    int date;
    int maxPriority;
};

struct pair dateArray[10000];


int loadDates(){
    int datesFound = 0;
    FILE *taskFile;
    taskFile = fopen("tasks.txt", "r");

    int id;
    int day;
    int month;
    int year;
    char title[100];
    char priority[100];
    char update[100];

    printf("Loading dates...\n");
    while (fscanf(taskFile, "%d|%d-%d-%d|%99[^|]|%9[^|]|%9[^\n]\n", &id, &day, &month, &year, title, priority, update)>0){
        if (strcmp(update, "Pending")==0){
            dateArray[datesFound].date = year*10000+month*100+day;
            dateArray[datesFound].maxPriority = (strcmp(priority, "High")==0)?3:(strcmp(priority, "Medium")==0)?2:1;
            datesFound++;
        }
    }
    fclose(taskFile);
    printf("Done.\n");
    return datesFound;
}

int searchDate(int count, int targetDate) {
    int low = 0;
    int high = count - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (dateArray[mid].date == targetDate) {
            return mid;
        }
        
        if (dateArray[mid].date < targetDate) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return -1;
}


int isLeapYear(int year){
    if ((year%4==0 && year%100!=0) || (year%400==0)) return 1;
    else return 0;
}

int getFirstDayOfYear(int year) {
    int d = (year - 1) * 365 + (year - 1) / 4 - (year - 1) / 100 + (year - 1) / 400 + 1;
    return d % 7;
}

int getWeekday(int year, int month, int day){
    month--;
    int cumSumMonths[] = {31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
    int d = getFirstDayOfYear(year);
    if (month!=0){
        d += cumSumMonths[month-1] + day;
        if (isLeapYear(year) && month>1) d += 1;
    }
    else d += day;

    return d%7;
}


void showCalendar(int year, int month){
    char *monthsName[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    int months[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (isLeapYear(year)) months[1] = 29;
    int numDates = loadDates();

    printf(GREEN "   %s, %d\n" RESET, monthsName[month-1], year );

    printf("   Sat  Sun  Mon  Tue  Wed  Thu  Fri\n");

    int weekday = getWeekday(year, month, 1);


    
    for (int i=0; i<weekday; i++){
        printf("     ");
    }

    for (int i=1; i<=months[month-1]; i++){
        int priority = dateArray[searchDate(numDates, year*10000+month*100+i)].maxPriority;
        if (priority==3) printf(RED "%5d" RESET, i);
        else if (priority==2) printf(YELLOW "%5d" RESET, i);
        else if (priority==1) printf(GREEN "%5d" RESET, i);
        else printf("%5d", i);

        

        if ((i+weekday)%7==0){
            printf("\n");
        }
    }

}
