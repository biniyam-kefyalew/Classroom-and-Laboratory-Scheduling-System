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
- Change room type (Classroom ↔ Lab)
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
  - Time range (start-end)
- Automatic priority calculation
- Input validation (time range checking)
- Unique ID assignment

#### 2.2 Read Schedules
- Display all schedules with complete details
- Show assignment status
- View priority scores
- Multiple viewing modes:
  - All schedules (unfiltered)
  - Final schedule (grouped by room)
  - By specific room
  - By department

#### 2.3 Update Schedules
- Edit course details
- Change department
- Modify student year
- Update time slots
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
Priority = (Student Year × 10) + Program Bonus + Room Type Bonus

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
  - Lab: +3 points (higher priority due to specialized equipment)
  - Classroom: +0 points
```

#### 3.2 Scheduling Rules
1. **Priority-Based Assignment**
   - Higher priority schedules assigned first
   - Ensures senior students get optimal slots
   - Major courses prioritized over minor courses

2. **Room Type Enforcement**
   - Lab courses MUST be assigned to lab rooms
   - Classroom courses assigned to classroom spaces
   - No cross-type assignments

3. **Conflict Prevention**
   - Checks for time overlaps
   - Prevents double-booking of rooms
   - Validates availability before assignment

4. **Automatic Room Assignment**
   - Scans available rooms of correct type
   - Checks for conflicts
   - Assigns first available conflict-free room
   - Reports unsuccessful assignments

#### 3.3 Algorithm Steps
1. Collect all active schedules
2. Sort by priority (descending)
3. For each schedule (highest priority first):
   - Find rooms matching required type
   - Check each room for time conflicts
   - Assign to first conflict-free room
   - Mark as unassigned if no room available
4. Save assignments
5. Report statistics (assigned/failed)

### 4. Data Persistence

#### 4.1 Binary File Storage
- Efficient storage format
- Fast read/write operations
- Two data files:
  - `rooms.dat`: Room records
  - `schedules.dat`: Schedule records

#### 4.2 Data Structure
```c
Files contain:
1. Record count
2. Next ID counter
3. Array of structs
```

#### 4.3 Auto-save
- Automatic save after every create/update/delete operation
- No manual save required
- Data persists between sessions
- Crash-safe (last completed operation saved)

### 5. User Interface

#### 5.1 Menu System
- Hierarchical menu structure:
  ```
  Main Menu
  ├── Room Management
  │   ├── Display Rooms
  │   ├── Add Room
  │   ├── Edit Room
  │   └── Delete Room
  └── Schedule Management
      ├── Display Schedules
      ├── Add Schedule
      ├── Edit Schedule
      ├── Delete Schedule
      ├── Generate Final Schedule
      ├── View Final Schedule
      ├── View by Room
      └── View by Department
  ```

#### 5.2 Display Features
- Box-drawing characters for visual appeal
- Formatted tables with aligned columns
- Clear headers and separators
- Status indicators (✓ success, ✗ error, ⚠ warning)
- Consistent styling throughout

#### 5.3 Navigation
- Number-based menu selection
- Clear prompts
- "Press Enter to continue" pause system
- Back to previous menu option
- Exit from any level

#### 5.4 Input Handling
- Buffer clearing to prevent input issues
- Input validation
- Error messages for invalid input
- Default value options (keep current)
- Confirmation prompts for destructive actions

### 6. Viewing and Reporting

#### 6.1 Final Schedule View
- Grouped by room
- Shows all assigned courses per room
- Displays complete course information
- Priority scores visible
- Clean, organized format

#### 6.2 Room-Based View
- Filter by specific room ID
- Shows all courses in selected room
- Time-ordered display
- Useful for room utilization analysis

#### 6.3 Department-Based View
- Filter by department name
- Case-insensitive search
- Shows all department courses
- Displays assignment status
- Useful for department coordinators

#### 6.4 Complete Schedule List
- Shows all active schedules
- Displays assignment status
- Priority scores visible
- Before/after assignment comparison

### 7. Conflict Management

#### 7.1 Time Conflict Detection
```
Conflict occurs when:
- Same room is assigned to two schedules
- Time ranges overlap

No conflict when:
- End time of schedule A ≤ Start time of schedule B
- Start time of schedule A ≥ End time of schedule B
```

#### 7.2 Room Type Conflicts
- Lab courses cannot be assigned to classrooms
- System only checks rooms of matching type
- Prevents inappropriate assignments

#### 7.3 Conflict Resolution
- Priority-based: Higher priority gets first choice
- Alternative room selection for lower priority
- Reports when no suitable room available

### 8. Data Validation

#### 8.1 Input Validation
- Non-empty course names
- Valid year ranges (1-5)
- Valid time ranges (end > start)
- Valid menu selections
- Buffer overflow prevention

#### 8.2 Data Integrity
- Referential integrity (room-schedule relationship)
- Soft deletes preserve history
- Cascading updates (room deletion cleans schedules)
- Unique ID management

### 9. Error Handling

#### 9.1 User Errors
- Invalid menu selections → Error message, retry
- Invalid input types → Buffer clearing, retry
- Invalid time ranges → Rejection with explanation
- Non-existent IDs → "Not found" message

#### 9.2 System Errors
- File I/O errors → Error message
- Capacity limits → Clear warning
- Memory allocation errors → Graceful handling

#### 9.3 User Feedback
- Success messages (✓)
- Error messages (✗)
- Warning messages (⚠)
- Informational messages
- Confirmation prompts

### 10. System Design

#### 10.1 Architecture
```
scheduling_system.c
├── Data Structures
│   ├── Room struct
│   └── Schedule struct
├── File I/O
│   ├── Load functions
│   └── Save functions
├── UI Functions
│   ├── Display functions
│   ├── Add functions
│   ├── Edit functions
│   └── Delete functions
├── Algorithm
│   ├── Priority calculation
│   ├── Conflict detection
│   └── Room assignment
└── Main Program
    ├── Main menu
    ├── Room management menu
    └── Schedule management menu
```

#### 10.2 Design Principles
- Single Responsibility: Each function has one clear purpose
- DRY: Reusable utility functions
- Modularity: Clear separation of concerns
- User-friendly: Clear prompts and feedback
- Robust: Error handling and validation

#### 10.3 Scalability
- Configurable limits (MAX_ROOMS, MAX_SCHEDULES)
- Efficient binary file format
- O(n²) worst-case for conflict detection (acceptable for typical use)
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
- No concurrency support (single-user)
- Terminal-based only (no GUI)

## Future Enhancement Possibilities

1. **Extended Features**
   - Weekly schedules (Monday-Friday)
   - Semester-long planning
   - Instructor assignment
   - Room capacity checking
   - Resource requirements (projector, computers)

2. **Reporting**
   - Room utilization statistics
   - Department workload analysis
   - Conflict reports
   - Export to CSV/PDF

3. **Advanced Scheduling**
   - Multiple time preferences
   - Instructor availability
   - Student group sizes
   - Consecutive class detection

4. **User Management**
   - Multi-user support
   - Role-based access
   - Audit logs
   - Change history

5. **Integration**
   - Database backend
   - Web interface
   - Mobile app
   - Email notifications

## Conclusion

The Bahir Dar University Scheduling System provides a comprehensive, intelligent solution for classroom and laboratory scheduling. With full CRUD operations, priority-based scheduling, conflict prevention, and multiple viewing options, it serves as a robust backend system that can be adapted for various educational institutions.
