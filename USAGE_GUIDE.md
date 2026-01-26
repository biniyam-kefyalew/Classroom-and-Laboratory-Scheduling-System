# Usage Guide - Bahir Dar University Scheduling System

## Quick Start

### Step 1: Compile and Run

```bash
make
./scheduling_system
```

## Detailed Walkthrough

### Main Menu

When you start the program, you'll see:

```
+------------------------------------------------------------+
|     CLASSROOM AND LABORATORY SCHEDULING SYSTEM             |
|            BAHIR DAR UNIVERSITY                            |
+------------------------------------------------------------+

  1. Room Management
  2. Schedule Management
  0. Exit
```

## Part 1: Room Management

### 1.1 Adding Rooms

Before creating schedules, you need to add rooms:

1. Select `1` (Room Management)
2. Select `2` (Add New Room)
3. Enter room details:

**Example Classrooms:**
```
Room Name: Lecture Hall A
Type: 1 (Classroom)

Room Name: Engineering Building Room 205
Type: 1 (Classroom)

Room Name: Science Block - Room 301
Type: 1 (Classroom)
```

**Example Labs:**
```
Room Name: Computer Lab 1
Type: 2 (Lab)

Room Name: Chemistry Laboratory
Type: 2 (Lab)

Room Name: Physics Lab - Block C
Type: 2 (Lab)
```

### 1.2 Viewing Rooms

Select `1` (Display All Rooms) to see your room list:

```
Room ID    Room Name                                Type
---------------------------------------------------------------
1          Lecture Hall A                           Classroom
2          Computer Lab 1                           Lab
3          Engineering Building Room 205            Classroom
4          Chemistry Laboratory                     Lab
```

### 1.3 Editing a Room

1. Select `3` (Edit Room)
2. Enter the Room ID
3. Modify name or type as needed

### 1.4 Deleting a Room

1. Select `4` (Delete Room)
2. Enter the Room ID
3. Confirm deletion

## Part 2: Schedule Management

### 2.1 Adding Schedules (NEW: Weekly Hours)

1. Select `2` (Schedule Management) from main menu
2. Select `2` (Add New Schedule)
3. Enter schedule details:

**Example Schedule 1: High Priority (5th Year Major Lab)**
```
Course Name: Advanced Embedded Systems Lab
Department: Computer Engineering
Student Year: 5
Program Type: 1 (Major)
Room Type: 2 (Lab)
Weekly Hours: 3

Priority Calculated: 58 (50 + 5 + 3)
```

**Example Schedule 2: Medium Priority (3rd Year Major Classroom)**
```
Course Name: Database Management Systems
Department: Computer Science
Student Year: 3
Program Type: 1 (Major)
Room Type: 1 (Classroom)
Weekly Hours: 4

Priority Calculated: 35 (30 + 5 + 0)
```

**Example Schedule 3: Low Priority (1st Year Minor Classroom)**
```
Course Name: Introduction to Computing
Department: Information Systems
Student Year: 1
Program Type: 2 (Minor)
Room Type: 1 (Classroom)
Weekly Hours: 3

Priority Calculated: 10 (10 + 0 + 0)
```

### 2.2 Viewing Schedules

Select `1` (Display All Schedules) to see all schedules:

```
ID    Course                    Dept            Year   Program    Room Type    Weekly Hrs   Priority
----------------------------------------------------------------------------------------------------
1     Advanced Embedded Systems Computer Engin. 5      Major      Lab          3            58
2     Database Management       Computer Scien. 3      Major      Classroom    4            35
3     Introduction to Computing Information Sys 1      Minor      Classroom    3            10
```

### 2.3 Generate Weekly Schedule

This is where the magic happens!

1. Select `5` (Generate Weekly Schedule)
2. The system will:
   - Sort schedules by priority (highest first)
   - **Phase 1**: Assign lab courses to labs, classroom courses to classrooms
   - **Phase 2**: Assign remaining classroom courses to available labs
   - Check for room AND student conflicts
   - Distribute weekly hours across Monday-Friday

**Algorithm in Action:**
```
Schedule Configuration:
  Days: Monday - Friday
  Morning: 08:00 - 12:30
  Afternoon: 14:00 - 17:30
  Slot Duration: 1 hour

Room Assignment Rules:
  1. Lab courses MUST use lab rooms (priority for labs)
  2. Classroom courses prefer classrooms
  3. If classrooms full, classroom courses can use free labs
  4. Same dept/year students cannot have overlapping classes

=== PHASE 1: Assigning with preferred room types ===

Processing: Advanced Embedded Systems Lab    (Priority: 58, Need: 3 hrs, Lab)
  [OK] Fully assigned: 3 hrs

Processing: Database Management Systems      (Priority: 35, Need: 4 hrs, Classroom)
  [OK] Fully assigned: 4 hrs

Processing: Introduction to Computing        (Priority: 10, Need: 3 hrs, Classroom)
  ... 1 hrs still needed (will try fallback)

=== PHASE 2: Using lab rooms for overflow classroom courses ===

Fallback for: Introduction to Computing      (Need: 1 more hrs)
    (Using lab room 'Computer Lab 1' as fallback)
  [OK] Fallback successful: Total 3 hrs assigned

=== FINAL SUMMARY ===

[OK] Schedule generation complete!
  Total hours assigned: 10
  Hours failed to assign: 0
  Generated slots: 10
```

### 2.4 View Weekly Timetable

Select `6` (View Weekly Timetable) to see the day-by-day schedule:

```
=== MONDAY ===

  Time          Course                    Room                 Dept            Year
  ---------------------------------------------------------------------------------
  08:00-09:00   Advanced Embedded Systems Computer Lab 1       Computer Engin. 5
  08:00-09:00   Database Management       Lecture Hall A       Computer Scien. 3
  09:00-10:00   Introduction to Computing Engineering Room 205 Information Sys 1

=== TUESDAY ===

  Time          Course                    Room                 Dept            Year
  ---------------------------------------------------------------------------------
  08:00-09:00   Advanced Embedded Systems Computer Lab 1       Computer Engin. 5
  08:00-09:00   Database Management       Lecture Hall A       Computer Scien. 3
  09:00-10:00   Introduction to Computing Engineering Room 205 Information Sys 1

... (continues for Wednesday, Thursday, Friday)
```

### 2.5 View Final Schedule (by Room)

Select `7` (View Final Schedule) to see schedule organized by room:

```
+-- ROOM: Computer Lab 1 (Lab) -----------------------------------------
|
|  Day        Time          Course                    Dept            Year
|  --------------------------------------------------------------------
|  Monday     08:00-09:00   Advanced Embedded Systems Computer Engin. 5
|  Tuesday    08:00-09:00   Advanced Embedded Systems Computer Engin. 5
|  Wednesday  08:00-09:00   Advanced Embedded Systems Computer Engin. 5
|  Friday     09:00-10:00   Introduction to Computing Information Sys 1
+-----------------------------------------------------------------------

+-- ROOM: Lecture Hall A (Classroom) -----------------------------------
|
|  Day        Time          Course                    Dept            Year
|  --------------------------------------------------------------------
|  Monday     08:00-09:00   Database Management       Computer Scien. 3
|  Tuesday    08:00-09:00   Database Management       Computer Scien. 3
|  Wednesday  08:00-09:00   Database Management       Computer Scien. 3
|  Thursday   08:00-09:00   Database Management       Computer Scien. 3
+-----------------------------------------------------------------------
```

### 2.6 View by Room

Filter schedules for a specific room:

1. Select `8` (View Schedule by Room)
2. Enter Room ID
3. See all courses scheduled in that room by day

### 2.7 View by Department

Filter schedules for a specific department:

1. Select `9` (View Schedule by Department)
2. Enter department name (e.g., "Computer Science")
3. See all courses for that department by day

## Priority Calculation Formula

```
Priority = (Student Year x 10) + Program Bonus + Room Type Bonus

Where:
- Student Year: 1-5 (x10)
- Program Bonus: Major = +5, Minor = +0
- Room Type Bonus: Lab = +3, Classroom = +0
```

**Priority Examples:**

| Student Year | Program | Room Type | Calculation | Total Priority |
|--------------|---------|-----------|-------------|----------------|
| 5            | Major   | Lab       | 50+5+3      | 58             |
| 5            | Major   | Classroom | 50+5+0      | 55             |
| 4            | Major   | Lab       | 40+5+3      | 48             |
| 4            | Major   | Classroom | 40+5+0      | 45             |
| 3            | Major   | Lab       | 30+5+3      | 38             |
| 3            | Minor   | Classroom | 30+0+0      | 30             |
| 2            | Major   | Classroom | 20+5+0      | 25             |
| 1            | Minor   | Classroom | 10+0+0      | 10             |

## Two-Phase Room Assignment

### Phase 1: Preferred Room Types
- Lab courses are assigned to lab rooms ONLY
- Classroom courses are assigned to classrooms ONLY
- Higher priority courses get first choice of time slots

### Phase 2: Lab Fallback (Classroom Courses Only)
- If classrooms are full, classroom courses can use available lab rooms
- Lab courses NEVER use classrooms (they need specialized equipment)
- This maximizes overall room utilization

**Example:**
```
Rooms: 2 Classrooms, 1 Lab
Courses: 4 Classroom courses (3 hrs each = 12 hrs total)

Phase 1 Result:
- 2 Classrooms x 9 slots/day x 5 days = 90 available slots
- 12 hrs assigned to classrooms

What if we had 20 classroom courses (60 hrs)?
- Phase 1: 60 hrs needed, only some fit in 2 classrooms
- Phase 2: Remaining hours assigned to available lab slots
```

## Conflict Resolution

### Room Conflicts
- Two courses cannot use the same room at the same time
- Higher priority courses get first choice
- Lower priority courses are assigned to alternative rooms

### Student Conflicts (NEW!)
- Same department AND same year students cannot have overlapping classes
- Example: CS Year 3 students can't have Database and Networks at the same time
- System automatically prevents this during generation

### Room Type Priority
- Lab courses MUST be in lab rooms (no fallback to classrooms)
- Classroom courses PREFER classrooms but can use labs as fallback
- Labs are "reserved" for lab courses first

## Tips for Best Results

1. **Add Sufficient Rooms**: Ensure you have enough rooms of each type
   - Labs for lab courses (lab courses cannot use classrooms)
   - Classrooms for theory courses (can overflow to labs)

2. **Set Realistic Weekly Hours**: 
   - Typical course: 2-4 hours per week
   - Lab courses: 2-3 hours per week
   - Total hours should fit within available room capacity

3. **Run Generate Schedule After Changes**: After adding/editing schedules, always regenerate

4. **Check for Unassigned Courses**: If hours remain unassigned:
   - Add more rooms of the required type
   - Reduce weekly hours for some courses
   - Check for too many same-dept/year conflicts

5. **Use Department Filtering**: For large schedules, use department view to verify each department's schedule

## Data Persistence

All data is automatically saved to:
- `rooms.dat` - Room information
- `schedules.dat` - Schedule information
- `generated_slots.dat` - Generated weekly timetable

Data persists between program runs. No manual save required!

## Common Scenarios

### Scenario 1: Starting Fresh
```
1. Delete old data files: make clean
2. Run program: ./scheduling_system
3. Add 5-10 rooms (mix of classrooms and labs)
4. Add 10-15 schedules with weekly hours
5. Generate weekly schedule
6. View weekly timetable
```

### Scenario 2: Adding New Course Mid-Semester
```
1. Run program
2. Add new schedule with weekly hours
3. Regenerate weekly schedule
4. Check for any conflicts or unassigned hours
```

### Scenario 3: Room Shortage
```
If you see "Hours failed to assign" > 0:
1. Check which courses are affected
2. Options:
   a. Add more rooms
   b. Reduce weekly hours for lower-priority courses
   c. Allow classroom courses to use labs (automatic in Phase 2)
```

## Troubleshooting

**Problem**: Lab course not assigned
- **Cause**: Not enough lab rooms available
- **Solution**: Add more lab rooms (lab courses MUST use labs)

**Problem**: "Student conflict" warnings
- **Cause**: Too many courses for same dept/year at overlapping times
- **Solution**: Reduce weekly hours or spread courses across more time slots

**Problem**: Classroom course using lab room
- **Expected behavior**: This happens in Phase 2 when classrooms are full
- **Note**: This maximizes room utilization - it's a feature, not a bug!

**Problem**: Data not persisting
- **Solution**: Check file permissions for .dat files

## Support

For issues or questions about the Bahir Dar University Scheduling System, please refer to the FEATURES.md file for technical details or PROJECT_OVERVIEW.md for system architecture.
