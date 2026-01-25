// scheduling_system.h
#ifndef SCHEDULING_SYSTEM_H
#define SCHEDULING_SYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROOMS 100
#define MAX_SCHEDULES 500
#define MAX_NAME_LENGTH 100
#define MAX_DEPT_LENGTH 50

typedef enum {
    CLASSROOM,
    LAB
} RoomType;

typedef enum {
    MAJOR,
    MINOR
} ProgramType;

typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    RoomType type;
    int is_active;
} Room;

typedef struct {
    int id;
    char course_name[MAX_NAME_LENGTH];
    char department[MAX_DEPT_LENGTH];
    int student_year;
    ProgramType program_type;
    RoomType required_room_type;
    int start_time;
    int end_time;
    int assigned_room_id;
    int is_active;
    int priority;
} Schedule;

/* ---------- GLOBAL VARIABLES ---------- */
extern Room rooms[MAX_ROOMS];
extern Schedule schedules[MAX_SCHEDULES];
extern int room_count;
extern int schedule_count;
extern int next_room_id;
extern int next_schedule_id;

/* ---------- FUNCTION DECLARATIONS ---------- */
// Core functions
void load_rooms();
void save_rooms();
void load_schedules();
void save_schedules();

// Utility functions
const char* get_room_type_string(RoomType type);
const char* get_program_type_string(ProgramType type);
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
// void generate_final_schedule();
void display_final_schedule();
void view_schedule_by_room();
void view_schedule_by_department();

// Menu functions
void room_management_menu();
void schedule_management_menu();


#endif