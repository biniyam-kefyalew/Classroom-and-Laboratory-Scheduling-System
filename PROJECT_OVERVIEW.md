# Bahir Dar University - Classroom and Laboratory Scheduling System

## Project Overview

A comprehensive, menu-driven C console application designed to manage classroom and laboratory scheduling for Bahir Dar University. The system implements intelligent priority-based scheduling with automatic conflict detection and resolution.

## What's Included

### Core Application
- **scheduling_system.c** - Complete C implementation (1000+ lines)
- **Makefile** - Build automation
- **package.json** - NPM integration for easy building

### Documentation
- **README.md** - Quick start and feature overview
- **USAGE_GUIDE.md** - Detailed step-by-step usage instructions with examples
- **FEATURES.md** - Comprehensive technical feature documentation
- **PROJECT_OVERVIEW.md** - This file

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

1. **Add Rooms** (Room Management → Add Room)
   - Create classrooms and labs
   - Minimum: 3-5 rooms recommended

2. **Add Schedules** (Schedule Management → Add Schedule)
   - Enter course details
   - Specify room type requirements
   - Set time slots

3. **Generate Schedule** (Schedule Management → Generate Final Schedule)
   - Automatic room assignment
   - Priority-based allocation
   - Conflict-free scheduling

4. **View Results** (Schedule Management → View Final Schedule)
   - Organized by room
   - Complete course information
   - Clear, readable format

## Key Features

### ✅ Full CRUD Operations
- **Rooms**: Create, Read, Update, Delete
- **Schedules**: Create, Read, Update, Delete

### 🎯 Intelligent Scheduling
- **Priority System**:
  - Higher student years get priority
  - Major courses prioritized over minor
  - Lab courses get additional priority
- **Conflict Prevention**:
  - No double-booking
  - No time overlaps
  - Room type enforcement (labs for lab courses only)

### 📊 Multiple Viewing Options
- Complete schedule list
- Final schedule grouped by room
- Filter by specific room
- Filter by department

### 💾 Data Persistence
- Binary file storage
- Automatic saving
- Data survives between sessions

### 🎨 User-Friendly Interface
- Menu-driven navigation
- Clear prompts and feedback
- Formatted table displays
- Visual indicators (✓, ✗, ⚠)

## System Architecture

```
┌─────────────────────────────────────────┐
│         Main Menu                       │
├─────────────────────────────────────────┤
│  1. Room Management                     │
│     - Display/Add/Edit/Delete Rooms     │
│  2. Schedule Management                 │
│     - Display/Add/Edit/Delete Schedules │
│     - Generate Final Schedule           │
│     - View Results (Multiple Ways)      │
└─────────────────────────────────────────┘
              ↓
┌─────────────────────────────────────────┐
│      Scheduling Algorithm               │
├─────────────────────────────────────────┤
│  1. Calculate priorities                │
│  2. Sort by priority (descending)       │
│  3. Assign rooms (highest priority first│
│  4. Check conflicts                     │
│  5. Match room types                    │
└─────────────────────────────────────────┘
              ↓
┌─────────────────────────────────────────┐
│      Data Persistence Layer             │
├─────────────────────────────────────────┤
│  - rooms.dat (binary file)              │
│  - schedules.dat (binary file)          │
└─────────────────────────────────────────┘
```

## Priority Calculation

The system uses a weighted priority formula:

```
Priority = (Student Year × 10) + Program Bonus + Room Type Bonus

Where:
- Student Year: 1-5 (×10 = 10-50 points)
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

## Conflict Resolution

### Time Conflicts
```
Schedule A: 08:00-10:00 → Room 1 ✓ Assigned
Schedule B: 09:00-11:00 → Room 1 ✗ Conflict! → Room 2 ✓ Assigned
```

### Room Type Enforcement
```
Lab Course → Can only use Lab rooms
Classroom Course → Can only use Classroom rooms
Lab Course + No Lab Available → Remains unassigned (won't use classroom)
```

## File Structure

```
project/
├── scheduling_system.c    # Main application (1000+ lines)
├── Makefile              # Build configuration
├── package.json          # NPM scripts
├── README.md             # Quick reference
├── USAGE_GUIDE.md        # Detailed instructions
├── FEATURES.md           # Technical documentation
├── PROJECT_OVERVIEW.md   # This file
├── rooms.dat             # Generated: Room data
└── schedules.dat         # Generated: Schedule data
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
    int start_time;          // Minutes since midnight
    int end_time;
    int assigned_room_id;
    int is_active;
    int priority;
} Schedule;
```

### Capacity
- **Maximum Rooms**: 100
- **Maximum Schedules**: 500
- **Easily Configurable**: Change MAX_ROOMS and MAX_SCHEDULES constants

### Performance
- **Room Operations**: O(n) - Linear search
- **Schedule Generation**: O(n × m) where n = schedules, m = rooms
- **Priority Sorting**: O(n log n) - Uses qsort
- **Typical Performance**: Instant for standard university department sizes

## Use Cases

### Scenario 1: Computer Science Department
```
Rooms:
- 2 Computer Labs (Lab)
- 3 Lecture Halls (Classroom)

Schedules:
- 10 theory courses
- 5 lab courses
- Multiple time slots

Result: All courses assigned with no conflicts
```

### Scenario 2: Engineering College
```
Rooms:
- Physics Lab
- Chemistry Lab
- Engineering Lab
- 5 Classrooms

Schedules:
- Mix of lab and theory courses
- Different years (1-5)
- Major and minor programs

Result: Priority-based assignments, senior labs get best time slots
```

### Scenario 3: Multi-Department
```
Departments:
- Computer Science
- Information Systems
- Software Engineering

Process:
1. Add all rooms
2. Add schedules from all departments
3. Generate final schedule
4. View by department to verify
```

## Development Notes

### Code Organization
- **Lines of Code**: ~1000
- **Functions**: 30+
- **Modular Design**: Separate functions for each operation
- **Clear Separation**: UI, logic, and data layers

### Design Principles Applied
1. **Single Responsibility**: Each function does one thing well
2. **DRY**: Reusable utility functions (formatting, validation)
3. **User-Centric**: Clear prompts, helpful feedback
4. **Robust**: Error handling, input validation
5. **Maintainable**: Well-commented, logical structure

### Extensibility
The system is designed for easy extension:
- Add new room types (modify RoomType enum)
- Change priority formula (modify priority calculation)
- Add new filters (create new display functions)
- Extend data fields (add to structs)

## Testing Recommendations

### Basic Testing
1. Add 5 rooms (3 classrooms, 2 labs)
2. Add 10 schedules (varying priorities)
3. Generate final schedule
4. Verify no conflicts
5. Test all viewing options

### Advanced Testing
1. Create conflicts intentionally (same time, same room type)
2. Verify priority system (higher priority gets better assignment)
3. Test room type enforcement (lab courses only in labs)
4. Test edge cases (no available rooms, invalid times)
5. Test data persistence (exit and restart)

### Stress Testing
1. Add maximum rooms (100)
2. Add many schedules (200+)
3. Generate schedule
4. Measure performance
5. Check memory usage

## Troubleshooting

### Common Issues

**Issue**: Compilation error
```bash
Solution: Ensure GCC is installed
$ gcc --version
$ sudo apt install gcc  # Ubuntu/Debian
$ brew install gcc      # macOS
```

**Issue**: Permission denied when running
```bash
Solution: Make executable
$ chmod +x scheduling_system
```

**Issue**: Data files not found
```bash
Solution: Run from project directory
$ cd /path/to/project
$ ./scheduling_system
```

**Issue**: Course not assigned
```bash
Solution:
1. Check if appropriate room type exists
2. Verify time slot availability
3. Add more rooms if needed
```

## Future Enhancements

### Potential Features
1. **Weekly Scheduling**: Monday-Friday support
2. **Instructor Management**: Assign instructors to courses
3. **Room Capacity**: Validate student count vs room size
4. **Export Functions**: Generate PDF/CSV reports
5. **Undo/Redo**: Track changes and allow reversal
6. **Search**: Find courses, rooms by keyword
7. **Statistics**: Room utilization, department workload
8. **Multi-semester**: Plan across academic terms

### Integration Possibilities
1. **Web Backend**: Convert to REST API
2. **Database**: PostgreSQL/MySQL integration
3. **Web Frontend**: React/Angular interface
4. **Mobile App**: iOS/Android client
5. **Email Notifications**: Send schedule updates

## Educational Value

This project demonstrates:
- **Data Structures**: Structs, arrays, enums
- **File I/O**: Binary file operations
- **Algorithms**: Sorting, conflict detection, priority scheduling
- **Software Engineering**: Modular design, error handling
- **User Interface**: Menu-driven console application
- **Problem Solving**: Real-world scheduling problem

## Credits

Developed as a comprehensive scheduling solution for Bahir Dar University to manage classroom and laboratory allocations efficiently and fairly using priority-based scheduling.

## License

Educational project for Bahir Dar University.

## Support

For questions or issues:
1. Check USAGE_GUIDE.md for detailed instructions
2. Review FEATURES.md for technical details
3. Consult README.md for quick reference

---

**Version**: 1.0.0
**Last Updated**: December 2024
**Status**: Production Ready
