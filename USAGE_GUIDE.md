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
╔════════════════════════════════════════════════════════════╗
║     CLASSROOM AND LABORATORY SCHEDULING SYSTEM             ║
║            BAHIR DAR UNIVERSITY                            ║
╚════════════════════════════════════════════════════════════╝

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
───────────────────────────────────────────────────────────────────
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

### 2.1 Adding Schedules

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
Start Time: 08 00 (8:00 AM)
End Time: 11 00 (11:00 AM)

Priority Calculated: 58 (50 + 5 + 3)
```

**Example Schedule 2: Medium Priority (3rd Year Major Classroom)**
```
Course Name: Database Management Systems
Department: Computer Science
Student Year: 3
Program Type: 1 (Major)
Room Type: 1 (Classroom)
Start Time: 08 00 (8:00 AM)
End Time: 10 00 (10:00 AM)

Priority Calculated: 35 (30 + 5 + 0)
```

**Example Schedule 3: Low Priority (1st Year Minor Classroom)**
```
Course Name: Introduction to Computing
Department: Information Systems
Student Year: 1
Program Type: 2 (Minor)
Room Type: 1 (Classroom)
Start Time: 10 00 (10:00 AM)
End Time: 12 00 (12:00 PM)

Priority Calculated: 10 (10 + 0 + 0)
```

**Example Schedule 4: Conflict Test (Same Time, Same Room Type)**
```
Course Name: Software Engineering
Department: Computer Science
Student Year: 4
Program Type: 1 (Major)
Room Type: 1 (Classroom)
Start Time: 08 00 (8:00 AM)
End Time: 10 00 (10:00 AM)

Priority Calculated: 45 (40 + 5 + 0)
Note: Will be scheduled in different room than Database Management Systems
```

### 2.2 Viewing Schedules

Select `1` (Display All Schedules) to see all schedules before room assignment:

```
ID    Course                    Dept            Year   Program    Room Type    Time         Assigned Room             Priority
────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
1     Advanced Embedded Systems Computer Engin. 5      Major      Lab          08:00-11:00  Not assigned              58
2     Database Management       Computer Scien. 3      Major      Classroom    08:00-10:00  Not assigned              35
3     Introduction to Computing Information Sys 1      Minor      Classroom    10:00-12:00  Not assigned              10
```

### 2.3 Generate Final Schedule

This is where the magic happens!

1. Select `5` (Generate Final Schedule)
2. The system will:
   - Sort schedules by priority (highest first)
   - Assign rooms based on type requirements
   - Check for time conflicts
   - Prevent double-booking

**Algorithm in Action:**
```
Step 1: Sort by priority
  1. Advanced Embedded Systems Lab (Priority: 58)
  2. Software Engineering (Priority: 45)
  3. Database Management Systems (Priority: 35)
  4. Introduction to Computing (Priority: 10)

Step 2: Assign rooms
  - Advanced Embedded Systems → Computer Lab 1 (Lab required)
  - Software Engineering → Lecture Hall A (No conflict)
  - Database Management Systems → Engineering Room 205 (Conflict with Software Eng in Lecture Hall A)
  - Introduction to Computing → Lecture Hall A (Different time slot)

Result: All courses assigned, no conflicts!
```

### 2.4 View Final Schedule

Select `6` (View Final Schedule) to see the organized result:

```
┌─ ROOM: Computer Lab 1 (Lab) ─────────────────────────────────────────
│
│  Course                    Dept            Year   Program    Time            Priority
│  ────────────────────────────────────────────────────────────────────────────
│  Advanced Embedded Systems Computer Engin. 5      Major      08:00-11:00     58
└─────────────────────────────────────────────────────────────────────────────

┌─ ROOM: Lecture Hall A (Classroom) ───────────────────────────────────────
│
│  Course                    Dept            Year   Program    Time            Priority
│  ────────────────────────────────────────────────────────────────────────────
│  Software Engineering      Computer Scien. 4      Major      08:00-10:00     45
│  Introduction to Computing Information Sys 1      Minor      10:00-12:00     10
└─────────────────────────────────────────────────────────────────────────────
```

### 2.5 View by Room

Filter schedules for a specific room:

1. Select `7` (View Schedule by Room)
2. Enter Room ID
3. See all courses scheduled in that room

**Example Output:**
```
Room: Computer Lab 1 (Lab)
════════════════════════════════════════════════════════════════════════

Course                    Dept            Year   Program    Time
────────────────────────────────────────────────────────────────────────
Advanced Embedded Systems Computer Engin. 5      Major      08:00-11:00
Network Security Lab      Computer Scien. 4      Major      14:00-17:00
```

### 2.6 View by Department

Filter schedules for a specific department:

1. Select `8` (View Schedule by Department)
2. Enter department name (e.g., "Computer Science")
3. See all courses for that department

**Example Output:**
```
Department: Computer Science
════════════════════════════════════════════════════════════════════════════════════

Course                    Year   Program    Room Type    Time            Assigned Room
────────────────────────────────────────────────────────────────────────────────────────────
Database Management       3      Major      Classroom    08:00-10:00     Engineering Room 205
Software Engineering      4      Major      Classroom    08:00-10:00     Lecture Hall A
Network Security Lab      4      Major      Lab          14:00-17:00     Computer Lab 1
```

## Priority Calculation Formula

```
Priority = (Student Year × 10) + Program Bonus + Room Type Bonus

Where:
- Student Year: 1-5 (×10)
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

## Conflict Resolution

The system automatically prevents conflicts:

### Time Conflicts
- Two courses cannot use the same room at overlapping times
- Example: Course A (08:00-10:00) and Course B (09:00-11:00) cannot share a room

### Room Type Conflicts
- Lab courses MUST be assigned to Lab rooms
- Classroom courses are assigned to Classroom rooms
- Lab courses will never be assigned to classrooms, even if available

### Priority Resolution
- Higher priority courses get first choice of rooms
- If a conflict occurs, lower priority courses are assigned to alternative rooms
- If no suitable room is available, the course remains unassigned

## Tips for Best Results

1. **Add Sufficient Rooms**: Ensure you have enough rooms of each type
   - Labs for lab courses
   - Classrooms for theory courses

2. **Spread Out Time Slots**: Avoid scheduling too many courses at the same time
   - Morning: 08:00-12:00
   - Afternoon: 13:00-17:00
   - Evening: 18:00-20:00

3. **Run Generate Schedule After Changes**: After adding/editing schedules, always regenerate the final schedule

4. **Check for Unassigned Courses**: If courses remain unassigned after generation:
   - Add more rooms of the required type
   - Adjust time slots to reduce conflicts
   - Check if lab courses have lab rooms available

5. **Use Department Filtering**: For large schedules, use department view to manage specific programs

## Data Persistence

All data is automatically saved to:
- `rooms.dat` - Room information
- `schedules.dat` - Schedule information

Data persists between program runs. No manual save required!

## Common Scenarios

### Scenario 1: Starting Fresh
```
1. Delete old data files: make clean
2. Run program: ./scheduling_system
3. Add 5-10 rooms (mix of classrooms and labs)
4. Add 10-15 schedules
5. Generate final schedule
6. View results
```

### Scenario 2: Mid-Semester Changes
```
1. Run program
2. Edit existing schedules (time changes, room type changes)
3. Regenerate final schedule
4. Check for conflicts
```

### Scenario 3: Adding New Department
```
1. Add department-specific rooms
2. Add all department courses
3. Generate schedule
4. Use "View by Department" to verify
```

## Troubleshooting

**Problem**: Course not assigned after generation
- **Solution**: Add more rooms of the required type or adjust time slots

**Problem**: Too many conflicts
- **Solution**: Spread out course times or add more rooms

**Problem**: Lab course assigned to classroom
- **Solution**: This should never happen. If it does, regenerate the schedule

**Problem**: Data not persisting
- **Solution**: Check file permissions for rooms.dat and schedules.dat

## Advanced Usage

### Managing Large Schedules
- Use department filtering to work with one department at a time
- Generate schedule multiple times if you add new courses
- Use room view to check room utilization

### Optimizing Room Usage
- Check final schedule to see which rooms are underutilized
- Adjust schedule times to maximize room usage
- Balance load across morning and afternoon slots

## Support

For issues or questions about the Bahir Dar University Scheduling System, please refer to the README.md file or contact the system administrator.
