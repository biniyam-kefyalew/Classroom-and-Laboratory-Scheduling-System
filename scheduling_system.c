#include "scheduling_system.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#define ROOMS_FILE "/var/www/data/rooms.dat"
#define SCHEDULES_FILE "/var/www/data/schedules.dat"
#define GENERATED_SLOTS_FILE "/var/www/data/generated_slots.dat"

/* ---------- GLOBAL DATA ---------- */
Room rooms[MAX_ROOMS];
Schedule schedules[MAX_SCHEDULES];
GeneratedSlot generated_slots[MAX_GENERATED_SLOTS];

int room_count = 0;
int schedule_count = 0;
int generated_slot_count = 0;
int next_room_id = 1;
int next_schedule_id = 1;

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pause_screen() {
    printf("\nPress Enter to continue...");
    fflush(stdout);

    clear_input_buffer();  
    getchar();             
}


void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void load_rooms() {
    FILE *file = fopen(ROOMS_FILE, "rb");
    if (file == NULL) {
        room_count = 0;
        next_room_id = 1;
        return;
    }

    fread(&room_count, sizeof(int), 1, file);
    fread(&next_room_id, sizeof(int), 1, file);
    fread(rooms, sizeof(Room), room_count, file);
    fclose(file);
}

void save_rooms() {
    FILE *file = fopen(ROOMS_FILE, "wb");
    if (file == NULL) {
        printf("Error saving rooms!\n");
        return;
    }

    fwrite(&room_count, sizeof(int), 1, file);
    fwrite(&next_room_id, sizeof(int), 1, file);
    fwrite(rooms, sizeof(Room), room_count, file);
    fclose(file);
}

void load_schedules() {
    FILE *file = fopen(SCHEDULES_FILE, "rb");
    if (file == NULL) {
        schedule_count = 0;
        next_schedule_id = 1;
        return;
    }

    fread(&schedule_count, sizeof(int), 1, file);
    fread(&next_schedule_id, sizeof(int), 1, file);
    fread(schedules, sizeof(Schedule), schedule_count, file);
    fclose(file);
}

void save_schedules() {
    FILE *file = fopen(SCHEDULES_FILE, "wb");
    if (file == NULL) {
        printf("Error saving schedules!\n");
        return;
    }

    fwrite(&schedule_count, sizeof(int), 1, file);
    fwrite(&next_schedule_id, sizeof(int), 1, file);
    fwrite(schedules, sizeof(Schedule), schedule_count, file);
    fclose(file);
}

void load_generated_slots() {
    FILE *file = fopen(GENERATED_SLOTS_FILE, "rb");
    if (file == NULL) {
        generated_slot_count = 0;
        return;
    }

    fread(&generated_slot_count, sizeof(int), 1, file);
    fread(generated_slots, sizeof(GeneratedSlot), generated_slot_count, file);
    fclose(file);
}

void save_generated_slots() {
    FILE *file = fopen(GENERATED_SLOTS_FILE, "wb");
    if (file == NULL) {
        printf("Error saving generated slots!\n");
        return;
    }

    fwrite(&generated_slot_count, sizeof(int), 1, file);
    fwrite(generated_slots, sizeof(GeneratedSlot), generated_slot_count, file);
    fclose(file);
}

const char* get_room_type_string(RoomType type) {
    return type == CLASSROOM ? "Classroom" : "Lab";
}

const char* get_program_type_string(ProgramType type) {
    return type == MAJOR ? "Major" : "Minor";
}

const char* get_day_string(DayOfWeek day) {
    switch(day) {
        case MONDAY: return "Monday";
        case TUESDAY: return "Tuesday";
        case WEDNESDAY: return "Wednesday";
        case THURSDAY: return "Thursday";
        case FRIDAY: return "Friday";
        default: return "Unknown";
    }
}

void format_time(int time, char *buffer) {
    int hour = time / 60;
    int minute = time % 60;
    sprintf(buffer, "%02d:%02d", hour, minute);
}

void display_rooms() {
    clear_screen();
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║              ROOM LIST - BAHIR DAR UNIVERSITY              ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");

    if (room_count == 0) {
        printf("No rooms available.\n");
        pause_screen();
        return;
    }

    printf("%-10s %-40s %-15s\n", "Room ID", "Room Name", "Type");
    printf("───────────────────────────────────────────────────────────────────\n");

    for (int i = 0; i < room_count; i++) {
        if (rooms[i].is_active) {
            printf("%-10d %-40s %-15s\n",
                   rooms[i].id,
                   rooms[i].name,
                   get_room_type_string(rooms[i].type));
        }
    }

    pause_screen();
}

void add_room() {
    clear_screen();
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                        ADD NEW ROOM                        ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");

    if (room_count >= MAX_ROOMS) {
        printf("Maximum room capacity reached!\n");
        pause_screen();
        return;
    }

    Room new_room;
    new_room.id = next_room_id++;
    new_room.is_active = 1;

    printf("Enter room name: ");
    clear_input_buffer();
    fgets(new_room.name, MAX_NAME_LENGTH, stdin);
    new_room.name[strcspn(new_room.name, "\n")] = 0;

    int type_choice;
    printf("\nSelect room type:\n");
    printf("1. Classroom\n");
    printf("2. Lab\n");
    printf("Enter choice: ");
    scanf("%d", &type_choice);

    new_room.type = (type_choice == 2) ? LAB : CLASSROOM;

    rooms[room_count++] = new_room;
    save_rooms();

    printf("\n✓ Room added successfully! (ID: %d)\n", new_room.id);
    pause_screen();
}

void edit_room() {
    clear_screen();
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                        EDIT ROOM                           ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");

    int room_id;
    printf("Enter room ID to edit: ");
    scanf("%d", &room_id);

    int found = -1;
    for (int i = 0; i < room_count; i++) {
        if (rooms[i].id == room_id && rooms[i].is_active) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        printf("\n✗ Room not found!\n");
        pause_screen();
        return;
    }

    printf("\nCurrent room details:\n");
    printf("Name: %s\n", rooms[found].name);
    printf("Type: %s\n", get_room_type_string(rooms[found].type));

    printf("\nEnter new room name (or press Enter to keep current): ");
    clear_input_buffer();
    char new_name[MAX_NAME_LENGTH];
    fgets(new_name, MAX_NAME_LENGTH, stdin);
    new_name[strcspn(new_name, "\n")] = 0;

    if (strlen(new_name) > 0) {
        strcpy(rooms[found].name, new_name);
    }

    int type_choice;
    printf("\nSelect new room type:\n");
    printf("1. Classroom\n");
    printf("2. Lab\n");
    printf("3. Keep current\n");
    printf("Enter choice: ");
    scanf("%d", &type_choice);

    if (type_choice == 1) {
        rooms[found].type = CLASSROOM;
    } else if (type_choice == 2) {
        rooms[found].type = LAB;
    }

    save_rooms();
    printf("\n✓ Room updated successfully!\n");
    pause_screen();
}

void delete_room() {
    clear_screen();
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                       DELETE ROOM                          ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");

    int room_id;
    printf("Enter room ID to delete: ");
    scanf("%d", &room_id);

    int found = -1;
    for (int i = 0; i < room_count; i++) {
        if (rooms[i].id == room_id && rooms[i].is_active) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        printf("\n✗ Room not found!\n");
        pause_screen();
        return;
    }

    printf("\nAre you sure you want to delete room '%s'? (y/n): ", rooms[found].name);
    clear_input_buffer();
    char confirm = getchar();

    if (tolower(confirm) == 'y') {
        rooms[found].is_active = 0;

        for (int i = 0; i < schedule_count; i++) {
            if (schedules[i].assigned_room_id == room_id) {
                schedules[i].assigned_room_id = -1;
            }
        }

        save_rooms();
        save_schedules();
        printf("\n✓ Room deleted successfully!\n");
    } else {
        printf("\nDeletion cancelled.\n");
    }

    pause_screen();
}

void display_schedules() {
    clear_screen();
    printf("\n╔════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                                       ALL SCHEDULES - BAHIR DAR UNIVERSITY                                             ║\n");
    printf("╚════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╝\n\n");

    if (schedule_count == 0) {
        printf("No schedules available.\n");
        pause_screen();
        return;
    }

    printf("%-5s %-25s %-15s %-6s %-10s %-12s %-12s %-25s %-8s\n",
           "ID", "Course", "Dept", "Year", "Program", "Room Type", "Hrs/Week", "Assigned Room", "Priority");
    printf("────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────\n");

    for (int i = 0; i < schedule_count; i++) {
        if (schedules[i].is_active) {
            char hours_str[15];
            sprintf(hours_str, "%d hrs", schedules[i].weekly_hours);

            char assigned_room[30] = "Not assigned";
            if (schedules[i].assigned_room_id != -1) {
                for (int j = 0; j < room_count; j++) {
                    if (rooms[j].id == schedules[i].assigned_room_id && rooms[j].is_active) {
                        strcpy(assigned_room, rooms[j].name);
                        break;
                    }
                }
            }

            printf("%-5d %-25s %-15s %-6d %-10s %-12s %-12s %-25s %-8d\n",
                   schedules[i].id,
                   schedules[i].course_name,
                   schedules[i].department,
                   schedules[i].student_year,
                   get_program_type_string(schedules[i].program_type),
                   get_room_type_string(schedules[i].required_room_type),
                   hours_str,
                   assigned_room,
                   schedules[i].priority);
        }
    }

    pause_screen();
}

void add_schedule() {
    clear_screen();
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                     ADD NEW SCHEDULE                       ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");

    if (schedule_count >= MAX_SCHEDULES) {
        printf("Maximum schedule capacity reached!\n");
        pause_screen();
        return;
    }

    Schedule new_schedule;
    new_schedule.id = next_schedule_id++;
    new_schedule.is_active = 1;
    new_schedule.assigned_room_id = -1;

    printf("Enter course name: ");
    clear_input_buffer();
    fgets(new_schedule.course_name, MAX_NAME_LENGTH, stdin);
    new_schedule.course_name[strcspn(new_schedule.course_name, "\n")] = 0;

    printf("Enter department: ");
    fgets(new_schedule.department, MAX_DEPT_LENGTH, stdin);
    new_schedule.department[strcspn(new_schedule.department, "\n")] = 0;

    printf("Enter student year (1-5): ");
    scanf("%d", &new_schedule.student_year);

    int program_choice;
    printf("\nSelect program type:\n");
    printf("1. Major\n");
    printf("2. Minor\n");
    printf("Enter choice: ");
    scanf("%d", &program_choice);
    new_schedule.program_type = (program_choice == 1) ? MAJOR : MINOR;

    int room_type_choice;
    printf("\nSelect required room type:\n");
    printf("1. Classroom\n");
    printf("2. Lab\n");
    printf("Enter choice: ");
    scanf("%d", &room_type_choice);
    new_schedule.required_room_type = (room_type_choice == 2) ? LAB : CLASSROOM;

    printf("\nEnter total weekly hours (e.g., 3 for 3 hours per week): ");
    scanf("%d", &new_schedule.weekly_hours);

    if (new_schedule.weekly_hours <= 0 || new_schedule.weekly_hours > 20) {
        printf("\n✗ Invalid weekly hours! Must be between 1 and 20.\n");
        pause_screen();
        return;
    }

    // Calculate priority:
    // - Higher year students get higher priority (year * 10)
    // - Major programs get +5 priority
    // - Lab courses get +3 priority (need special rooms)
    new_schedule.priority = (new_schedule.student_year * 10) +
                           (new_schedule.program_type == MAJOR ? 5 : 0) +
                           (new_schedule.required_room_type == LAB ? 3 : 0);

    schedules[schedule_count++] = new_schedule;
    save_schedules();

    printf("\n✓ Schedule added successfully! (ID: %d, Priority: %d)\n",
           new_schedule.id, new_schedule.priority);
    printf("  Weekly hours: %d hours\n", new_schedule.weekly_hours);
    pause_screen();
}

void edit_schedule() {
    clear_screen();
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                      EDIT SCHEDULE                         ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");

    int schedule_id;
    printf("Enter schedule ID to edit: ");
    scanf("%d", &schedule_id);

    int found = -1;
    for (int i = 0; i < schedule_count; i++) {
        if (schedules[i].id == schedule_id && schedules[i].is_active) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        printf("\n✗ Schedule not found!\n");
        pause_screen();
        return;
    }

    printf("\nCurrent schedule details:\n");
    printf("Course: %s\n", schedules[found].course_name);
    printf("Department: %s\n", schedules[found].department);
    printf("Year: %d\n", schedules[found].student_year);
    printf("Program: %s\n", get_program_type_string(schedules[found].program_type));
    printf("Room Type: %s\n", get_room_type_string(schedules[found].required_room_type));
    printf("Weekly Hours: %d hours\n", schedules[found].weekly_hours);

    printf("\nEnter new course name (or press Enter to keep current): ");
    clear_input_buffer();
    char new_course[MAX_NAME_LENGTH];
    fgets(new_course, MAX_NAME_LENGTH, stdin);
    new_course[strcspn(new_course, "\n")] = 0;

    if (strlen(new_course) > 0) {
        strcpy(schedules[found].course_name, new_course);
    }

    printf("Enter new department (or press Enter to keep current): ");
    char new_dept[MAX_DEPT_LENGTH];
    fgets(new_dept, MAX_DEPT_LENGTH, stdin);
    new_dept[strcspn(new_dept, "\n")] = 0;

    if (strlen(new_dept) > 0) {
        strcpy(schedules[found].department, new_dept);
    }

    int new_year;
    printf("Enter new student year (1-5, or 0 to keep current): ");
    scanf("%d", &new_year);
    if (new_year > 0) {
        schedules[found].student_year = new_year;
    }

    int new_hours;
    printf("Enter new weekly hours (1-20, or 0 to keep current): ");
    scanf("%d", &new_hours);
    if (new_hours > 0 && new_hours <= 20) {
        schedules[found].weekly_hours = new_hours;
    }

    // Recalculate priority
    schedules[found].priority = (schedules[found].student_year * 10) +
                               (schedules[found].program_type == MAJOR ? 5 : 0) +
                               (schedules[found].required_room_type == LAB ? 3 : 0);

    schedules[found].assigned_room_id = -1;

    save_schedules();
    printf("\n✓ Schedule updated successfully!\n");
    pause_screen();
}

void delete_schedule() {
    clear_screen();
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                     DELETE SCHEDULE                        ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");

    int schedule_id;
    printf("Enter schedule ID to delete: ");
    scanf("%d", &schedule_id);

    int found = -1;
    for (int i = 0; i < schedule_count; i++) {
        if (schedules[i].id == schedule_id && schedules[i].is_active) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        printf("\n✗ Schedule not found!\n");
        pause_screen();
        return;
    }

    printf("\nAre you sure you want to delete schedule for '%s'? (y/n): ",
           schedules[found].course_name);
    clear_input_buffer();
    char confirm = getchar();

    if (tolower(confirm) == 'y') {
        schedules[found].is_active = 0;
        save_schedules();
        printf("\n✓ Schedule deleted successfully!\n");
    } else {
        printf("\nDeletion cancelled.\n");
    }

    pause_screen();
}

int compare_schedules_by_priority(const void *a, const void *b) {
    Schedule *s1 = (Schedule *)a;
    Schedule *s2 = (Schedule *)b;
    return s2->priority - s1->priority;  // Descending order (highest priority first)
}

// Check if a time slot conflicts with existing generated slots
int check_slot_conflict(int room_id, DayOfWeek day, int start_time, int end_time) {
    for (int i = 0; i < generated_slot_count; i++) {
        if (!generated_slots[i].is_active) continue;
        if (generated_slots[i].room_id != room_id) continue;
        if (generated_slots[i].day != day) continue;
        
        // Check time overlap
        int overlap = !(end_time <= generated_slots[i].start_time ||
                       start_time >= generated_slots[i].end_time);
        if (overlap) {
            return 1;  // Conflict found
        }
    }
    return 0;  // No conflict
}

// Check if same department/year has class at this time (avoid student conflicts)
int check_student_conflict(const char* department, int year, DayOfWeek day, int start_time, int end_time) {
    for (int i = 0; i < generated_slot_count; i++) {
        if (!generated_slots[i].is_active) continue;
        if (generated_slots[i].day != day) continue;
        
        // Find the schedule for this slot
        for (int j = 0; j < schedule_count; j++) {
            if (schedules[j].id == generated_slots[i].schedule_id && schedules[j].is_active) {
                // Check if same department and year
                if (strcasecmp(schedules[j].department, department) == 0 && 
                    schedules[j].student_year == year) {
                    // Check time overlap
                    int overlap = !(end_time <= generated_slots[i].start_time ||
                                   start_time >= generated_slots[i].end_time);
                    if (overlap) {
                        return 1;  // Student conflict
                    }
                }
                break;
            }
        }
    }
    return 0;
}

// Helper function to check if room is suitable for a schedule
// Returns: 0 = not suitable, 1 = exact match (preferred), 2 = fallback match
int check_room_suitability(RoomType room_type, RoomType required_type, int allow_fallback) {
    // Exact match - always preferred
    if (room_type == required_type) {
        return 1;  // Exact match
    }
    
    // Lab courses MUST use lab rooms (they need special equipment)
    if (required_type == LAB) {
        return 0;  // Lab courses cannot use classrooms
    }
    
    // Classroom courses can use lab rooms as fallback IF allowed
    // (only when classrooms are not available)
    if (required_type == CLASSROOM && room_type == LAB && allow_fallback) {
        return 2;  // Fallback match - classroom course can use lab if needed
    }
    
    return 0;  // Not suitable
}

// Try to assign a slot to a schedule, returns 1 if successful
int try_assign_slot(Schedule *sched, DayOfWeek day, int start_time, int end_time, int allow_lab_fallback) {
    int assigned = 0;
    int best_room_idx = -1;
    int best_suitability = 0;
    
    // Find the best available room
    for (int r = 0; r < room_count; r++) {
        if (!rooms[r].is_active) continue;
        
        // Check room suitability
        int suitability = check_room_suitability(rooms[r].type, sched->required_room_type, allow_lab_fallback);
        if (suitability == 0) continue;
        
        // Check room availability (no double booking)
        if (check_slot_conflict(rooms[r].id, day, start_time, end_time)) continue;
        
        // Check student availability (same dept/year shouldn't have overlapping classes)
        if (check_student_conflict(sched->department, sched->student_year, 
                                   day, start_time, end_time)) continue;
        
        // Prefer exact matches over fallback matches
        if (suitability > best_suitability) {
            best_suitability = suitability;
            best_room_idx = r;
            
            // If we found an exact match, use it immediately
            if (suitability == 1) break;
        }
    }
    
    // Assign to best available room
    if (best_room_idx >= 0 && generated_slot_count < MAX_GENERATED_SLOTS) {
        generated_slots[generated_slot_count].schedule_id = sched->id;
        generated_slots[generated_slot_count].room_id = rooms[best_room_idx].id;
        generated_slots[generated_slot_count].day = day;
        generated_slots[generated_slot_count].start_time = start_time;
        generated_slots[generated_slot_count].end_time = end_time;
        generated_slots[generated_slot_count].is_active = 1;
        generated_slot_count++;
        
        // Update schedule's assigned room
        for (int k = 0; k < schedule_count; k++) {
            if (schedules[k].id == sched->id) {
                schedules[k].assigned_room_id = rooms[best_room_idx].id;
                break;
            }
        }
        
        if (best_suitability == 2) {
            printf("    (Using lab room '%s' as fallback)\n", rooms[best_room_idx].name);
        }
        
        assigned = 1;
    }
    
    return assigned;
}

// Count how many slots already assigned for a schedule on a given day
int count_slots_on_day(int schedule_id, DayOfWeek day) {
    int count = 0;
    for (int i = 0; i < generated_slot_count; i++) {
        if (generated_slots[i].is_active &&
            generated_slots[i].schedule_id == schedule_id &&
            generated_slots[i].day == day) {
            count++;
        }
    }
    return count;
}

// Calculate max slots per day for even distribution
// e.g., 6 hours across 5 days = max 2 slots per day (some days get 1)
int calculate_max_slots_per_day(int total_hours) {
    int base = total_hours / DAYS_PER_WEEK;
    int remainder = total_hours % DAYS_PER_WEEK;
    // If there's remainder, some days will have base+1
    return (remainder > 0) ? base + 1 : base;
}

void generate_weekly_schedule() {
    clear_screen();
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║           GENERATING WEEKLY SCHEDULE...                    ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");

    printf("Schedule Configuration:\n");
    printf("  Days: Monday - Friday\n");
    printf("  Morning: 08:00 - 12:30\n");
    printf("  Afternoon: 14:00 - 17:30\n");
    printf("  Minimum Class Duration: 1 hour (or 1.5 hrs)\n");
    printf("  Distribution: Classes spread evenly across 5 days\n\n");
    
    printf("Room Assignment Rules:\n");
    printf("  1. Lab courses MUST use lab rooms (priority for labs)\n");
    printf("  2. Classroom courses prefer classrooms\n");
    printf("  3. If classrooms full, classroom courses can use free labs\n");
    printf("  4. Same dept/year students cannot have overlapping classes\n\n");

    // Clear existing generated slots
    generated_slot_count = 0;

    // Create sorted copy of active schedules by priority
    Schedule *sorted_schedules = malloc(schedule_count * sizeof(Schedule));
    if (!sorted_schedules) {
        printf("Memory allocation failed.\n");
        pause_screen();
        return;
    }

    int active_count = 0;
    for (int i = 0; i < schedule_count; i++) {
        if (schedules[i].is_active) {
            sorted_schedules[active_count++] = schedules[i];
        }
    }

    // Sort by priority (highest first)
    qsort(sorted_schedules, active_count, sizeof(Schedule), compare_schedules_by_priority);

    int total_assigned = 0;
    int total_failed = 0;

    // Time slots: 1.5 hour blocks for better class sessions
    // Morning: 08:00-09:30, 09:30-11:00, 11:00-12:30 (3 slots)
    // Afternoon: 14:00-15:30, 15:30-17:00, 17:00-17:30 (2.5 slots, last one shorter)
    // We use 90-minute slots (1.5 hours) as minimum, but count as 1 "session"
    int morning_slots[] = {480, 570, 660};     // 08:00, 09:30, 11:00
    int afternoon_slots[] = {840, 930, 1020};  // 14:00, 15:30, 17:00
    int slot_duration = 90;  // 1.5 hours (90 min) per slot - minimum class duration

    // =========================================================================
    // PHASE 1: Assign all courses using PREFERRED room types only
    // Distribute evenly: max 1-2 slots per day depending on weekly hours
    // =========================================================================
    printf("═══ PHASE 1: Assigning with even distribution ═══\n\n");
    
    int *hours_assigned = calloc(active_count, sizeof(int));
    if (!hours_assigned) {
        free(sorted_schedules);
        printf("Memory allocation failed.\n");
        pause_screen();
        return;
    }

    for (int s = 0; s < active_count; s++) {
        Schedule *sched = &sorted_schedules[s];
        int hours_remaining = sched->weekly_hours;
        int max_per_day = calculate_max_slots_per_day(sched->weekly_hours);

        printf("Processing: %-25s (Priority: %d, Need: %d hrs, Max %d/day, %s)\n",
               sched->course_name, sched->priority, sched->weekly_hours,
               max_per_day, get_room_type_string(sched->required_room_type));

        // Round-robin through days to distribute evenly
        int pass = 0;
        while (hours_remaining > 0 && pass < 10) {  // max 10 passes to prevent infinite loop
            pass++;
            
            for (int day = MONDAY; day <= FRIDAY && hours_remaining > 0; day++) {
                // Check if this day already has enough slots for even distribution
                int slots_on_day = count_slots_on_day(sched->id, day);
                if (slots_on_day >= max_per_day) continue;  // Skip this day, already has enough

                int assigned_this_day = 0;

                // Try morning slots
                for (int slot_idx = 0; slot_idx < 3 && hours_remaining > 0 && !assigned_this_day; slot_idx++) {
                    int start_time = morning_slots[slot_idx];
                    int end_time = start_time + slot_duration;
                    
                    if (end_time > MORNING_END) {
                        end_time = MORNING_END;
                        if (end_time - start_time < 60) continue;  // Min 1 hour
                    }

                    // Check if already assigned at this time
                    int already_assigned = 0;
                    for (int g = 0; g < generated_slot_count; g++) {
                        if (generated_slots[g].schedule_id == sched->id &&
                            generated_slots[g].day == day &&
                            generated_slots[g].start_time == start_time) {
                            already_assigned = 1;
                            break;
                        }
                    }
                    if (already_assigned) continue;

                    if (try_assign_slot(sched, day, start_time, end_time, 0)) {
                        hours_remaining--;
                        hours_assigned[s]++;
                        assigned_this_day = 1;  // Move to next day for even distribution
                    }
                }

                // Try afternoon slots if nothing assigned in morning
                for (int slot_idx = 0; slot_idx < 3 && hours_remaining > 0 && !assigned_this_day; slot_idx++) {
                    int start_time = afternoon_slots[slot_idx];
                    int end_time = start_time + slot_duration;
                    
                    if (end_time > AFTERNOON_END) {
                        end_time = AFTERNOON_END;
                        if (end_time - start_time < 60) continue;  // Min 1 hour
                    }

                    int already_assigned = 0;
                    for (int g = 0; g < generated_slot_count; g++) {
                        if (generated_slots[g].schedule_id == sched->id &&
                            generated_slots[g].day == day &&
                            generated_slots[g].start_time == start_time) {
                            already_assigned = 1;
                            break;
                        }
                    }
                    if (already_assigned) continue;

                    if (try_assign_slot(sched, day, start_time, end_time, 0)) {
                        hours_remaining--;
                        hours_assigned[s]++;
                        assigned_this_day = 1;
                    }
                }
            }
        }

        if (hours_remaining > 0) {
            printf("  ... %d hrs still needed (will try fallback)\n", hours_remaining);
        } else {
            printf("  ✓ Fully assigned: %d hrs across week\n", hours_assigned[s]);
        }
    }

    // =========================================================================
    // PHASE 2: Lab fallback for classroom courses (with even distribution)
    // =========================================================================
    printf("\n═══ PHASE 2: Using lab rooms for overflow classroom courses ═══\n\n");

    for (int s = 0; s < active_count; s++) {
        Schedule *sched = &sorted_schedules[s];
        
        if (sched->required_room_type != CLASSROOM) continue;
        
        int hours_remaining = sched->weekly_hours - hours_assigned[s];
        if (hours_remaining <= 0) continue;

        int max_per_day = calculate_max_slots_per_day(sched->weekly_hours);

        printf("Fallback for: %-25s (Need: %d more hrs)\n",
               sched->course_name, hours_remaining);

        int pass = 0;
        while (hours_remaining > 0 && pass < 10) {
            pass++;
            
            for (int day = MONDAY; day <= FRIDAY && hours_remaining > 0; day++) {
                int slots_on_day = count_slots_on_day(sched->id, day);
                if (slots_on_day >= max_per_day) continue;

                int assigned_this_day = 0;

                // Try morning slots with lab fallback
                for (int slot_idx = 0; slot_idx < 3 && hours_remaining > 0 && !assigned_this_day; slot_idx++) {
                    int start_time = morning_slots[slot_idx];
                    int end_time = start_time + slot_duration;
                    
                    if (end_time > MORNING_END) {
                        end_time = MORNING_END;
                        if (end_time - start_time < 60) continue;
                    }

                    int already_assigned = 0;
                    for (int g = 0; g < generated_slot_count; g++) {
                        if (generated_slots[g].schedule_id == sched->id &&
                            generated_slots[g].day == day &&
                            generated_slots[g].start_time == start_time) {
                            already_assigned = 1;
                            break;
                        }
                    }
                    if (already_assigned) continue;

                    if (try_assign_slot(sched, day, start_time, end_time, 1)) {
                        hours_remaining--;
                        hours_assigned[s]++;
                        assigned_this_day = 1;
                    }
                }

                // Try afternoon slots with lab fallback
                for (int slot_idx = 0; slot_idx < 3 && hours_remaining > 0 && !assigned_this_day; slot_idx++) {
                    int start_time = afternoon_slots[slot_idx];
                    int end_time = start_time + slot_duration;
                    
                    if (end_time > AFTERNOON_END) {
                        end_time = AFTERNOON_END;
                        if (end_time - start_time < 60) continue;
                    }

                    int already_assigned = 0;
                    for (int g = 0; g < generated_slot_count; g++) {
                        if (generated_slots[g].schedule_id == sched->id &&
                            generated_slots[g].day == day &&
                            generated_slots[g].start_time == start_time) {
                            already_assigned = 1;
                            break;
                        }
                    }
                    if (already_assigned) continue;

                    if (try_assign_slot(sched, day, start_time, end_time, 1)) {
                        hours_remaining--;
                        hours_assigned[s]++;
                        assigned_this_day = 1;
                    }
                }
            }
        }

        if (hours_remaining > 0) {
            printf("  ⚠ Still could not assign %d hrs (no rooms available)\n", hours_remaining);
        } else {
            printf("  ✓ Fallback successful: Total %d hrs assigned\n", hours_assigned[s]);
        }
    }

    // =========================================================================
    // Summary
    // =========================================================================
    printf("\n═══ FINAL SUMMARY ═══\n\n");
    
    for (int s = 0; s < active_count; s++) {
        int remaining = sorted_schedules[s].weekly_hours - hours_assigned[s];
        if (remaining > 0) {
            total_failed += remaining;
        }
        total_assigned += hours_assigned[s];
    }

    free(hours_assigned);
    free(sorted_schedules);
    save_schedules();
    save_generated_slots();

    printf("─────────────────────────────────────────\n");
    printf("✓ Schedule generation complete!\n");
    printf("  Total hours assigned: %d\n", total_assigned);
    printf("  Hours failed to assign: %d\n", total_failed);
    printf("  Generated slots: %d\n", generated_slot_count);
    printf("─────────────────────────────────────────\n");

    if (total_failed > 0) {
        printf("\n⚠ Some hours could not be assigned.\n");
        printf("  Possible reasons:\n");
        printf("  - Not enough rooms available\n");
        printf("  - Time conflicts with same dept/year courses\n");
        printf("  - Consider adding more rooms or adjusting hours\n");
    }

    pause_screen();
}

void display_weekly_timetable() {
    clear_screen();
    printf("\n╔════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                                  WEEKLY TIMETABLE - BAHIR DAR UNIVERSITY                                               ║\n");
    printf("╚════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╝\n\n");

    load_generated_slots();

    if (generated_slot_count == 0) {
        printf("No generated schedule available.\n");
        printf("Please run 'Generate Weekly Schedule' first.\n");
        pause_screen();
        return;
    }

    // Display by day
    for (int day = MONDAY; day <= FRIDAY; day++) {
        printf("\n┌─ %s ─────────────────────────────────────────────────────────────────────────────────────────\n", 
               get_day_string(day));
        printf("│\n");
        printf("│  %-12s %-25s %-15s %-10s %-20s\n",
               "Time", "Course", "Department", "Year", "Room");
        printf("│  ────────────────────────────────────────────────────────────────────────────────────────────\n");

        int has_slots = 0;
        for (int i = 0; i < generated_slot_count; i++) {
            if (!generated_slots[i].is_active) continue;
            if (generated_slots[i].day != day) continue;

            // Find schedule and room info
            char course_name[MAX_NAME_LENGTH] = "Unknown";
            char department[MAX_DEPT_LENGTH] = "Unknown";
            int year = 0;
            char room_name[MAX_NAME_LENGTH] = "Unknown";

            for (int j = 0; j < schedule_count; j++) {
                if (schedules[j].id == generated_slots[i].schedule_id) {
                    strcpy(course_name, schedules[j].course_name);
                    strcpy(department, schedules[j].department);
                    year = schedules[j].student_year;
                    break;
                }
            }

            for (int j = 0; j < room_count; j++) {
                if (rooms[j].id == generated_slots[i].room_id) {
                    strcpy(room_name, rooms[j].name);
                    break;
                }
            }

            char start_str[10], end_str[10], time_range[25];
            format_time(generated_slots[i].start_time, start_str);
            format_time(generated_slots[i].end_time, end_str);
            sprintf(time_range, "%s-%s", start_str, end_str);

            printf("│  %-12s %-25s %-15s %-10d %-20s\n",
                   time_range, course_name, department, year, room_name);
            has_slots = 1;
        }

        if (!has_slots) {
            printf("│  No classes scheduled for this day.\n");
        }
        printf("└─────────────────────────────────────────────────────────────────────────────────────────────────\n");
    }

    pause_screen();
}

void view_schedule_by_room() {
    clear_screen();
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                   SCHEDULE BY ROOM                         ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");

    load_generated_slots();

    int room_id;
    printf("Enter room ID: ");
    scanf("%d", &room_id);

    int found = -1;
    for (int i = 0; i < room_count; i++) {
        if (rooms[i].id == room_id && rooms[i].is_active) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        printf("\n✗ Room not found!\n");
        pause_screen();
        return;
    }

    printf("\nRoom: %s (%s)\n", rooms[found].name, get_room_type_string(rooms[found].type));
    printf("════════════════════════════════════════════════════════════════════════\n\n");

    for (int day = MONDAY; day <= FRIDAY; day++) {
        printf("\n%s:\n", get_day_string(day));
        printf("─────────────────────────────────────────────────────────────────────\n");

        int has_schedule = 0;
        for (int i = 0; i < generated_slot_count; i++) {
            if (!generated_slots[i].is_active) continue;
            if (generated_slots[i].room_id != room_id) continue;
            if (generated_slots[i].day != day) continue;

            char course_name[MAX_NAME_LENGTH] = "Unknown";
            char department[MAX_DEPT_LENGTH] = "Unknown";
            int year = 0;

            for (int j = 0; j < schedule_count; j++) {
                if (schedules[j].id == generated_slots[i].schedule_id) {
                    strcpy(course_name, schedules[j].course_name);
                    strcpy(department, schedules[j].department);
                    year = schedules[j].student_year;
                    break;
                }
            }

            char start_str[10], end_str[10];
            format_time(generated_slots[i].start_time, start_str);
            format_time(generated_slots[i].end_time, end_str);

            printf("  %s-%s: %-25s (%s, Year %d)\n",
                   start_str, end_str, course_name, department, year);
            has_schedule = 1;
        }

        if (!has_schedule) {
            printf("  No classes\n");
        }
    }

    pause_screen();
}

void view_schedule_by_department() {
    clear_screen();
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                 SCHEDULE BY DEPARTMENT                     ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");

    load_generated_slots();

    char dept[MAX_DEPT_LENGTH];
    printf("Enter department name: ");
    clear_input_buffer();
    fgets(dept, MAX_DEPT_LENGTH, stdin);
    dept[strcspn(dept, "\n")] = 0;

    printf("\nDepartment: %s\n", dept);
    printf("════════════════════════════════════════════════════════════════════════════════════\n");

    for (int day = MONDAY; day <= FRIDAY; day++) {
        printf("\n%s:\n", get_day_string(day));
        printf("─────────────────────────────────────────────────────────────────────────────────────\n");
        printf("%-12s %-25s %-6s %-10s %-20s\n", "Time", "Course", "Year", "Program", "Room");
        printf("─────────────────────────────────────────────────────────────────────────────────────\n");

        int has_schedule = 0;
        for (int i = 0; i < generated_slot_count; i++) {
            if (!generated_slots[i].is_active) continue;
            if (generated_slots[i].day != day) continue;

            // Find schedule
            for (int j = 0; j < schedule_count; j++) {
                if (schedules[j].id == generated_slots[i].schedule_id && 
                    schedules[j].is_active &&
                    strcasecmp(schedules[j].department, dept) == 0) {
                    
                    char room_name[MAX_NAME_LENGTH] = "Unknown";
                    for (int k = 0; k < room_count; k++) {
                        if (rooms[k].id == generated_slots[i].room_id) {
                            strcpy(room_name, rooms[k].name);
                            break;
                        }
                    }

                    char start_str[10], end_str[10], time_range[25];
                    format_time(generated_slots[i].start_time, start_str);
                    format_time(generated_slots[i].end_time, end_str);
                    sprintf(time_range, "%s-%s", start_str, end_str);

                    printf("%-12s %-25s %-6d %-10s %-20s\n",
                           time_range,
                           schedules[j].course_name,
                           schedules[j].student_year,
                           get_program_type_string(schedules[j].program_type),
                           room_name);
                    has_schedule = 1;
                    break;
                }
            }
        }

        if (!has_schedule) {
            printf("No classes\n");
        }
    }

    pause_screen();
}

void view_schedule_by_year_and_department() {
    clear_screen();
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║            SCHEDULE BY YEAR AND DEPARTMENT                 ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");

    load_generated_slots();

    if (generated_slot_count == 0) {
        printf("No generated schedule available.\n");
        printf("Please run 'Generate Weekly Schedule' first.\n");
        pause_screen();
        return;
    }

    char dept[MAX_DEPT_LENGTH];
    int year;
    
    printf("Enter department name: ");
    clear_input_buffer();
    fgets(dept, MAX_DEPT_LENGTH, stdin);
    dept[strcspn(dept, "\n")] = 0;

    printf("Enter student year (1-5): ");
    scanf("%d", &year);

    printf("\n═══════════════════════════════════════════════════════════════════════════════════════\n");
    printf("   WEEKLY SCHEDULE: %s - Year %d\n", dept, year);
    printf("═══════════════════════════════════════════════════════════════════════════════════════\n");

    int total_classes = 0;
    
    for (int day = MONDAY; day <= FRIDAY; day++) {
        printf("\n┌─ %s ─────────────────────────────────────────────────────────────────\n", 
               get_day_string(day));
        printf("│  %-14s %-30s %-12s %-20s\n", "Time", "Course", "Type", "Room");
        printf("│  ────────────────────────────────────────────────────────────────────────\n");

        int has_schedule = 0;
        for (int i = 0; i < generated_slot_count; i++) {
            if (!generated_slots[i].is_active) continue;
            if (generated_slots[i].day != day) continue;

            for (int j = 0; j < schedule_count; j++) {
                if (schedules[j].id == generated_slots[i].schedule_id && 
                    schedules[j].is_active &&
                    strcasecmp(schedules[j].department, dept) == 0 &&
                    schedules[j].student_year == year) {
                    
                    char room_name[MAX_NAME_LENGTH] = "Unknown";
                    char room_type_str[20] = "";
                    for (int k = 0; k < room_count; k++) {
                        if (rooms[k].id == generated_slots[i].room_id) {
                            strcpy(room_name, rooms[k].name);
                            strcpy(room_type_str, rooms[k].type == LAB ? "[LAB]" : "[CLASS]");
                            break;
                        }
                    }

                    char start_str[10], end_str[10], time_range[25];
                    format_time(generated_slots[i].start_time, start_str);
                    format_time(generated_slots[i].end_time, end_str);
                    sprintf(time_range, "%s - %s", start_str, end_str);

                    printf("│  %-14s %-30s %-12s %-20s\n",
                           time_range,
                           schedules[j].course_name,
                           get_room_type_string(schedules[j].required_room_type),
                           room_name);
                    has_schedule = 1;
                    total_classes++;
                    break;
                }
            }
        }

        if (!has_schedule) {
            printf("│  No classes scheduled\n");
        }
        printf("└──────────────────────────────────────────────────────────────────────────\n");
    }

    printf("\n─────────────────────────────────────────\n");
    printf("Total classes this week: %d\n", total_classes);
    printf("─────────────────────────────────────────\n");

    pause_screen();
}

void view_all_rooms_weekly() {
    clear_screen();
    printf("\n╔════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                                  ALL ROOMS WEEKLY SCHEDULE - BAHIR DAR UNIVERSITY                                      ║\n");
    printf("╚════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╝\n\n");

    load_generated_slots();

    if (generated_slot_count == 0) {
        printf("No generated schedule available.\n");
        printf("Please run 'Generate Weekly Schedule' first.\n");
        pause_screen();
        return;
    }

    // Display each room's weekly schedule
    for (int r = 0; r < room_count; r++) {
        if (!rooms[r].is_active) continue;

        // Check if room has any slots
        int has_slots = 0;
        for (int i = 0; i < generated_slot_count; i++) {
            if (generated_slots[i].is_active && generated_slots[i].room_id == rooms[r].id) {
                has_slots = 1;
                break;
            }
        }

        printf("\n╔══ ROOM: %-40s (%s) ══════════════════════════════════════════════╗\n",
               rooms[r].name, get_room_type_string(rooms[r].type));

        if (!has_slots) {
            printf("║  No classes scheduled for this room                                                                        ║\n");
            printf("╚════════════════════════════════════════════════════════════════════════════════════════════════════════════╝\n");
            continue;
        }

        for (int day = MONDAY; day <= FRIDAY; day++) {
            printf("║  %-10s: ", get_day_string(day));

            int day_has_slots = 0;
            int first = 1;
            for (int i = 0; i < generated_slot_count; i++) {
                if (!generated_slots[i].is_active) continue;
                if (generated_slots[i].room_id != rooms[r].id) continue;
                if (generated_slots[i].day != day) continue;

                char course_name[MAX_NAME_LENGTH] = "Unknown";
                char dept[MAX_DEPT_LENGTH] = "";
                int year = 0;

                for (int j = 0; j < schedule_count; j++) {
                    if (schedules[j].id == generated_slots[i].schedule_id) {
                        strcpy(course_name, schedules[j].course_name);
                        strcpy(dept, schedules[j].department);
                        year = schedules[j].student_year;
                        break;
                    }
                }

                char start_str[10], end_str[10];
                format_time(generated_slots[i].start_time, start_str);
                format_time(generated_slots[i].end_time, end_str);

                if (!first) printf("║              ");
                printf("%s-%s %-20.20s (%s Y%d)\n",
                       start_str, end_str, course_name, dept, year);
                day_has_slots = 1;
                first = 0;
            }

            if (!day_has_slots) {
                printf("No classes\n");
            }
        }
        printf("╚════════════════════════════════════════════════════════════════════════════════════════════════════════════════╝\n");
    }

    pause_screen();
}

void display_final_schedule() {
    clear_screen();
    printf("\n╔════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                                    FINAL SCHEDULE - BAHIR DAR UNIVERSITY                                               ║\n");
    printf("╚════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╝\n\n");

    load_generated_slots();

    if (generated_slot_count == 0) {
        printf("No schedules have been generated yet.\n");
        printf("Please run 'Generate Weekly Schedule' first.\n");
        pause_screen();
        return;
    }

    // Display by room
    for (int r = 0; r < room_count; r++) {
        if (!rooms[r].is_active) continue;

        // Check if room has any slots
        int has_slots = 0;
        for (int i = 0; i < generated_slot_count; i++) {
            if (generated_slots[i].is_active && generated_slots[i].room_id == rooms[r].id) {
                has_slots = 1;
                break;
            }
        }

        if (!has_slots) continue;

        printf("\n┌─ ROOM: %s (%s) ─────────────────────────────────────────────────────────────────\n",
               rooms[r].name, get_room_type_string(rooms[r].type));
        printf("│\n");

        for (int day = MONDAY; day <= FRIDAY; day++) {
            printf("│  %s:\n", get_day_string(day));

            int day_has_slots = 0;
            for (int i = 0; i < generated_slot_count; i++) {
                if (!generated_slots[i].is_active) continue;
                if (generated_slots[i].room_id != rooms[r].id) continue;
                if (generated_slots[i].day != day) continue;

                char course_name[MAX_NAME_LENGTH] = "Unknown";
                char department[MAX_DEPT_LENGTH] = "";
                int year = 0;
                int priority = 0;

                for (int j = 0; j < schedule_count; j++) {
                    if (schedules[j].id == generated_slots[i].schedule_id) {
                        strcpy(course_name, schedules[j].course_name);
                        strcpy(department, schedules[j].department);
                        year = schedules[j].student_year;
                        priority = schedules[j].priority;
                        break;
                    }
                }

                char start_str[10], end_str[10];
                format_time(generated_slots[i].start_time, start_str);
                format_time(generated_slots[i].end_time, end_str);

                printf("│    %s-%s: %-20s (%s, Y%d, P%d)\n",
                       start_str, end_str, course_name, department, year, priority);
                day_has_slots = 1;
            }

            if (!day_has_slots) {
                printf("│    No classes\n");
            }
        }
        printf("└─────────────────────────────────────────────────────────────────────────────────\n");
    }

    pause_screen();
}

void room_management_menu() {
    int choice;

    do {
        clear_screen();
        printf("\n╔════════════════════════════════════════════════════════════╗\n");
        printf("║                    ROOM MANAGEMENT                         ║\n");
        printf("║                BAHIR DAR UNIVERSITY                        ║\n");
        printf("╚════════════════════════════════════════════════════════════╝\n\n");
        printf("  1. Display All Rooms\n");
        printf("  2. Add New Room\n");
        printf("  3. Edit Room\n");
        printf("  4. Delete Room\n");
        printf("  0. Back to Main Menu\n");
        printf("\n─────────────────────────────────────────────────────────────\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                display_rooms();
                break;
            case 2:
                add_room();
                break;
            case 3:
                edit_room();
                break;
            case 4:
                delete_room();
                break;
            case 0:
                break;
            default:
                printf("\n✗ Invalid choice! Please try again.\n");
                pause_screen();
        }
    } while (choice != 0);
}

void schedule_management_menu() {
    int choice;

    do {
        clear_screen();
        printf("\n╔════════════════════════════════════════════════════════════╗\n");
        printf("║                 SCHEDULE MANAGEMENT                        ║\n");
        printf("║                BAHIR DAR UNIVERSITY                        ║\n");
        printf("╚════════════════════════════════════════════════════════════╝\n\n");
        printf("  ─── Schedule Data ───\n");
        printf("  1. Display All Schedules\n");
        printf("  2. Add New Schedule\n");
        printf("  3. Edit Schedule\n");
        printf("  4. Delete Schedule\n");
        printf("\n  ─── Generate ───\n");
        printf("  5. Generate Weekly Schedule\n");
        printf("\n  ─── View Options ───\n");
        printf("  6. View Weekly Timetable (All)\n");
        printf("  7. View Final Schedule (by Room)\n");
        printf("  8. View Schedule by Room (select one)\n");
        printf("  9. View Schedule by Department\n");
        printf(" 10. View Schedule by Year & Department\n");
        printf(" 11. View All Rooms Weekly Schedule\n");
        printf("\n  0. Back to Main Menu\n");
        printf("\n─────────────────────────────────────────────────────────────\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                display_schedules();
                break;
            case 2:
                add_schedule();
                break;
            case 3:
                edit_schedule();
                break;
            case 4:
                delete_schedule();
                break;
            case 5:
                generate_weekly_schedule();
                break;
            case 6:
                display_weekly_timetable();
                break;
            case 7:
                display_final_schedule();
                break;
            case 8:
                view_schedule_by_room();
                break;
            case 9:
                view_schedule_by_department();
                break;
            case 10:
                view_schedule_by_year_and_department();
                break;
            case 11:
                view_all_rooms_weekly();
                break;
            case 0:
                break;
            default:
                printf("\n✗ Invalid choice! Please try again.\n");
                pause_screen();
        }
    } while (choice != 0);
}
