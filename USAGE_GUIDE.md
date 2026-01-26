# Usage Guide - Bahir Dar University Scheduling System

## Quick Start

### Step 1: Compile and Run

```bash
make
./scheduling_system

# Or for web interface:
make open
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
```

**Example Labs:**
```
Room Name: Computer Lab 1
Type: 2 (Lab)

Room Name: Chemistry Laboratory
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
```

## Part 2: Schedule Management

### 2.1 Adding Schedules with Weekly Hours

1. Select `2` (Schedule Management) from main menu
2. Select `2` (Add New Schedule)
3. Enter schedule details:

**Example: 6-Hour Lab Course**
```
Course Name: Advanced Embedded Systems Lab
Department: Computer Engineering
Student Year: 5
Program Type: 1 (Major)
Room Type: 2 (Lab)
Weekly Hours: 6

Priority Calculated: 58 (50 + 5 + 3)
```

**Example: 4-Hour Classroom Course**
```
Course Name: Database Management Systems
Department: Computer Science
Student Year: 3
Program Type: 1 (Major)
Room Type: 1 (Classroom)
Weekly Hours: 4

Priority Calculated: 35 (30 + 5 + 0)
```

### 2.2 Generate Distributed Schedule (NEW!)

This is where the **distributed scheduling** happens:

1. Select `5` (Generate Weekly Schedule)
2. The system will:
   - Sort schedules by priority (highest first)
   - **Distribute hours evenly across Monday-Friday**
   - Ensure minimum 90-minute sessions
   - Check for room AND student conflicts

**Algorithm Output:**
```
=== Schedule Generation - Distributed Across 5 Days ===

Distribution Rules:
1. Classes are distributed evenly across Monday-Friday
2. Minimum session duration: 1 hour 30 minutes (90 min)
3. Example: 6 hrs/week = ~72 min/day across 5 days
4. Lab courses MUST use lab rooms
5. Same department/year students cannot have overlapping classes

=== Phase 1: Even Distribution (Mon-Fri) ===

+--------+----------+--------+------------+-------+------------------+------------------+
| Course | Priority | Weekly | Target/Day | Type  | Distribution     | Status           |
+--------+----------+--------+------------+-------+------------------+------------------+
| Adv... | 58       | 6      | 72 min     | Lab   | M:90 T:90 W:90.. | Complete (6h0m)  |
| Data.. | 35       | 4      | 48 min     | Class | M:90 T:90 W:0... | Complete (3h0m)  |
+--------+----------+--------+------------+-------+------------------+------------------+

=== FINAL SUMMARY ===

Schedule Generation Complete!
Total time assigned: 9 hours 0 minutes
Time failed to assign: 0 hours 0 minutes
Session duration: 1 hour 30 minutes (90 min)
```

### 2.3 View by Year & Department

Filter schedules for specific student groups:

1. Select **By Year/Dept** from menu
2. Choose Year (1-5)
3. Choose Department
4. See weekly schedule for that student group

**Example Output:**
```
Weekly Schedule: Computer Science - Year 3

=== MONDAY ===
+-------------+----------+--------------------------+------------------+
| Time        | Duration | Course                   | Room             |
+-------------+----------+--------------------------+------------------+
| 08:00-09:30 | 90 min   | Database Management      | Lecture Hall A   |
| 14:00-15:30 | 90 min   | Data Structures Lab      | Computer Lab 1   |
+-------------+----------+--------------------------+------------------+

=== TUESDAY ===
+-------------+----------+--------------------------+------------------+
| 08:00-09:30 | 90 min   | Database Management      | Lecture Hall A   |
+-------------+----------+--------------------------+------------------+

... (continues for Wed, Thu, Fri)
```

### 2.4 View by Room (NEW!)

See schedule for a specific room:

1. Select **By Room** from menu
2. Choose room from dropdown
3. See weekly schedule with utilization summary

**Example Output:**
```
Select Room: [Computer Lab 1 ▼]

Weekly Schedule: Computer Lab 1 (Laboratory)

=== MONDAY ===
+-------------+----------+--------------------------+-------+------+
| Time        | Duration | Course                   | Dept  | Year |
+-------------+----------+--------------------------+-------+------+
| 08:00-09:30 | 90 min   | Adv Embedded Systems Lab | CE    | 5    |
| 14:00-15:30 | 90 min   | Data Structures Lab      | CS    | 3    |
+-------------+----------+--------------------------+-------+------+

... (continues for all days)

Room Utilization Summary:
Total time used: 12 hours 0 minutes per week
Available time: ~40 hours per week (8 hrs x 5 days)
```

### 2.5 View All Rooms Overview (Enhanced)

See utilization for all rooms:

**Output:**
```
Room Utilization Summary
+------------------+------+-------+-------+-------+-------+-------+---------+
| Room             | Type | Mon   | Tue   | Wed   | Thu   | Fri   | Total   |
+------------------+------+-------+-------+-------+-------+-------+---------+
| Computer Lab 1   | Lab  | 3h0m  | 3h0m  | 3h0m  | 3h0m  | 0h0m  | 12h0m   |
| Lecture Hall A   | Class| 1h30m | 1h30m | 0h0m  | 0h0m  | 0h0m  | 3h0m    |
+------------------+------+-------+-------+-------+-------+-------+---------+

[Click room name to view detailed schedule]
```

## Distribution Algorithm Explained

### How It Works

1. **Calculate target per day**: Total weekly hours / 5 days
2. **Find day with minimum assigned time**
3. **Assign 90-minute session** to that day
4. **Repeat** until weekly hours fulfilled

### Example: 6-Hour Course

```
Weekly Hours: 6 (360 minutes)
Target per day: 72 minutes
Minimum session: 90 minutes

Pass 1: Monday has 0 min -> Assign 90 min to Monday
Pass 2: Tue-Fri have 0 min, Monday has 90 -> Assign 90 min to Tuesday
Pass 3: Wed-Fri have 0 min -> Assign 90 min to Wednesday
Pass 4: Thu-Fri have 0 min -> Assign 90 min to Thursday

Final Distribution:
Mon: 90 | Tue: 90 | Wed: 90 | Thu: 90 | Fri: 0
Total: 360 min = 6 hours
```

### Why This Matters

**Before (Old System):**
```
6 hours might be assigned as:
Monday: 6 hours straight (exhausting!)
```

**After (Distributed System):**
```
6 hours spread as:
Mon: 1.5h | Tue: 1.5h | Wed: 1.5h | Thu: 1.5h
Much better for learning!
```

## Priority Calculation Formula

```
Priority = (Student Year x 10) + Program Bonus + Room Type Bonus

Where:
- Student Year: 1-5 (x10)
- Program Bonus: Major = +5, Minor = +0
- Room Type Bonus: Lab = +3, Classroom = +0
```

**Priority Examples:**

| Student Year | Program | Room Type | Calculation | Total |
|--------------|---------|-----------|-------------|-------|
| 5            | Major   | Lab       | 50+5+3      | 58    |
| 5            | Major   | Classroom | 50+5+0      | 55    |
| 3            | Major   | Lab       | 30+5+3      | 38    |
| 1            | Minor   | Classroom | 10+0+0      | 10    |

## Two-Phase Room Assignment

### Phase 1: Preferred Room Types
- Lab courses -> Lab rooms ONLY
- Classroom courses -> Classrooms ONLY
- Classes distributed evenly across 5 days

### Phase 2: Lab Fallback
- If classrooms are full, classroom courses can use available lab rooms
- Lab courses NEVER use classrooms

## Conflict Prevention

### Room Conflicts
- Two courses cannot use the same room at the same time
- Higher priority courses get first choice

### Student Conflicts
- Same department AND same year cannot have overlapping classes
- System automatically prevents this

## Tips for Best Results

1. **Add Sufficient Rooms**: Ensure enough rooms of each type
2. **Set Realistic Weekly Hours**: 2-6 hours per course is typical
3. **Regenerate After Changes**: Always regenerate after adding/editing
4. **Check Distribution**: Look at M:XX T:XX output to verify spread
5. **Use View Options**: Check by Year/Dept for student perspective, by Room for utilization

## Troubleshooting

**Problem**: Hours not evenly distributed
- **Check**: Are there enough time slots available?
- **Solution**: Add more rooms or reduce weekly hours

**Problem**: Lab course not assigned
- **Cause**: Not enough lab rooms
- **Solution**: Add more lab rooms (lab courses cannot use classrooms)

**Problem**: "Student conflict" warnings
- **Cause**: Too many courses for same dept/year
- **Solution**: Reduce weekly hours or add more rooms

## Common Commands

```bash
make          # Compile
make run      # Compile and run console
make open     # Compile and run web interface
make clean    # Remove compiled files and data
```

## Support

For detailed technical information, see:
- **FEATURES.md** - Technical documentation
- **PROJECT_OVERVIEW.md** - System architecture
