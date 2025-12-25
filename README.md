# Classroom and Laboratory Scheduling System
## Bahir Dar University

A comprehensive menu-driven C console application for managing classroom and laboratory scheduling with intelligent priority-based scheduling algorithm.

## Features

### Room Management (Full CRUD)
- **Display Rooms**: View all available rooms with their types
- **Add Room**: Create new classroom or laboratory spaces
- **Edit Room**: Modify room details (name, type)
- **Delete Room**: Remove rooms from the system

### Schedule Management (Full CRUD)
- **Display Schedules**: View all schedules in a detailed table format
- **Add Schedule**: Create new course schedules with:
  - Course name
  - Department
  - Student year (1-5)
  - Program type (Major/Minor)
  - Required room type (Classroom/Lab)
  - Time range (start and end times)
- **Edit Schedule**: Modify existing schedule details
- **Delete Schedule**: Remove schedules from the system

### Intelligent Scheduling Algorithm

The system implements a **Priority-Based Scheduling Algorithm** that:

1. **Calculates Priority Scores**:
   - Student Year × 10 (Higher years get higher priority)
   - +5 points for Major courses
   - +3 points for Lab courses

2. **Applies Scheduling Rules**:
   - Sorts all schedules by priority (highest first)
   - Assigns rooms based on priority order
   - Lab courses MUST be assigned to lab rooms only
   - Classroom courses are assigned to classroom spaces
   - Prevents room and time conflicts automatically

3. **Conflict Detection**:
   - Checks for overlapping time slots in the same room
   - Ensures no double-booking of spaces

### Viewing Options

- **Final Schedule**: Clean, organized display of all assigned schedules grouped by room
- **Schedule by Room**: Filter schedules for a specific room
- **Schedule by Department**: View all schedules for a particular department

## Compilation and Running

### Using Make (Recommended)

```bash
# Compile the program
make

# Run the program
make run

# Clean compiled files and data
make clean
```

### Manual Compilation

```bash
# Compile
gcc -Wall -Wextra -std=c99 scheduling_system.c -o scheduling_system

# Run
./scheduling_system
```

## Usage Guide

### Getting Started

1. **Launch the program**: Run `./scheduling_system`
2. **Start with Room Management**: Add rooms before creating schedules
   - Add at least one classroom
   - Add at least one lab (if you plan to schedule lab courses)

3. **Create Schedules**: Add course schedules with all required details
4. **Generate Final Schedule**: Run the automatic scheduling algorithm
5. **View Results**: Check the final schedule or filter by room/department

### Example Workflow

1. Add Rooms:
   ```
   Room 1: Science Lab 101 (Lab)
   Room 2: Lecture Hall A (Classroom)
   Room 3: Computer Lab 202 (Lab)
   Room 4: Classroom B-12 (Classroom)
   ```

2. Add Schedules:
   ```
   Course: Data Structures
   Department: Computer Science
   Year: 3
   Program: Major
   Room Type: Classroom
   Time: 08:00 - 10:00
   ```

3. Generate Final Schedule
   - System automatically assigns rooms based on priority
   - Prevents conflicts
   - Matches lab courses to lab rooms

4. View Results
   - Check final schedule grouped by room
   - Filter by specific room or department

## Priority Examples

| Course | Year | Program | Room Type | Priority Score |
|--------|------|---------|-----------|----------------|
| Advanced Programming | 5 | Major | Lab | 58 (50+5+3) |
| Database Systems | 4 | Major | Classroom | 45 (40+5+0) |
| Web Development | 3 | Major | Lab | 38 (30+5+3) |
| Intro to CS | 2 | Major | Classroom | 25 (20+5+0) |
| Python Basics | 2 | Minor | Lab | 23 (20+0+3) |
| General Education | 1 | Minor | Classroom | 10 (10+0+0) |

## Data Persistence

The system uses binary file storage:
- `rooms.dat`: Stores room information
- `schedules.dat`: Stores schedule information

Data persists between sessions automatically.

## System Requirements

- C Compiler (GCC recommended)
- Unix-like system (Linux, macOS) or Windows with MinGW
- Terminal/Console access

## Key Features

✓ Complete CRUD operations for rooms and schedules
✓ Intelligent priority-based scheduling algorithm
✓ Automatic conflict detection and prevention
✓ Lab room type enforcement
✓ Multiple viewing and filtering options
✓ Persistent data storage
✓ Clean, user-friendly console interface
✓ Input validation and error handling

## Notes

- Time format: 24-hour format (HH:MM)
- Student years: 1-5
- Lab courses are automatically given priority and must be assigned to lab rooms
- The system prevents double-booking and time conflicts
- Deleted rooms automatically unassign affected schedules
