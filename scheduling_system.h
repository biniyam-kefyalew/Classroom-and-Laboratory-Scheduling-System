// scheduling_system.h
#ifndef SCHEDULING_SYSTEM_H
#define SCHEDULING_SYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROOMS 100
#define MAX_SCHEDULES 500
#define MAX_GENERATED_SLOTS 2000
#define MAX_NAME_LENGTH 100
#define MAX_DEPT_LENGTH 50

// Time slots configuration (in minutes from midnight)
#define MORNING_START 480    // 08:00
#define MORNING_END 750      // 12:30
#define AFTERNOON_START 840  // 14:00
#define AFTERNOON_END 1050   // 17:30
#define DAYS_PER_WEEK 5      // Monday to Friday

typedef enum {
    CLASSROOM,
    LAB
} RoomType;

typedef enum {
    MAJOR,
    MINOR
} ProgramType;

typedef enum {
    MONDAY = 0,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY
} DayOfWeek;

typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    RoomType type;
    int is_active;
} Room;

// Schedule now uses weekly_hours instead of start_time/end_time
typedef struct {
    int id;
    char course_name[MAX_NAME_LENGTH];
    char department[MAX_DEPT_LENGTH];
    int student_year;
    ProgramType program_type;
    RoomType required_room_type;
    int weekly_hours;           // Total hours per week (e.g., 5 = 5 hours/week)
    int assigned_room_id;       // -1 if not assigned
    int is_active;
    int priority;
} Schedule;

// Generated time slot for the weekly schedule
typedef struct {
    int schedule_id;            // Reference to the schedule
    int room_id;                // Assigned room
    DayOfWeek day;              // Day of week (0=Monday, 4=Friday)
    int start_time;             // Start time in minutes from midnight
    int end_time;               // End time in minutes from midnight
    int is_active;
} GeneratedSlot;

/* ---------- GLOBAL VARIABLES ---------- */
extern Room rooms[MAX_ROOMS];
extern Schedule schedules[MAX_SCHEDULES];
extern GeneratedSlot generated_slots[MAX_GENERATED_SLOTS];
extern int room_count;
extern int schedule_count;
extern int generated_slot_count;
extern int next_room_id;
extern int next_schedule_id;

/* ---------- FUNCTION DECLARATIONS ---------- */
// Core functions
void load_rooms();
void save_rooms();
void load_schedules();
void save_schedules();
void load_generated_slots();
void save_generated_slots();

// Utility functions
const char* get_room_type_string(RoomType type);
const char* get_program_type_string(ProgramType type);
const char* get_day_string(DayOfWeek day);
void format_time(int time, char *buffer);
void clear_input_buffer();
void pause_screen();
void clear_screen();

// Room management
void display_rooms();
void add_room();
void edit_room();
void delete_room();

// Schedule management
void display_schedules();
void add_schedule();
void edit_schedule();
void delete_schedule();
void generate_weekly_schedule();
void display_final_schedule();
void display_weekly_timetable();
void view_schedule_by_room();
void view_schedule_by_department();

// Menu functions
void room_management_menu();
void schedule_management_menu();


#endif
