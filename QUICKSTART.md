# Quick Start Guide
## Get Running in 2 Minutes

## Step 1: Compile

```bash
make
```

## Step 2: Run

```bash
./scheduling_system

# Or for web interface:
make open
```

## Step 3: Add Your First Room

```
Main Menu -> 1 (Room Management)
-> 2 (Add New Room)
-> Enter: "Computer Lab 1"
-> Select: 2 (Lab)
```

## Step 4: Add Another Room

```
-> 2 (Add New Room)
-> Enter: "Lecture Hall A"
-> Select: 1 (Classroom)
-> 0 (Back to Main Menu)
```

## Step 5: Add Your First Schedule

```
Main Menu -> 2 (Schedule Management)
-> 2 (Add New Schedule)
-> Course: "Data Structures Lab"
-> Department: "Computer Science"
-> Year: 3
-> Program: 1 (Major)
-> Room Type: 2 (Lab)
-> Weekly Hours: 6
```

## Step 6: Generate Distributed Schedule

```
-> 5 (Generate Weekly Schedule)
```

You'll see the **distributed** output:

```
=== Schedule Generation - Distributed Across 5 Days ===

Distribution Rules:
1. Classes are distributed evenly across Monday-Friday
2. Minimum session duration: 1 hour 30 minutes (90 min)
3. Example: 6 hrs/week = ~72 min/day across 5 days

Processing: Data Structures Lab (Priority: 38, Weekly: 6 hrs)
  Distribution: M:90 T:90 W:90 Th:90 F:0
  Status: Complete (6h0m)

=== FINAL SUMMARY ===
Total time assigned: 6 hours 0 minutes
Session duration: 1 hour 30 minutes (90 min)
```

## Step 7: View Results

### Option A: View by Year & Department
```
-> By Year/Dept
-> Select Year: 3
-> Select Department: Computer Science
-> See weekly schedule for CS Year 3 students
```

### Option B: View by Room
```
-> By Room
-> Select: Computer Lab 1
-> See weekly schedule and utilization for that room
```

### Option C: View All Rooms Overview
```
-> All Rooms
-> See utilization table: hours per day per room
```

## That's It!

You now have a working schedule with:
- Classes distributed evenly across Mon-Fri
- 90-minute minimum sessions
- Multiple viewing options

## Key Concepts

### Distributed Scheduling (NEW!)
Classes are **automatically spread across 5 days**:

```
6-hour course:
  Monday:    90 min
  Tuesday:   90 min
  Wednesday: 90 min
  Thursday:  90 min
  Total: 6 hours (NOT bunched on 1-2 days)
```

### Minimum Session Duration
- Each class session is **at least 90 minutes** (1.5 hours)
- Ensures meaningful learning time per session

### Viewing Options

| View | Use Case |
|------|----------|
| By Year/Dept | Students checking their weekly schedule |
| By Room | Room managers checking room utilization |
| All Rooms | Overview of all room usage |
| Final Schedule | Complete schedule grouped by room |

## Common Commands

```bash
make          # Compile
make run      # Compile and run console
make open     # Compile and run web interface
make clean    # Remove compiled files and data
npm run build # Alternative: compile
npm start     # Alternative: run
```

## Tips

1. **Add rooms first** before schedules
2. **Set realistic weekly hours**: 2-6 hours per course
3. **Generate after changes**: Always regenerate after adding/editing
4. **Check distribution**: Look at M:XX T:XX W:XX output to verify spread

## Priority Quick Reference

| Year | Program | Room Type | Priority |
|------|---------|-----------|----------|
| 5    | Major   | Lab       | 58       |
| 5    | Major   | Classroom | 55       |
| 4    | Major   | Lab       | 48       |
| 3    | Major   | Lab       | 38       |
| 1    | Minor   | Classroom | 10       |

## Distribution Example

```
Course: Database Systems
Weekly Hours: 6
Session Duration: 90 min

Distribution across week:
+-------+-------+-------+-------+-------+
|  Mon  |  Tue  |  Wed  |  Thu  |  Fri  |
+-------+-------+-------+-------+-------+
| 90min | 90min | 90min | 90min |   -   |
+-------+-------+-------+-------+-------+
Total: 360 min = 6 hours (evenly distributed)
```

---

**Ready to start?**

```bash
make && ./scheduling_system
```

**Or for web interface:**

```bash
make open
```
