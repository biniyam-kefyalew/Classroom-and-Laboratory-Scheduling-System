// #include "scheduling_system.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <ctype.h>

// // Use writable directories
// #define ROOMS_FILE "/tmp/rooms.dat"
// #define SCHEDULES_FILE "/tmp/schedules.dat"

// // Global variables (already declared in header)
// // Room rooms[MAX_ROOMS];
// // Schedule schedules[MAX_SCHEDULES];
// // int room_count = 0;
// // int schedule_count = 0;
// // int next_room_id = 1;
// // int next_schedule_id = 1;
// // Custom strdup implementation for C99
// char* my_strdup(const char* s) {
//     if (s == NULL) return NULL;
//     size_t len = strlen(s) + 1;
//     char* new = malloc(len);
//     if (new == NULL) return NULL;
//     memcpy(new, s, len);
//     return new;
// }
// void print_html_header() {
//     printf("Content-type: text/html\n\n");
//     printf("<!DOCTYPE html>\n");
//     printf("<html lang=\"en\">\n");
//     printf("<head>\n");
//     printf("    <meta charset=\"UTF-8\">\n");
//     printf("    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n");
//     printf("    <title>Room Scheduling System - Bahir Dar University</title>\n");
//     printf("    <style>\n");
//     printf("        body { font-family: Arial, sans-serif; margin: 20px; background: #f5f5f5; }\n");
//     printf("        .container { max-width: 1200px; margin: 0 auto; background: white; padding: 20px; border-radius: 10px; box-shadow: 0 0 10px rgba(0,0,0,0.1); }\n");
//     printf("        header { background: #2c3e50; color: white; padding: 20px; border-radius: 5px; margin-bottom: 20px; }\n");
//     printf("        nav { background: #34495e; padding: 10px; border-radius: 5px; margin-bottom: 20px; }\n");
//     printf("        nav a { color: white; text-decoration: none; padding: 10px 15px; margin: 0 5px; border-radius: 3px; }\n");
//     printf("        nav a:hover { background: #1abc9c; }\n");
//     printf("        .card { background: white; border: 1px solid #ddd; border-radius: 5px; padding: 15px; margin-bottom: 15px; }\n");
//     printf("        table { width: 100%%; border-collapse: collapse; margin-bottom: 20px; }\n");
//     printf("        th, td { border: 1px solid #ddd; padding: 8px; text-align: left; }\n");
//     printf("        th { background-color: #3498db; color: white; }\n");
//     printf("        tr:nth-child(even) { background-color: #f2f2f2; }\n");
//     printf("        .btn { background: #3498db; color: white; padding: 10px 15px; border: none; border-radius: 3px; cursor: pointer; }\n");
//     printf("        .btn:hover { background: #2980b9; }\n");
//     printf("        .form-group { margin-bottom: 15px; }\n");
//     printf("        label { display: block; margin-bottom: 5px; font-weight: bold; }\n");
//     printf("        input, select, textarea { width: 100%%; padding: 8px; border: 1px solid #ddd; border-radius: 3px; box-sizing: border-box; }\n");
//     printf("        .success { color: #27ae60; background: #d5f4e6; padding: 10px; border-radius: 3px; margin: 10px 0; }\n");
//     printf("        .error { color: #c0392b; background: #fadbd8; padding: 10px; border-radius: 3px; margin: 10px 0; }\n");
//     printf("    </style>\n");
//     printf("</head>\n");
//     printf("<body>\n");
//     printf("<div class=\"container\">\n");
//     printf("    <header>\n");
//     printf("        <h1>Classroom and Laboratory Scheduling System</h1>\n");
//     printf("        <h2>Bahir Dar University</h2>\n");
//     printf("    </header>\n");
// }

// void print_html_footer() {
//     printf("</div>\n");
//     printf("</body>\n");
//     printf("</html>\n");
// }

// void print_navigation() {
//     printf("    <nav>\n");
//     printf("        <a href=\"?page=home\">Home</a>\n");
//     printf("        <a href=\"?page=rooms\">Rooms</a>\n");
//     printf("        <a href=\"?page=add_room\">Add Room</a>\n");
//     printf("        <a href=\"?page=schedules\">Schedules</a>\n");
//     printf("        <a href=\"?page=add_schedule\">Add Schedule</a>\n");
//     printf("        <a href=\"?page=generate\">Generate Schedule</a>\n");
//     printf("        <a href=\"?page=final\">Final Schedule</a>\n");
//     printf("    </nav>\n");
// }

// void url_decode(char *str) {
//     char *p = str;
//     char buffer[strlen(str) + 1];
//     int i = 0;
    
//     while (*p) {
//         if (*p == '%' && *(p+1) && *(p+2)) {
//             char hex[3] = {*(p+1), *(p+2), '\0'};
//             buffer[i++] = (char)strtol(hex, NULL, 16);
//             p += 3;
//         } else if (*p == '+') {
//             buffer[i++] = ' ';
//             p++;
//         } else {
//             buffer[i++] = *p++;
//         }
//     }
//     buffer[i] = '\0';
//     strcpy(str, buffer);
// }

// void parse_query_param(char *query, const char *key, char *value) {
//     value[0] = '\0';
//     if (!query) return;
    
//     char *query_copy = my_strdup(query);
//     char *token = strtok(query_copy, "&");
    
//     while (token) {
//         char *equals = strchr(token, '=');
//         if (equals) {
//             *equals = '\0';
//             char decoded_key[256];
//             strcpy(decoded_key, token);
//             url_decode(decoded_key);
            
//             if (strcmp(decoded_key, key) == 0) {
//                 url_decode(equals + 1);
//                 strcpy(value, equals + 1);
//                 break;
//             }
//         }
//         token = strtok(NULL, "&");
//     }
    
//     free(query_copy);
// }

// int parse_time_string(const char *time_str) {
//     int hour, minute;
//     if (sscanf(time_str, "%d:%d", &hour, &minute) == 2) {
//         if (hour >= 0 && hour < 24 && minute >= 0 && minute < 60) {
//             return hour * 60 + minute;
//         }
//     }
//     return -1;
// }

// void generate_final_schedule() {
//     // Sort schedules by priority (highest first)
//     Schedule active_schedules[MAX_SCHEDULES];
//     int active_count = 0;
    
//     for (int i = 0; i < schedule_count; i++) {
//         if (schedules[i].is_active && schedules[i].assigned_room_id == -1) {
//             active_schedules[active_count++] = schedules[i];
//         }
//     }
    
//     // Sort by priority
//     for (int i = 0; i < active_count - 1; i++) {
//         for (int j = i + 1; j < active_count; j++) {
//             if (active_schedules[j].priority > active_schedules[i].priority) {
//                 Schedule temp = active_schedules[i];
//                 active_schedules[i] = active_schedules[j];
//                 active_schedules[j] = temp;
//             }
//         }
//     }
    
//     int assigned_count = 0;
    
//     // Try to assign each schedule
//     for (int i = 0; i < active_count; i++) {
//         int assigned = 0;
        
//         // Find suitable room
//         for (int j = 0; j < room_count && !assigned; j++) {
//             if (!rooms[j].is_active) continue;
//             if (rooms[j].type != active_schedules[i].required_room_type) continue;
            
//             // Check for conflicts
//             int conflict = 0;
//             for (int k = 0; k < schedule_count; k++) {
//                 if (schedules[k].is_active && 
//                     schedules[k].assigned_room_id == rooms[j].id) {
                    
//                     // Check time overlap
//                     int overlap = !(active_schedules[i].end_time <= schedules[k].start_time ||
//                                    active_schedules[i].start_time >= schedules[k].end_time);
//                     if (overlap) {
//                         conflict = 1;
//                         break;
//                     }
//                 }
//             }
            
//             if (!conflict) {
//                 // Assign this room
//                 for (int k = 0; k < schedule_count; k++) {
//                     if (schedules[k].id == active_schedules[i].id) {
//                         schedules[k].assigned_room_id = rooms[j].id;
//                         assigned_count++;
//                         assigned = 1;
//                         break;
//                     }
//                 }
//             }
//         }
//     }
    
//     save_schedules();
    
//     printf("<div class=\"success\">");
//     printf("Schedule generation completed!<br>");
//     printf("Assigned %d out of %d active schedules.", assigned_count, active_count);
//     printf("</div>");
// }

// void handle_home_page() {
//     printf("    <div class=\"card\">\n");
//     printf("        <h2>Welcome to Scheduling System</h2>\n");
//     printf("        <p>This system helps manage classroom and laboratory schedules at Bahir Dar University.</p>\n");
    
//     load_rooms();
//     load_schedules();
    
//     int active_rooms = 0;
//     int active_schedules = 0;
//     int assigned_schedules = 0;
    
//     for (int i = 0; i < room_count; i++) {
//         if (rooms[i].is_active) active_rooms++;
//     }
    
//     for (int i = 0; i < schedule_count; i++) {
//         if (schedules[i].is_active) {
//             active_schedules++;
//             if (schedules[i].assigned_room_id != -1) assigned_schedules++;
//         }
//     }
    
//     printf("        <div style=\"display: flex; gap: 20px; margin-top: 20px;\">\n");
//     printf("            <div style=\"flex: 1; background: #e8f4fc; padding: 15px; border-radius: 5px;\">\n");
//     printf("                <h3>Rooms</h3>\n");
//     printf("                <p style=\"font-size: 24px; font-weight: bold;\">%d</p>\n", active_rooms);
//     printf("                <p>Active rooms available</p>\n");
//     printf("            </div>\n");
//     printf("            <div style=\"flex: 1; background: #e8f8f1; padding: 15px; border-radius: 5px;\">\n");
//     printf("                <h3>Schedules</h3>\n");
//     printf("                <p style=\"font-size: 24px; font-weight: bold;\">%d</p>\n", active_schedules);
//     printf("                <p>Active schedules</p>\n");
//     printf("            </div>\n");
//     printf("            <div style=\"flex: 1; background: #fef9e7; padding: 15px; border-radius: 5px;\">\n");
//     printf("                <h3>Assigned</h3>\n");
//     printf("                <p style=\"font-size: 24px; font-weight: bold;\">%d</p>\n", assigned_schedules);
//     printf("                <p>Schedules assigned to rooms</p>\n");
//     printf("            </div>\n");
//     printf("        </div>\n");
//     printf("    </div>\n");
// }

// void handle_rooms_page() {
//     printf("    <div class=\"card\">\n");
//     printf("        <h2>Room List</h2>\n");
    
//     load_rooms();
    
//     if (room_count == 0) {
//         printf("        <p>No rooms available.</p>\n");
//     } else {
//         printf("        <table>\n");
//         printf("            <tr>\n");
//         printf("                <th>ID</th>\n");
//         printf("                <th>Name</th>\n");
//         printf("                <th>Type</th>\n");
//         printf("                <th>Actions</th>\n");
//         printf("            </tr>\n");
        
//         for (int i = 0; i < room_count; i++) {
//             if (rooms[i].is_active) {
//                 printf("            <tr>\n");
//                 printf("                <td>%d</td>\n", rooms[i].id);
//                 printf("                <td>%s</td>\n", rooms[i].name);
//                 printf("                <td>%s</td>\n", 
//                        rooms[i].type == CLASSROOM ? "Classroom" : "Laboratory");
//                 printf("                <td>\n");
//                 printf("                    <a href=\"?page=delete_room&id=%d\" style=\"background: #e74c3c; color: white; padding: 5px 10px; text-decoration: none; border-radius: 3px;\" onclick=\"return confirm('Are you sure you want to delete this room?')\">Delete</a>\n", rooms[i].id);
//                 printf("                </td>\n");
//                 printf("            </tr>\n");
//             }
//         }
//         printf("        </table>\n");
//     }
//     printf("    </div>\n");
// }

// void handle_add_room_form() {
//     char *method = getenv("REQUEST_METHOD");
    
//     if (method && strcmp(method, "POST") == 0) {
//         char *content_length_str = getenv("CONTENT_LENGTH");
//         int content_length = content_length_str ? atoi(content_length_str) : 0;
        
//         if (content_length > 0) {
//             char *post_data = malloc(content_length + 1);
//             fread(post_data, 1, content_length, stdin);
//             post_data[content_length] = '\0';
            
//             char room_name[MAX_NAME_LENGTH] = "";
//             char room_type[20] = "";
            
//             // Parse post data
//             char *token = strtok(post_data, "&");
//             while (token) {
//                 char *equals = strchr(token, '=');
//                 if (equals) {
//                     *equals = '\0';
                    
//                     if (strcmp(token, "name") == 0) {
//                         url_decode(equals + 1);
//                         strncpy(room_name, equals + 1, MAX_NAME_LENGTH - 1);
//                         room_name[MAX_NAME_LENGTH - 1] = '\0';
//                     } else if (strcmp(token, "type") == 0) {
//                         url_decode(equals + 1);
//                         strncpy(room_type, equals + 1, 19);
//                     }
//                 }
//                 token = strtok(NULL, "&");
//             }
            
//             if (strlen(room_name) > 0 && room_count < MAX_ROOMS) {
//                 Room new_room;
//                 new_room.id = next_room_id++;
//                 strcpy(new_room.name, room_name);
//                 new_room.type = (strcmp(room_type, "LAB") == 0) ? LAB : CLASSROOM;
//                 new_room.is_active = 1;
                
//                 rooms[room_count++] = new_room;
//                 save_rooms();
                
//                 printf("<div class=\"success\">✓ Room '%s' added successfully! (ID: %d)</div>\n", 
//                        new_room.name, new_room.id);
//             }
            
//             free(post_data);
//         }
//     }
// }

// void handle_add_schedule_form() {
//     char *method = getenv("REQUEST_METHOD");
    
//     if (method && strcmp(method, "POST") == 0) {
//         char *content_length_str = getenv("CONTENT_LENGTH");
//         int content_length = content_length_str ? atoi(content_length_str) : 0;
        
//         if (content_length > 0) {
//             char *post_data = malloc(content_length + 1);
//             fread(post_data, 1, content_length, stdin);
//             post_data[content_length] = '\0';
            
//             char course_name[MAX_NAME_LENGTH] = "";
//             char department[MAX_DEPT_LENGTH] = "";
//             char year_str[10] = "";
//             char program_str[20] = "";
//             char room_type_str[20] = "";
//             char start_time_str[10] = "";
//             char end_time_str[10] = "";
            
//             // Parse post data
//             char *token = strtok(post_data, "&");
//             while (token) {
//                 char *equals = strchr(token, '=');
//                 if (equals) {
//                     *equals = '\0';
//                     url_decode(equals + 1);
                    
//                     if (strcmp(token, "course") == 0) {
//                         strncpy(course_name, equals + 1, MAX_NAME_LENGTH - 1);
//                     } else if (strcmp(token, "dept") == 0) {
//                         strncpy(department, equals + 1, MAX_DEPT_LENGTH - 1);
//                     } else if (strcmp(token, "year") == 0) {
//                         strncpy(year_str, equals + 1, 9);
//                     } else if (strcmp(token, "program") == 0) {
//                         strncpy(program_str, equals + 1, 19);
//                     } else if (strcmp(token, "room_type") == 0) {
//                         strncpy(room_type_str, equals + 1, 19);
//                     } else if (strcmp(token, "start_time") == 0) {
//                         strncpy(start_time_str, equals + 1, 9);
//                     } else if (strcmp(token, "end_time") == 0) {
//                         strncpy(end_time_str, equals + 1, 9);
//                     }
//                 }
//                 token = strtok(NULL, "&");
//             }
            
//             if (strlen(course_name) > 0 && schedule_count < MAX_SCHEDULES) {
//                 Schedule new_schedule;
//                 new_schedule.id = next_schedule_id++;
//                 strcpy(new_schedule.course_name, course_name);
//                 strcpy(new_schedule.department, department);
//                 new_schedule.student_year = atoi(year_str);
//                 new_schedule.program_type = (strcmp(program_str, "MAJOR") == 0) ? MAJOR : MINOR;
//                 new_schedule.required_room_type = (strcmp(room_type_str, "LAB") == 0) ? LAB : CLASSROOM;
//                 new_schedule.start_time = parse_time_string(start_time_str);
//                 new_schedule.end_time = parse_time_string(end_time_str);
//                 new_schedule.assigned_room_id = -1;
//                 new_schedule.is_active = 1;
                
//                 // Calculate priority
//                 new_schedule.priority = (new_schedule.student_year * 10) +
//                                        (new_schedule.program_type == MAJOR ? 5 : 0) +
//                                        (new_schedule.required_room_type == LAB ? 3 : 0);
                
//                 schedules[schedule_count++] = new_schedule;
//                 save_schedules();
                
//                 printf("<div class=\"success\">✓ Schedule for '%s' added successfully! (ID: %d)</div>\n",
//                        new_schedule.course_name, new_schedule.id);
//             }
            
//             free(post_data);
//         }
//     }
// }

// int main() {
//     // Load initial data
//     load_rooms();
//     load_schedules();
    
//     // Determine requested page
//     char *query_string = getenv("QUERY_STRING");
//     char page[50] = "home";
    
//     if (query_string) {
//         parse_query_param(query_string, "page", page);
//     }
    
//     // Print HTML
//     print_html_header();
//     print_navigation();
    
//     // Handle page requests
//     if (strcmp(page, "home") == 0) {
//         handle_home_page();
//     } 
//     else if (strcmp(page, "rooms") == 0) {
//         handle_rooms_page();
//     }
//     else if (strcmp(page, "add_room") == 0) {
//         handle_add_room_form();
        
//         // Show add room form
//         printf("    <div class=\"card\">\n");
//         printf("        <h2>Add New Room</h2>\n");
//         printf("        <form method=\"POST\">\n");
//         printf("            <div class=\"form-group\">\n");
//         printf("                <label for=\"name\">Room Name:</label>\n");
//         printf("                <input type=\"text\" id=\"name\" name=\"name\" required>\n");
//         printf("            </div>\n");
//         printf("            <div class=\"form-group\">\n");
//         printf("                <label for=\"type\">Room Type:</label>\n");
//         printf("                <select id=\"type\" name=\"type\">\n");
//         printf("                    <option value=\"CLASSROOM\">Classroom</option>\n");
//         printf("                    <option value=\"LAB\">Laboratory</option>\n");
//         printf("                </select>\n");
//         printf("            </div>\n");
//         printf("            <button type=\"submit\" class=\"btn\">Add Room</button>\n");
//         printf("        </form>\n");
//         printf("    </div>\n");
//     }
//     else if (strcmp(page, "schedules") == 0) {
//         printf("    <div class=\"card\">\n");
//         printf("        <h2>All Schedules</h2>\n");
        
//         load_schedules();
//         load_rooms();
        
//         if (schedule_count == 0) {
//             printf("        <p>No schedules available.</p>\n");
//         } 
//         else {
//             printf("        <table>\n");
//             printf("            <tr>\n");
//             printf("                <th>ID</th>\n");
//             printf("                <th>Course</th>\n");
//             printf("                <th>Department</th>\n");
//             printf("                <th>Year</th>\n");
//             printf("                <th>Time</th>\n");
//             printf("                <th>Assigned Room</th>\n");
//             printf("                <th>Priority</th>\n");
//             printf("            </tr>\n");
            
//             for (int i = 0; i < schedule_count; i++) {
//                 if (schedules[i].is_active) {
//                     char start_time[10], end_time[10];
//                     format_time(schedules[i].start_time, start_time);
//                     format_time(schedules[i].end_time, end_time);
                    
//                     char assigned_room[30] = "Not assigned";
//                     if (schedules[i].assigned_room_id != -1) {
//                         for (int j = 0; j < room_count; j++) {
//                             if (rooms[j].id == schedules[i].assigned_room_id && rooms[j].is_active) {
//                                 strcpy(assigned_room, rooms[j].name);
//                                 break;
//                             }
//                         }
//                     }
                    
//                     printf("            <tr>\n");
//                     printf("                <td>%d</td>\n", schedules[i].id);
//                     printf("                <td>%s</td>\n", schedules[i].course_name);
//                     printf("                <td>%s</td>\n", schedules[i].department);
//                     printf("                <td>%d</td>\n", schedules[i].student_year);
//                     printf("                <td>%s - %s</td>\n", start_time, end_time);
//                     printf("                <td>%s</td>\n", assigned_room);
//                     printf("                <td>%d</td>\n", schedules[i].priority);
//                     printf("            </tr>\n");
//                 }
//             }
//             printf("        </table>\n");
//         }
//         printf("    </div>\n");
//     }
//     else if (strcmp(page, "add_schedule") == 0) {
//         handle_add_schedule_form();
        
//         // Show add schedule form
//         printf("    <div class=\"card\">\n");
//         printf("        <h2>Add New Schedule</h2>\n");
//         printf("        <form method=\"POST\">\n");
//         printf("            <div class=\"form-group\">\n");
//         printf("                <label for=\"course\">Course Name:</label>\n");
//         printf("                <input type=\"text\" id=\"course\" name=\"course\" required>\n");
//         printf("            </div>\n");
//         printf("            <div class=\"form-group\">\n");
//         printf("                <label for=\"dept\">Department:</label>\n");
//         printf("                <input type=\"text\" id=\"dept\" name=\"dept\" required>\n");
//         printf("            </div>\n");
//         printf("            <div class=\"form-group\">\n");
//         printf("                <label for=\"year\">Student Year:</label>\n");
//         printf("                <select id=\"year\" name=\"year\">\n");
//         printf("                    <option value=\"1\">1</option>\n");
//         printf("                    <option value=\"2\">2</option>\n");
//         printf("                    <option value=\"3\">3</option>\n");
//         printf("                    <option value=\"4\">4</option>\n");
//         printf("                    <option value=\"5\">5</option>\n");
//         printf("                </select>\n");
//         printf("            </div>\n");
//         printf("            <div class=\"form-group\">\n");
//         printf("                <label for=\"program\">Program Type:</label>\n");
//         printf("                <select id=\"program\" name=\"program\">\n");
//         printf("                    <option value=\"MAJOR\">Major</option>\n");
//         printf("                    <option value=\"MINOR\">Minor</option>\n");
//                 printf("                </select>\n");
//         printf("            </div>\n");
//         printf("            <div class=\"form-group\">\n");
//         printf("                <label for=\"room_type\">Required Room Type:</label>\n");
//         printf("                <select id=\"room_type\" name=\"room_type\">\n");
//         printf("                    <option value=\"CLASSROOM\">Classroom</option>\n");
//         printf("                    <option value=\"LAB\">Laboratory</option>\n");
//         printf("                </select>\n");
//         printf("            </div>\n");
//         printf("            <div class=\"form-group\">\n");
//         printf("                <label for=\"start_time\">Start Time (HH:MM):</label>\n");
//         printf("                <input type=\"text\" id=\"start_time\" name=\"start_time\" placeholder=\"09:00\" required>\n");
//         printf("            </div>\n");
//         printf("            <div class=\"form-group\">\n");
//         printf("                <label for=\"end_time\">End Time (HH:MM):</label>\n");
//         printf("                <input type=\"text\" id=\"end_time\" name=\"end_time\" placeholder=\"10:30\" required>\n");
//         printf("            </div>\n");
//         printf("            <button type=\"submit\" class=\"btn\">Add Schedule</button>\n");
//         printf("        </form>\n");
//         printf("    </div>\n");
//     }
//     else if (strcmp(page, "generate") == 0) {
//         printf("    <div class=\"card\">\n");
//         printf("        <h2>Generate Final Schedule</h2>\n");
//         generate_final_schedule();
//         printf("    </div>\n");
//     }
//     else if (strcmp(page, "final") == 0) {
//         printf("    <div class=\"card\">\n");
//         printf("        <h2>Final Schedule</h2>\n");
        
//         load_schedules();
//         load_rooms();
        
//         int has_assigned = 0;
        
//         for (int i = 0; i < room_count; i++) {
//             if (!rooms[i].is_active) continue;
            
//             int has_schedule_for_room = 0;
//             for (int j = 0; j < schedule_count; j++) {
//                 if (schedules[j].is_active && schedules[j].assigned_room_id == rooms[i].id) {
//                     has_schedule_for_room = 1;
//                     break;
//                 }
//             }
            
//             if (!has_schedule_for_room) continue;
            
//             printf("        <h3>%s (%s)</h3>\n", 
//                    rooms[i].name, 
//                    get_room_type_string(rooms[i].type));
//             printf("        <table>\n");
//             printf("            <tr>\n");
//             printf("                <th>Course</th>\n");
//             printf("                <th>Department</th>\n");
//             printf("                <th>Year</th>\n");
//             printf("                <th>Time</th>\n");
//             printf("                <th>Priority</th>\n");
//             printf("            </tr>\n");
            
//             for (int j = 0; j < schedule_count; j++) {
//                 if (schedules[j].is_active && schedules[j].assigned_room_id == rooms[i].id) {
//                     char start_time[10], end_time[10];
//                     format_time(schedules[j].start_time, start_time);
//                     format_time(schedules[j].end_time, end_time);
                    
//                     char time_range[25];
//                     sprintf(time_range, "%s - %s", start_time, end_time);
                    
//                     printf("            <tr>\n");
//                     printf("                <td>%s</td>\n", schedules[j].course_name);
//                     printf("                <td>%s</td>\n", schedules[j].department);
//                     printf("                <td>%d</td>\n", schedules[j].student_year);
//                     printf("                <td>%s</td>\n", time_range);
//                     printf("                <td>%d</td>\n", schedules[j].priority);
//                     printf("            </tr>\n");
//                     has_assigned = 1;
//                 }
//             }
//             printf("        </table><br>\n");
//         }
        
//         if (!has_assigned) {
//             printf("        <p>No schedules have been assigned yet.</p>\n");
//             printf("        <p>Please add schedules and run 'Generate Final Schedule' first.</p>\n");
//         }
//         printf("    </div>\n");
//     }
//     else if (strcmp(page, "delete_room") == 0) {
//         char room_id_str[10];
//         parse_query_param(query_string, "id", room_id_str);
//         int room_id = atoi(room_id_str);
        
//         // Find and delete room
//         for (int i = 0; i < room_count; i++) {
//             if (rooms[i].id == room_id && rooms[i].is_active) {
//                 rooms[i].is_active = 0;
//                 save_rooms();
                
//                 // Remove room assignments from schedules
//                 for (int j = 0; j < schedule_count; j++) {
//                     if (schedules[j].assigned_room_id == room_id) {
//                         schedules[j].assigned_room_id = -1;
//                     }
//                 }
//                 save_schedules();
                
//                 printf("    <div class=\"success\">✓ Room deleted successfully!</div>\n");
//                 break;
//             }
//         }
        
//         handle_rooms_page();
//     }
//     else {
//         handle_home_page();
//     }
    
//     print_html_footer();
//     return 0;
// }

// // this is good but i want you to add ths functionlaity
// void view_schedule_by_room() {
//     clear_screen();
//     printf("\n╔════════════════════════════════════════════════════════════╗\n");
//     printf("║                   SCHEDULE BY ROOM                         ║\n");
//     printf("╚════════════════════════════════════════════════════════════╝\n\n");

//     int room_id;
//     printf("Enter room ID: ");
//     scanf("%d", &room_id);

//     int found = -1;
//     for (int i = 0; i < room_count; i++) {
//         if (rooms[i].id == room_id && rooms[i].is_active) {
//             found = i;
//             break;
//         }
//     }

//     if (found == -1) {
//         printf("\n✗ Room not found!\n");
//         pause_screen();
//         return;
//     }

//     printf("\nRoom: %s (%s)\n", rooms[found].name, get_room_type_string(rooms[found].type));
//     printf("════════════════════════════════════════════════════════════════════════\n\n");

//     printf("%-25s %-15s %-6s %-10s %-15s\n",
//            "Course", "Dept", "Year", "Program", "Time");
//     printf("────────────────────────────────────────────────────────────────────────\n");

//     int has_schedule = 0;
//     for (int i = 0; i < schedule_count; i++) {
//         if (schedules[i].is_active && schedules[i].assigned_room_id == room_id) {
//             char start_time[10], end_time[10];
//             format_time(schedules[i].start_time, start_time);
//             format_time(schedules[i].end_time, end_time);

//             char time_range[25];
//             sprintf(time_range, "%s-%s", start_time, end_time);

//             printf("%-25s %-15s %-6d %-10s %-15s\n",
//                    schedules[i].course_name,
//                    schedules[i].department,
//                    schedules[i].student_year,
//                    get_program_type_string(schedules[i].program_type),
//                    time_range);
//             has_schedule = 1;
//         }
//     }

//     if (!has_schedule) {
//         printf("No schedules assigned to this room.\n");
//     }

//     pause_screen();
// }

// void view_schedule_by_department() {
//     clear_screen();
//     printf("\n╔════════════════════════════════════════════════════════════╗\n");
//     printf("║                 SCHEDULE BY DEPARTMENT                     ║\n");
//     printf("╚════════════════════════════════════════════════════════════╝\n\n");

//     char dept[MAX_DEPT_LENGTH];
//     printf("Enter department name: ");
//     clear_input_buffer();
//     fgets(dept, MAX_DEPT_LENGTH, stdin);
//     dept[strcspn(dept, "\n")] = 0;

//     printf("\nDepartment: %s\n", dept);
//     printf("════════════════════════════════════════════════════════════════════════════════════\n\n");

//     printf("%-25s %-6s %-10s %-12s %-15s %-20s\n",
//            "Course", "Year", "Program", "Room Type", "Time", "Assigned Room");
//     printf("────────────────────────────────────────────────────────────────────────────────────\n");

//     int has_schedule = 0;
//     for (int i = 0; i < schedule_count; i++) {
//         if (schedules[i].is_active && case_insensitive_compare(schedules[i].department, dept) == 0) {
//             char start_time[10], end_time[10];
//             format_time(schedules[i].start_time, start_time);
//             format_time(schedules[i].end_time, end_time);

//             char time_range[25];
//             sprintf(time_range, "%s-%s", start_time, end_time);

//             char assigned_room[30] = "Not assigned";
//             if (schedules[i].assigned_room_id != -1) {
//                 for (int j = 0; j < room_count; j++) {
//                     if (rooms[j].id == schedules[i].assigned_room_id && rooms[j].is_active) {
//                         strcpy(assigned_room, rooms[j].name);
//                         break;
//                     }
//                 }
//             }

//             printf("%-25s %-6d %-10s %-12s %-15s %-20s\n",
//                    schedules[i].course_name,
//                    schedules[i].student_year,
//                    get_program_type_string(schedules[i].program_type),
//                    get_room_type_string(schedules[i].required_room_type),
//                    time_range,
//                    assigned_room);
//             has_schedule = 1;
//         }
//     }

//     if (!has_schedule) {
//         printf("No schedules found for this department.\n");
//     }

//     pause_screen();
// }
#include "scheduling_system.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Use writable directories
#define ROOMS_FILE "/tmp/rooms.dat"
#define SCHEDULES_FILE "/tmp/schedules.dat"

// Custom strdup implementation for C99
char* my_strdup(const char* s) {
    if (s == NULL) return NULL;
    size_t len = strlen(s) + 1;
    char* new = malloc(len);
    if (new == NULL) return NULL;
    memcpy(new, s, len);
    return new;
}
// Add this helper function for case-insensitive string comparison
int case_insensitive_compare(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        if (tolower((unsigned char)*s1) != tolower((unsigned char)*s2)) {
            return (tolower((unsigned char)*s1) - tolower((unsigned char)*s2));
        }
        s1++;
        s2++;
    }
    return (tolower((unsigned char)*s1) - tolower((unsigned char)*s2));
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
    printf("        .container { background: white; padding: 20px; border-radius: 10px; box-shadow: 0 0 10px rgba(0,0,0,0.1); }\n");
    printf("        header { background: #2c3e50; color: white; padding: 20px; border-radius: 5px; margin-bottom: 20px; }\n");
    printf("        nav { background: #34495e; padding: 10px; border-radius: 5px; margin-bottom: 20px; }\n");
    printf("        nav a { color: white; text-decoration: none; padding: 10px 15px; margin: 0 5px; border-radius: 3px; }\n");
    printf("        nav a:hover { background: #1abc9c; }\n");
    printf("        .card { background: white; border: 1px solid #ddd; border-radius: 5px; padding: 15px; margin-bottom: 15px; }\n");
    printf("        table { width: 100%%; border-collapse: collapse; margin-bottom: 20px; }\n");
    printf("        th, td { border: 1px solid #ddd; padding: 8px; text-align: left; }\n");
    printf("        th { background-color: #3498db; color: white; }\n");
    printf("        tr:nth-child(even) { background-color: #f2f2f2; }\n");
    printf("        .btn { background: #3498db; color: white; padding: 10px 15px; border: none; border-radius: 3px; cursor: pointer; }\n");
    printf("        .btn:hover { background: #2980b9; }\n");
    printf("        .form-group { margin-bottom: 15px; }\n");
    printf("        label { display: block; margin-bottom: 5px; font-weight: bold; }\n");
    printf("        input, select, textarea { width: 100%%; padding: 8px; border: 1px solid #ddd; border-radius: 3px; box-sizing: border-box; }\n");
    printf("        .success { color: #27ae60; background: #d5f4e6; padding: 10px; border-radius: 3px; margin: 10px 0; }\n");
    printf("        .error { color: #c0392b; background: #fadbd8; padding: 10px; border-radius: 3px; margin: 10px 0; }\n");
    printf("        .info { color: #2980b9; background: #d6eaf8; padding: 10px; border-radius: 3px; margin: 10px 0; }\n");
    printf("        .filter-form { background: #f8f9fa; padding: 15px; border-radius: 5px; margin: 15px 0; }\n");
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
    printf("        <a href=\"?page=view_by_room\">View by Room</a>\n");
    printf("        <a href=\"?page=view_by_dept\">View by Department</a>\n");
    printf("        <a href=\"?page=generate\">Generate Schedule</a>\n");
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

void parse_query_param(const char *query, const char *key, char *value) {
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

int parse_time_string(const char *time_str) {
    int hour, minute;
    if (sscanf(time_str, "%d:%d", &hour, &minute) == 2) {
        if (hour >= 0 && hour < 24 && minute >= 0 && minute < 60) {
            return hour * 60 + minute;
        }
    }
    return -1;
}

void generate_final_schedule() {
    // Sort schedules by priority (highest first)
    Schedule active_schedules[MAX_SCHEDULES];
    int active_count = 0;
    
    for (int i = 0; i < schedule_count; i++) {
        if (schedules[i].is_active && schedules[i].assigned_room_id == -1) {
            active_schedules[active_count++] = schedules[i];
        }
    }
    
    // Sort by priority
    for (int i = 0; i < active_count - 1; i++) {
        for (int j = i + 1; j < active_count; j++) {
            if (active_schedules[j].priority > active_schedules[i].priority) {
                Schedule temp = active_schedules[i];
                active_schedules[i] = active_schedules[j];
                active_schedules[j] = temp;
            }
        }
    }
    
    int assigned_count = 0;
    
    // Try to assign each schedule
    for (int i = 0; i < active_count; i++) {
        int assigned = 0;
        
        // Find suitable room
        for (int j = 0; j < room_count && !assigned; j++) {
            if (!rooms[j].is_active) continue;
            if (rooms[j].type != active_schedules[i].required_room_type) continue;
            
            // Check for conflicts
            int conflict = 0;
            for (int k = 0; k < schedule_count; k++) {
                if (schedules[k].is_active && 
                    schedules[k].assigned_room_id == rooms[j].id) {
                    
                    // Check time overlap
                    int overlap = !(active_schedules[i].end_time <= schedules[k].start_time ||
                                   active_schedules[i].start_time >= schedules[k].end_time);
                    if (overlap) {
                        conflict = 1;
                        break;
                    }
                }
            }
            
            if (!conflict) {
                // Assign this room
                for (int k = 0; k < schedule_count; k++) {
                    if (schedules[k].id == active_schedules[i].id) {
                        schedules[k].assigned_room_id = rooms[j].id;
                        assigned_count++;
                        assigned = 1;
                        break;
                    }
                }
            }
        }
    }
    
    save_schedules();
    
    printf("<div class=\"success\">");
    printf("Schedule generation completed!<br>");
    printf("Assigned %d out of %d active schedules.", assigned_count, active_count);
    printf("</div>");
}

// ========== NEW: View Schedule by Room (Web Version) ==========
void handle_view_by_room() {
    printf("    <div class=\"card\">\n");
    printf("        <h2>View Schedule by Room</h2>\n");
    
    load_rooms();
    load_schedules();
    
    char *query_string = getenv("QUERY_STRING");
    char room_id_str[10] = "";
    
    // Check if room_id is provided in query string
    if (query_string) {
        parse_query_param(query_string, "room_id", room_id_str);
    }
    
    // Show room selection form
    printf("        <div class=\"filter-form\">\n");
    printf("            <form method=\"GET\">\n");
    printf("                <input type=\"hidden\" name=\"page\" value=\"view_by_room\">\n");
    printf("                <div class=\"form-group\">\n");
    printf("                    <label for=\"room_id\">Select Room:</label>\n");
    printf("                    <select id=\"room_id\" name=\"room_id\" onchange=\"this.form.submit()\">\n");
    printf("                        <option value=\"\">-- Select a Room --</option>\n");
    
    // Populate room dropdown
    for (int i = 0; i < room_count; i++) {
        if (rooms[i].is_active) {
            printf("                        <option value=\"%d\" %s>%s (%s)</option>\n",
                   rooms[i].id,
                   (atoi(room_id_str) == rooms[i].id) ? "selected" : "",
                   rooms[i].name,
                   get_room_type_string(rooms[i].type));
        }
    }
    
    printf("                    </select>\n");
    printf("                </div>\n");
    printf("            </form>\n");
    printf("        </div>\n");
    
    // If room is selected, show its schedule
    if (strlen(room_id_str) > 0) {
        int room_id = atoi(room_id_str);
        int found = -1;
        
        // Find the room
        for (int i = 0; i < room_count; i++) {
            if (rooms[i].id == room_id && rooms[i].is_active) {
                found = i;
                break;
            }
        }
        
        if (found == -1) {
            printf("<div class=\"error\">✗ Room not found!</div>\n");
        } else {
            printf("        <h3>%s - %s (%s)</h3>\n", 
                   rooms[found].name, 
                   get_room_type_string(rooms[found].type),
                   rooms[found].type == CLASSROOM ? "Classroom" : "Laboratory");
            
            int has_schedule = 0;
            
            // Check if room has any schedules
            for (int i = 0; i < schedule_count; i++) {
                if (schedules[i].is_active && schedules[i].assigned_room_id == room_id) {
                    has_schedule = 1;
                    break;
                }
            }
            
            if (!has_schedule) {
                printf("<div class=\"info\">No schedules assigned to this room.</div>\n");
            } else {
                printf("        <table>\n");
                printf("            <tr>\n");
                printf("                <th>Course</th>\n");
                printf("                <th>Department</th>\n");
                printf("                <th>Year</th>\n");
                printf("                <th>Program</th>\n");
                printf("                <th>Time</th>\n");
                printf("                <th>Priority</th>\n");
                printf("            </tr>\n");
                
                for (int i = 0; i < schedule_count; i++) {
                    if (schedules[i].is_active && schedules[i].assigned_room_id == room_id) {
                        char start_time[10], end_time[10];
                        format_time(schedules[i].start_time, start_time);
                        format_time(schedules[i].end_time, end_time);
                        
                        char time_range[25];
                        sprintf(time_range, "%s - %s", start_time, end_time);
                        
                        printf("            <tr>\n");
                        printf("                <td>%s</td>\n", schedules[i].course_name);
                        printf("                <td>%s</td>\n", schedules[i].department);
                        printf("                <td>%d</td>\n", schedules[i].student_year);
                        printf("                <td>%s</td>\n", get_program_type_string(schedules[i].program_type));
                        printf("                <td>%s</td>\n", time_range);
                        printf("                <td>%d</td>\n", schedules[i].priority);
                        printf("            </tr>\n");
                    }
                }
                printf("        </table>\n");
            }
        }
    }
    printf("    </div>\n");
}

// ========== NEW: View Schedule by Department (Web Version) ==========
void handle_view_by_department() {
    printf("    <div class=\"card\">\n");
    printf("        <h2>View Schedule by Department</h2>\n");
    
    load_schedules();
    load_rooms();
    
    char *query_string = getenv("QUERY_STRING");
    char dept[100] = "";
    
    // Check if department is provided in query string
    if (query_string) {
        parse_query_param(query_string, "dept", dept);
    }
    
    // Get unique departments for dropdown
    char departments[MAX_SCHEDULES][MAX_DEPT_LENGTH];
    int dept_count = 0;
    
    for (int i = 0; i < schedule_count; i++) {
        if (schedules[i].is_active) {
            int found = 0;
            for (int j = 0; j < dept_count; j++) {
                if (case_insensitive_compare(departments[j], schedules[i].department) == 0) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                strcpy(departments[dept_count++], schedules[i].department);
            }
        }
    }
    
    // Show department selection form
    printf("        <div class=\"filter-form\">\n");
    printf("            <form method=\"GET\">\n");
    printf("                <input type=\"hidden\" name=\"page\" value=\"view_by_dept\">\n");
    printf("                <div class=\"form-group\">\n");
    printf("                    <label for=\"dept\">Select Department:</label>\n");
    printf("                    <select id=\"dept\" name=\"dept\" onchange=\"this.form.submit()\">\n");
    printf("                        <option value=\"\">-- Select a Department --</option>\n");
    
    // Populate department dropdown
    for (int i = 0; i < dept_count; i++) {
        printf("                        <option value=\"%s\" %s>%s</option>\n",
               departments[i],
               (case_insensitive_compare(dept, departments[i]) == 0) ? "selected" : "",
               departments[i]);
    }
    
    printf("                    </select>\n");
    printf("                </div>\n");
    printf("            </form>\n");
    printf("        </div>\n");
    
    // If department is selected, show its schedules
    if (strlen(dept) > 0) {
        printf("        <h3>Department: %s</h3>\n", dept);
        
        int has_schedule = 0;
        
        // Check if department has any schedules
        for (int i = 0; i < schedule_count; i++) {
            if (schedules[i].is_active && case_insensitive_compare(schedules[i].department, dept) == 0) {
                has_schedule = 1;
                break;
            }
        }
        
        if (!has_schedule) {
            printf("<div class=\"info\">No schedules found for this department.</div>\n");
        } else {
            printf("        <table>\n");
            printf("            <tr>\n");
            printf("                <th>Course</th>\n");
            printf("                <th>Year</th>\n");
            printf("                <th>Program</th>\n");
            printf("                <th>Room Type</th>\n");
            printf("                <th>Time</th>\n");
            printf("                <th>Assigned Room</th>\n");
            printf("                <th>Status</th>\n");
            printf("            </tr>\n");
            
            for (int i = 0; i < schedule_count; i++) {
                if (schedules[i].is_active && case_insensitive_compare(schedules[i].department, dept) == 0) {
                    char start_time[10], end_time[10];
                    format_time(schedules[i].start_time, start_time);
                    format_time(schedules[i].end_time, end_time);
                    
                    char time_range[25];
                    sprintf(time_range, "%s - %s", start_time, end_time);
                    
                    char assigned_room[30] = "Not assigned";
                    char status[20] = "Unassigned";
                    if (schedules[i].assigned_room_id != -1) {
                        for (int j = 0; j < room_count; j++) {
                            if (rooms[j].id == schedules[i].assigned_room_id && rooms[j].is_active) {
                                strcpy(assigned_room, rooms[j].name);
                                strcpy(status, "Assigned");
                                break;
                            }
                        }
                    }
                    
                    printf("            <tr>\n");
                    printf("                <td>%s</td>\n", schedules[i].course_name);
                    printf("                <td>%d</td>\n", schedules[i].student_year);
                    printf("                <td>%s</td>\n", get_program_type_string(schedules[i].program_type));
                    printf("                <td>%s</td>\n", get_room_type_string(schedules[i].required_room_type));
                    printf("                <td>%s</td>\n", time_range);
                    printf("                <td>%s</td>\n", assigned_room);
                    printf("                <td><span style=\"color: %s;\">%s</span></td>\n",
                           (schedules[i].assigned_room_id != -1) ? "#27ae60" : "#e74c3c",
                           status);
                    printf("            </tr>\n");
                }
            }
            printf("        </table>\n");
            
            // Show department statistics
            int total = 0;
            int assigned = 0;
            for (int i = 0; i < schedule_count; i++) {
                if (schedules[i].is_active && case_insensitive_compare(schedules[i].department, dept) == 0) {
                    total++;
                    if (schedules[i].assigned_room_id != -1) assigned++;
                }
            }
            
            printf("        <div style=\"margin-top: 20px; padding: 10px; background: #f8f9fa; border-radius: 5px;\">\n");
            printf("            <h4>Department Statistics</h4>\n");
            printf("            <p>Total Schedules: %d</p>\n", total);
            printf("            <p>Assigned Schedules: %d (%.1f%%)</p>\n", assigned, total > 0 ? (assigned * 100.0 / total) : 0);
            printf("            <p>Unassigned Schedules: %d (%.1f%%)</p>\n", total - assigned, total > 0 ? ((total - assigned) * 100.0 / total) : 0);
            printf("        </div>\n");
        }
    }
    printf("    </div>\n");
}

void handle_home_page() {
    printf("    <div class=\"card\">\n");
    printf("        <h2>Welcome to Scheduling System</h2>\n");
    printf("        <p>This system helps manage classroom and laboratory schedules at Bahir Dar University.</p>\n");
    
    load_rooms();
    load_schedules();
    
    int active_rooms = 0;
    int active_schedules = 0;
    int assigned_schedules = 0;
    
    for (int i = 0; i < room_count; i++) {
        if (rooms[i].is_active) active_rooms++;
    }
    
    for (int i = 0; i < schedule_count; i++) {
        if (schedules[i].is_active) {
            active_schedules++;
            if (schedules[i].assigned_room_id != -1) assigned_schedules++;
        }
    }
    
    printf("        <div style=\"display: flex; gap: 20px; margin-top: 20px;\">\n");
    printf("            <div style=\"flex: 1; background: #e8f4fc; padding: 15px; border-radius: 5px;\">\n");
    printf("                <h3>Rooms</h3>\n");
    printf("                <p style=\"font-size: 24px; font-weight: bold;\">%d</p>\n", active_rooms);
    printf("                <p>Active rooms available</p>\n");
    printf("            </div>\n");
    printf("            <div style=\"flex: 1; background: #e8f8f1; padding: 15px; border-radius: 5px;\">\n");
    printf("                <h3>Schedules</h3>\n");
    printf("                <p style=\"font-size: 24px; font-weight: bold;\">%d</p>\n", active_schedules);
    printf("                <p>Active schedules</p>\n");
    printf("            </div>\n");
    printf("            <div style=\"flex: 1; background: #fef9e7; padding: 15px; border-radius: 5px;\">\n");
    printf("                <h3>Assigned</h3>\n");
    printf("                <p style=\"font-size: 24px; font-weight: bold;\">%d</p>\n", assigned_schedules);
    printf("                <p>Schedules assigned to rooms</p>\n");
    printf("            </div>\n");
    printf("        </div>\n");
    
    // Quick links to new features
    printf("        <div style=\"margin-top: 30px; padding: 20px; background: #f8f9fa; border-radius: 5px;\">\n");
    printf("            <h3>Quick Access</h3>\n");
    printf("            <div style=\"display: flex; gap: 15px; margin-top: 15px;\">\n");
    printf("                <a href=\"?page=view_by_room\" style=\"background: #3498db; color: white; padding: 10px 20px; text-decoration: none; border-radius: 5px; display: inline-block;\">View by Room</a>\n");
    printf("                <a href=\"?page=view_by_dept\" style=\"background: #2ecc71; color: white; padding: 10px 20px; text-decoration: none; border-radius: 5px; display: inline-block;\">View by Department</a>\n");
    printf("                <a href=\"?page=generate\" style=\"background: #e74c3c; color: white; padding: 10px 20px; text-decoration: none; border-radius: 5px; display: inline-block;\">Generate Schedule</a>\n");
    printf("            </div>\n");
    printf("        </div>\n");
    
    printf("    </div>\n");
}

void handle_rooms_page() {
    printf("    <div class=\"card\">\n");
    printf("        <h2>Room List</h2>\n");
    
    load_rooms();
    
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
                printf("                    <a href=\"?page=view_by_room&room_id=%d\" style=\"background: #3498db; color: white; padding: 5px 10px; text-decoration: none; border-radius: 3px; margin-right: 5px;\">View Schedule</a>\n", rooms[i].id);
                printf("                    <a href=\"?page=delete_room&id=%d\" style=\"background: #e74c3c; color: white; padding: 5px 10px; text-decoration: none; border-radius: 3px;\" onclick=\"return confirm('Are you sure you want to delete this room?')\">Delete</a>\n", rooms[i].id);
                printf("                </td>\n");
                printf("            </tr>\n");
            }
        }
        printf("        </table>\n");
    }
    printf("    </div>\n");
}

void handle_add_room_form() {
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
            
            // Parse post data
            char *token = strtok(post_data, "&");
            while (token) {
                char *equals = strchr(token, '=');
                if (equals) {
                    *equals = '\0';
                    
                    if (strcmp(token, "name") == 0) {
                        url_decode(equals + 1);
                        strncpy(room_name, equals + 1, MAX_NAME_LENGTH - 1);
                        room_name[MAX_NAME_LENGTH - 1] = '\0';
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
                
                printf("<div class=\"success\">✓ Room '%s' added successfully! (ID: %d)</div>\n", 
                       new_room.name, new_room.id);
            }
            
            free(post_data);
        }
    }
}

void handle_add_schedule_form() {
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
            char start_time_str[10] = "";
            char end_time_str[10] = "";
            
            // Parse post data
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
                    } else if (strcmp(token, "start_time") == 0) {
                        strncpy(start_time_str, equals + 1, 9);
                    } else if (strcmp(token, "end_time") == 0) {
                        strncpy(end_time_str, equals + 1, 9);
                    }
                }
                token = strtok(NULL, "&");
            }
            
            if (strlen(course_name) > 0 && schedule_count < MAX_SCHEDULES) {
                Schedule new_schedule;
                new_schedule.id = next_schedule_id++;
                strcpy(new_schedule.course_name, course_name);
                strcpy(new_schedule.department, department);
                new_schedule.student_year = atoi(year_str);
                new_schedule.program_type = (strcmp(program_str, "MAJOR") == 0) ? MAJOR : MINOR;
                new_schedule.required_room_type = (strcmp(room_type_str, "LAB") == 0) ? LAB : CLASSROOM;
                new_schedule.start_time = parse_time_string(start_time_str);
                new_schedule.end_time = parse_time_string(end_time_str);
                new_schedule.assigned_room_id = -1;
                new_schedule.is_active = 1;
                
                // Calculate priority
                new_schedule.priority = (new_schedule.student_year * 10) +
                                       (new_schedule.program_type == MAJOR ? 5 : 0) +
                                       (new_schedule.required_room_type == LAB ? 3 : 0);
                
                schedules[schedule_count++] = new_schedule;
                save_schedules();
                
                printf("<div class=\"success\">✓ Schedule for '%s' added successfully! (ID: %d)</div>\n",
                       new_schedule.course_name, new_schedule.id);
            }
            
            free(post_data);
        }
    }
}

int main() {
    // Load initial data
    load_rooms();
    load_schedules();
    
    // Determine requested page
    char *query_string = getenv("QUERY_STRING");
    char page[50] = "home";
    
    if (query_string) {
        parse_query_param(query_string, "page", page);
    }
    
    // Print HTML
    print_html_header();
    print_navigation();
    
    // Handle page requests
    if (strcmp(page, "home") == 0) {
        handle_home_page();
    } 
    else if (strcmp(page, "rooms") == 0) {
        handle_rooms_page();
    }
    else if (strcmp(page, "add_room") == 0) {
        handle_add_room_form();
        
        // Show add room form
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
    else if (strcmp(page, "schedules") == 0) {
        printf("    <div class=\"card\">\n");
        printf("        <h2>All Schedules</h2>\n");
        
        load_schedules();
        load_rooms();
        
        if (schedule_count == 0) {
            printf("        <p>No schedules available.</p>\n");
        } 
        else {
            printf("        <table>\n");
            printf("            <tr>\n");
            printf("                <th>ID</th>\n");
            printf("                <th>Course</th>\n");
            printf("                <th>Department</th>\n");
            printf("                <th>Year</th>\n");
            printf("                <th>Time</th>\n");
            printf("                <th>Assigned Room</th>\n");
            printf("                <th>Priority</th>\n");
            printf("            </tr>\n");
            
            for (int i = 0; i < schedule_count; i++) {
                if (schedules[i].is_active) {
                    char start_time[10], end_time[10];
                    format_time(schedules[i].start_time, start_time);
                    format_time(schedules[i].end_time, end_time);
                    
                    char assigned_room[30] = "Not assigned";
                    if (schedules[i].assigned_room_id != -1) {
                        for (int j = 0; j < room_count; j++) {
                            if (rooms[j].id == schedules[i].assigned_room_id && rooms[j].is_active) {
                                strcpy(assigned_room, rooms[j].name);
                                break;
                            }
                        }
                    }
                    
                    printf("            <tr>\n");
                    printf("                <td>%d</td>\n", schedules[i].id);
                    printf("                <td>%s</td>\n", schedules[i].course_name);
                    printf("                <td>%s</td>\n", schedules[i].department);
                    printf("                <td>%d</td>\n", schedules[i].student_year);
                    printf("                <td>%s - %s</td>\n", start_time, end_time);
                    printf("                <td>%s</td>\n", assigned_room);
                    printf("                <td>%d</td>\n", schedules[i].priority);
                    printf("            </tr>\n");
                }
            }
            printf("        </table>\n");
        }
        printf("    </div>\n");
    }
    else if (strcmp(page, "add_schedule") == 0) {
        handle_add_schedule_form();
        
        // Show add schedule form
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
        printf("                    <option value=\"1\">1</option>\n");
        printf("                    <option value=\"2\">2</option>\n");
        printf("                    <option value=\"3\">3</option>\n");
        printf("                    <option value=\"4\">4</option>\n");
        printf("                    <option value=\"5\">5</option>\n");
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
        printf("                <label for=\"start_time\">Start Time (HH:MM):</label>\n");
        printf("                <input type=\"text\" id=\"start_time\" name=\"start_time\" placeholder=\"09:00\" required>\n");
        printf("            </div>\n");
        printf("            <div class=\"form-group\">\n");
        printf("                <label for=\"end_time\">End Time (HH:MM):</label>\n");
        printf("                <input type=\"text\" id=\"end_time\" name=\"end_time\" placeholder=\"10:30\" required>\n");
        printf("            </div>\n");
        printf("            <button type=\"submit\" class=\"btn\">Add Schedule</button>\n");
        printf("        </form>\n");
        printf("    </div>\n");
    }
    else if (strcmp(page, "view_by_room") == 0) {
        handle_view_by_room();
    }
    else if (strcmp(page, "view_by_dept") == 0) {
        handle_view_by_department();
    }
    else if (strcmp(page, "generate") == 0) {
        printf("    <div class=\"card\">\n");
        printf("        <h2>Generate Final Schedule</h2>\n");
        generate_final_schedule();
        printf("    </div>\n");
    }
    else if (strcmp(page, "final") == 0) {
        printf("    <div class=\"card\">\n");
        printf("        <h2>Final Schedule</h2>\n");
        
        load_schedules();
        load_rooms();
        
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
            
            printf("        <h3>%s (%s)</h3>\n", 
                   rooms[i].name, 
                   get_room_type_string(rooms[i].type));
            printf("        <table>\n");
            printf("            <tr>\n");
            printf("                <th>Course</th>\n");
            printf("                <th>Department</th>\n");
            printf("                <th>Year</th>\n");
            printf("                <th>Time</th>\n");
            printf("                <th>Priority</th>\n");
            printf("            </tr>\n");
            
            for (int j = 0; j < schedule_count; j++) {
                if (schedules[j].is_active && schedules[j].assigned_room_id == rooms[i].id) {
                    char start_time[10], end_time[10];
                    format_time(schedules[j].start_time, start_time);
                    format_time(schedules[j].end_time, end_time);
                    
                    char time_range[25];
                    sprintf(time_range, "%s - %s", start_time, end_time);
                    
                    printf("            <tr>\n");
                    printf("                <td>%s</td>\n", schedules[j].course_name);
                    printf("                <td>%s</td>\n", schedules[j].department);
                    printf("                <td>%d</td>\n", schedules[j].student_year);
                    printf("                <td>%s</td>\n", time_range);
                    printf("                <td>%d</td>\n", schedules[j].priority);
                    printf("            </tr>\n");
                    has_assigned = 1;
                }
            }
            printf("        </table><br>\n");
        }
        
        if (!has_assigned) {
            printf("        <p>No schedules have been assigned yet.</p>\n");
            printf("        <p>Please add schedules and run 'Generate Final Schedule' first.</p>\n");
        }
        printf("    </div>\n");
    }
    else if (strcmp(page, "delete_room") == 0) {
        char room_id_str[10];
        parse_query_param(query_string, "id", room_id_str);
        int room_id = atoi(room_id_str);
        
        // Find and delete room
        for (int i = 0; i < room_count; i++) {
            if (rooms[i].id == room_id && rooms[i].is_active) {
                rooms[i].is_active = 0;
                save_rooms();
                
                // Remove room assignments from schedules
                for (int j = 0; j < schedule_count; j++) {
                    if (schedules[j].assigned_room_id == room_id) {
                        schedules[j].assigned_room_id = -1;
                    }
                }
                save_schedules();
                
                printf("    <div class=\"success\">✓ Room deleted successfully!</div>\n");
                break;
            }
        }
        
        handle_rooms_page();
    }
    else {
        handle_home_page();
    }
    
    print_html_footer();
    return 0;
}