# Quick Start Guide
## Get Running in 2 Minutes

## Step 1: Compile

```bash
make
```

## Step 2: Run

```bash
./scheduling_system
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
-> Weekly Hours: 3
```

## Step 6: Add Another Schedule

```
-> 2 (Add New Schedule)
-> Course: "Database Systems"
-> Department: "Computer Science"
-> Year: 4
-> Program: 1 (Major)
-> Room Type: 1 (Classroom)
-> Weekly Hours: 4
```

## Step 7: Generate Weekly Schedule

```
-> 5 (Generate Weekly Schedule)
```

You'll see:
```
=== PHASE 1: Assigning with preferred room types ===

Processing: Data Structures Lab         (Priority: 38, Need: 3 hrs, Lab)
  [OK] Fully assigned: 3 hrs

Processing: Database Systems            (Priority: 45, Need: 4 hrs, Classroom)
  [OK] Fully assigned: 4 hrs

=== FINAL SUMMARY ===

[OK] Schedule generation complete!
  Total hours assigned: 7
  Hours failed to assign: 0
  Generated slots: 7
```

## Step 8: View Results

```
-> 6 (View Weekly Timetable)
```

You'll see your courses distributed across Monday-Friday!

```
=== MONDAY ===
  08:00-09:00   Data Structures Lab      Computer Lab 1       Computer Science  3
  08:00-09:00   Database Systems         Lecture Hall A       Computer Science  4

=== TUESDAY ===
  08:00-09:00   Data Structures Lab      Computer Lab 1       Computer Science  3
  08:00-09:00   Database Systems         Lecture Hall A       Computer Science  4

... (continues for the week)
```

## That's It!

You now have a working weekly schedule with:
- 2 rooms (1 lab, 1 classroom)
- 2 courses assigned
- 7 total hours distributed across the week
- No room conflicts
- No student conflicts (different years)
- Priority-based allocation

## Key Concepts

### Weekly Hours (NEW!)
Instead of specifying start/end times, you now specify **total hours per week**:
- Example: "3" means 3 hours per week
- System automatically distributes across Monday-Friday
- Time slots: 08:00-12:30 (morning), 14:00-17:30 (afternoon)

### Two-Phase Room Assignment (NEW!)
1. **Phase 1**: Lab courses -> Labs, Classroom courses -> Classrooms
2. **Phase 2**: If classrooms full, classroom courses can use available labs
3. Lab courses NEVER use classrooms (they need specialized equipment)

### Student Conflict Prevention (NEW!)
- Same department AND same year cannot have overlapping classes
- System automatically prevents scheduling conflicts for students

## Next Steps

- Add more rooms and schedules
- Try editing existing entries
- View by room or department
- Experiment with different weekly hours

## Need More Help?

- **Detailed Guide**: See USAGE_GUIDE.md
- **Features**: See FEATURES.md
- **Overview**: See PROJECT_OVERVIEW.md

## Common Commands

```bash
make          # Compile
make run      # Compile and run
make clean    # Remove compiled files and data
make open     # Compile and run web interface
npm run build # Alternative: compile
npm start     # Alternative: run
```

## Tips

1. **Add rooms first** before schedules
2. **Mix of room types**: Add both classrooms and labs
3. **Set realistic weekly hours**: 2-5 hours per course is typical
4. **Generate after changes**: Always regenerate after adding/editing
5. **Check views**: Use weekly timetable, room, and department filters

## Priority Quick Reference

| Year | Program | Room Type | Priority |
|------|---------|-----------|----------|
| 5    | Major   | Lab       | 58       |
| 5    | Major   | Classroom | 55       |
| 4    | Major   | Lab       | 48       |
| 3    | Major   | Lab       | 38       |
| 1    | Minor   | Classroom | 10       |

**Higher numbers = Higher priority = Gets scheduled first**

## Room Assignment Rules

| Course Type | Phase 1 | Phase 2 (Fallback) |
|-------------|---------|-------------------|
| Lab Course  | Lab rooms only | NO fallback (must use lab) |
| Classroom Course | Classrooms only | Can use available labs |

---

**Ready to start? Just run:**

```bash
make && ./scheduling_system
```
