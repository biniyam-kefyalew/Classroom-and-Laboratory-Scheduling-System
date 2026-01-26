# Features Documentation
## Bahir Dar University Scheduling System

## Core Features

### 1. Room Management (Full CRUD)

#### 1.1 Create Rooms
- Add unlimited rooms (up to MAX_ROOMS = 100)
- Support for two room types:
  - Classrooms
  - Laboratories
- Automatic ID assignment
- Input validation

#### 1.2 Read Rooms
- Display all active rooms in formatted table
- Show room ID, name, and type
- Clean, organized output
- Real-time data from file system

#### 1.3 Update Rooms
- Edit room name
- Change room type (Classroom <-> Lab)
- Keep existing values option
- Immediate feedback

#### 1.4 Delete Rooms
- Soft delete (marks as inactive)
- Confirmation prompt
- Automatic schedule cleanup (unassigns deleted room)
- Data integrity maintenance

### 2. Schedule Management (Full CRUD)

#### 2.1 Create Schedules
- Comprehensive course information:
  - Course name
  - Department
  - Student year (1-5)
  - Program type (Major/Minor)
  - Required room type
  - **Weekly hours** (e.g., 3 hours/week, 5 hours/week)
- Automatic priority calculation
- Input validation
- Unique ID assignment

#### 2.2 Read Schedules
- Display all schedules with complete details
- Show assignment status
- View priority scores
- Multiple viewing modes:
  - All schedules (unfiltered)
  - Weekly timetable (Monday-Friday)
  - Final schedule (grouped by room)
  - By specific room
  - By department

#### 2.3 Update Schedules
- Edit course details
- Change department
- Modify student year
- Update weekly hours
- Automatic priority recalculation
- Clears room assignment (requires regeneration)

#### 2.4 Delete Schedules
- Soft delete (marks as inactive)
- Confirmation prompt
- Preserves data integrity
- Immediate effect

### 3. Intelligent Scheduling Algorithm

#### 3.1 Priority Calculation
```
Priority = (Student Year x 10) + Program Bonus + Room Type Bonus

Components:
- Student Year: 1-5 (multiplied by 10)
  - Year 5: 50 points
  - Year 4: 40 points
  - Year 3: 30 points
  - Year 2: 20 points
  - Year 1: 10 points

- Program Type:
  - Major: +5 points
  - Minor: +0 points

- Room Type:
  - Lab: +3 points (higher priority to ensure lab courses get lab rooms)
  - Classroom: +0 points
```

#### 3.2 Two-Phase Room Assignment

**Phase 1: Preferred Room Types**
1. Lab courses are assigned ONLY to lab rooms
2. Classroom courses are assigned ONLY to classrooms
3. Higher priority courses get first choice

**Phase 2: Lab Fallback for Classrooms**
1. If classrooms are full, classroom courses can use available lab rooms
2. Lab courses NEVER use classrooms (they need specialized equipment)
3. This maximizes room utilization

#### 3.3 Scheduling Rules
1. **Priority-Based Assignment**
   - Higher priority schedules assigned first
   - Ensures senior students get optimal slots
   - Major courses prioritized over minor courses

2. **Room Type Priority**
   - Lab courses MUST be assigned to lab rooms
   - Classroom courses PREFER classrooms
   - Classroom courses CAN use labs as fallback if classrooms are full
   - Labs are "reserved" for lab courses first

3. **Conflict Prevention**
   - **Room Conflict**: No two courses can use same room at same time
   - **Student Conflict**: Same department/year students cannot have overlapping classes
   - Validates availability before assignment

4. **Automatic Weekly Distribution**
   - System automatically distributes weekly hours across Monday-Friday
   - Time slots: Morning (08:00-12:30) and Afternoon (14:00-17:30)
   - 1-hour slot duration
   - Reports unsuccessful assignments

#### 3.4 Algorithm Steps
1. Collect all active schedules
2. Sort by priority (descending)
3. **Phase 1**: For each schedule (highest priority first):
   - Find rooms matching required type ONLY
   - Check each room for time conflicts
   - Check for student conflicts (same dept/year)
   - Assign to first conflict-free room
4. **Phase 2**: For classroom courses with unassigned hours:
   - Try to use available lab rooms as fallback
   - Lab courses are NOT included in this phase
5. Save assignments
6. Report statistics (assigned/failed)

### 4. Weekly Schedule Generation

#### 4.1 Time Configuration
- **Days**: Monday through Friday (5 days)
- **Morning Session**: 08:00 - 12:30 (4.5 hours)
- **Afternoon Session**: 14:00 - 17:30 (3.5 hours)
- **Slot Duration**: 1 hour per slot
- **Total Slots per Day**: 9 slots (5 morning + 4 afternoon)

#### 4.2 Weekly Hours Distribution
```
Example: Course with 5 weekly hours
- Monday: 08:00-09:00 (1 hr)
- Tuesday: 08:00-09:00 (1 hr)
- Wednesday: 08:00-09:00 (1 hr)
- Thursday: 08:00-09:00 (1 hr)
- Friday: 08:00-09:00 (1 hr)
Total: 5 hours distributed across the week
```

#### 4.3 Generated Slots
Each generated slot contains:
- Schedule ID (reference to course)
- Room ID (assigned room)
- Day of week (Monday-Friday)
- Start time
- End time
- Active status

### 5. Data Persistence

#### 5.1 Binary File Storage
- Efficient storage format
- Fast read/write operations
- Three data files:
  - `rooms.dat`: Room records
  - `schedules.dat`: Schedule records
  - `generated_slots.dat`: Generated weekly timetable

#### 5.2 Data Structure
```c
Files contain:
1. Record count
2. Next ID counter
3. Array of structs
```

#### 5.3 Auto-save
- Automatic save after every create/update/delete operation
- No manual save required
- Data persists between sessions
- Crash-safe (last completed operation saved)

### 6. User Interface

#### 6.1 Menu System
- Hierarchical menu structure:
  ```
  Main Menu
  |-- Room Management
  |   |-- Display Rooms
  |   |-- Add Room
  |   |-- Edit Room
  |   +-- Delete Room
  +-- Schedule Management
      |-- Display Schedules
      |-- Add Schedule
      |-- Edit Schedule
      |-- Delete Schedule
      |-- Generate Weekly Schedule
      |-- View Weekly Timetable
      |-- View Final Schedule
      |-- View by Room
      +-- View by Department
  ```

#### 6.2 Display Features
- Box-drawing characters for visual appeal
- Formatted tables with aligned columns
- Clear headers and separators
- Status indicators (success, error, warning)
- Consistent styling throughout
- Day-by-day timetable view

#### 6.3 Navigation
- Number-based menu selection
- Clear prompts
- "Press Enter to continue" pause system
- Back to previous menu option
- Exit from any level

#### 6.4 Input Handling
- Buffer clearing to prevent input issues
- Input validation
- Error messages for invalid input
- Default value options (keep current)
- Confirmation prompts for destructive actions

### 7. Viewing and Reporting

#### 7.1 Weekly Timetable View
- Organized by day (Monday-Friday)
- Shows morning and afternoon sessions
- Displays room assignments
- Complete course information per slot

#### 7.2 Final Schedule View
- Grouped by room
- Shows all assigned courses per room
- Displays complete course information
- Priority scores visible
- Clean, organized format

#### 7.3 Room-Based View
- Filter by specific room ID
- Shows all courses in selected room
- Day-by-day breakdown
- Useful for room utilization analysis

#### 7.4 Department-Based View
- Filter by department name
- Case-insensitive search
- Shows all department courses
- Displays assignment status
- Day-by-day breakdown
- Useful for department coordinators

#### 7.5 Complete Schedule List
- Shows all active schedules
- Displays weekly hours
- Priority scores visible
- Assignment status

### 8. Conflict Management

#### 8.1 Room Conflict Detection
```
Conflict occurs when:
- Same room is assigned to two schedules
- Same day AND time ranges overlap

No conflict when:
- Different rooms
- Different days
- End time of schedule A <= Start time of schedule B
- Start time of schedule A >= End time of schedule B
```

#### 8.2 Student Conflict Detection
```
Conflict occurs when:
- Same department AND same year
- Same day AND time ranges overlap

This prevents students from having two classes at the same time
```

#### 8.3 Room Type Priority
- Lab courses MUST use lab rooms (cannot use classrooms)
- Classroom courses prefer classrooms but can use labs as fallback
- System first assigns all courses to preferred room types
- Then fills remaining classroom courses into available labs

#### 8.4 Conflict Resolution
- Priority-based: Higher priority gets first choice
- Two-phase assignment ensures labs are reserved for lab courses
- Alternative room selection for lower priority
- Reports when no suitable room available

### 9. Data Validation

#### 9.1 Input Validation
- Non-empty course names
- Valid year ranges (1-5)
- Valid weekly hours (positive integer)
- Valid menu selections
- Buffer overflow prevention

#### 9.2 Data Integrity
- Referential integrity (room-schedule relationship)
- Soft deletes preserve history
- Cascading updates (room deletion cleans schedules)
- Unique ID management

### 10. Error Handling

#### 10.1 User Errors
- Invalid menu selections -> Error message, retry
- Invalid input types -> Buffer clearing, retry
- Invalid weekly hours -> Rejection with explanation
- Non-existent IDs -> "Not found" message

#### 10.2 System Errors
- File I/O errors -> Error message
- Capacity limits -> Clear warning
- Memory allocation errors -> Graceful handling

#### 10.3 User Feedback
- Success messages
- Error messages
- Warning messages
- Informational messages
- Confirmation prompts

### 11. System Design

#### 11.1 Architecture
```
scheduling_system.c
|-- Data Structures
|   |-- Room struct
|   |-- Schedule struct
|   +-- GeneratedSlot struct
|-- File I/O
|   |-- Load functions
|   +-- Save functions
|-- UI Functions
|   |-- Display functions
|   |-- Add functions
|   |-- Edit functions
|   +-- Delete functions
|-- Algorithm
|   |-- Priority calculation
|   |-- Room suitability check
|   |-- Conflict detection (room + student)
|   |-- Two-phase room assignment
|   +-- Weekly distribution
+-- Main Program
    |-- Main menu
    |-- Room management menu
    +-- Schedule management menu
```

#### 11.2 Design Principles
- Single Responsibility: Each function has one clear purpose
- DRY: Reusable utility functions
- Modularity: Clear separation of concerns
- User-friendly: Clear prompts and feedback
- Robust: Error handling and validation

#### 11.3 Scalability
- Configurable limits (MAX_ROOMS, MAX_SCHEDULES, MAX_GENERATED_SLOTS)
- Efficient binary file format
- O(n^2) worst-case for conflict detection (acceptable for typical use)
- qsort for efficient priority sorting

## Technical Specifications

### Supported Platforms
- Linux
- macOS
- Windows (with MinGW/GCC)

### Requirements
- C99 standard compiler
- Standard C libraries
- 100KB+ free disk space

### Performance
- Instant room/schedule operations
- Sub-second schedule generation (typical cases)
- Efficient binary file I/O
- Memory-efficient struct design

### Limitations
- Maximum 100 rooms
- Maximum 500 schedules
- Maximum 2000 generated time slots
- No concurrency support (single-user)
- Terminal-based only (no GUI)
- Web interface available via CGI

## Conclusion

The Bahir Dar University Scheduling System provides a comprehensive, intelligent solution for classroom and laboratory scheduling. With weekly hours-based scheduling, two-phase room assignment (lab priority with classroom fallback), student conflict prevention, and multiple viewing options, it serves as a robust system that can be adapted for various educational institutions.
