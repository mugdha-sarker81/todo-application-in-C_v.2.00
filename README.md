# Task Manager, Habit Tracker & Team Work

A terminal-based productivity application written in C. Manage dated tasks, track daily habits and streaks, and organize project progress with a simple team dashboard. Data is saved locally in text files in the directory where the program is run.

> This is a local, educational productivity program. Team accounts and project records are stored on the same computer; the application does not provide a network service or synchronize data between users or devices.

## Contents

- [Features](#features)
- [Requirements](#requirements)
- [Build](#build)
- [Run](#run)
- [Using the application](#using-the-application)
- [Command-line reference](#command-line-reference)
- [Data storage and formats](#data-storage-and-formats)
- [Project layout](#project-layout)
- [Implementation notes and limits](#implementation-notes-and-limits)
- [Troubleshooting](#troubleshooting)
- [Privacy and security](#privacy-and-security)
- [License](#license)

## Features

### Task manager

- Add tasks with a due date, title, and priority (`High`, `Medium`, or `Low`).
- Assign each task an automatically generated numeric ID and the initial `Pending` status.
- View pending tasks, search tasks by date, mark a task as done, or delete a task.
- Keep tasks ordered by date and then by ID when the application saves them.
- View a month at a glance, with pending task dates colored by their highest priority:
  - **Red:** High
  - **Yellow:** Medium
  - **Green:** Low

Task dates are entered as `DD-MM-YYYY`. The current validator checks the format and basic ranges (month 1–12 and day 1–31); it does not reject every impossible calendar date, such as 31 February.

### Habit tracker

- Add a habit with a name and scheduled time in `HH:MM` 24-hour format.
- Show today's habits and mark each habit done or not done for today.
- Delete a habit.
- Review today's completion percentage, a current streak, and the last seven days of completion rates.
- Identify habits with a completion rate below 40% over that seven-day period.

A day counts toward the current streak when at least 80% of the current habits are marked complete. The application supports up to 10 habits and stores up to 5,000 completion log entries.

### Team work

- Register a local username and password, or log in to an existing local account.
- Create projects and open projects associated with the logged-in username.
- Add progress messages to a project, view its history, or delete a message.
- Add collaborator usernames to a project.

The current implementation allows up to 15 projects and 500 progress messages in memory. Collaborators are usernames entered by the project member; the application does not send invitations or share project data over a network.

## Requirements

- A C compiler compatible with the GNU C build used by this project.
- GNU Make, or MinGW Make on Windows.
- A terminal or console for the interactive menus.

The Makefile compiles with `-std=gnu11 -Wall -Wextra`. The HTML exporter uses a GNU-compatible raw-string extension, so a compiler that rejects that extension may need a code change before it can build this project.

## Build

Clone the repository and enter its directory:

```sh
git clone https://github.com/mugdha-sarker81/todo-application-in-C_v.2.00.git
cd todo-application-in-C_v.2.00
```

### Linux and macOS

With GNU Make and GCC installed:

```sh
make
```

The executable is named `todo`.

### Windows with MinGW

Open a MinGW or Git Bash terminal in the project directory and run:

```sh
mingw32-make
```

The executable is named `todo.exe`.

If the compiler reports that it cannot write the executable, close any running copy of the app and retry the build.

### Build directly with GCC

The same source files can be compiled without Make:

```sh
gcc -std=gnu11 -Wall -Wextra main.c tasks.c habits.c calendar.c export.c team.c team_auth.c -o todo
```

On Windows, use `-o todo.exe` instead:

```powershell
gcc -std=gnu11 -Wall -Wextra main.c tasks.c habits.c calendar.c export.c team.c team_auth.c -o todo.exe
```

## Run

Start the interactive home menu:

```sh
./todo
```

On Windows PowerShell:

```powershell
.\todo.exe
```

Or run it through Make:

```sh
make run
```

On Windows:

```powershell
mingw32-make run
```

The home menu opens the Task Manager, Habit Tracker, or Team Work dashboard. Select the exit option to close the application.

> Run the program from the directory where you want its text data files to live. File paths are relative to the current working directory, not automatically to the executable's location.

## Using the application

### Tasks

1. Open **Task Manager** from the home menu.
2. Select **Add Task**.
3. Enter a date as `DD-MM-YYYY`, a title, and one of `High`, `Medium`, or `Low`.
4. Use **View All Pending Tasks** to find task IDs.
5. Use **Search Task by Date** to find all tasks on a specific date.
6. Use **Mark a Task done** or **Delete a Task** and enter the task ID.
7. Select **Show Calendar** and enter the month and year to see pending task dates.
8. Select **Export to HTML report** to create a report from saved task data.

Completed tasks are hidden from the pending-task list but remain in `tasks.txt` and can appear in date searches and HTML reports.

### Habits

1. Open **Habit Tracker**.
2. Select **Add Habit**, then enter its name and a time such as `07:30`.
3. Select **Show Today's Habits** to see the current completion state and habit IDs.
4. Select **Mark Habit Done** and enter a habit ID to toggle its completion for today.
5. Select **Show Performance** to see seven-day rates and neglected habits.

Marking a habit a second time on the same day toggles it back to incomplete. Habit records and daily completion history are stored separately, so deleting a habit does not automatically erase its old log entries.

### Team work

1. Open **Team Work** from the home menu, or launch with `--team`.
2. Choose **Register** to create a local account, or **Login** to use an existing account.
3. After successful registration or login, choose an existing project or choose `0` to create one.
4. In a project, add a progress message, delete a previous message, add a collaborator, switch projects, or return to the home menu.

Project membership is matched against the username exactly. Adding a collaborator records the username in the project; it does not create the collaborator's account.

### HTML report

The report command reads `tasks.txt`, writes `report.html` in the current working directory, and asks the operating system to open the file in a browser. The report includes task titles, due dates, completion counts, and a progress graphic. A report cannot be generated when there is no readable task file or when the task file is empty.

The report is generated output. It is not required to build or run the application and can be regenerated with `--report` or the task dashboard.

## Command-line reference

Use the executable name appropriate for the operating system:

| Command | Description |
| --- | --- |
| `todo --help` or `todo -h` | Show the available command-line options. |
| `todo --tut` | Print the built-in quick tutorial. |
| `todo --data` | Print the task and habit data-file names. |
| `todo --date DD-MM-YYYY` | Print tasks scheduled for the specified date. |
| `todo --habit` | Open the Habit Tracker directly. |
| `todo --team` | Open the Team Work dashboard directly. |
| `todo --report` | Generate and open the HTML task report. |
| `todo --clear` | Remove `tasks.txt`, `habits.txt`, and `habit_log.txt`. |

For Linux and macOS, prefix the command with `./`, for example `./todo --help`. In PowerShell, use `.\todo.exe --help`.

`--clear` does not remove Team Work files. It also does not ask for confirmation, so make backups before using it if you want to keep task or habit data.

## Data storage and formats

The app uses plain-text files in its current working directory. Files are created or updated when the corresponding feature saves data. Back up these files before deleting, editing, or moving them.

| File | Contents | Record format |
| --- | --- | --- |
| `tasks.txt` | Tasks and their status | `id|DD-MM-YYYY|title|priority|status` |
| `habits.txt` | Habit definitions | `id|name|HH:MM` |
| `habit_log.txt` | Completion state for a habit on a date | `YYYY-MM-DD|habit_id|completed` |
| `team_users.txt` | Local team accounts | `username|password` |
| `team_project_name.txt` | Project IDs, names, and comma-separated members | `project_id|project_name|member1,member2` |
| `commit_msg.txt` | Team project progress entries | `project_id|message|username|HH:MM|DD-MM-YYYY` |
| `report.html` | Generated task report | HTML document |

Example records below show the format only; replace the values with your own data:

```text
# tasks.txt
1|24-10-2026|Finish project README|High|Pending

# habits.txt
1|Read for 20 minutes|07:30

# habit_log.txt
2026-10-24|1|1

# team_project_name.txt
p1|Website redesign|alice,bob

# commit_msg.txt
p1|Drafted the home page|alice|09:45|24-10-2026
```

The `|` character separates fields in these formats. Avoid using it in values that are saved in these files. Team account passwords are saved as plain text, not encrypted or hashed.

The current `.gitignore` ignores untracked text files and generated reports by default. This helps avoid adding local data and account files by mistake. Files already tracked by Git remain tracked until you explicitly remove them from the index.

## Project layout

```text
.
├── main.c          # Program entry point, home menu, CLI options, task dashboard
├── tasks.c         # Task records, operations, sorting, and persistence
├── tasks.h         # Task structure and declarations
├── habits.c        # Habit tracking, completion logs, streaks, performance
├── habits.h        # Habit structures, limits, and declarations
├── calendar.c      # Monthly calendar and pending-task priority colors
├── calendar.h      # Calendar declaration
├── export.c        # HTML task report generation
├── export.h        # Report generator declaration
├── team.c          # Projects, collaborators, and progress history
├── team.h          # Team dashboard declaration
├── team_auth.c     # Local registration and login
├── team_auth.h     # Authentication declarations
├── Makefile        # GNU Make build rules
├── README.md       # Project documentation
└── .gitignore      # Build and local-data ignore rules
```

Object files, executables, runtime text data, and the generated HTML report may appear after building or using the application; they are not source files.

## Implementation notes and limits

- **Language and build:** C compiled with GCC-compatible GNU C mode (`gnu11`).
- **Task storage:** Tasks are loaded into a dynamically allocated array and sorted by due date and then task ID when saved.
- **Habit storage:** Habit definitions and daily completion records use separate arrays and files.
- **Calendar:** The calendar displays pending tasks only; if multiple pending tasks fall on a date, the date is shown using the highest priority found.
- **Team work:** Projects and progress messages are local records. There is no remote server, multi-device synchronization, or access control between operating-system users who can read the data files.
- **Input formats:** Task and habit data are delimiter-separated text rather than a database. Editing them manually can make records unreadable if separators or malformed values are introduced.
- **Screen clearing:** The current implementation uses the Windows `cls` command, so screen-clearing behavior may need adjustment on non-Windows terminals.

## Troubleshooting

### `mingw32-make` is not recognized

Install a MinGW GCC toolchain that includes MinGW Make, or compile directly with the GCC command in [Build](#build). Ensure `gcc` is available in the terminal's `PATH`.

### The linker cannot write `todo.exe`

Close any running copy of the application and retry. Windows may lock an executable while it is running.

### The app says it cannot find saved data

Run the app from the directory containing the data files, or create new records so the app can save files there. Data paths are relative to the terminal's current directory.

### Team login fails

Register the username first. Accounts are local to the `team_users.txt` file in the current working directory. Check that the app is being launched from the same data directory in which the account was created.

### A project is missing after login

The project is listed only for usernames recorded in that project's member list. Check `team_project_name.txt` and confirm the username matches exactly.

### The HTML report does not open

Check that `tasks.txt` exists and contains task records. The program creates `report.html` in the current directory; if the automatic browser launch fails, open that file manually.

## Privacy and security

- Team account passwords are stored in plain text in `team_users.txt`. Do not use a password that protects another account or service.
- These local accounts are for basic application flow, not secure authentication.
- Anyone who can read the data directory can read or modify the app's saved data.
- Avoid committing real task, habit, team, or account files to a public repository.
- The `--clear` option permanently removes the task and habit files listed in the command reference; back them up first if needed.

## License

No license file is currently included. Ask the repository owner before redistributing this project or incorporating it into another project.
