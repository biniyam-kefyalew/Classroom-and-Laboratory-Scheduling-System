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
  - **Weekly hours** (e.g., 3 hours/week, 6 hours/week)
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
  - **By Year & Department** (weekly view)
  - **By specific Room** (with selection dropdown)
  - All Rooms overview

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

### 3. Intelligent Distributed Scheduling Algorithm

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

#### 3.2 **Distributed Class Scheduling (NEW)**

The system now **distributes class hours evenly across 5 days** (Monday-Friday):

```
Example: 6-hour course distribution
- Monday:    90 min (1.5 hrs)
- Tuesday:   90 min (1.5 hrs)
- Wednesday: 90 min (1.5 hrs)
- Thursday:  90 min (1.5 hrs)
- Friday:    (rest or additional if needed)
Total: 6 hours distributed across the week

Key Rules:
- Minimum session duration: 90 minutes (1 hour 30 min)
- Maximum sessions per day: Calculated to prevent bunching
- System finds day with least assigned time first
- Ensures even spread across Mon-Fri
```

#### 3.3 Two-Phase Room Assignment

**Phase 1: Preferred Room Types**
1. Lab courses are assigned ONLY to lab rooms
2. Classroom courses are assigned ONLY to classrooms
3. Higher priority courses get first choice
4. Classes distributed evenly across 5 days

**Phase 2: Lab Fallback for Classrooms**
1. If classrooms are full, classroom courses can use available lab rooms
2. Lab courses NEVER use classrooms (they need specialized equipment)
3. This maximizes room utilization

#### 3.4 Scheduling Rules
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

4. **Even Distribution Across 5 Days**
   - System automatically distributes weekly hours across Monday-Friday
   - Prevents bunching all hours on one or two days
   - Minimum session duration: 90 minutes
   - Reports distribution per day (e.g., M:90 T:90 W:90 Th:90 F:0)

#### 3.5 Algorithm Steps
1. Collect all active schedules
2. Sort by priority (descending)
3. **Phase 1**: For each schedule (highest priority first):
   - Find day with minimum assigned time (for even distribution)
   - Find rooms matching required type ONLY
   - Check each room for time conflicts
   - Check for student conflicts (same dept/year)
   - Assign 90-min session to first conflict-free slot
   - Repeat until weekly hours fulfilled or no slots available
4. **Phase 2**: For classroom courses with unassigned hours:
   - Try to use available lab rooms as fallback
   - Lab courses are NOT included in this phase
5. Save assignments
6. Report statistics (assigned/failed with minutes breakdown)

### 4. Weekly Schedule Generation

#### 4.1 Time Configuration
- **Days**: Monday through Friday (5 days)
- **Morning Session**: 08:00 - 12:30 (4.5 hours)
- **Afternoon Session**: 14:00 - 17:30 (3.5 hours)
- **Slot Duration**: 90 minutes (1 hour 30 min) minimum
- **Time Slots**:
  - Morning: 08:00-09:30, 09:30-11:00, 11:00-12:30
  - Afternoon: 14:00-15:30, 15:30-17:00, 17:00-17:30

#### 4.2 Weekly Hours Distribution (NEW)
```
Example: Course with 6 weekly hours
- Target per day: 72 minutes (6 hrs / 5 days)
- Actual distribution with 90-min sessions:
  - Monday:    90 min
  - Tuesday:   90 min
  - Wednesday: 90 min
  - Thursday:  90 min
  Total: 360 min = 6 hours evenly distributed

Example: Course with 3 weekly hours
- Target per day: 36 minutes
- Actual distribution:
  - Monday:    90 min
  - Wednesday: 90 min
  Total: 180 min = 3 hours across 2 days
```

#### 4.3 Generated Slots
Each generated slot contains:
- Schedule ID (reference to course)
- Room ID (assigned room)
- Day of week (Monday-Friday)
- Start time (in minutes from midnight)
- End time (in minutes from midnight)
- Duration tracking (for distribution calculation)
- Active status

### 5. Viewing Options

#### 5.1 Weekly Timetable View
- Organized by day (Monday-Friday)
- Shows morning and afternoon sessions
- Displays room assignments
- Complete course information per slot

#### 5.2 Final Schedule View
- Grouped by room
- Shows all assigned courses per room
- Displays complete course information
- Priority scores visible
- Clean, organized format

#### 5.3 **By Year & Department View (Enhanced)**
- Filter by specific year AND department
- Weekly view showing all 5 days
- Shows time, duration, course, room for each slot
- Useful for students to see their weekly schedule

#### 5.4 **By Room View (NEW)**
- **Dropdown selection** to choose specific room
- Weekly schedule for selected room
- Shows utilization summary (total hours used)
- Day-by-day breakdown with duration
- Links to detailed view from All Rooms page

#### 5.5 **All Rooms Overview (Enhanced)**
- **Utilization summary table** showing hours per day per room
- Quick links to individual room schedules
- Shows total weekly hours per room
- Detailed schedule below summary

### 6. Data Persistence

#### 6.1 Binary File Storage
- Efficient storage format
- Fast read/write operations
- Three data files:
  - `rooms.dat`: Room records
  - `schedules.dat`: Schedule records
  - `generated_slots.dat`: Generated weekly timetable

#### 6.2 Auto-save
- Automatic save after every create/update/delete operation
- No manual save required
- Data persists between sessions

### 7. Web Interface Features

#### 7.1 Navigation Menu
```
Home | Rooms | Add Room | Schedules | Add Schedule | Generate | 
Weekly Timetable | By Year/Dept | By Room | All Rooms | Final Schedule
```

#### 7.2 Generate Schedule Page
- Clear explanation of distribution algorithm
- Shows example of how hours are spread
- Displays time slot configuration
- One-click schedule generation

#### 7.3 View Options
- **By Year/Dept**: Select year and department, see weekly schedule
- **By Room**: Select room from dropdown, see weekly utilization
- **All Rooms**: Overview with utilization stats and links

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

### 9. System Design

#### 9.1 Architecture
```
Web Interface (web_interface.c)
|-- Navigation and routing
|-- Form handling
|-- Schedule generation
|-- Multiple view handlers
|
Console Application (scheduling_system.c)
|-- Menu-driven interface
|-- Same scheduling algorithm
|-- Same view functions
|
Data Layer
|-- rooms.dat
|-- schedules.dat
+-- generated_slots.dat
```

#### 9.2 Key Functions (Web Interface)
```c
web_count_minutes_on_day()     // Track time assigned per day
web_calc_ideal_minutes_per_day() // Calculate target distribution
web_calc_max_minutes_per_day() // Prevent over-assignment
web_generate_schedule()        // Main scheduling with distribution
handle_schedule_by_room_form() // Room selection and display
handle_all_rooms_schedule()    // Overview with utilization
```

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
- Sub-second schedule generation
- Efficient binary file I/O

### Limitations
- Maximum 100 rooms
- Maximum 500 schedules
- Maximum 2000 generated time slots
- Minimum session duration: 90 minutes
- No concurrency support (single-user)

## Summary of New Features

1. **Distributed Scheduling**: Classes spread evenly across Mon-Fri
2. **90-Minute Minimum Sessions**: Each class is at least 1.5 hours
3. **By Room View**: Dropdown to select and view specific room schedule
4. **Room Utilization Stats**: Hours per day per room in overview
5. **Enhanced Generation Display**: Shows distribution per day (M:90 T:90 etc.)
6. **Minutes Tracking**: Precise tracking in minutes for accurate distribution

## Conclusion

The Bahir Dar University Scheduling System provides comprehensive, intelligent scheduling with automatic distribution of class hours across the week. The system ensures no student has overlapping classes, respects room type requirements, and maximizes room utilization through the two-phase assignment algorithm.
