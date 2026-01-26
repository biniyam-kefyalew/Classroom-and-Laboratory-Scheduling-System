#include "scheduling_system.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

// Use writable directories for web
#define WEB_ROOMS_FILE "/tmp/rooms.dat"
#define WEB_SCHEDULES_FILE "/tmp/schedules.dat"
#define WEB_GENERATED_SLOTS_FILE "/tmp/generated_slots.dat"
int next_fifo_order = 0;

// Custom strdup implementation for C99
char* my_strdup(const char* s) {
    if (s == NULL) return NULL;
    size_t len = strlen(s) + 1;
    char* new_str = malloc(len);
    if (new_str == NULL) return NULL;
    memcpy(new_str, s, len);
    return new_str;
}

void print_html_header() {
    printf("Content-type: text/html\n\n");
    printf("<!DOCTYPE html>\n");
    printf("<html lang=\"en\">\n");
    printf("<head>\n");
    printf("    <meta charset=\"UTF-8\">\n");
    printf("    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n");
    printf("    <title>Room Scheduling System - Bahir Dar University</title>\n");
    printf("    <style>\n");
    printf("        body { font-family: Arial, sans-serif; margin: 20px; background: #f5f5f5; }\n");
    printf("        .container { max-width: 1400px; margin: 0 auto; background: white; padding: 20px; border-radius: 10px; box-shadow: 0 0 10px rgba(0,0,0,0.1); }\n");
    printf("        header { background: #2c3e50; color: white; padding: 20px; border-radius: 5px; margin-bottom: 20px; }\n");
    printf("        nav { background: #34495e; padding: 10px; border-radius: 5px; margin-bottom: 20px; display: flex; flex-wrap: wrap; gap: 5px; }\n");
    printf("        nav a { color: white; text-decoration: none; padding: 10px 15px; border-radius: 3px; }\n");
    printf("        nav a:hover { background: #1abc9c; }\n");
    printf("        .card { background: white; border: 1px solid #ddd; border-radius: 5px; padding: 15px; margin-bottom: 15px; }\n");
    printf("        table { width: 100%%; border-collapse: collapse; margin-bottom: 20px; }\n");
    printf("        th, td { border: 1px solid #ddd; padding: 8px; text-align: left; font-size: 14px; }\n");
    printf("        th { background-color: #3498db; color: white; }\n");
    printf("        tr:nth-child(even) { background-color: #f2f2f2; }\n");
    printf("        .btn { background: #3498db; color: white; padding: 10px 15px; border: none; border-radius: 3px; cursor: pointer; text-decoration: none; display: inline-block; }\n");
    printf("        .btn:hover { background: #2980b9; }\n");
    printf("        .btn-danger { background: #e74c3c; }\n");
    printf("        .btn-danger:hover { background: #c0392b; }\n");
    printf("        .btn-success { background: #27ae60; }\n");
    printf("        .btn-success:hover { background: #1e8449; }\n");
    printf("        .form-group { margin-bottom: 15px; }\n");
    printf("        label { display: block; margin-bottom: 5px; font-weight: bold; }\n");
    printf("        input, select { width: 100%%; padding: 8px; border: 1px solid #ddd; border-radius: 3px; box-sizing: border-box; }\n");
    printf("        .success { color: #27ae60; background: #d5f4e6; padding: 10px; border-radius: 3px; margin: 10px 0; }\n");
    printf("        .error { color: #c0392b; background: #fadbd8; padding: 10px; border-radius: 3px; margin: 10px 0; }\n");
    printf("        .warning { color: #d68910; background: #fef9e7; padding: 10px; border-radius: 3px; margin: 10px 0; }\n");
    printf("        .day-header { background: #34495e; color: white; padding: 10px; margin: 15px 0 5px 0; border-radius: 3px; }\n");
    printf("        .timetable { margin-bottom: 30px; }\n");
    printf("        .room-section { border: 2px solid #3498db; border-radius: 5px; margin-bottom: 20px; overflow: hidden; }\n");
    printf("        .room-header { background: #3498db; color: white; padding: 10px 15px; font-weight: bold; }\n");
    printf("        .stats-grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); gap: 20px; margin: 20px 0; }\n");
    printf("        .stat-card { background: linear-gradient(135deg, #667eea 0%%, #764ba2 100%%); color: white; padding: 20px; border-radius: 10px; text-align: center; }\n");
    printf("        .stat-card.green { background: linear-gradient(135deg, #11998e 0%%, #38ef7d 100%%); }\n");
    printf("        .stat-card.orange { background: linear-gradient(135deg, #f093fb 0%%, #f5576c 100%%); }\n");
    printf("        .stat-card.blue { background: linear-gradient(135deg, #4facfe 0%%, #00f2fe 100%%); }\n");
    printf("        .stat-number { font-size: 36px; font-weight: bold; }\n");
    printf("        .stat-label { font-size: 14px; opacity: 0.9; }\n");
    printf("    </style>\n");
    printf("</head>\n");
    printf("<body>\n");
    printf("<div class=\"container\">\n");
    printf("    <header>\n");
    printf("        <h1>Classroom and Laboratory Scheduling System</h1>\n");
    printf("        <h2>Bahir Dar University</h2>\n");
    printf("    </header>\n");
}

void print_html_footer() {
    printf("</div>\n");
    printf("</body>\n");
    printf("</html>\n");
}

void print_navigation() {
    printf("    <nav>\n");
    printf("        <a href=\"?page=home\">Home</a>\n");
    printf("        <a href=\"?page=rooms\">Rooms</a>\n");
    printf("        <a href=\"?page=add_room\">Add Room</a>\n");
    printf("        <a href=\"?page=schedules\">Schedules</a>\n");
    printf("        <a href=\"?page=add_schedule\">Add Schedule</a>\n");
    printf("        <a href=\"?page=generate\">Generate</a>\n");
    printf("        <a href=\"?page=timetable\">Weekly Timetable</a>\n");
    printf("        <a href=\"?page=schedule_year_dept\">By Year/Dept</a>\n");
    printf("        <a href=\"?page=schedule_by_room\">By Room</a>\n");
    printf("        <a href=\"?page=all_rooms\">All Rooms</a>\n");
    printf("        <a href=\"?page=final\">Final Schedule</a>\n");
    printf("    </nav>\n");
}

void url_decode(char *str) {
    char *p = str;
    char buffer[strlen(str) + 1];
    int i = 0;
    
    while (*p) {
        if (*p == '%' && *(p+1) && *(p+2)) {
            char hex[3] = {*(p+1), *(p+2), '\0'};
            buffer[i++] = (char)strtol(hex, NULL, 16);
            p += 3;
        } else if (*p == '+') {
            buffer[i++] = ' ';
            p++;
        } else {
            buffer[i++] = *p++;
        }
    }
    buffer[i] = '\0';
    strcpy(str, buffer);
}

void parse_query_param(char *query, const char *key, char *value) {
    value[0] = '\0';
    if (!query) return;
    
    char *query_copy = my_strdup(query);
    char *token = strtok(query_copy, "&");
    
    while (token) {
        char *equals = strchr(token, '=');
        if (equals) {
            *equals = '\0';
            char decoded_key[256];
            strcpy(decoded_key, token);
            url_decode(decoded_key);
            
            if (strcmp(decoded_key, key) == 0) {
                url_decode(equals + 1);
                strcpy(value, equals + 1);
                break;
            }
        }
        token = strtok(NULL, "&");
    }
    
    free(query_copy);
}

const char* web_get_day_string(DayOfWeek day) {
    switch(day) {
        case 0: return "Monday";
        case 1: return "Tuesday";
        case 2: return "Wednesday";
        case 3: return "Thursday";
        case 4: return "Friday";
        default: return "Unknown";
    }
}

void web_format_time(int time, char *buffer) {
    int hour = time / 60;
    int minute = time % 60;
    sprintf(buffer, "%02d:%02d", hour, minute);
}

// Check room suitability: 0=not suitable, 1=exact match, 2=fallback match
int web_check_room_suitability(RoomType room_type, RoomType required_type, int allow_fallback) {
    if (room_type == required_type) return 1;  // Exact match
    if (required_type == LAB) return 0;        // Lab courses MUST use labs
    if (required_type == CLASSROOM && room_type == LAB && allow_fallback) return 2;  // Classroom can use lab as fallback
    return 0;
}

// Check for room time conflict
int web_check_room_conflict(int room_id, DayOfWeek day, int start_time, int end_time) {
    for (int i = 0; i < generated_slot_count; i++) {
        if (!generated_slots[i].is_active) continue;
        if (generated_slots[i].room_id != room_id) continue;
        if (generated_slots[i].day != day) continue;
        if (!(end_time <= generated_slots[i].start_time || start_time >= generated_slots[i].end_time)) {
            return 1;  // Conflict
        }
    }
    return 0;
}

// Check for student time conflict (same dept/year)
int web_check_student_conflict(const char* dept, int year, DayOfWeek day, int start_time, int end_time) {
    for (int i = 0; i < generated_slot_count; i++) {
        if (!generated_slots[i].is_active) continue;
        if (generated_slots[i].day != day) continue;
        
        for (int j = 0; j < schedule_count; j++) {
            if (schedules[j].id == generated_slots[i].schedule_id && schedules[j].is_active) {
                if (strcasecmp(schedules[j].department, dept) == 0 && schedules[j].student_year == year) {
                    if (!(end_time <= generated_slots[i].start_time || start_time >= generated_slots[i].end_time)) {
                        return 1;  // Student conflict
                    }
                }
                break;
            }
        }
    }
    return 0;
}

// Try to assign a slot, returns 1 if successful
int web_try_assign_slot(Schedule *sched, DayOfWeek day, int start_time, int end_time, int allow_lab_fallback, char *fallback_note) {
    int best_room_idx = -1;
    int best_suitability = 0;
    
    for (int r = 0; r < room_count; r++) {
        if (!rooms[r].is_active) continue;
        
        int suitability = web_check_room_suitability(rooms[r].type, sched->required_room_type, allow_lab_fallback);
        if (suitability == 0) continue;
        
        if (web_check_room_conflict(rooms[r].id, day, start_time, end_time)) continue;
        if (web_check_student_conflict(sched->department, sched->student_year, day, start_time, end_time)) continue;
        
        if (suitability > best_suitability) {
            best_suitability = suitability;
            best_room_idx = r;
            if (suitability == 1) break;  // Exact match, use immediately
        }
    }
    
    if (best_room_idx >= 0 && generated_slot_count < MAX_GENERATED_SLOTS) {
        generated_slots[generated_slot_count].schedule_id = sched->id;
        generated_slots[generated_slot_count].room_id = rooms[best_room_idx].id;
        generated_slots[generated_slot_count].day = day;
        generated_slots[generated_slot_count].start_time = start_time;
        generated_slots[generated_slot_count].end_time = end_time;
        generated_slots[generated_slot_count].is_active = 1;
        generated_slot_count++;
        
        for (int k = 0; k < schedule_count; k++) {
            if (schedules[k].id == sched->id) {
                schedules[k].assigned_room_id = rooms[best_room_idx].id;
                break;
            }
        }
        
        if (best_suitability == 2 && fallback_note) {
            sprintf(fallback_note, " (Lab: %s)", rooms[best_room_idx].name);
        }
        return 1;
    }
    return 0;
}

// Count total minutes assigned for a schedule on a specific day
int web_count_minutes_on_day(int schedule_id, DayOfWeek day) {
    int total_minutes = 0;
    for (int i = 0; i < generated_slot_count; i++) {
        if (generated_slots[i].is_active &&
            generated_slots[i].schedule_id == schedule_id &&
            generated_slots[i].day == day) {
            total_minutes += (generated_slots[i].end_time - generated_slots[i].start_time);
        }
    }
    return total_minutes;
}

// Count slots for a schedule on a specific day
int web_count_slots_on_day(int schedule_id, DayOfWeek day) {
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

// Calculate ideal minutes per day for even distribution
// e.g., 6 hours = 360 min / 5 days = 72 min/day, but minimum session is 60 min
// So we distribute as evenly as possible while respecting minimum session duration
int web_calc_ideal_minutes_per_day(int total_weekly_hours) {
    int total_minutes = total_weekly_hours * 60;
    int base_per_day = total_minutes / 5;
    return base_per_day;
}

// Calculate max minutes allowed per day (to prevent bunching on one day)
int web_calc_max_minutes_per_day(int total_weekly_hours) {
    int total_minutes = total_weekly_hours * 60;
    // Allow up to ceil(total/5) + one extra slot buffer
    int base = (total_minutes + 4) / 5;  // Ceiling division
    return base + 90;  // Allow one extra 90-min slot for flexibility
}

void web_generate_schedule() {
    // Clear existing slots
    generated_slot_count = 0;
    
    // Create sorted copy by priority
    Schedule sorted[MAX_SCHEDULES];
    int active_count = 0;
    
    for (int i = 0; i < schedule_count; i++) {
        if (schedules[i].is_active) {
            sorted[active_count++] = schedules[i];
        }
    }
    
    // Sort by priority (bubble sort - highest priority first)
    for (int i = 0; i < active_count - 1; i++) {
        for (int j = i + 1; j < active_count; j++) {
            if (sorted[j].priority > sorted[i].priority) {
                Schedule temp = sorted[i];
                sorted[i] = sorted[j];
                sorted[j] = temp;
            }
        }
    }
    
    // Time slots configuration
    // Morning slots: 08:00, 09:30, 11:00 (90-min sessions)
    // Afternoon slots: 14:00, 15:30, 17:00 (90-min sessions)
    int morning_slots[] = {480, 570, 660};     // 08:00, 09:30, 11:00
    int afternoon_slots[] = {840, 930, 1020};  // 14:00, 15:30, 17:00
    int slot_duration = 90;  // 1.5 hours (90 min) minimum class duration
    
    // int total_assigned = 0;
    // int total_failed = 0;
    int minutes_assigned[MAX_SCHEDULES] = {0};  // Track in minutes for precision
    char fallback_notes[MAX_SCHEDULES][256] = {{0}};
    
    printf("<div class=\"card\">\n");
    printf("<h2>Schedule Generation - Distributed Across 5 Days</h2>\n");
    
    printf("<div class=\"warning\">\n");
    printf("<strong>Distribution Rules:</strong><br>\n");
    printf("1. Classes are distributed evenly across Monday-Friday<br>\n");
    printf("2. Minimum session duration: 1 hour 30 minutes (90 min)<br>\n");
    printf("3. Example: 6 hrs/week = ~72 min/day across 5 days<br>\n");
    printf("4. Lab courses MUST use lab rooms<br>\n");
    printf("5. Same department/year students cannot have overlapping classes<br>\n");
    printf("</div>\n");
    
    // ===== PHASE 1: Even Distribution Assignment =====
    printf("<h3>Phase 1: Even Distribution (Mon-Fri)</h3>\n");
    printf("<table>\n");
    printf("<tr><th>Course</th><th>Priority</th><th>Weekly Hrs</th><th>Target/Day</th><th>Type</th><th>Distribution</th><th>Status</th></tr>\n");
    
    for (int s = 0; s < active_count; s++) {
        Schedule *sched = &sorted[s];
        int total_minutes_needed = sched->weekly_hours * 60;
        int max_minutes_per_day = web_calc_max_minutes_per_day(sched->weekly_hours);
        int target_per_day = web_calc_ideal_minutes_per_day(sched->weekly_hours);
        
        // Track which days have been assigned for this schedule
        // DayOfWeek day_assigned[5] = {0, 0, 0, 0, 0};
        DayOfWeek days_distribution[5] = {0, 0, 0, 0, 0};  // Minutes per day
        
        // Round-robin through days, trying to spread evenly
        int minutes_remaining = total_minutes_needed;
        int pass = 0;
        
        while (minutes_remaining >= 60 && pass < 15) {  // Min 60 min (1 hr) sessions
            pass++;
            
            // Find the day with least assigned minutes (for even distribution)
            for (int attempt = 0; attempt < 5 && minutes_remaining >= 60; attempt++) {
                // Find day with minimum assignment
                DayOfWeek min_day = DAY_INVALID;
                int min_minutes = 999999;
                for (int d = 0; d < 5; d++) {
                    int current_minutes = web_count_minutes_on_day(sched->id, d);
                    if (current_minutes < min_minutes && current_minutes < max_minutes_per_day) {
                        min_minutes = current_minutes;
                        min_day = d;
                    }
                }
                

                if (min_day == DAY_INVALID) break;
                int assigned_this_iteration = 0;
                
                // Try morning slots first
                for (int slot_idx = 0; slot_idx < 3 && !assigned_this_iteration; slot_idx++) {
                    int start_time = morning_slots[slot_idx];
                    int end_time = start_time + slot_duration;
                    if (end_time > 750) end_time = 750;
                    if (end_time - start_time < 60) continue;
                    
                    // Check if slot already used
                    int already = 0;
                    for (int g = 0; g < generated_slot_count; g++) {
                        if (generated_slots[g].schedule_id == sched->id &&
                            generated_slots[g].day == min_day &&
                            generated_slots[g].start_time == start_time) {
                            already = 1;
                            break;
                        }
                    }
                    if (already) continue;
                    
                    if (web_try_assign_slot(sched, min_day, start_time, end_time, 0, NULL)) {
                        int slot_minutes = end_time - start_time;
                        minutes_remaining -= slot_minutes;
                        minutes_assigned[s] += slot_minutes;
                        days_distribution[min_day] += slot_minutes;
                        assigned_this_iteration = 1;
                    }
                }
                
                // Try afternoon slots
                for (int slot_idx = 0; slot_idx < 3 && !assigned_this_iteration; slot_idx++) {
                    int start_time = afternoon_slots[slot_idx];
                    int end_time = start_time + slot_duration;
                    if (end_time > 1050) end_time = 1050;
                    if (end_time - start_time < 60) continue;
                    
                    int already = 0;
                    for (int g = 0; g < generated_slot_count; g++) {
                        if (generated_slots[g].schedule_id == sched->id &&
                            generated_slots[g].day == min_day &&
                            generated_slots[g].start_time == start_time) {
                            already = 1;
                            break;
                        }
                    }
                    if (already) continue;
                    
                    if (web_try_assign_slot(sched, min_day, start_time, end_time, 0, NULL)) {
                        int slot_minutes = end_time - start_time;
                        minutes_remaining -= slot_minutes;
                        minutes_assigned[s] += slot_minutes;
                        days_distribution[min_day] += slot_minutes;
                        assigned_this_iteration = 1;
                    }
                }
                
                if (!assigned_this_iteration) {
                    // Mark this day as maxed for this pass
                    break;
                }
            }
        }
        
        // Build distribution string showing minutes per day
        char dist_str[100];
        sprintf(dist_str, "M:%d T:%d W:%d Th:%d F:%d", 
                days_distribution[0], days_distribution[1], days_distribution[2],
                days_distribution[3], days_distribution[4]);
        
        int hours_got = minutes_assigned[s] / 60;
        int mins_got = minutes_assigned[s] % 60;
        
        if (minutes_remaining >= 60) {
            printf("<tr style=\"background:#fef9e7;\"><td>%s</td><td>%d</td><td>%d</td><td>%d min</td><td>%s</td><td>%s</td><td>%dh%dm (needs %dm)</td></tr>\n",
                   sched->course_name, sched->priority, sched->weekly_hours, target_per_day,
                   sched->required_room_type == LAB ? "Lab" : "Class",
                   dist_str, hours_got, mins_got, minutes_remaining);
        } else {
            printf("<tr style=\"background:#d5f4e6;\"><td>%s</td><td>%d</td><td>%d</td><td>%d min</td><td>%s</td><td>%s</td><td>Complete (%dh%dm)</td></tr>\n",
                   sched->course_name, sched->priority, sched->weekly_hours, target_per_day,
                   sched->required_room_type == LAB ? "Lab" : "Class",
                   dist_str, hours_got, mins_got);
        }
    }
    printf("</table>\n");
    
    // ===== PHASE 2: Lab fallback for classroom courses =====
    printf("<h3>Phase 2: Lab Fallback for Remaining Classroom Courses</h3>\n");
    printf("<table>\n");
    printf("<tr><th>Course</th><th>Needed</th><th>Status</th><th>Notes</th></tr>\n");
    
    int phase2_needed = 0;
    for (int s = 0; s < active_count; s++) {
        Schedule *sched = &sorted[s];
        
        if (sched->required_room_type != CLASSROOM) continue;
        
        int total_minutes_needed = sched->weekly_hours * 60;
        int minutes_remaining = total_minutes_needed - minutes_assigned[s];
        if (minutes_remaining < 60) continue;  // Less than 1 hour remaining, skip
        
        phase2_needed = 1;
        int phase2_assigned_minutes = 0;
        int max_minutes_per_day = web_calc_max_minutes_per_day(sched->weekly_hours);
        
        int pass = 0;
        while (minutes_remaining >= 60 && pass < 15) {
            pass++;
            
            for (int attempt = 0; attempt < 5 && minutes_remaining >= 60; attempt++) {
                DayOfWeek min_day = DAY_INVALID;
                int min_minutes = 999999;
                for (int d = 0; d < 5; d++) {
                    int current_minutes = web_count_minutes_on_day(sched->id, d);
                    if (current_minutes < min_minutes && current_minutes < max_minutes_per_day) {
                        min_minutes = current_minutes;
                        min_day = d;
                    }
                }
                
                if (min_day == DAY_INVALID) break;

                int assigned_this_iteration = 0;
                
                for (int slot_idx = 0; slot_idx < 3 && !assigned_this_iteration; slot_idx++) {
                    int start_time = morning_slots[slot_idx];
                    int end_time = start_time + slot_duration;
                    if (end_time > 750) end_time = 750;
                    if (end_time - start_time < 60) continue;
                    
                    int already = 0;
                    for (int g = 0; g < generated_slot_count; g++) {
                        if (generated_slots[g].schedule_id == sched->id &&
                            generated_slots[g].day == min_day &&
                            generated_slots[g].start_time == start_time) {
                            already = 1;
                            break;
                        }
                    }
                    if (already) continue;
                    
                    if (web_try_assign_slot(sched, min_day, start_time, end_time, 1, fallback_notes[s])) {
                        int slot_minutes = end_time - start_time;
                        minutes_remaining -= slot_minutes;
                        minutes_assigned[s] += slot_minutes;
                        phase2_assigned_minutes += slot_minutes;
                        assigned_this_iteration = 1;
                    }
                }
                
                for (int slot_idx = 0; slot_idx < 3 && !assigned_this_iteration; slot_idx++) {
                    int start_time = afternoon_slots[slot_idx];
                    int end_time = start_time + slot_duration;
                    if (end_time > 1050) end_time = 1050;
                    if (end_time - start_time < 60) continue;
                    
                    int already = 0;
                    for (int g = 0; g < generated_slot_count; g++) {
                        if (generated_slots[g].schedule_id == sched->id &&
                            generated_slots[g].day == min_day &&
                            generated_slots[g].start_time == start_time) {
                            already = 1;
                            break;
                        }
                    }
                    if (already) continue;
                    
                    if (web_try_assign_slot(sched, min_day, start_time, end_time, 1, fallback_notes[s])) {
                        int slot_minutes = end_time - start_time;
                        minutes_remaining -= slot_minutes;
                        minutes_assigned[s] += slot_minutes;
                        phase2_assigned_minutes += slot_minutes;
                        assigned_this_iteration = 1;
                    }
                }
                
                if (!assigned_this_iteration) break;
            }
        }
        
        if (minutes_remaining >= 60) {
            printf("<tr style=\"background:#fadbd8;\"><td>%s</td><td>%d min</td><td>Still missing %d min</td><td>No rooms available</td></tr>\n",
                   sched->course_name, total_minutes_needed - minutes_assigned[s] + phase2_assigned_minutes, minutes_remaining);
        } else if (phase2_assigned_minutes > 0) {
            printf("<tr style=\"background:#d5f4e6;\"><td>%s</td><td>%d min</td><td>Assigned via lab fallback</td><td>%s</td></tr>\n",
                   sched->course_name, phase2_assigned_minutes, fallback_notes[s]);
        }
    }
    
    if (!phase2_needed) {
        printf("<tr><td colspan=\"4\">No classroom courses needed lab fallback</td></tr>\n");
    }
    printf("</table>\n");
    
    // Calculate totals
    int total_minutes_assigned = 0;
    int total_minutes_failed = 0;
    for (int s = 0; s < active_count; s++) {
        total_minutes_assigned += minutes_assigned[s];
        int needed = sorted[s].weekly_hours * 60;
        int remaining = needed - minutes_assigned[s];
        if (remaining > 0) total_minutes_failed += remaining;
    }
    
    save_schedules();
    save_generated_slots();
    
    printf("<div class=\"success\">\n");
    printf("<strong>Schedule Generation Complete!</strong><br>\n");
    printf("Total time assigned: %d hours %d minutes<br>\n", total_minutes_assigned / 60, total_minutes_assigned % 60);
    printf("Time failed to assign: %d hours %d minutes<br>\n", total_minutes_failed / 60, total_minutes_failed % 60);
    printf("Generated slots: %d<br>\n", generated_slot_count);
    printf("Session duration: 1 hour 30 minutes (90 min)\n");
    printf("</div>\n");
    
    if (total_minutes_failed > 0) {
        printf("<div class=\"error\">\n");
        printf("<strong>Warning:</strong> Some time could not be assigned.<br>\n");
        printf("Possible reasons:<br>\n");
        printf("- Not enough rooms available<br>\n");
        printf("- Time conflicts with same department/year courses<br>\n");
        printf("- Consider adding more rooms or adjusting weekly hours\n");
        printf("</div>\n");
    }
    
    printf("</div>\n");
}

void handle_home_page() {
    printf("    <div class=\"card\">\n");
    printf("        <h2>Welcome to Scheduling System</h2>\n");
    printf("        <p>This system helps manage classroom and laboratory schedules at Bahir Dar University.</p>\n");
    printf("        <p><strong>Schedule Configuration:</strong></p>\n");
    printf("        <ul>\n");
    printf("            <li>Days: Monday - Friday (5 days)</li>\n");
    printf("            <li>Morning Session: 08:00 - 12:30</li>\n");
    printf("            <li>Afternoon Session: 14:00 - 17:30</li>\n");
    printf("            <li>Minimum Session Duration: 1 hour 30 minutes (90 min)</li>\n");
    printf("        </ul>\n");
    printf("        <p><strong>Class Distribution Feature:</strong></p>\n");
    printf("        <ul>\n");
    printf("            <li>Classes are evenly distributed across 5 days</li>\n");
    printf("            <li>Example: 6 hrs/week spreads as ~72 min/day across Mon-Fri</li>\n");
    printf("            <li>Prevents bunching all hours on one or two days</li>\n");
    printf("            <li>Each session is at least 1-1.5 hours</li>\n");
    printf("        </ul>\n");
    printf("        <p><strong>View Options:</strong></p>\n");
    printf("        <ul>\n");
    printf("            <li><a href=\"?page=schedule_year_dept\">By Year/Department</a> - Weekly view for specific student groups</li>\n");
    printf("            <li><a href=\"?page=schedule_by_room\">By Room</a> - Weekly schedule for a specific room</li>\n");
    printf("            <li><a href=\"?page=all_rooms\">All Rooms</a> - Overview of all room utilization</li>\n");
    printf("        </ul>\n");
    
    int active_rooms = 0;
    int lab_rooms = 0;
    int active_schedules_count = 0;
    int total_hours = 0;
    
    for (int i = 0; i < room_count; i++) {
        if (rooms[i].is_active) {
            active_rooms++;
            if (rooms[i].type == LAB) lab_rooms++;
        }
    }
    
    for (int i = 0; i < schedule_count; i++) {
        if (schedules[i].is_active) {
            active_schedules_count++;
            total_hours += schedules[i].weekly_hours;
        }
    }
    
    printf("        <div class=\"stats-grid\">\n");
    printf("            <div class=\"stat-card\">\n");
    printf("                <div class=\"stat-number\">%d</div>\n", active_rooms);
    printf("                <div class=\"stat-label\">Total Rooms (%d Labs)</div>\n", lab_rooms);
    printf("            </div>\n");
    printf("            <div class=\"stat-card green\">\n");
    printf("                <div class=\"stat-number\">%d</div>\n", active_schedules_count);
    printf("                <div class=\"stat-label\">Active Schedules</div>\n");
    printf("            </div>\n");
    printf("            <div class=\"stat-card orange\">\n");
    printf("                <div class=\"stat-number\">%d</div>\n", total_hours);
    printf("                <div class=\"stat-label\">Total Weekly Hours</div>\n");
    printf("            </div>\n");
    printf("            <div class=\"stat-card blue\">\n");
    printf("                <div class=\"stat-number\">%d</div>\n", generated_slot_count);
    printf("                <div class=\"stat-label\">Generated Slots</div>\n");
    printf("            </div>\n");
    printf("        </div>\n");
    printf("    </div>\n");
}

void handle_rooms_page() {
    printf("    <div class=\"card\">\n");
    printf("        <h2>Room List</h2>\n");
    
    if (room_count == 0) {
        printf("        <p>No rooms available.</p>\n");
    } else {
        printf("        <table>\n");
        printf("            <tr>\n");
        printf("                <th>ID</th>\n");
        printf("                <th>Name</th>\n");
        printf("                <th>Type</th>\n");
        printf("                <th>Actions</th>\n");
        printf("            </tr>\n");
        
        for (int i = 0; i < room_count; i++) {
            if (rooms[i].is_active) {
                printf("            <tr>\n");
                printf("                <td>%d</td>\n", rooms[i].id);
                printf("                <td>%s</td>\n", rooms[i].name);
                printf("                <td>%s</td>\n", 
                       rooms[i].type == CLASSROOM ? "Classroom" : "Laboratory");
                printf("                <td>\n");
                printf("                    <a href=\"?page=delete_room&id=%d\" class=\"btn btn-danger\" onclick=\"return confirm('Delete this room?')\">Delete</a>\n", rooms[i].id);
                printf("                </td>\n");
                printf("            </tr>\n");
            }
        }
        printf("        </table>\n");
    }
    printf("    </div>\n");
}

void handle_add_room_form(char *query_string) {
    (void)query_string;
    char *method = getenv("REQUEST_METHOD");
    if (method && strcmp(method, "POST") == 0) {
        char *content_length_str = getenv("CONTENT_LENGTH");
        int content_length = content_length_str ? atoi(content_length_str) : 0;
        
        if (content_length > 0) {
            char *post_data = malloc(content_length + 1);
            fread(post_data, 1, content_length, stdin);
            post_data[content_length] = '\0';
            
            char room_name[MAX_NAME_LENGTH] = "";
            char room_type[20] = "";
            
            char *token = strtok(post_data, "&");
            while (token) {
                char *equals = strchr(token, '=');
                if (equals) {
                    *equals = '\0';
                    
                    if (strcmp(token, "name") == 0) {
                        url_decode(equals + 1);
                        strncpy(room_name, equals + 1, MAX_NAME_LENGTH - 1);
                    } else if (strcmp(token, "type") == 0) {
                        url_decode(equals + 1);
                        strncpy(room_type, equals + 1, 19);
                    }
                }
                token = strtok(NULL, "&");
            }
            
            if (strlen(room_name) > 0 && room_count < MAX_ROOMS) {
                Room new_room;
                new_room.id = next_room_id++;
                strcpy(new_room.name, room_name);
                new_room.type = (strcmp(room_type, "LAB") == 0) ? LAB : CLASSROOM;
                new_room.is_active = 1;
                
                rooms[room_count++] = new_room;
                save_rooms();
                
                printf("<div class=\"success\">Room '%s' added successfully! (ID: %d)</div>\n", 
                       new_room.name, new_room.id);
            }
            
            free(post_data);
        }
    }
    
    printf("    <div class=\"card\">\n");
    printf("        <h2>Add New Room</h2>\n");
    printf("        <form method=\"POST\">\n");
    printf("            <div class=\"form-group\">\n");
    printf("                <label for=\"name\">Room Name:</label>\n");
    printf("                <input type=\"text\" id=\"name\" name=\"name\" required>\n");
    printf("            </div>\n");
    printf("            <div class=\"form-group\">\n");
    printf("                <label for=\"type\">Room Type:</label>\n");
    printf("                <select id=\"type\" name=\"type\">\n");
    printf("                    <option value=\"CLASSROOM\">Classroom</option>\n");
    printf("                    <option value=\"LAB\">Laboratory</option>\n");
    printf("                </select>\n");
    printf("            </div>\n");
    printf("            <button type=\"submit\" class=\"btn\">Add Room</button>\n");
    printf("        </form>\n");
    printf("    </div>\n");
}

void handle_schedules_page() {
    printf("    <div class=\"card\">\n");
    printf("        <h2>All Schedules</h2>\n");
    
    if (schedule_count == 0) {
        printf("        <p>No schedules available.</p>\n");
    } else {
        printf("        <table>\n");
        printf("            <tr>\n");
        printf("                <th>ID</th>\n");
        printf("                <th>Course</th>\n");
        printf("                <th>Department</th>\n");
        printf("                <th>Year</th>\n");
        printf("                <th>Program</th>\n");
        printf("                <th>Room Type</th>\n");
        printf("                <th>Hrs/Week</th>\n");
        printf("                <th>Priority</th>\n");
        printf("                <th>Actions</th>\n");
        printf("            </tr>\n");
        
        for (int i = 0; i < schedule_count; i++) {
            if (schedules[i].is_active) {
                printf("            <tr>\n");
                printf("                <td>%d</td>\n", schedules[i].id);
                printf("                <td>%s</td>\n", schedules[i].course_name);
                printf("                <td>%s</td>\n", schedules[i].department);
                printf("                <td>%d</td>\n", schedules[i].student_year);
                printf("                <td>%s</td>\n", schedules[i].program_type == MAJOR ? "Major" : "Minor");
                printf("                <td>%s</td>\n", schedules[i].required_room_type == LAB ? "Lab" : "Classroom");
                printf("                <td>%d hrs</td>\n", schedules[i].weekly_hours);
                printf("                <td>%d</td>\n", schedules[i].priority);
                printf("                <td>\n");
                printf("                    <a href=\"?page=delete_schedule&id=%d\" class=\"btn btn-danger\" onclick=\"return confirm('Delete this schedule?')\">Delete</a>\n", schedules[i].id);
                printf("                </td>\n");
                printf("            </tr>\n");
            }
        }
        printf("        </table>\n");
    }
    printf("    </div>\n");
}

void handle_add_schedule_form(char *query_string) {
    (void)query_string;
  
    char *method = getenv("REQUEST_METHOD");
    
    if (method && strcmp(method, "POST") == 0) {
        char *content_length_str = getenv("CONTENT_LENGTH");
        int content_length = content_length_str ? atoi(content_length_str) : 0;
        
        if (content_length > 0) {
            char *post_data = malloc(content_length + 1);
            fread(post_data, 1, content_length, stdin);
            post_data[content_length] = '\0';
            
            char course_name[MAX_NAME_LENGTH] = "";
            char department[MAX_DEPT_LENGTH] = "";
            char year_str[10] = "";
            char program_str[20] = "";
            char room_type_str[20] = "";
            char hours_str[10] = "";
            
            char *token = strtok(post_data, "&");
            while (token) {
                char *equals = strchr(token, '=');
                if (equals) {
                    *equals = '\0';
                    url_decode(equals + 1);
                    
                    if (strcmp(token, "course") == 0) {
                        strncpy(course_name, equals + 1, MAX_NAME_LENGTH - 1);
                    } else if (strcmp(token, "dept") == 0) {
                        strncpy(department, equals + 1, MAX_DEPT_LENGTH - 1);
                    } else if (strcmp(token, "year") == 0) {
                        strncpy(year_str, equals + 1, 9);
                    } else if (strcmp(token, "program") == 0) {
                        strncpy(program_str, equals + 1, 19);
                    } else if (strcmp(token, "room_type") == 0) {
                        strncpy(room_type_str, equals + 1, 19);
                    } else if (strcmp(token, "hours") == 0) {
                        strncpy(hours_str, equals + 1, 9);
                    }
                }
                token = strtok(NULL, "&");
            }
            
            if (strlen(course_name) > 0 && schedule_count < MAX_SCHEDULES) {
                Schedule new_schedule;
                new_schedule.id = next_schedule_id++;
                new_schedule.fifo_order = next_fifo_order++;
                strcpy(new_schedule.course_name, course_name);
                strcpy(new_schedule.department, department);
                new_schedule.student_year = atoi(year_str);
                new_schedule.program_type = (strcmp(program_str, "MAJOR") == 0) ? MAJOR : MINOR;
                new_schedule.required_room_type = (strcmp(room_type_str, "LAB") == 0) ? LAB : CLASSROOM;
                new_schedule.weekly_hours = atoi(hours_str);
                new_schedule.assigned_room_id = -1;
                new_schedule.is_active = 1;
                
                // Calculate priority
                new_schedule.priority = (new_schedule.student_year * 10) +
                                       (new_schedule.program_type == MAJOR ? 5 : 0) +
                                       (new_schedule.required_room_type == LAB ? 3 : 0);
                
                schedules[schedule_count++] = new_schedule;
                save_schedules();
                
                printf("<div class=\"success\">Schedule for '%s' added! (ID: %d, Priority: %d, %d hrs/week)</div>\n",
                       new_schedule.course_name, new_schedule.id, new_schedule.priority, new_schedule.weekly_hours);
            }
            
            free(post_data);
        }
    }
    
    printf("    <div class=\"card\">\n");
    printf("        <h2>Add New Schedule</h2>\n");
    printf("        <form method=\"POST\">\n");
    printf("            <div class=\"form-group\">\n");
    printf("                <label for=\"course\">Course Name:</label>\n");
    printf("                <input type=\"text\" id=\"course\" name=\"course\" required>\n");
    printf("            </div>\n");
    printf("            <div class=\"form-group\">\n");
    printf("                <label for=\"dept\">Department:</label>\n");
    printf("                <input type=\"text\" id=\"dept\" name=\"dept\" required>\n");
    printf("            </div>\n");
    printf("            <div class=\"form-group\">\n");
    printf("                <label for=\"year\">Student Year:</label>\n");
    printf("                <select id=\"year\" name=\"year\">\n");
    printf("                    <option value=\"1\">Year 1</option>\n");
    printf("                    <option value=\"2\">Year 2</option>\n");
    printf("                    <option value=\"3\">Year 3</option>\n");
    printf("                    <option value=\"4\">Year 4</option>\n");
    printf("                    <option value=\"5\">Year 5</option>\n");
    printf("                </select>\n");
    printf("            </div>\n");
    printf("            <div class=\"form-group\">\n");
    printf("                <label for=\"program\">Program Type:</label>\n");
    printf("                <select id=\"program\" name=\"program\">\n");
    printf("                    <option value=\"MAJOR\">Major</option>\n");
    printf("                    <option value=\"MINOR\">Minor</option>\n");
    printf("                </select>\n");
    printf("            </div>\n");
    printf("            <div class=\"form-group\">\n");
    printf("                <label for=\"room_type\">Required Room Type:</label>\n");
    printf("                <select id=\"room_type\" name=\"room_type\">\n");
    printf("                    <option value=\"CLASSROOM\">Classroom</option>\n");
    printf("                    <option value=\"LAB\">Laboratory</option>\n");
    printf("                </select>\n");
    printf("            </div>\n");
    printf("            <div class=\"form-group\">\n");
    printf("                <label for=\"hours\">Weekly Hours (1-20):</label>\n");
    printf("                <input type=\"number\" id=\"hours\" name=\"hours\" min=\"1\" max=\"20\" value=\"3\" required>\n");
    printf("            </div>\n");
    printf("            <p><small><strong>Priority Calculation:</strong> Year*10 + Major(+5) + Lab(+3)</small></p>\n");
    printf("            <button type=\"submit\" class=\"btn\">Add Schedule</button>\n");
    printf("        </form>\n");
    printf("    </div>\n");
}

void handle_generate_page() {
    printf("    <div class=\"card\">\n");
    printf("        <h2>Generate Weekly Schedule (Distributed)</h2>\n");
    printf("        <p>This will automatically assign time slots for all schedules with <strong>even distribution across 5 days</strong>.</p>\n");
    printf("        <p><strong>Distribution Algorithm:</strong></p>\n");
    printf("        <ul>\n");
    printf("            <li><strong>Even Distribution:</strong> Classes spread across Monday-Friday (e.g., 6 hrs = ~72 min/day)</li>\n");
    printf("            <li><strong>Minimum Session:</strong> Each class session is at least 1 hour 30 minutes (90 min)</li>\n");
    printf("            <li><strong>Priority:</strong> Higher priority courses are scheduled first</li>\n");
    printf("            <li><strong>Room Type:</strong> Lab courses MUST use lab rooms</li>\n");
    printf("            <li><strong>Conflict Avoidance:</strong> Same dept/year students won't have overlapping classes</li>\n");
    printf("        </ul>\n");
    printf("        <p><strong>Time Slots (90-minute sessions):</strong></p>\n");
    printf("        <ul>\n");
    printf("            <li>Morning: 08:00-09:30, 09:30-11:00, 11:00-12:30</li>\n");
    printf("            <li>Afternoon: 14:00-15:30, 15:30-17:00, 17:00-17:30</li>\n");
    printf("        </ul>\n");
    printf("        <p><strong>Example:</strong> A 6-hour course will be distributed as follows:</p>\n");
    printf("        <ul>\n");
    printf("            <li>Monday: 90 min, Tuesday: 90 min, Wednesday: 90 min, Thursday: 90 min (Total: 6 hrs)</li>\n");
    printf("        </ul>\n");
    printf("        <a href=\"?page=generate&action=run\" class=\"btn btn-success\">Generate Distributed Schedule</a>\n");
    printf("    </div>\n");
}

void handle_timetable_page() {
    printf("    <div class=\"card\">\n");
    printf("        <h2>Weekly Timetable</h2>\n");
    
    if (generated_slot_count == 0) {
        printf("        <div class=\"warning\">No schedule generated yet. Please run 'Generate Schedule' first.</div>\n");
        printf("    </div>\n");
        return;
    }
    
    for (DayOfWeek day = 0; day < 5; day++) {
        printf("        <div class=\"day-header\">%s</div>\n", web_get_day_string(day));
        printf("        <table>\n");
        printf("            <tr><th>Time</th><th>Course</th><th>Department</th><th>Year</th><th>Room</th></tr>\n");
        
        int has_slots = 0;
        for (int i = 0; i < generated_slot_count; i++) {
            if (!generated_slots[i].is_active) continue;
            if (generated_slots[i].day != day) continue;
            
            char course_name[MAX_NAME_LENGTH] = "Unknown";
            char department[MAX_DEPT_LENGTH] = "";
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
            
            char start_str[10], end_str[10];
            web_format_time(generated_slots[i].start_time, start_str);
            web_format_time(generated_slots[i].end_time, end_str);
            
            printf("            <tr><td>%s-%s</td><td>%s</td><td>%s</td><td>%d</td><td>%s</td></tr>\n",
                   start_str, end_str, course_name, department, year, room_name);
            has_slots = 1;
        }
        
        if (!has_slots) {
            printf("            <tr><td colspan=\"5\">No classes scheduled</td></tr>\n");
        }
        printf("        </table>\n");
    }
    printf("    </div>\n");
}

void handle_final_schedule_page() {
    printf("    <div class=\"card\">\n");
    printf("        <h2>Final Schedule (By Room)</h2>\n");
    
    if (generated_slot_count == 0) {
        printf("        <div class=\"warning\">No schedule generated yet. Please run 'Generate Schedule' first.</div>\n");
        printf("    </div>\n");
        return;
    }
    
    for (int r = 0; r < room_count; r++) {
        if (!rooms[r].is_active) continue;
        
        int has_slots = 0;
        for (int i = 0; i < generated_slot_count; i++) {
            if (generated_slots[i].is_active && generated_slots[i].room_id == rooms[r].id) {
                has_slots = 1;
                break;
            }
        }
        
        if (!has_slots) continue;
        
        printf("        <div class=\"room-section\">\n");
        printf("            <div class=\"room-header\">%s (%s)</div>\n", 
               rooms[r].name, rooms[r].type == LAB ? "Laboratory" : "Classroom");
        
        for (DayOfWeek day = 0; day < 5; day++) {
            printf("            <div style=\"padding: 5px 15px; background: #ecf0f1;\"><strong>%s</strong></div>\n", web_get_day_string(day));
            printf("            <table style=\"margin: 0;\">\n");
            
            DayOfWeek day_has_slots = 0;
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
                web_format_time(generated_slots[i].start_time, start_str);
                web_format_time(generated_slots[i].end_time, end_str);
                
                printf("                <tr><td>%s-%s</td><td>%s</td><td>%s</td><td>Year %d</td><td>P:%d</td></tr>\n",
                       start_str, end_str, course_name, department, year, priority);
                day_has_slots = 1;
            }
            
            if (!day_has_slots) {
                printf("                <tr><td colspan=\"5\" style=\"text-align:center; color:#999;\">No classes</td></tr>\n");
            }
            printf("            </table>\n");
        }
        printf("        </div>\n");
    }
    printf("    </div>\n");
}

void handle_schedule_by_year_dept_form(char *query_string) {
    char dept[MAX_DEPT_LENGTH] = "";
    char year_str[10] = "";
    
    parse_query_param(query_string, "dept", dept);
    parse_query_param(query_string, "year", year_str);
    
    printf("    <div class=\"card\">\n");
    printf("        <h2>View Schedule by Year & Department</h2>\n");
    printf("        <form method=\"GET\">\n");
    printf("            <input type=\"hidden\" name=\"page\" value=\"schedule_year_dept\">\n");
    printf("            <div class=\"form-group\">\n");
    printf("                <label for=\"dept\">Department:</label>\n");
    printf("                <input type=\"text\" id=\"dept\" name=\"dept\" value=\"%s\" required>\n", dept);
    printf("            </div>\n");
    printf("            <div class=\"form-group\">\n");
    printf("                <label for=\"year\">Student Year:</label>\n");
    printf("                <select id=\"year\" name=\"year\">\n");
    for (int y = 1; y <= 5; y++) {
        printf("                    <option value=\"%d\" %s>Year %d</option>\n", 
               y, (atoi(year_str) == y) ? "selected" : "", y);
    }
    printf("                </select>\n");
    printf("            </div>\n");
    printf("            <button type=\"submit\" class=\"btn\">View Schedule</button>\n");
    printf("        </form>\n");
    printf("    </div>\n");
    
    // Display results if dept and year provided
    if (strlen(dept) > 0 && strlen(year_str) > 0) {
        int year = atoi(year_str);
        
        printf("    <div class=\"card\">\n");
        printf("        <h2>Weekly Schedule: %s - Year %d</h2>\n", dept, year);
        
        if (generated_slot_count == 0) {
            printf("        <div class=\"warning\">No schedule generated yet.</div>\n");
            printf("    </div>\n");
            return;
        }
        
        int total_classes = 0;
        
        for (DayOfWeek day = 0; day < 5; day++) {
            printf("        <div class=\"day-header\">%s</div>\n", web_get_day_string(day));
            printf("        <table>\n");
            printf("            <tr><th>Time</th><th>Course</th><th>Room Type</th><th>Room</th></tr>\n");
            
            int has_slots = 0;
            for (int i = 0; i < generated_slot_count; i++) {
                if (!generated_slots[i].is_active) continue;
                if (generated_slots[i].day != day) continue;
                
                for (int j = 0; j < schedule_count; j++) {
                    if (schedules[j].id == generated_slots[i].schedule_id &&
                        schedules[j].is_active &&
                        strcasecmp(schedules[j].department, dept) == 0 &&
                        schedules[j].student_year == year) {
                        
                        char room_name[MAX_NAME_LENGTH] = "Unknown";
                        for (int k = 0; k < room_count; k++) {
                            if (rooms[k].id == generated_slots[i].room_id) {
                                strcpy(room_name, rooms[k].name);
                                break;
                            }
                        }
                        
                        char start_str[10], end_str[10];
                        web_format_time(generated_slots[i].start_time, start_str);
                        web_format_time(generated_slots[i].end_time, end_str);
                        
                        printf("            <tr><td>%s-%s</td><td>%s</td><td>%s</td><td>%s</td></tr>\n",
                               start_str, end_str, schedules[j].course_name,
                               schedules[j].required_room_type == LAB ? "Lab" : "Classroom",
                               room_name);
                        has_slots = 1;
                        total_classes++;
                        break;
                    }
                }
            }
            
            if (!has_slots) {
                printf("            <tr><td colspan=\"4\">No classes</td></tr>\n");
            }
            printf("        </table>\n");
        }
        
        printf("        <div class=\"success\">Total classes this week: %d</div>\n", total_classes);
        printf("    </div>\n");
    }
}

// New: View schedule by selecting a specific room
void handle_schedule_by_room_form(char *query_string) {
    char room_id_str[20] = "";
    parse_query_param(query_string, "room_id", room_id_str);
    int selected_room_id = atoi(room_id_str);
    
    printf("    <div class=\"card\">\n");
    printf("        <h2>View Schedule by Room</h2>\n");
    printf("        <form method=\"GET\">\n");
    printf("            <input type=\"hidden\" name=\"page\" value=\"schedule_by_room\">\n");
    printf("            <div class=\"form-group\">\n");
    printf("                <label for=\"room_id\">Select Room:</label>\n");
    printf("                <select id=\"room_id\" name=\"room_id\" required>\n");
    printf("                    <option value=\"\">-- Select a Room --</option>\n");
    
    for (int r = 0; r < room_count; r++) {
        if (!rooms[r].is_active) continue;
        printf("                    <option value=\"%d\" %s>%s (%s)</option>\n",
               rooms[r].id,
               (rooms[r].id == selected_room_id) ? "selected" : "",
               rooms[r].name,
               rooms[r].type == LAB ? "Lab" : "Classroom");
    }
    
    printf("                </select>\n");
    printf("            </div>\n");
    printf("            <button type=\"submit\" class=\"btn\">View Room Schedule</button>\n");
    printf("        </form>\n");
    printf("    </div>\n");
    
    // Display room schedule if selected
    if (selected_room_id > 0) {
        // Find room info
        char room_name[MAX_NAME_LENGTH] = "Unknown";
        RoomType room_type = CLASSROOM;
        int room_found = 0;
        
        for (int r = 0; r < room_count; r++) {
            if (rooms[r].id == selected_room_id && rooms[r].is_active) {
                strcpy(room_name, rooms[r].name);
                room_type = rooms[r].type;
                room_found = 1;
                break;
            }
        }
        
        if (!room_found) {
            printf("    <div class=\"error\">Room not found.</div>\n");
            return;
        }
        
        printf("    <div class=\"card\">\n");
        printf("        <h2>Weekly Schedule: %s (%s)</h2>\n", room_name, room_type == LAB ? "Laboratory" : "Classroom");
        
        if (generated_slot_count == 0) {
            printf("        <div class=\"warning\">No schedule generated yet. Please run 'Generate Schedule' first.</div>\n");
            printf("    </div>\n");
            return;
        }
        
        int total_hours = 0;
        int total_minutes = 0;
        
        for (DayOfWeek day = 0; day < 5; day++) {
            printf("        <div class=\"day-header\">%s</div>\n", web_get_day_string(day));
            printf("        <table>\n");
            printf("            <tr><th>Time</th><th>Duration</th><th>Course</th><th>Dept</th><th>Year</th><th>Program</th></tr>\n");
            
            int has_slots = 0;
            for (int i = 0; i < generated_slot_count; i++) {
                if (!generated_slots[i].is_active) continue;
                if (generated_slots[i].room_id != selected_room_id) continue;
                if (generated_slots[i].day != day) continue;
                
                char course_name[MAX_NAME_LENGTH] = "Unknown";
                char department[MAX_DEPT_LENGTH] = "";
                int year = 0;
                char program[10] = "";
                
                for (int j = 0; j < schedule_count; j++) {
                    if (schedules[j].id == generated_slots[i].schedule_id) {
                        strcpy(course_name, schedules[j].course_name);
                        strcpy(department, schedules[j].department);
                        year = schedules[j].student_year;
                        strcpy(program, schedules[j].program_type == MAJOR ? "Major" : "Minor");
                        break;
                    }
                }
                
                int duration = generated_slots[i].end_time - generated_slots[i].start_time;
                total_minutes += duration;
                
                char start_str[10], end_str[10];
                web_format_time(generated_slots[i].start_time, start_str);
                web_format_time(generated_slots[i].end_time, end_str);
                
                printf("            <tr><td>%s - %s</td><td>%d min</td><td>%s</td><td>%s</td><td>Year %d</td><td>%s</td></tr>\n",
                       start_str, end_str, duration, course_name, department, year, program);
                has_slots = 1;
            }
            
            if (!has_slots) {
                printf("            <tr><td colspan=\"6\" style=\"text-align:center; color:#999;\">No classes scheduled</td></tr>\n");
            }
            printf("        </table>\n");
        }
        
        total_hours = total_minutes / 60;
        int remaining_mins = total_minutes % 60;
        
        printf("        <div class=\"success\">\n");
        printf("            <strong>Room Utilization Summary:</strong><br>\n");
        printf("            Total time used: %d hours %d minutes per week<br>\n", total_hours, remaining_mins);
        printf("            Available time: ~40 hours per week (8 hrs x 5 days)\n");
        printf("        </div>\n");
        printf("    </div>\n");
    }
}

void handle_all_rooms_schedule() {
    printf("    <div class=\"card\">\n");
    printf("        <h2>All Rooms Weekly Schedule Overview</h2>\n");
    
    if (generated_slot_count == 0) {
        printf("        <div class=\"warning\">No schedule generated yet. Please run 'Generate Schedule' first.</div>\n");
        printf("    </div>\n");
        return;
    }
    
    // Summary table first
    printf("        <h3>Room Utilization Summary</h3>\n");
    printf("        <table>\n");
    printf("            <tr><th>Room</th><th>Type</th><th>Mon</th><th>Tue</th><th>Wed</th><th>Thu</th><th>Fri</th><th>Total</th></tr>\n");
    
    for (int r = 0; r < room_count; r++) {
        if (!rooms[r].is_active) continue;
        
        DayOfWeek day_minutes[5] = {0, 0, 0, 0, 0};
        int total_room_minutes = 0;
        
        for (int i = 0; i < generated_slot_count; i++) {
            if (!generated_slots[i].is_active) continue;
            if (generated_slots[i].room_id != rooms[r].id) continue;
            
            int duration = generated_slots[i].end_time - generated_slots[i].start_time;
            day_minutes[generated_slots[i].day] += duration;
            total_room_minutes += duration;
        }
        
        printf("            <tr>\n");
        printf("                <td><a href=\"?page=schedule_by_room&room_id=%d\">%s</a></td>\n", rooms[r].id, rooms[r].name);
        printf("                <td>%s</td>\n", rooms[r].type == LAB ? "Lab" : "Class");
        for (int d = 0; d < 5; d++) {
            printf("                <td>%dh%dm</td>\n", day_minutes[d] / 60, day_minutes[d] % 60);
        }
        printf("                <td><strong>%dh%dm</strong></td>\n", total_room_minutes / 60, total_room_minutes % 60);
        printf("            </tr>\n");
    }
    printf("        </table>\n");
    
    printf("        <h3>Detailed Schedule by Room</h3>\n");
    
    for (int r = 0; r < room_count; r++) {
        if (!rooms[r].is_active) continue;
        
        printf("        <div class=\"room-section\">\n");
        printf("            <div class=\"room-header\">%s (%s) - <a href=\"?page=schedule_by_room&room_id=%d\" style=\"color:white;\">View Details</a></div>\n", 
               rooms[r].name, rooms[r].type == LAB ? "Laboratory" : "Classroom", rooms[r].id);
        printf("            <table>\n");
        printf("                <tr><th>Day</th><th>Time</th><th>Course</th><th>Dept</th><th>Year</th></tr>\n");
        
        int has_any = 0;
        for (DayOfWeek day = 0; day < 5; day++) {
            for (int i = 0; i < generated_slot_count; i++) {
                if (!generated_slots[i].is_active) continue;
                if (generated_slots[i].room_id != rooms[r].id) continue;
                if (generated_slots[i].day != day) continue;
                
                char course_name[MAX_NAME_LENGTH] = "Unknown";
                char department[MAX_DEPT_LENGTH] = "";
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
                web_format_time(generated_slots[i].start_time, start_str);
                web_format_time(generated_slots[i].end_time, end_str);
                
                printf("                <tr><td>%s</td><td>%s-%s</td><td>%s</td><td>%s</td><td>%d</td></tr>\n",
                       web_get_day_string(day), start_str, end_str, course_name, department, year);
                has_any = 1;
            }
        }
        
        if (!has_any) {
            printf("                <tr><td colspan=\"5\" style=\"text-align:center; color:#999;\">No classes assigned to this room</td></tr>\n");
        }
        
        printf("            </table>\n");
        printf("        </div>\n");
    }
    printf("    </div>\n");
}

void handle_delete_room(char *query_string) {
    char id_str[20] = "";
    parse_query_param(query_string, "id", id_str);
    int id = atoi(id_str);
    
    for (int i = 0; i < room_count; i++) {
        if (rooms[i].id == id && rooms[i].is_active) {
            rooms[i].is_active = 0;
            save_rooms();
            printf("<div class=\"success\">Room deleted successfully!</div>\n");
            break;
        }
    }
    
    handle_rooms_page();
}

void handle_delete_schedule(char *query_string) {
    char id_str[20] = "";
    parse_query_param(query_string, "id", id_str);
    int id = atoi(id_str);
    
    for (int i = 0; i < schedule_count; i++) {
        if (schedules[i].id == id && schedules[i].is_active) {
            schedules[i].is_active = 0;
            save_schedules();
            printf("<div class=\"success\">Schedule deleted successfully!</div>\n");
            break;
        }
    }
    
    handle_schedules_page();
}

int main() {
    // Load data
    load_rooms();
    load_schedules();
    load_generated_slots();
    
    char *query_string = getenv("QUERY_STRING");
    char page[50] = "home";
    char action[50] = "";
    
    if (query_string) {
        parse_query_param(query_string, "page", page);
        parse_query_param(query_string, "action", action);
    }
    
    print_html_header();
    print_navigation();
    
    if (strcmp(page, "home") == 0) {
        handle_home_page();
    } 
    else if (strcmp(page, "rooms") == 0) {
        handle_rooms_page();
    }
    else if (strcmp(page, "add_room") == 0) {
        handle_add_room_form(query_string);
    }
    else if (strcmp(page, "schedules") == 0) {
        handle_schedules_page();
    }
    else if (strcmp(page, "add_schedule") == 0) {
        handle_add_schedule_form(query_string);
    }
    else if (strcmp(page, "generate") == 0) {
        if (strcmp(action, "run") == 0) {
            web_generate_schedule();
        } else {
            handle_generate_page();
        }
    }
    else if (strcmp(page, "timetable") == 0) {
        handle_timetable_page();
    }
    else if (strcmp(page, "final") == 0) {
        handle_final_schedule_page();
    }
    else if (strcmp(page, "schedule_year_dept") == 0) {
        handle_schedule_by_year_dept_form(query_string);
    }
    else if (strcmp(page, "schedule_by_room") == 0) {
        handle_schedule_by_room_form(query_string);
    }
    else if (strcmp(page, "all_rooms") == 0) {
        handle_all_rooms_schedule();
    }
    else if (strcmp(page, "delete_room") == 0) {
        handle_delete_room(query_string);
    }
    else if (strcmp(page, "delete_schedule") == 0) {
        handle_delete_schedule(query_string);
    }
    else {
        handle_home_page();
    }
    
    print_html_footer();
    
    return 0;
}
