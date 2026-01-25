# Classroom and Laboratory Scheduling System

## Presentation Notes

---

## 1. PROJECT OVERVIEW

### Problem Statement

Bahir Dar University faces challenges in efficiently managing classroom and laboratory allocations for various departments and courses. The manual scheduling process leads to:

- Time conflicts between classes
- Room utilization inefficiencies
- Priority mismatches for senior students
- Difficulty in tracking department-specific schedules

### Our Solution

A C-based CGI Web Application that automates room scheduling with intelligent conflict resolution and priority-based allocation.

---

## 2. SYSTEM ARCHITECTURE

### Core Components

1. Backend Engine (C Programming)
   - File-based persistent storage
   - Binary data management
   - Scheduling algorithms

2. Web Interface (CGI Implementation)
   - HTML/CSS frontend
   - HTTP request handling
   - Form processing

3. Data Model
   ```
   Rooms: ID, Name, Type (Classroom/Lab), Status
   Schedules: Course, Department, Year, Time, Priority, Assigned Room
   ```

### Technology Stack

- Programming Language: C99 Standard
- Web Server: Apache HTTPD with CGI module
- Operating System: Fedora Linux
- Data Storage: Binary files with persistent storage
- Protocol: HTTP/1.1 with GET/POST methods

---

## 3. KEY FEATURES IMPLEMENTED

### A. Room Management

- Add new rooms (Classrooms/Labs)
- View all available rooms
- Delete rooms (soft deletion)
- Room type categorization

### B. Schedule Management

- Add course schedules with:
  - Course name and department
  - Student year and program type
  - Time requirements
  - Priority calculation
- Edit existing schedules
- View all schedules

### C. Intelligent Scheduling Algorithm

```c
Priority Formula:
Priority = (Year × 10) + (Major:5/Minor:0) + (Lab:3/Classroom:0)

Assignment Logic:
1. Sort schedules by priority (highest first)
2. Find suitable room (type match)
3. Check for time conflicts
4. Assign if no conflict
5. Continue for all schedules
```

### D. Advanced Viewing Options

1. View by Room: See all schedules assigned to a specific room
2. View by Department: Filter schedules by department with statistics
3. Final Schedule: Comprehensive view of all assignments

### E. Web Interface Features

- Responsive HTML/CSS design
- Form validation and error handling
- Real-time feedback
- Navigation menu for all features
- Status indicators (assigned/unassigned)

---

## 4. IMPLEMENTATION DETAILS

### Data Persistence

```c
// Binary file storage
File Structure:
- Header: [count, next_id]
- Data: Array of structures

Files:
- ./data/rooms.dat
- ./data/schedules.dat

Benefits:
- Fast read/write operations
- Compact storage
- Type safety
```

### CGI Implementation

```c
// Key Functions:
1. Parse GET/POST requests
2. URL decode form data
3. Generate dynamic HTML
4. Handle file uploads/downloads

Environment Variables Used:
- QUERY_STRING
- REQUEST_METHOD
- CONTENT_LENGTH
```

### Conflict Resolution Algorithm

```
For each schedule (sorted by priority):
   For each suitable room:
      Check time overlap with existing assignments
      If no conflict → Assign room
      Else → Try next room
```

---

## 5. TECHNICAL CHALLENGES & SOLUTIONS

### Challenge 1: C99 Compatibility

- Problem: Missing POSIX functions (`strdup`, `strcasecmp`)
- Solution: Implemented custom versions
  ```c
  char my_strdup(const char s);
  int my_strcasecmp(const char s1, const char s2);
  ```

### Challenge 2: Data Persistence

- Problem: `/tmp` directory clears on reboot
- Solution: Created persistent `./data` directory
  ```c
  void ensure_data_directory() {
      mkdir("./data", 0755);
  }
  ```

### Challenge 3: Web Form Handling

- Problem: Parsing URL-encoded form data
- Solution: Implemented URL decoder
  ```c
  void url_decode(char str);
  ```

### Challenge 4: Memory Management

- Problem: Buffer overflows and memory leaks
- Solution:
  - Used fixed-size buffers with limits
  - Added input validation
  - Proper malloc/free pairing

---

## 6. SYSTEM DEPLOYMENT

### Setup Steps:

```bash
# 1. Install dependencies
sudo dnf install httpd gcc make

# 2. Enable CGI
sudo dnf install mod_cgi
sudo systemctl restart httpd

# 3. Compile application
gcc -Wall -Wextra -std=c99 \
    web_interface.c scheduling_system.c \
    -o web_interface

# 4. Deploy to CGI directory
sudo cp web_interface /var/www/cgi-bin/
sudo chmod +x /var/www/cgi-bin/web_interface

# 5. Create data directory
sudo mkdir -p /var/www/cgi-bin/data
sudo chmod 777 /var/www/cgi-bin/data
```

### Access URL:

```
http://your-server-ip/cgi-bin/web_interface
```

---

## 7. SYSTEM DEMONSTRATION

### Live Demo Flow:

1. Home Page: Show system statistics
2. Add Room: Create "Computer Science Lab"
3. Add Schedule: Add "Operating Systems" course
4. Generate Schedule: Run auto-assignment
5. View Results: Check by Room and Department
6. Final Schedule: Show complete allocation

### Expected Output:

- Efficient room utilization
- No time conflicts
- Priority-based assignments
- Department-specific views

---

## 8. FUTURE ENHANCEMENTS

### Planned Features:

1. Database Integration: Migrate from files to SQLite/MySQL
2. User Authentication: Login system for different roles
3. Calendar View: Visual timeline of room usage
4. Conflict Reporting: Detailed conflict analysis
5. Mobile Interface: Responsive design for mobile devices
6. Export Options: PDF/Excel schedule exports
7. API Endpoints: REST API for integration with other systems

### Scalability Improvements:

- Multi-threading for schedule generation
- Load balancing for multiple users
- Caching for frequent queries
- Backup and restore functionality

---

## 9. LEARNING OUTCOMES

### Technical Skills Gained:

1. C Programming: Advanced file I/O, memory management
2. Web Development: CGI programming, HTTP protocol
3. Algorithm Design: Priority scheduling, conflict resolution
4. System Administration: Apache configuration, Linux deployment
5. Problem Solving: Debugging, optimization, testing

### Project Management:

- Requirement analysis
- System design
- Incremental development
- Testing and validation
- Documentation

---

## 10. CONCLUSION

### Project Impact:

- Efficiency: Reduces manual scheduling time by ~80%
- Accuracy: Eliminates human errors in conflict detection
- Transparency: Clear view of room utilization
- Scalability: Handles large number of rooms and schedules
- Accessibility: Web-based interface accessible from anywhere

### Key Takeaways:

1. C language is powerful for system-level applications
2. CGI provides a simple way to create web applications in C
3. Proper algorithm design is crucial for scheduling systems
4. Data persistence and integrity are critical for reliable systems

### Final Message:

"This system demonstrates how traditional C programming can be effectively combined with web technologies to solve real-world problems in educational institutions, providing an efficient, reliable, and user-friendly solution for room scheduling."

---

## Q&A PREPARATION

### Potential Questions:

1. Why choose C over web frameworks like Django/Flask?
2. How does the priority algorithm ensure fairness?
3. What are the limitations of file-based storage?
4. How would you handle multiple simultaneous users?
5. Can the system be extended for other scheduling needs?

### Answers Brief:

1. C choice: Performance, control, educational value
2. Fairness: Seniority-based + program type consideration
3. Limitations: Concurrency issues, solved by database in future
4. Multiple users: Session management, locking mechanisms
5. Extensibility: Modular design allows for adaptation

---
