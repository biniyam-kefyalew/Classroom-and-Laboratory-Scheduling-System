# Bahir Dar University - Classroom and Laboratory Scheduling System

## Project Overview

A comprehensive, menu-driven C console application designed to manage classroom and laboratory scheduling for Bahir Dar University. The system implements intelligent priority-based scheduling with automatic weekly timetable generation, lab room priority, and conflict detection.

## What's Included

### Core Application
- **scheduling_system.h** - Header file with data structures and function declarations
- **scheduling_system.c** - Complete C implementation (1100+ lines)
- **web_interface.c** - CGI-based web interface
- **Makefile** - Build automation
- **package.json** - NPM integration for easy building

### Documentation
- **USAGE_GUIDE.md** - Detailed step-by-step usage instructions with examples
- **FEATURES.md** - Comprehensive technical feature documentation
- **PROJECT_OVERVIEW.md** - This file
- **QUICKSTART.md** - Quick start guide

## Quick Start

### Compile and Run

```bash
# Using Make
make
./scheduling_system

# Or using NPM
npm run build
npm start
```

### Basic Workflow

1. **Add Rooms** (Room Management -> Add Room)
   - Create classrooms and labs
   - Minimum: 3-5 rooms recommended

2. **Add Schedules** (Schedule Management -> Add Schedule)
   - Enter course details
   - Specify room type requirements
   - Set **weekly hours** (e.g., 3 hrs/week, 5 hrs/week)

3. **Generate Weekly Schedule** (Schedule Management -> Generate Weekly Schedule)
   - Automatic room assignment
   - Priority-based allocation
   - Two-phase assignment (lab priority, then classroom fallback)
   - Distributes hours across Monday-Friday

4. **View Results**
   - Weekly Timetable (day-by-day view)
   - Final Schedule (grouped by room)
   - By Department or By Room filters

## Key Features

### Full CRUD Operations
- **Rooms**: Create, Read, Update, Delete
- **Schedules**: Create, Read, Update, Delete

### Intelligent Scheduling
- **Weekly Hours System**:
  - Specify total hours per week (e.g., 5 hours)
  - System automatically distributes across Monday-Friday
  - Time slots: 08:00-12:30 (morning), 14:00-17:30 (afternoon)

- **Priority System**:
  - Higher student years get priority
  - Major courses prioritized over minor
  - Lab courses get additional priority (+3)

- **Two-Phase Room Assignment**:
  - Phase 1: Lab courses -> Labs, Classroom courses -> Classrooms
  - Phase 2: If classrooms full, classroom courses can use available labs
  - Labs are NEVER used by classroom courses until all classrooms are full

- **Conflict Prevention**:
  - No double-booking of rooms
  - No student conflicts (same dept/year can't have overlapping classes)
  - Room type enforcement (lab courses must use labs)

### Multiple Viewing Options
- Weekly timetable (Monday-Friday view)
- Complete schedule list
- Final schedule grouped by room
- Filter by specific room
- Filter by department

### Data Persistence
- Binary file storage
- Automatic saving
- Data survives between sessions

### User-Friendly Interface
- Menu-driven navigation
- Clear prompts and feedback
- Formatted table displays
- Visual status indicators

## System Architecture

```
+--------------------------------------------+
|         Main Menu                          |
+--------------------------------------------+
|  1. Room Management                        |
|     - Display/Add/Edit/Delete Rooms        |
|  2. Schedule Management                    |
|     - Display/Add/Edit/Delete Schedules    |
|     - Generate Weekly Schedule             |
|     - View Weekly Timetable                |
|     - View Final Schedule                  |
|     - View by Room / Department            |
+--------------------------------------------+
              |
              v
+--------------------------------------------+
|      Two-Phase Scheduling Algorithm        |
+--------------------------------------------+
|  Phase 1: Preferred Room Types             |
|    - Lab courses -> Lab rooms only         |
|    - Classroom courses -> Classrooms only  |
|                                            |
|  Phase 2: Lab Fallback                     |
|    - Unassigned classroom courses can use  |
|      available lab rooms                   |
+--------------------------------------------+
              |
              v
+--------------------------------------------+
|      Data Persistence Layer                |
+--------------------------------------------+
|  - rooms.dat (room records)                |
|  - schedules.dat (schedule records)        |
|  - generated_slots.dat (weekly timetable)  |
+--------------------------------------------+
```

## Priority Calculation

The system uses a weighted priority formula:

```
Priority = (Student Year x 10) + Program Bonus + Room Type Bonus

Where:
- Student Year: 1-5 (x10 = 10-50 points)
- Program: Major (+5) or Minor (+0)
- Room Type: Lab (+3) or Classroom (+0)
```

### Priority Examples

| Course Example | Year | Program | Room | Priority | Reason |
|----------------|------|---------|------|----------|--------|
| Advanced Lab Project | 5 | Major | Lab | 58 | Highest: Senior major lab |
| Senior Seminar | 5 | Major | Classroom | 55 | High: Senior major theory |
| Junior Lab | 3 | Major | Lab | 38 | Medium: Mid-level major lab |
| Intro Course | 1 | Minor | Classroom | 10 | Lowest: Freshman minor theory |

## Room Assignment Rules

### Lab Courses
- MUST always use lab rooms
- Cannot be assigned to classrooms (need specialized equipment)
- Get +3 priority bonus to ensure they're processed first

### Classroom Courses
- PREFER classroom rooms (Phase 1)
- CAN use lab rooms as fallback if classrooms are full (Phase 2)
- This maximizes room utilization

### Example Scenario
```
Available Rooms:
- 2 Classrooms
- 2 Labs

Courses to Schedule:
- 3 Lab courses (must use labs)
- 5 Classroom courses (prefer classrooms)

Result:
- 3 Lab courses -> 2 Labs (assigned)
- 5 Classroom courses:
  - 2 -> Classrooms (preferred)
  - 2 -> Labs (fallback, since classrooms full)
  - 1 -> May be unassigned if all rooms full at that time
```

## Conflict Resolution

### Room Conflicts
```
Schedule A: Monday 08:00-09:00 -> Room 1 (Assigned)
Schedule B: Monday 08:30-09:30 -> Room 1 (Conflict!) -> Room 2 (Assigned)
```

### Student Conflicts
```
CS Year 3 - Database: Monday 08:00-09:00 (Assigned)
CS Year 3 - Networks: Monday 08:00-09:00 (Conflict - same students!)
  -> Assigned to different time slot
```

### Room Type Priority
```
Lab Course + No Lab Available -> Remains unassigned (won't use classroom)
Classroom Course + No Classroom -> Can use Lab (Phase 2 fallback)
```

## File Structure

```
project/
|-- scheduling_system.h    # Header file
|-- scheduling_system.c    # Main application (1100+ lines)
|-- web_interface.c        # Web CGI interface
|-- Makefile               # Build configuration
|-- package.json           # NPM scripts
|-- USAGE_GUIDE.md         # Detailed instructions
|-- FEATURES.md            # Technical documentation
|-- PROJECT_OVERVIEW.md    # This file
|-- QUICKSTART.md          # Quick start guide
|-- rooms.dat              # Generated: Room data
|-- schedules.dat          # Generated: Schedule data
+-- generated_slots.dat    # Generated: Weekly timetable
```

## Technical Specifications

### Language & Standards
- **Language**: C
- **Standard**: C99
- **Compiler**: GCC (or compatible)

### Data Structures
```c
typedef struct {
    int id;
    char name[100];
    RoomType type;           // CLASSROOM or LAB
    int is_active;
} Room;

typedef struct {
    int id;
    char course_name[100];
    char department[50];
    int student_year;        // 1-5
    ProgramType program_type; // MAJOR or MINOR
    RoomType required_room_type;
    int weekly_hours;        // Hours per week (e.g., 3, 5)
    int assigned_room_id;
    int is_active;
    int priority;
} Schedule;

typedef struct {
    int schedule_id;         // Reference to schedule
    int room_id;             // Assigned room
    DayOfWeek day;           // MONDAY-FRIDAY
    int start_time;          // Minutes from midnight
    int end_time;
    int is_active;
} GeneratedSlot;
```

### Capacity
- **Maximum Rooms**: 100
- **Maximum Schedules**: 500
- **Maximum Generated Slots**: 2000
- **Easily Configurable**: Change MAX_* constants

### Time Configuration
- **Days**: Monday - Friday (5 days)
- **Morning**: 08:00 - 12:30
- **Afternoon**: 14:00 - 17:30
- **Slot Duration**: 1 hour

## Use Cases

### Scenario 1: Computer Science Department
```
Rooms:
- 2 Computer Labs (Lab)
- 3 Lecture Halls (Classroom)

Schedules:
- 10 theory courses (3-5 hrs/week each)
- 5 lab courses (2-3 hrs/week each)

Result: 
- Lab courses assigned to labs first
- Theory courses fill classrooms
- Overflow theory courses use empty lab slots
```

### Scenario 2: Multi-Department
```
Departments:
- Computer Science (10 courses)
- Information Systems (8 courses)
- Software Engineering (6 courses)

Process:
1. Add all rooms
2. Add schedules with weekly hours
3. Generate weekly schedule
4. View by department to verify each
```

## Development Notes

### Code Organization
- **Lines of Code**: ~1100 (scheduling_system.c)
- **Functions**: 35+
- **Modular Design**: Separate functions for each operation
- **Clear Separation**: UI, logic, and data layers

### Design Principles Applied
1. **Two-Phase Assignment**: Ensures lab priority while maximizing utilization
2. **Student Conflict Prevention**: Same dept/year cannot overlap
3. **Single Responsibility**: Each function does one thing well
4. **User-Centric**: Clear prompts, helpful feedback
5. **Robust**: Error handling, input validation

## Troubleshooting

### Common Issues

**Issue**: Compilation error with strcasecmp
```bash
Solution: Ensure #include <strings.h> is present
```

**Issue**: Lab course not assigned
```bash
Solution: Add more lab rooms - lab courses cannot use classrooms
```

**Issue**: Classroom course not assigned
```bash
Solution: 
1. Add more classrooms, OR
2. Wait for Phase 2 to assign to available labs
```

**Issue**: Student conflict warning
```bash
Solution: Same department/year courses have overlapping times
- Reduce weekly hours, or
- Add more rooms
```

## Credits

Developed as a comprehensive scheduling solution for Bahir Dar University to manage classroom and laboratory allocations efficiently and fairly using priority-based scheduling with lab room priority.

## License

Educational project for Bahir Dar University.

---

**Version**: 2.0.0
**Last Updated**: January 2026
**Status**: Production Ready
