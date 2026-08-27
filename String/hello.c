#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_STR 100
#define MAX_ITEMS 50



//    DATA STRUCTURES

// User structure for Student, Admin, and InstAdmin
typedef struct {
    char username[MAX_STR];
    char password[MAX_STR];
    char role[MAX_STR]; 
} User;

// Exam details structure
typedef struct {
    int exam_id;
    char title[MAX_STR];
    int duration;
    char date_time[MAX_STR];
    int total_questions;
    int pass_marks;
} Exam;

// Question structure
typedef struct {
    int question_id;
    int exam_id;
    char text[MAX_STR * 2];
    char optionA[MAX_STR];
    char optionB[MAX_STR];
    char optionC[MAX_STR];
    char optionD[MAX_STR];
    char correct_option;
    int marks;
} Question;

// Result structure
typedef struct {
    int result_id;
    char student_name[MAX_STR];
    int exam_id;
    char exam_title[MAX_STR];
    int score;
    int total_marks;
    char status[20];
} Result;

// Global session variables
char currentUser[MAX_STR] = "";
char currentRole[MAX_STR] = "";


 //  FUNCTION PROTOTYPES

void clearBuffer(void);
void readString(char *str, int size);
void printString(const char *str);
void recordLog(const char *action);

int isUsernameTaken(const char *uname);
void registerUser(void);
int authenticateUser(const char *uname, const char *pass, char *outRole);
void handleLogin(void);

void instAdminMenu(void);
void viewLogs(void);
void systemPerformance(void);

void adminMenu(void);
void createExam(void);
void addQuestion(void);
void configureExam(void);
void scheduleExam(void);
void generateReports(void);
void backupData(void);
void restoreData(void);

void studentMenu(void);
void viewExams(void);
void searchExam(void);
void startExam(void);
void evaluateExam(Question questions[], char answers[], int qCount, int examId, const char *title, int passMarks);
void viewResults(void);

  
// PART 1: SYSTEM HELPERS, AUTHENTICATION & INSTITUTION AUDITOR

// Clears residual newline characters from the input buffer
void clearBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Reads string with spaces and converts spaces to '_' for simple file storage
void readString(char *str, int size) {
    if (fgets(str, size, stdin) != NULL) {
        str[strcspn(str, "\n")] = '\0'; // Remove newline character
        for (int i = 0; str[i] != '\0'; i++) {
            if (str[i] == ' ') {
                str[i] = '_';
            }
        }
    }
}

// Prints formatted text by replacing underscores back with spaces
void printString(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '_') {
            putchar(' ');
        } else {
            putchar(str[i]);
        }
    }
}

// Writes action logs to logs.txt
void recordLog(const char *action) {
    FILE *file = fopen("logs.txt", "a");
    if (file != NULL) {
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        char timeStr[30];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", t);

        fprintf(file, "[%s] User: %s | Role: %s | Action: %s\n",
                timeStr,
                strlen(currentUser) > 0 ? currentUser : "GUEST",
                strlen(currentRole) > 0 ? currentRole : "NONE",
                action);
        fclose(file);
    }
}

// Checks if username already exists in users.txt
int isUsernameTaken(const char *uname) {
    FILE *file = fopen("users.txt", "r");
    if (file == NULL) return 0;

    char u[MAX_STR], p[MAX_STR], r[MAX_STR];
    while (fscanf(file, "%s %s %s", u, p, r) == 3) {
        if (strcmp(uname, u) == 0) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

// Registers a new user
void registerUser(void) {
    char u[MAX_STR], p[MAX_STR], r[MAX_STR];
    int choice;

    printf("\n--- USER REGISTRATION ---\n");
    printf("Enter Username: ");
    if (scanf("%s", u) != 1) { clearBuffer(); return; }

    if (isUsernameTaken(u)) {
        printf("\n[ERROR] Username '%s' is already taken!\n", u);
        recordLog("Registration Failed (Duplicate Username)");
        clearBuffer();
        return;
    }

    printf("Enter Password: ");
    if (scanf("%s", p) != 1) { clearBuffer(); return; }

    printf("\nSelect Role:\n");
    printf("1. Student\n");
    printf("2. Admin\n");
    printf("3. Institution Admin (InstAdmin)\n");
    printf("Choice (1-3): ");
    if (scanf("%d", &choice) != 1) choice = 1;
    clearBuffer();

    if (choice == 2) strcpy(r, "Admin");
    else if (choice == 3) strcpy(r, "InstAdmin");
    else strcpy(r, "Student");

    FILE *file = fopen("users.txt", "a");
    if (file != NULL) {
        fprintf(file, "%s %s %s\n", u, p, r);
        fclose(file);
        printf("\n[SUCCESS] Account created successfully!\n");
        printf("Username: %s | Role: %s\n", u, r);
        recordLog("New User Registered");
    } else {
        printf("\n[ERROR] Failed to save user data!\n");
    }
}

// Validates user credentials
int authenticateUser(const char *uname, const char *pass, char *outRole) {
    FILE *file = fopen("users.txt", "r");
    if (file == NULL) return 0;

    char u[MAX_STR], p[MAX_STR], r[MAX_STR];
    while (fscanf(file, "%s %s %s", u, p, r) == 3) {
        if (strcmp(uname, u) == 0 && strcmp(pass, p) == 0) {
            strcpy(outRole, r);
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

// Handles user login procedure
void handleLogin(void) {
    char u[MAX_STR], p[MAX_STR], r[MAX_STR];

    printf("\n--- USER LOGIN ---\n");
    printf("Enter Username: ");
    if (scanf("%s", u) != 1) { clearBuffer(); return; }
    printf("Enter Password: ");
    if (scanf("%s", p) != 1) { clearBuffer(); return; }
    clearBuffer();

    if (authenticateUser(u, p, r)) {
        strcpy(currentUser, u);
        strcpy(currentRole, r);
        printf("\n[SUCCESS] Welcome, %s [%s]!\n", currentUser, currentRole);
        recordLog("User Logged In");

        if (strcmp(currentRole, "Admin") == 0) {
            adminMenu();
        } else if (strcmp(currentRole, "Student") == 0) {
            studentMenu();
        } else if (strcmp(currentRole, "InstAdmin") == 0) {
            instAdminMenu();
        }
    } else {
        printf("\n[ERROR] Invalid Username or Password!\n");
        recordLog("Login Attempt Failed");
    }
}

/* --- INSTITUTION ADMINISTRATOR MODULE --- */

void instAdminMenu(void) {
    int choice = 0;
    while (choice != 4) {
        printf("\n========================================\n");
        printf("  INSTITUTION ADMIN CONTROL PANEL       \n");
        printf("========================================\n");
        printf("1. View Performance Reports\n");
        printf("2. View Security Audit Logs\n");
        printf("3. System Performance & Health\n");
        printf("4. Logout\n");
        printf("----------------------------------------\n");
        printf("Enter choice (1-4): ");

        if (scanf("%d", &choice) != 1) {
            clearBuffer();
            choice = 0;
            continue;
        }
        clearBuffer();

        switch (choice) {
            case 1: generateReports(); break;
            case 2: viewLogs(); break;
            case 3: systemPerformance(); break;
            case 4:
                recordLog("Institution Admin Logged Out");
                strcpy(currentUser, "");
                strcpy(currentRole, "");
                printf("Logged out successfully.\n");
                break;
            default: printf("Invalid choice!\n");
        }
    }
}

void viewLogs(void) {
    printf("\n--- SYSTEM AUDIT LOGS ---\n");
    FILE *file = fopen("logs.txt", "r");
    if (file == NULL) {
        printf("No log data found.\n");
        return;
    }

    char line[MAX_STR * 2];
    int count = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);
        count++;
    }
    fclose(file);

    if (count == 0) {
        printf("No log data found.\n");
    }
}

void systemPerformance(void) {
    printf("\n--- SYSTEM PERFORMANCE & HEALTH ---\n");
    printf("Language Standard : C99 / ANSI C\n");
    printf("Storage Type      : File Handling (.txt)\n");
    printf("System Status     : Active & Healthy\n");
    printf("File Stream Check : Normal\n");
    recordLog("Checked System Performance");
}

//    PART 2: ADMINISTRATOR CONTROL PANEL & EXAM MANAGEMENT
void adminMenu(void) {
    int choice = 0;
    while (choice != 8) {
        printf("\n========================================\n");
        printf("       ADMINISTRATOR CONTROL PANEL      \n");
        printf("========================================\n");
        printf("1. Create New Exam\n");
        printf("2. Set Questions for an Exam (Batch Input)\n");
        printf("3. Configure Exam Duration & Marks\n");
        printf("4. Schedule Exam\n");
        printf("5. View Performance Reports\n");
        printf("6. Backup System Data\n");
        printf("7. Restore System Data\n");
        printf("8. Logout\n");
        printf("----------------------------------------\n");
        printf("Enter choice (1-8): ");

        if (scanf("%d", &choice) != 1) {
            clearBuffer();
            choice = 0;
            continue;
        }
        clearBuffer();

        switch (choice) {
            case 1: createExam(); break;
            case 2: addQuestion(); break;
            case 3: configureExam(); break;
            case 4: scheduleExam(); break;
            case 5: generateReports(); break;
            case 6: backupData(); break;
            case 7: restoreData(); break;
            case 8:
                recordLog("Admin Logged Out");
                strcpy(currentUser, "");
                strcpy(currentRole, "");
                printf("Logged out successfully.\n");
                break;
            default:
                printf("Invalid choice! Try again.\n");
        }
    }
}

void createExam(void) {
    Exam e;
    printf("\n--- CREATE NEW EXAM ---\n");
    printf("Enter Exam ID (Number): ");
    if (scanf("%d", &e.exam_id) != 1) { clearBuffer(); return; }
    clearBuffer();

    printf("Enter Exam Title: ");
    readString(e.title, sizeof(e.title));

    printf("Enter Duration (in Minutes): ");
    if (scanf("%d", &e.duration) != 1) { clearBuffer(); return; }

    printf("Enter Schedule Date/Time (e.g., 2026-06-15_10:00AM): ");
    if (scanf("%s", e.date_time) != 1) { clearBuffer(); return; }

    printf("Enter Total Questions Count: ");
    if (scanf("%d", &e.total_questions) != 1) { clearBuffer(); return; }

    printf("Enter Pass Marks: ");
    if (scanf("%d", &e.pass_marks) != 1) { clearBuffer(); return; }
    clearBuffer();

    FILE *file = fopen("exams.txt", "a");
    if (file != NULL) {
        fprintf(file, "%d %s %d %s %d %d\n",
                e.exam_id, e.title, e.duration, e.date_time, e.total_questions, e.pass_marks);
        fclose(file);
        printf("\n[SUCCESS] Exam created successfully!\n");
        recordLog("Created New Exam");
    } else {
        printf("\n[ERROR] Failed to save exam data!\n");
    }
}

void addQuestion(void) {
    printf("\n--- SET EXAM QUESTIONS ---\n");
    int targetId;
    printf("Enter Target Exam ID: ");
    if (scanf("%d", &targetId) != 1) { clearBuffer(); return; }
    clearBuffer();

    FILE *file = fopen("exams.txt", "r");
    if (file == NULL) {
        printf("\n[ERROR] No exams found. Please create an exam first!\n");
        return;
    }

    Exam e;
    int found = 0;
    while (fscanf(file, "%d %s %d %s %d %d",
           &e.exam_id, e.title, &e.duration, e.date_time, &e.total_questions, &e.pass_marks) == 6) {
        if (e.exam_id == targetId) {
            found = 1;
            break;
        }
    }
    fclose(file);

    if (!found) {
        printf("\n[ERROR] Exam ID %d not found! Please create the exam first.\n", targetId);
        return;
    }

    printf("\n=======================================================\n");
    printf(" EXAM TITLE            : "); printString(e.title); printf("\n");
    printf(" TOTAL QUESTIONS TO SET: %d\n", e.total_questions);
    printf("=======================================================\n");
    printf(" Please enter all %d questions for this examination below.\n\n", e.total_questions);

    FILE *qFile = fopen("questions.txt", "a");
    if (qFile == NULL) {
        printf("\n[ERROR] Could not open questions database!\n");
        return;
    }

    for (int i = 1; i <= e.total_questions; i++) {
        Question q;
        q.exam_id = targetId;
        q.question_id = i;

        printf("--- Question %d of %d ---\n", i, e.total_questions);

        printf("Enter Question Text: ");
        readString(q.text, sizeof(q.text));

        printf("Enter Option A: ");
        readString(q.optionA, sizeof(q.optionA));

        printf("Enter Option B: ");
        readString(q.optionB, sizeof(q.optionB));

        printf("Enter Option C: ");
        readString(q.optionC, sizeof(q.optionC));

        printf("Enter Option D: ");
        readString(q.optionD, sizeof(q.optionD));

        printf("Enter Correct Option (A/B/C/D): ");
        if (scanf(" %c", &q.correct_option) != 1) { q.correct_option = 'A'; }
        if (q.correct_option >= 'a' && q.correct_option <= 'z') {
            q.correct_option -= 32;
        }

        printf("Enter Marks for Question %d: ", i);
        if (scanf("%d", &q.marks) != 1) q.marks = 1;
        clearBuffer();

        fprintf(qFile, "%d %d %s %s %s %s %s %c %d\n",
                q.question_id, q.exam_id, q.text,
                q.optionA, q.optionB, q.optionC, q.optionD,
                q.correct_option, q.marks);

        printf("-> Question %d saved!\n-------------------------------------------------------\n", i);
    }

    fclose(qFile);
    printf("\n[SUCCESS] All %d questions for Exam ID %d have been successfully set!\n", e.total_questions, targetId);
    recordLog("Set All Exam Questions");
}

void configureExam(void) {
    int targetId, newDuration, newPassMarks;
    printf("\n--- CONFIGURE EXAM DURATION & MARKS ---\n");
    printf("Enter Exam ID to configure: ");
    if (scanf("%d", &targetId) != 1) { clearBuffer(); return; }

    FILE *file = fopen("exams.txt", "r");
    if (file == NULL) {
        printf("\n[ERROR] No examinations found.\n");
        clearBuffer();
        return;
    }

    Exam list[MAX_ITEMS];
    int count = 0, found = 0;

    while (count < MAX_ITEMS && fscanf(file, "%d %s %d %s %d %d",
           &list[count].exam_id, list[count].title, &list[count].duration,
           list[count].date_time, &list[count].total_questions, &list[count].pass_marks) == 6) {
        if (list[count].exam_id == targetId) {
            found = 1;
            printf("Current Duration: %d Mins | Pass Marks: %d\n", list[count].duration, list[count].pass_marks);
            printf("Enter New Duration (Mins): ");
            if (scanf("%d", &newDuration) == 1) list[count].duration = newDuration;
            printf("Enter New Pass Marks: ");
            if (scanf("%d", &newPassMarks) == 1) list[count].pass_marks = newPassMarks;
        }
        count++;
    }
    fclose(file);
    clearBuffer();

    if (found) {
        file = fopen("exams.txt", "w");
        if (file != NULL) {
            for (int i = 0; i < count; i++) {
                fprintf(file, "%d %s %d %s %d %d\n",
                        list[i].exam_id, list[i].title, list[i].duration,
                        list[i].date_time, list[i].total_questions, list[i].pass_marks);
            }
            fclose(file);
            printf("\n[SUCCESS] Exam updated successfully!\n");
            recordLog("Updated Exam Configuration");
        }
    } else {
        printf("\n[ERROR] Exam ID not found!\n");
    }
}

void scheduleExam(void) {
    int targetId;
    char newTime[MAX_STR];
    printf("\n--- RESCHEDULE EXAM ---\n");
    printf("Enter Exam ID to reschedule: ");
    if (scanf("%d", &targetId) != 1) { clearBuffer(); return; }
    printf("Enter New Date/Time (e.g., 2026-07-01_02:00PM): ");
    if (scanf("%s", newTime) != 1) { clearBuffer(); return; }
    clearBuffer();

    FILE *file = fopen("exams.txt", "r");
    if (file == NULL) {
        printf("\n[ERROR] No examinations found.\n");
        return;
    }

    Exam list[MAX_ITEMS];
    int count = 0, found = 0;

    while (count < MAX_ITEMS && fscanf(file, "%d %s %d %s %d %d",
           &list[count].exam_id, list[count].title, &list[count].duration,
           list[count].date_time, &list[count].total_questions, &list[count].pass_marks) == 6) {
        if (list[count].exam_id == targetId) {
            found = 1;
            strcpy(list[count].date_time, newTime);
        }
        count++;
    }
    fclose(file);

    if (found) {
        file = fopen("exams.txt", "w");
        if (file != NULL) {
            for (int i = 0; i < count; i++) {
                fprintf(file, "%d %s %d %s %d %d\n",
                        list[i].exam_id, list[i].title, list[i].duration,
                        list[i].date_time, list[i].total_questions, list[i].pass_marks);
            }
            fclose(file);
            printf("\n[SUCCESS] Exam rescheduled successfully!\n");
            recordLog("Rescheduled Exam");
        }
    } else {
        printf("\n[ERROR] Exam ID not found!\n");
    }
}

void generateReports(void) {
    printf("\n--- STUDENT PERFORMANCE REPORTS ---\n");
    FILE *file = fopen("results.txt", "r");

    if (file == NULL) {
        printf("No performance records found.\n");
        return;
    }

    Result r;
    int count = 0;
    printf("\n%-8s | %-15s | %-25s | %-10s | %-8s\n", "Res_ID", "Student", "Exam Title", "Score", "Status");
    printf("-----------------------------------------------------------------------\n");

    while (fscanf(file, "%d %s %d %s %d %d %s",
           &r.result_id, r.student_name, &r.exam_id, r.exam_title,
           &r.score, &r.total_marks, r.status) == 7) {
        count++;
        printf("%-8d | %-15s | ", r.result_id, r.student_name);
        printString(r.exam_title);
        printf(" | %d/%-8d | %-8s\n", r.score, r.total_marks, r.status);
    }

    fclose(file);

    if (count == 0) {
        printf("No performance records found.\n");
    } else {
        recordLog("Viewed Performance Reports");
    }
}

void backupData(void) {
    printf("\n--- SYSTEM BACKUP ---\n");
    FILE *src = fopen("exams.txt", "r");
    FILE *dest = fopen("exams_backup.txt", "w");

    if (src == NULL || dest == NULL) {
        printf("[ERROR] Backup failed! File 'exams.txt' not found or empty.\n");
        if (src) fclose(src);
        if (dest) fclose(dest);
        return;
    }

    int ch;
    while ((ch = fgetc(src)) != EOF) {
        fputc(ch, dest);
    }

    fclose(src);
    fclose(dest);

    printf("[SUCCESS] Exam data backed up successfully to 'exams_backup.txt'!\n");
    recordLog("Performed Data Backup");
}

void restoreData(void) {
    printf("\n--- SYSTEM RESTORE ---\n");
    FILE *src = fopen("exams_backup.txt", "r");
    FILE *dest = fopen("exams.txt", "w");

    if (src == NULL || dest == NULL) {
        printf("[ERROR] Restore failed! Backup file 'exams_backup.txt' not found.\n");
        if (src) fclose(src);
        if (dest) fclose(dest);
        return;
    }

    int ch;
    while ((ch = fgetc(src)) != EOF) {
        fputc(ch, dest);
    }

    fclose(src);
    fclose(dest);

    printf("[SUCCESS] Data restored successfully from backup!\n");
    recordLog("Performed Data Restore");
}


//    PART 3: STUDENT MODULE & TIMED ASSESSMENT ENGINE

void studentMenu(void) {
    int choice = 0;
    while (choice != 5) {
        printf("\n========================================\n");
        printf("         STUDENT PORTAL DASHBOARD       \n");
        printf("========================================\n");
        printf("1. View Scheduled Exams\n");
        printf("2. Search Exam by ID\n");
        printf("3. Start Timed Exam\n");
        printf("4. View My Results\n");
        printf("5. Logout\n");
        printf("----------------------------------------\n");
        printf("Enter choice (1-5): ");

        if (scanf("%d", &choice) != 1) {
            clearBuffer();
            choice = 0;
            continue;
        }
        clearBuffer();

        switch (choice) {
            case 1: viewExams(); break;
            case 2: searchExam(); break;
            case 3: startExam(); break;
            case 4: viewResults(); break;
            case 5:
                recordLog("Student Logged Out");
                strcpy(currentUser, "");
                strcpy(currentRole, "");
                printf("Logged out successfully.\n");
                break;
            default: printf("Invalid choice!\n");
        }
    }
}

void viewExams(void) {
    printf("\n--- SCHEDULED EXAMINATIONS ---\n");
    FILE *file = fopen("exams.txt", "r");

    if (file == NULL) {
        printf("No examinations found.\n");
        return;
    }

    Exam e;
    int count = 0;
    printf("\n%-8s | %-25s | %-12s | %-20s\n", "Exam_ID", "Title", "Duration", "Schedule Date");
    printf("-------------------------------------------------------------------\n");

    while (fscanf(file, "%d %s %d %s %d %d",
           &e.exam_id, e.title, &e.duration, &e.date_time, &e.total_questions, &e.pass_marks) == 6) {
        count++;
        printf("%-8d | ", e.exam_id);
        printString(e.title);
        printf(" | %d Mins    | %-20s\n", e.duration, e.date_time);
    }

    fclose(file);

    if (count == 0) {
        printf("No examinations found.\n");
    }
}

void searchExam(void) {
    printf("\n--- SEARCH EXAMINATION ---\n");
    int targetId;
    printf("Enter Exam ID to search: ");
    if (scanf("%d", &targetId) != 1) { clearBuffer(); return; }
    clearBuffer();

    FILE *file = fopen("exams.txt", "r");
    if (file == NULL) {
        printf("\n[NOT FOUND] No examinations found.\n");
        return;
    }

    Exam e;
    int found = 0;

    while (fscanf(file, "%d %s %d %s %d %d",
           &e.exam_id, e.title, &e.duration, e.date_time, &e.total_questions, &e.pass_marks) == 6) {
        if (e.exam_id == targetId) {
            found = 1;
            printf("\n[MATCH FOUND]\n");
            printf("Exam ID        : %d\n", e.exam_id);
            printf("Title          : "); printString(e.title); printf("\n");
            printf("Duration       : %d Minutes\n", e.duration);
            printf("Scheduled Time : %s\n", e.date_time);
            printf("Total Questions: %d\n", e.total_questions);
            printf("Pass Marks     : %d\n", e.pass_marks);
            break;
        }
    }

    fclose(file);

    if (!found) {
        printf("\n[NOT FOUND] No examination matches ID: %d\n", targetId);
    }
}

void startExam(void) {
    printf("\n--- START TIMED ONLINE EXAM ---\n");
    int selectedId;
    printf("Enter Exam ID to start: ");
    if (scanf("%d", &selectedId) != 1) { clearBuffer(); return; }
    clearBuffer();

    FILE *fExam = fopen("exams.txt", "r");
    if (fExam == NULL) {
        printf("\n[ERROR] No examinations found.\n");
        return;
    }

    Exam currentExam;
    int found = 0;

    while (fscanf(fExam, "%d %s %d %s %d %d",
           &currentExam.exam_id, currentExam.title, &currentExam.duration,
           currentExam.date_time, &currentExam.total_questions, &currentExam.pass_marks) == 6) {
        if (currentExam.exam_id == selectedId) {
            found = 1;
            break;
        }
    }
    fclose(fExam);

    if (!found) {
        printf("\n[ERROR] Examination not found!\n");
        return;
    }

    FILE *fQ = fopen("questions.txt", "r");
    if (fQ == NULL) {
        printf("\n[ERROR] Question bank unavailable or empty!\n");
        return;
    }

    Question questions[MAX_ITEMS];
    int qCount = 0;
    Question tempQ;

    while (qCount < MAX_ITEMS && fscanf(fQ, "%d %d %s %s %s %s %s %c %d",
           &tempQ.question_id, &tempQ.exam_id, tempQ.text,
           tempQ.optionA, tempQ.optionB, tempQ.optionC, tempQ.optionD,
           &tempQ.correct_option, &tempQ.marks) == 9) {
        if (tempQ.exam_id == selectedId) {
            questions[qCount] = tempQ;
            qCount++;
        }
    }
    fclose(fQ);

    if (qCount == 0) {
        printf("\n[ERROR] No questions found for this exam!\n");
        return;
    }

    // Simple question sequence shuffling
    srand((unsigned int)time(NULL));
    for (int i = 0; i < qCount; i++) {
        int r = rand() % qCount;
        Question temp = questions[i];
        questions[i] = questions[r];
        questions[r] = temp;
    }

    printf("\n=======================================================\n");
    printf(" EXAM: "); printString(currentExam.title);
    printf(" | DURATION: %d Mins\n", currentExam.duration);
    printf(" TOTAL QUESTIONS: %d | PASS MARKS: %d\n", qCount, currentExam.pass_marks);
    printf("=======================================================\n");
    printf(" Starting assessment now. Answer each question carefully.\n\n");

    char answers[MAX_ITEMS];
    memset(answers, ' ', sizeof(answers));
    time_t startTime = time(NULL);

    for (int i = 0; i < qCount; i++) {
        time_t currentTime = time(NULL);
        double elapsedMins = difftime(currentTime, startTime) / 60.0;

        if (elapsedMins >= currentExam.duration) {
            printf("\n[TIME EXPIRED!] Duration limit reached. Auto-submitting...\n");
            recordLog("Exam Auto-Submitted (Time Expired)");
            break;
        }

        printf("Q%d: ", i + 1);
        printString(questions[i].text);
        printf(" (Marks: %d)\n", questions[i].marks);

        printf("  A) "); printString(questions[i].optionA); printf("\n");
        printf("  B) "); printString(questions[i].optionB); printf("\n");
        printf("  C) "); printString(questions[i].optionC); printf("\n");
        printf("  D) "); printString(questions[i].optionD); printf("\n");
        printf("  Your Answer (A/B/C/D): ");

        if (scanf(" %c", &answers[i]) != 1) {
            answers[i] = ' ';
        }
        clearBuffer();
        printf("-------------------------------------------------------\n");
    }

    printf("\n[SUBMITTED] Answers recorded successfully!\n");
    recordLog("Submitted Exam Answers");

    evaluateExam(questions, answers, qCount, currentExam.exam_id, currentExam.title, currentExam.pass_marks);
}

void evaluateExam(Question questions[], char answers[], int qCount, int examId, const char *title, int passMarks) {
    int totalObtained = 0;
    int maxMarks = 0;

    for (int i = 0; i < qCount; i++) {
        maxMarks += questions[i].marks;

        char ans = answers[i];
        if (ans >= 'a' && ans <= 'z') {
            ans -= 32; // Uppercase conversion
        }

        if (ans == questions[i].correct_option) {
            totalObtained += questions[i].marks;
        }
    }

    char status[20];
    if (totalObtained >= passMarks) {
        strcpy(status, "PASSED");
    } else {
        strcpy(status, "FAILED");
    }

    printf("\n=======================================================\n");
    printf(" EVALUATION RESULT SUMMARY\n");
    printf(" Score Obtained : %d / %d\n", totalObtained, maxMarks);
    printf(" Pass Marks     : %d\n", passMarks);
    printf(" Final Status   : %s\n", status);
    printf("=======================================================\n");

    int resultId = (int)time(NULL) % 10000;

    FILE *file = fopen("results.txt", "a");
    if (file != NULL) {
        fprintf(file, "%d %s %d %s %d %d %s\n",
                resultId, currentUser, examId, title,
                totalObtained, maxMarks, status);
        fclose(file);
    }
}

void viewResults(void) {
    printf("\n--- MY EXAMINATION RESULTS ---\n");
    FILE *file = fopen("results.txt", "r");

    if (file == NULL) {
        printf("No exam results found.\n");
        return;
    }

    Result r;
    int found = 0;

    printf("\n%-8s | %-25s | %-10s | %-8s\n", "Res_ID", "Exam Title", "Score", "Status");
    printf("-----------------------------------------------------------\n");

    while (fscanf(file, "%d %s %d %s %d %d %s",
           &r.result_id, r.student_name, &r.exam_id, r.exam_title,
           &r.score, &r.total_marks, r.status) == 7) {
        if (strcmp(r.student_name, currentUser) == 0) {
            found = 1;
            printf("%-8d | ", r.result_id);
            printString(r.exam_title);
            printf(" | %d/%-8d | %-8s\n", r.score, r.total_marks, r.status);
        }
    }

    fclose(file);

    if (!found) {
        printf("No results found for student: %s\n", currentUser);
    }
}

  

// MAIN ENTRY POINT

int main(void) {
    recordLog("System Startup");

    int choice = 0;
    while (choice != 3) {
        printf("\n====================================================\n");
        printf("        ONLINE EXAMINATION SYSTEM                   \n");
        printf("====================================================\n");
        printf("1. Login (Admin / Student / InstAdmin)\n");
        printf("2. Register New Account\n");
        printf("3. Exit\n");
        printf("----------------------------------------------------\n");
        printf("Enter Choice (1-3): ");

        if (scanf("%d", &choice) != 1) {
            clearBuffer();
            choice = 0;
            printf("\nInvalid choice!\n");
            continue;
        }
        clearBuffer();

        if (choice == 1) {
            handleLogin();
        } else if (choice == 2) {
            registerUser();
        } else if (choice == 3) {
            printf("\nThank you for using the Online Examination System!\n");
            recordLog("System Shutdown");
        } else {
            printf("\nInvalid choice!\n");
        }
    }

    return 0;
}