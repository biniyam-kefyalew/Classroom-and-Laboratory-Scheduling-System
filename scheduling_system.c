#include "scheduling_system.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#define ROOMS_FILE "/var/www/data/rooms.dat"
#define SCHEDULES_FILE "/var/www/data/schedules.dat"

/* ---------- GLOBAL DATA ---------- */
Room rooms[MAX_ROOMS];
Schedule schedules[MAX_SCHEDULES];

int room_count = 0;
int schedule_count = 0;
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

const char* get_room_type_string(RoomType type) {
    return type == CLASSROOM ? "Classroom" : "Lab";
}

const char* get_program_type_string(ProgramType type) {
    return type == MAJOR ? "Major" : "Minor";
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
           "ID", "Course", "Dept", "Year", "Program", "Room Type", "Time", "Assigned Room", "Priority");
    printf("────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────\n");

    for (int i = 0; i < schedule_count; i++) {
        if (schedules[i].is_active) {
            char start_time[10], end_time[10];
            format_time(schedules[i].start_time, start_time);
            format_time(schedules[i].end_time, end_time);

            char time_range[25];
            sprintf(time_range, "%s-%s", start_time, end_time);

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
                   time_range,
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

    int start_hour, start_minute, end_hour, end_minute;
    printf("\nEnter start time (HH MM): ");
    scanf("%d %d", &start_hour, &start_minute);
    new_schedule.start_time = start_hour * 60 + start_minute;

    printf("Enter end time (HH MM): ");
    scanf("%d %d", &end_hour, &end_minute);
    new_schedule.end_time = end_hour * 60 + end_minute;

    if (new_schedule.end_time <= new_schedule.start_time) {
        printf("\n✗ Invalid time range! End time must be after start time.\n");
        pause_screen();
        return;
    }

    new_schedule.priority = (new_schedule.student_year * 10) +
                           (new_schedule.program_type == MAJOR ? 5 : 0) +
                           (new_schedule.required_room_type == LAB ? 3 : 0);

    schedules[schedule_count++] = new_schedule;
    save_schedules();

    printf("\n✓ Schedule added successfully! (ID: %d, Priority: %d)\n",
           new_schedule.id, new_schedule.priority);
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

    char start_time[10], end_time[10];
    format_time(schedules[found].start_time, start_time);
    format_time(schedules[found].end_time, end_time);

    printf("\nCurrent schedule details:\n");
    printf("Course: %s\n", schedules[found].course_name);
    printf("Department: %s\n", schedules[found].department);
    printf("Year: %d\n", schedules[found].student_year);
    printf("Program: %s\n", get_program_type_string(schedules[found].program_type));
    printf("Room Type: %s\n", get_room_type_string(schedules[found].required_room_type));
    printf("Time: %s-%s\n", start_time, end_time);

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

int compare_schedules(const void *a, const void *b) {
    Schedule *s1 = (Schedule *)a;
    Schedule *s2 = (Schedule *)b;
    return s2->priority - s1->priority;
}

int check_conflict(Schedule *s1, int room_id, Schedule *existing_schedules, int count) {
    for (int i = 0; i < count; i++) {
        if (!existing_schedules[i].is_active) continue;

        if (existing_schedules[i].assigned_room_id == room_id) {
            int overlap = !(s1->end_time <= existing_schedules[i].start_time ||
                          s1->start_time >= existing_schedules[i].end_time);
            if (overlap) {
                return 1;
            }
        }
    }
    return 0;
}

// void generate_final_schedule() {
//     clear_screen();
//     printf("\n╔════════════════════════════════════════════════════════════╗\n");
//     printf("║              GENERATING FINAL SCHEDULE...                  ║\n");
//     printf("╚════════════════════════════════════════════════════════════╝\n\n");

//     Schedule *sorted_schedules = malloc(schedule_count * sizeof(Schedule));
//     if (!sorted_schedules) {
//         printf("Memory allocation failed.\n");
//         pause_screen();
//         return;
//     }

//     int active_count = 0;

//     // 1️⃣ Copy active schedules (FIFO preserved)
//     for (int i = 0; i < schedule_count; i++) {
//         if (schedules[i].is_active) {
//             sorted_schedules[active_count++] = schedules[i];
//         }
//     }

//     // 2️⃣ Sort by priority only
//     qsort(sorted_schedules, active_count, sizeof(Schedule), compare_schedules);

//     int assigned = 0;
//     int failed = 0;

//     // 3️⃣ Assign rooms
//     for (int i = 0; i < active_count; i++) {
//         int room_assigned = 0;

//         for (int j = 0; j < room_count; j++) {
//             if (!rooms[j].is_active)
//                 continue;

//             if (rooms[j].type != sorted_schedules[i].required_room_type)
//                 continue;

//             // Check conflict at SAME TIME, different room
//             if (!check_conflict(&sorted_schedules[i],
//                                 rooms[j].id,
//                                 schedules,
//                                 schedule_count)) {

//                 // Commit assignment
//                 for (int k = 0; k < schedule_count; k++) {
//                     if (schedules[k].id == sorted_schedules[i].id) {
//                         schedules[k].assigned_room_id = rooms[j].id;
//                         break;
//                     }
//                 }

//                 assigned++;
//                 room_assigned = 1;
//                 break; // FIFO room assignment
//             }
//         }

//         if (!room_assigned) {
//             failed++;
//             printf("⚠ Could not assign room for: %s (ID: %d, %d-%d)\n",
//                    sorted_schedules[i].course_name,
//                    sorted_schedules[i].id,
//                    sorted_schedules[i].start_time,
//                    sorted_schedules[i].end_time);
//         }
//     }

//     free(sorted_schedules);
//     save_schedules();

//     printf("\n─────────────────────────────────────────\n");
//     printf("✓ Schedule generation complete!\n");
//     printf("  Successfully assigned: %d\n", assigned);
//     printf("  Failed to assign: %d\n", failed);
//     printf("─────────────────────────────────────────\n");

//     pause_screen();
// }

void view_schedule_by_room() {
    clear_screen();
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                   SCHEDULE BY ROOM                         ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");

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

    printf("%-25s %-15s %-6s %-10s %-15s\n",
           "Course", "Dept", "Year", "Program", "Time");
    printf("────────────────────────────────────────────────────────────────────────\n");

    int has_schedule = 0;
    for (int i = 0; i < schedule_count; i++) {
        if (schedules[i].is_active && schedules[i].assigned_room_id == room_id) {
            char start_time[10], end_time[10];
            format_time(schedules[i].start_time, start_time);
            format_time(schedules[i].end_time, end_time);

            char time_range[25];
            sprintf(time_range, "%s-%s", start_time, end_time);

            printf("%-25s %-15s %-6d %-10s %-15s\n",
                   schedules[i].course_name,
                   schedules[i].department,
                   schedules[i].student_year,
                   get_program_type_string(schedules[i].program_type),
                   time_range);
            has_schedule = 1;
        }
    }

    if (!has_schedule) {
        printf("No schedules assigned to this room.\n");
    }

    pause_screen();
}

void view_schedule_by_department() {
    clear_screen();
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                 SCHEDULE BY DEPARTMENT                     ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");

    char dept[MAX_DEPT_LENGTH];
    printf("Enter department name: ");
    clear_input_buffer();
    fgets(dept, MAX_DEPT_LENGTH, stdin);
    dept[strcspn(dept, "\n")] = 0;

    printf("\nDepartment: %s\n", dept);
    printf("════════════════════════════════════════════════════════════════════════════════════\n\n");

    printf("%-25s %-6s %-10s %-12s %-15s %-20s\n",
           "Course", "Year", "Program", "Room Type", "Time", "Assigned Room");
    printf("────────────────────────────────────────────────────────────────────────────────────\n");

    int has_schedule = 0;
    for (int i = 0; i < schedule_count; i++) {
        if (schedules[i].is_active && strcasecmp(schedules[i].department, dept) == 0) {
            char start_time[10], end_time[10];
            format_time(schedules[i].start_time, start_time);
            format_time(schedules[i].end_time, end_time);

            char time_range[25];
            sprintf(time_range, "%s-%s", start_time, end_time);

            char assigned_room[30] = "Not assigned";
            if (schedules[i].assigned_room_id != -1) {
                for (int j = 0; j < room_count; j++) {
                    if (rooms[j].id == schedules[i].assigned_room_id && rooms[j].is_active) {
                        strcpy(assigned_room, rooms[j].name);
                        break;
                    }
                }
            }

            printf("%-25s %-6d %-10s %-12s %-15s %-20s\n",
                   schedules[i].course_name,
                   schedules[i].student_year,
                   get_program_type_string(schedules[i].program_type),
                   get_room_type_string(schedules[i].required_room_type),
                   time_range,
                   assigned_room);
            has_schedule = 1;
        }
    }

    if (!has_schedule) {
        printf("No schedules found for this department.\n");
    }

    pause_screen();
}

void display_final_schedule() {
    clear_screen();
    printf("\n╔════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                                    FINAL SCHEDULE - BAHIR DAR UNIVERSITY                                               ║\n");
    printf("╚════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╝\n\n");

    int has_assigned = 0;

    for (int i = 0; i < room_count; i++) {
        if (!rooms[i].is_active) continue;

        int has_schedule_for_room = 0;
        for (int j = 0; j < schedule_count; j++) {
            if (schedules[j].is_active && schedules[j].assigned_room_id == rooms[i].id) {
                has_schedule_for_room = 1;
                break;
            }
        }

        if (!has_schedule_for_room) continue;

        printf("\n┌─ ROOM: %s (%s) ─────────────────────────────────────────────────────────────────\n",
               rooms[i].name, get_room_type_string(rooms[i].type));
        printf("│\n");
        printf("│  %-25s %-15s %-6s %-10s %-15s %-8s\n",
               "Course", "Dept", "Year", "Program", "Time", "Priority");
        printf("│  ────────────────────────────────────────────────────────────────────────────────\n");

        for (int j = 0; j < schedule_count; j++) {
            if (schedules[j].is_active && schedules[j].assigned_room_id == rooms[i].id) {
                char start_time[10], end_time[10];
                format_time(schedules[j].start_time, start_time);
                format_time(schedules[j].end_time, end_time);

                char time_range[25];
                sprintf(time_range, "%s-%s", start_time, end_time);

                printf("│  %-25s %-15s %-6d %-10s %-15s %-8d\n",
                       schedules[j].course_name,
                       schedules[j].department,
                       schedules[j].student_year,
                       get_program_type_string(schedules[j].program_type),
                       time_range,
                       schedules[j].priority);
                has_assigned = 1;
            }
        }
        printf("└─────────────────────────────────────────────────────────────────────────────────\n");
    }

    if (!has_assigned) {
        printf("No schedules have been assigned yet.\n");
        printf("Please add schedules and run 'Generate Final Schedule' first.\n");
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
        printf("  1. Display All Schedules\n");
        printf("  2. Add New Schedule\n");
        printf("  3. Edit Schedule\n");
        printf("  4. Delete Schedule\n");
        printf("  5. Generate Final Schedule\n");
        printf("  6. View Final Schedule\n");
        printf("  7. View Schedule by Room\n");
        printf("  8. View Schedule by Department\n");
        printf("  0. Back to Main Menu\n");
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
                // generate_final_schedule();
                break;
            case 6:
                display_final_schedule();
                break;
            case 7:
                view_schedule_by_room();
                break;
            case 8:
                view_schedule_by_department();
                break;
            case 0:
                break;
            default:
                printf("\n✗ Invalid choice! Please try again.\n");
                pause_screen();
        }
    } while (choice != 0);
}

// int main() {
//     load_rooms();
//     load_schedules();

//     int choice;

//     do {
//         clear_screen();
//         printf("\n╔════════════════════════════════════════════════════════════╗\n");
//         printf("║     CLASSROOM AND LABORATORY SCHEDULING SYSTEM             ║\n");
//         printf("║            BAHIR DAR UNIVERSITY                            ║\n");
//         printf("╚════════════════════════════════════════════════════════════╝\n\n");
//         printf("  1. Room Management\n");
//         printf("  2. Schedule Management\n");
//         printf("  0. Exit\n");
//         printf("\n─────────────────────────────────────────────────────────────\n");
//         printf("Enter your choice: ");
//         scanf("%d", &choice);

//         switch (choice) {
//             case 1:
//                 room_management_menu();
//                 break;
//             case 2:
//                 schedule_management_menu();
//                 break;
//             case 0:
//                 printf("\n✓ Thank you for using the scheduling system!\n");
//                 printf("  Goodbye!\n\n");
//                 break;
//             default:
//                 printf("\n✗ Invalid choice! Please try again.\n");
//                 pause_screen();
//         }
//     } while (choice != 0);

//     return 0;
// }
