# Bahir Dar University - Classroom and Laboratory Scheduling System

## Project Overview

A comprehensive, menu-driven C application designed to manage classroom and laboratory scheduling for Bahir Dar University. The system implements intelligent priority-based scheduling with **automatic distribution of class hours across 5 days**, lab room priority, and conflict detection.

## What's Included

### Core Application
- **scheduling_system.h** - Header file with data structures and function declarations
- **scheduling_system.c** - Complete C implementation (1500+ lines)
- **web_interface.c** - CGI-based web interface (1500+ lines)
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

# Web Interface
make open
```

### Basic Workflow

1. **Add Rooms** (Room Management -> Add Room)
   - Create classrooms and labs
   - Minimum: 3-5 rooms recommended

2. **Add Schedules** (Schedule Management -> Add Schedule)
   - Enter course details
   - Specify room type requirements
   - Set **weekly hours** (e.g., 3 hrs/week, 6 hrs/week)

3. **Generate Schedule** (Generate -> Generate Distributed Schedule)
   - Automatic room assignment
   - **Distributes hours evenly across Mon-Fri**
   - Minimum 90-minute sessions
   - Two-phase assignment (lab priority, then classroom fallback)

4. **View Results**
   - **By Year/Dept**: Weekly schedule for specific student group
   - **By Room**: Select a room to see its weekly schedule
   - **All Rooms**: Overview with utilization statistics
   - **Final Schedule**: Grouped by room

## Key Features

### Full CRUD Operations
- **Rooms**: Create, Read, Update, Delete
- **Schedules**: Create, Read, Update, Delete

### Distributed Scheduling Algorithm (NEW)

The system **automatically distributes class hours evenly across 5 days**:

```
Example: 6-hour course
- Monday:    90 min
- Tuesday:   90 min
- Wednesday: 90 min
- Thursday:  90 min
Total: 6 hours spread across 4 days (not bunched on 1-2 days)

Key Rules:
- Minimum session: 90 minutes (1 hour 30 min)
- Even distribution across Mon-Fri
- Prevents schedule bunching
```

### Priority System
- Higher student years get priority
- Major courses prioritized over minor
- Lab courses get additional priority (+3)

### Two-Phase Room Assignment
- **Phase 1**: Lab courses -> Labs, Classroom courses -> Classrooms
- **Phase 2**: If classrooms full, classroom courses can use available labs
- Labs are NEVER used by classroom courses until all classrooms are full

### Conflict Prevention
- No double-booking of rooms
- No student conflicts (same dept/year can't have overlapping classes)
- Room type enforcement (lab courses must use labs)

### Multiple Viewing Options
- **By Year & Department**: Weekly schedule filtered by student group
- **By Room**: Select room from dropdown, view weekly schedule with utilization
- **All Rooms**: Overview showing hours/day per room with links
- Weekly timetable (Monday-Friday view)
- Final schedule grouped by room

### Data Persistence
- Binary file storage
- Automatic saving
- Data survives between sessions

## System Architecture

```
+--------------------------------------------+
|         Main Menu                          |
+--------------------------------------------+
|  1. Room Management                        |
|  2. Schedule Management                    |
|     - Generate Distributed Schedule        |
|     - View by Year/Dept                    |
|     - View by Room                         |
|     - View All Rooms                       |
+--------------------------------------------+
              |
              v
+--------------------------------------------+
|   Distributed Scheduling Algorithm         |
+--------------------------------------------+
|  1. Sort by priority                       |
|  2. For each course:                       |
|     - Find day with least assigned time    |
|     - Assign 90-min session                |
|     - Repeat until weekly hours met        |
|  3. Phase 2: Lab fallback for classrooms   |
+--------------------------------------------+
              |
              v
+--------------------------------------------+
|      Data Persistence Layer                |
+--------------------------------------------+
|  - rooms.dat                               |
|  - schedules.dat                           |
|  - generated_slots.dat                     |
+--------------------------------------------+
```

## Time Configuration

| Setting | Value |
|---------|-------|
| Days | Monday - Friday (5 days) |
| Morning Session | 08:00 - 12:30 |
| Afternoon Session | 14:00 - 17:30 |
| **Minimum Session Duration** | **90 minutes (1.5 hrs)** |
| Morning Slots | 08:00, 09:30, 11:00 |
| Afternoon Slots | 14:00, 15:30, 17:00 |

## Distribution Examples

### 6-Hour Course
```
Target: 72 min/day (360 min / 5 days)
Actual with 90-min sessions:
  Mon: 90 min | Tue: 90 min | Wed: 90 min | Thu: 90 min
  Total: 360 min = 6 hours
```

### 3-Hour Course
```
Target: 36 min/day
Actual with 90-min sessions:
  Mon: 90 min | Wed: 90 min
  Total: 180 min = 3 hours
```

## Priority Calculation

```
Priority = (Student Year x 10) + Program Bonus + Room Type Bonus

Where:
- Student Year: 1-5 (x10 = 10-50 points)
- Program: Major (+5) or Minor (+0)
- Room Type: Lab (+3) or Classroom (+0)
```

### Priority Examples

| Course | Year | Program | Room | Priority |
|--------|------|---------|------|----------|
| Senior Lab Project | 5 | Major | Lab | 58 |
| Senior Seminar | 5 | Major | Classroom | 55 |
| Junior Lab | 3 | Major | Lab | 38 |
| Intro Course | 1 | Minor | Classroom | 10 |

## Web Interface Navigation

```
Home | Rooms | Add Room | Schedules | Add Schedule | Generate | 
Weekly Timetable | By Year/Dept | By Room | All Rooms | Final Schedule
```

### View Options

| View | Description |
|------|-------------|
| By Year/Dept | Select year and department, see weekly schedule |
| By Room | Dropdown to select room, see utilization summary |
| All Rooms | Table with hours/day per room, links to details |
| Final Schedule | Grouped by room with all assignments |

## File Structure

```
project/
|-- scheduling_system.h    # Header file
|-- scheduling_system.c    # Console application
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

### Capacity
- **Maximum Rooms**: 100
- **Maximum Schedules**: 500
- **Maximum Generated Slots**: 2000

### New Functions for Distribution
```c
web_count_minutes_on_day()      // Track minutes assigned per day
web_calc_ideal_minutes_per_day() // Calculate target distribution  
web_calc_max_minutes_per_day()  // Prevent over-assignment per day
```

## Troubleshooting

### Common Issues

**Issue**: Lab course not assigned
```
Solution: Add more lab rooms - lab courses cannot use classrooms
```

**Issue**: Hours bunched on one day
```
Solution: This should not happen with the new distributed algorithm.
The system finds the day with minimum assigned time before each assignment.
```

**Issue**: "Student conflict" warnings
```
Solution: Too many courses for same dept/year at overlapping times
- Reduce weekly hours or add more rooms
```

## Credits

Developed as a comprehensive scheduling solution for Bahir Dar University to manage classroom and laboratory allocations efficiently using priority-based scheduling with automatic distribution across the week.

## License

Educational project for Bahir Dar University.

---

**Version**: 3.0.0  
**Last Updated**: January 2026  
**Status**: Production Ready  
**New in v3.0**: Distributed scheduling across 5 days, 90-min minimum sessions, By Room view with selection
