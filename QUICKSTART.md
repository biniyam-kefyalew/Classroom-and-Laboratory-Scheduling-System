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
Main Menu → 1 (Room Management)
→ 2 (Add New Room)
→ Enter: "Computer Lab 1"
→ Select: 2 (Lab)
```

## Step 4: Add Another Room

```
→ 2 (Add New Room)
→ Enter: "Lecture Hall A"
→ Select: 1 (Classroom)
→ 0 (Back to Main Menu)
```

## Step 5: Add Your First Schedule

```
Main Menu → 2 (Schedule Management)
→ 2 (Add New Schedule)
→ Course: "Data Structures Lab"
→ Department: "Computer Science"
→ Year: 3
→ Program: 1 (Major)
→ Room Type: 2 (Lab)
→ Start Time: 08 00 (8:00 AM)
→ End Time: 11 00 (11:00 AM)
```

## Step 6: Add Another Schedule

```
→ 2 (Add New Schedule)
→ Course: "Database Systems"
→ Department: "Computer Science"
→ Year: 3
→ Program: 1 (Major)
→ Room Type: 1 (Classroom)
→ Start Time: 08 00
→ End Time: 10 00
```

## Step 7: Generate Final Schedule

```
→ 5 (Generate Final Schedule)
```

You'll see:
```
✓ Schedule generation complete!
  Successfully assigned: 2
  Failed to assign: 0
```

## Step 8: View Results

```
→ 6 (View Final Schedule)
```

You'll see your courses assigned to appropriate rooms with no conflicts!

## That's It!

You now have a working schedule with:
- ✓ 2 rooms (1 lab, 1 classroom)
- ✓ 2 courses assigned
- ✓ No conflicts
- ✓ Priority-based allocation

## Next Steps

- Add more rooms and schedules
- Try editing existing entries
- Filter by room or department
- Experiment with different priorities

## Need More Help?

- **Detailed Guide**: See USAGE_GUIDE.md
- **Features**: See FEATURES.md
- **Overview**: See PROJECT_OVERVIEW.md
- **Basic Info**: See README.md

## Common Commands

```bash
make          # Compile
make run      # Compile and run
make clean    # Remove compiled files and data
npm run build # Alternative: compile
npm start     # Alternative: run
```

## Tips

1. **Add rooms first** before schedules
2. **Mix of room types**: Add both classrooms and labs
3. **Vary priorities**: Try different years and program types
4. **Generate after changes**: Always regenerate after adding/editing
5. **Check views**: Use room and department filters

## Priority Quick Reference

| Year | Program | Room Type | Priority |
|------|---------|-----------|----------|
| 5    | Major   | Lab       | 58       |
| 5    | Major   | Classroom | 55       |
| 4    | Major   | Lab       | 48       |
| 3    | Major   | Lab       | 38       |
| 1    | Minor   | Classroom | 10       |

**Higher numbers = Higher priority = Gets scheduled first**

---

**Ready to start? Just run:**

```bash
make && ./scheduling_system
```
