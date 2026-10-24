# Task Manager, Habit Tracker & Team Work

A feature-rich C application for managing daily tasks, tracking habits, coordinating team projects, reviewing progress, and planning work visually. Its home menu keeps task management, habit tracking, and team work in separate dashboards.

This project is designed as a practical C programming exercise and also works as a real personal productivity tool for daily planning.

---

## Overview

The application provides:

- Task creation with date and priority
- Search and filtering by date
- Pending-task viewing
- Marking tasks as completed
- Task deletion
- Habit tracking with daily completion logs
- Team project progress messages and collaborators
- Streak calculation and performance statistics
- Calendar visualization with color-coded priority levels
- HTML export for quick reporting and sharing
- Persistent storage through text files
- Command-line options for quick access and automation

It is built as a multi-file C project using modular code, dynamic memory, structured data, and file-based persistence.

---

## Main Features

### 1. Task Management

The core of the app is the task manager.

- Add a task with:
  - date in `DD-MM-YYYY` format
  - title
  - priority: `High`, `Medium`, or `Low`
  - automatic status: `Pending`
- Each task receives a unique numeric ID automatically.
- Tasks are stored in memory and saved permanently in `tasks.txt`.
- Tasks can be listed in pending-state view.
- Tasks can be searched by exact date.
- Tasks can be marked as `Done`.
- Tasks can be removed from the list.
- Tasks are sorted by date, then by task ID.

This makes the planner more useful for daily planning, milestone tracking, and short-term scheduling.

### 2. Priority-Based Planning

Each task includes a priority level:

- High: critical or urgent work
- Medium: normal priority work
- Low: optional or flexible tasks

This priority is used in the calendar display and helps the user quickly identify what needs attention soon.

### 3. Search by Date

The app allows the user to search all tasks scheduled for a specific date.

Example:

```bash
./todo --date 26-09-2026
```

This shows every task scheduled on that date, making it easier to organize the day or review specific deadlines.

### 4. Home and Dashboard Interface

At startup, choose one of three independent areas:

- Task Manager
- Habit Tracker
- Team Work

Each area has its own dashboard, and users can return to the home menu.

The Task Manager dashboard includes:

- Add Task
- View All Pending Tasks
- Search Task by Date
- Mark a Task Done
- Delete Task
- Show Calendar
- Export Tasks to HTML
- Back to Home

This keeps the application easy to use in a terminal environment while still being feature-rich.

### 5. Daily Habit Tracker

The habit system helps users build good routines and monitor consistency.

Features:

- Add a habit with name and scheduled time (`HH:MM`)
- View today’s habits
- Toggle a habit as complete or incomplete
- Delete habits when no longer needed
- Track completion logs in `habit_log.txt`
- Calculate habit completion percentage
- Measure current streak of successful days
- Identify neglected habits based on recent performance

Habit logic includes a threshold-based completion rule:

- A day is considered successful when at least 80% of habits are completed.
- The current streak increases when consecutive days qualify.
- Performance reporting shows recent completion rates over a configured number of days.

### 6. Streak and Performance Analytics

The app includes a lightweight habit analytics system that helps users monitor consistency.

It tracks:

- Today’s habit completion percentage
- Current streak count
- Short-term habit completion rate over recent days
- Habits marked as neglected if they fall below a threshold

These statistics encourage habit consistency and make the app feel like a daily progress tracker instead of just a task list.

### 7. Team Work

Team Work lets users create or select projects assigned to their username, record progress messages, delete previous messages, and add collaborators. Projects are stored in `team_project_name.txt`; progress history is stored in `commit_msg.txt`.

### 8. Calendar View with Color Coding

The calendar module displays a month view and color-codes days with pending tasks based on task importance.

Color meanings:

- Red: high-priority tasks
- Yellow: medium-priority tasks
- Green: low-priority tasks
- Plain: no pending tasks

This visual planning aid helps users see which days are busier or more urgent at a glance.

### 9. HTML Task Export

The app can generate a simple HTML report from saved tasks.

Command:

```bash
./todo --report
```

This creates a file named `report.html` and opens it in the default browser. The exported report includes:

- task list
- done/undone summary
- progress chart
- due date information
- visual task grouping

This makes it easy to view work summaries outside the terminal and share the report if needed.

### 10. Data Persistence

The program stores user information in local files so data remains available across sessions.

Files created/used:

- `tasks.txt` — stores tasks
- `habits.txt` — stores habit definitions
- `habit_log.txt` — stores habit completion records
- `team_project_name.txt` — stores team projects and collaborator usernames
- `commit_msg.txt` — stores team progress messages
- `report.html` — generated export report

This gives the project a realistic personal management tool feel, not just a demo console app.

### 11. Command-Line Convenience

The application supports multiple command-line operations directly from the terminal.

Supported options:

```bash
./todo --help
./todo --tut
./todo --data
./todo --report
./todo --clear
./todo --date DD-MM-YYYY
./todo --habit
./todo --team
```

Examples:

```bash
./todo --help
./todo --date 20-10-2026
./todo --habit
./todo --team
./todo --report
```

This makes the program suitable for quick runs, automation, and efficient daily usage.

---

## Detailed Project Structure

```text
to-do-list-app-in-C/
├── main.c            # Main application loop and command-line options
├── tasks.c           # Task management logic
├── tasks.h           # Task-related structures and function declarations
├── habits.c          # Habit tracking, logs, streaks, and performance
├── habits.h          # Habit structures and declarations
├── calendar.c        # Monthly calendar visualization with color-coding
├── calendar.h        # Calendar function declarations
├── export.c          # HTML report generator
├── export.h          # Export declarations
├── Makefile          # Build automation
├── README.md         # Project documentation
├── tasks.txt         # Saved task records
├── habits.txt        # Saved habit list
├── habit_log.txt     # Saved habit completion records
├── report.html       # Generated HTML report
├── todo.exe          # Windows compiled binary (if built)
└── .gitignore        # Git ignore file
```

---

## How the App Works

### Task Data Model

Tasks are stored using a structure similar to:

```c
struct Task {
    int id;
    char date[20];
    char title[100];
    char priority[10];
    char status[15];
};
```

This stores:

- a unique task ID
- due date
- task description/title
- priority
- completion state

### Habit Data Model

Habits are tracked using a structured record containing:

- habit ID
- habit name
- scheduled time
- daily completion log entries

Each log entry tracks:

- date
- habit ID
- completion state (`0` or `1`)

### Input Validation

The app validates key inputs before saving records:

- date format must be `DD-MM-YYYY`
- month must be between `1` and `12`
- day must be valid for the month
- priority must be one of `High`, `Medium`, or `Low`
- habit time must be in `HH:MM` format

This reduces invalid data and prevents broken scheduling.

### File Persistence Logic

The project explicitly reads and writes from text files with `fopen()`, `fscanf()`, `fprintf()`, and related C file functions.

This means:

- tasks are not lost when the application exits
- habit records remain available
- progress continues from a previous session
- report generation works using stored data

---

## Build and Run Instructions

### Linux/macOS

```bash
git clone https://github.com/mugdha-sarker81/to-do-list-app-in-C.git
cd to-do-list-app-in-C
make
./todo
```

Or directly:

```bash
gcc main.c tasks.c habits.c calendar.c export.c -o todo
./todo
```

### Windows with PowerShell

If `make` is available:

```powershell
mingw32-make
mingw32-make run
```

If not, compile directly:

```powershell
gcc main.c tasks.c habits.c calendar.c export.c -o todo.exe
.\todo.exe
```

---

## Command-Line Options

```bash
./todo --help
./todo --tut
./todo --data
./todo --report
./todo --clear
./todo --date DD-MM-YYYY
./todo --habit
```

### Option Details

- `--help` : show available commands
- `--tut` : print a quick usage tutorial
- `--data` : show where the saved data is stored
- `--report` : generate and open the HTML task report
- `--clear` : delete saved task and habit data
- `--date DD-MM-YYYY` : view tasks for a specific day
- `--habit` : jump directly into the habits dashboard

---

## Example User Flow

### Add a task

1. Start the app
2. Select `1` from the menu
3. Enter the date in `DD-MM-YYYY`
4. Enter the task title
5. Choose a priority
6. The task is saved to `tasks.txt`

### Mark a task as done

1. View pending tasks
2. Choose the task to complete
3. Enter the task ID
4. The task status changes to `Done`

### Search tasks by date

```bash
./todo --date 25-10-2026
```

### Update habit progress

1. Open the habit dashboard
2. Add a habit
3. Mark the habit complete for today
4. View the performance and streak stats

### Generate a report

```bash
./todo --report
```

This generates a visual HTML page with task progress information.

---

## Data File Format

### tasks.txt

Example:

```text
1|15-10-2026|Finish assignment|High|Pending
2|16-10-2026|Buy groceries|Medium|Done
3|18-10-2026|Read chapter 5|Low|Pending
```

Format:

```text
id|date|title|priority|status
```

### habits.txt

Example:

```text
1|Morning Run|06:30
2|Read 20 pages|21:00
```

Format:

```text
id|habit_name|time
```

### habit_log.txt

Example:

```text
2026-10-10|1|1
2026-10-10|2|0
```

Format:

```text
YYYY-MM-DD|habitId|completed
```

---

## Feature Highlights in Summary

This project is more than a simple to-do list. It includes:

- Full task lifecycle management
- Priority-based organization
- Date search and scheduling support
- Visual calendar planning
- Daily habit tracking
- Streak and performance monitoring
- HTML report generation
- Persistent storage
- Multi-file modular C architecture
- Command-line support for quick usage

---

## Why This Project Is Useful

This project demonstrates several important C programming concepts in a practical way:

- dynamic memory allocation
- file I/O
- string handling
- data structures
- sorting and searching
- modular code design
- menu-driven application flow
- user input validation
- data persistence and analytics

It is suitable for:

- academic C assignments
- personal productivity tools
- beginner-to-intermediate software engineering practice
- learning how to build menu-driven terminal applications

---

## Notes

- The app is optimized for terminal usage and works best in a console environment.
- Some behavior, such as clearing the screen, uses system commands and may vary slightly across operating systems.
- The generated HTML report is saved in the same directory as the application.
- Use `--clear` carefully because it removes saved progress files.

---

## Quick Start

```bash
make
./todo
```

Then choose from the menu and begin managing tasks and habits immediately.

---

## License and Usage

This project is intended for educational and personal use. It can be modified and extended according to your needs.
