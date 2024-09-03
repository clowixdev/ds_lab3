#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define FIELDMAXLEN 50
#define MAXEVENTLEN 22 + FIELDMAXLEN * 2
#define MAXSUBSTRINGLEN 25

enum DAY {
    MONDAY = 1,
    TUESDAY = 2,
    WEDNESDAY = 3,
    THURSDAY = 4,
    FRIDAY = 5,
    SUNDAY = 6,
    SATURDAY = 7
};

struct Event {
    short minute;
    short hour;
    short day;
    short month;
    short year;
    
    enum DAY weekday;
    char* description;
    char* place;
    short priority;
};

struct Node {
    short id;
    struct Event* event;
    struct Node* pRight;
    struct Node* pLeft;
};

void copy(char* str1, char* str2, int len) {
    for (int i = 0; i < len; i++) {
        str1[i] = str2[i];
    }
}

void strip(char* str) {
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] == ' ') {
            str[i] = '_';
        }
        if (str[i] == '\n') {
            str[i] = '\0';
            return;
        }
        i++;
    }
}

void clear(char* str, int strlen) {
	for (int i = 0; i < strlen; i++) {
		str[i] = '\0';
	}
}

int is_earlier(struct Event* to_put, struct Event* curr) {
    if (to_put->year < curr->year) {
        return 1;
    } 
    if (to_put->year > curr->year) {
        return 0;
    }
    if (to_put->year == curr->year) {
        if (to_put->month < curr->month) {
            return 1;
        }
        if (to_put->month > curr->month) {
            return 0;
        }
        if (to_put->month == curr->month) {
            if (to_put->day < curr->day) {
                return 1;
            }
            if (to_put->day > curr->day) {
                return 0;
            }
            if (to_put->day == curr->day) {
                if (to_put->hour < curr->hour) {
                    return 1;
                }
                if (to_put->hour > curr->hour) {
                    return 0;
                }
                if (to_put->hour == curr->hour) {
                    if (to_put->minute < curr->minute) {
                        return 1;
                    }
                    if (to_put->minute > curr->minute) {
                        return 0;
                    }
                    if (to_put->minute == curr->minute) {
                        return (to_put->priority > curr->priority) ? 1 : 0;
                    }
                }
            }
        }
    }
    return 0;
}

struct Node* create_node(struct Event* user_event) {
    struct Node* pNode = (struct Node*)malloc(sizeof(struct Node));
    if (pNode == NULL) {
        printf("error occured while allocating memory");
        return NULL;
    }
    pNode->id = -1;
    pNode->event = user_event;
    pNode->pRight = NULL;
    pNode->pLeft = NULL;
    return pNode;
}

struct Node* add_event(struct Node* pNode, struct Event* user_event) {
    if (pNode == NULL) {
        return create_node(user_event);
    } else {
        if (is_earlier(user_event, pNode->event)) {
            pNode->pLeft = add_event(pNode->pLeft, user_event);
        } else {
            pNode->pRight = add_event(pNode->pRight, user_event);
        }
    }

    return pNode;
}

void show_event(struct Node* pNode) {

    if (pNode->event == NULL || pNode == NULL) {
        printf("Nothing to show!\n");
        return;
    }

    char weekday[10] = { 0 };
    switch (pNode->event->weekday) {
    case 1:
        copy(weekday, "Monday", 7);
        break;
    case 2:
        copy(weekday, "Tuesday", 8);
        break;
    case 3:
        copy(weekday, "Wednesday", 10);
        break;
    case 4:
        copy(weekday, "Thursday", 9);
        break;
    case 5:
        copy(weekday, "Friday", 7);
        break;
    case 6:
        copy(weekday, "Sunday", 7);
        break;
    case 7:
        copy(weekday, "Saturday", 9);
        break;
    default:
        printf("Unknown error occured in show_event()!\n");
        return;
    }
    char day_zero = ' ';
    char month_zero = ' ';
    char hour_zero = ' ';
    char minute_zero = ' ';

    if (pNode->event->day < 10) {
        day_zero = '0';
    }
    if (pNode->event->month < 10) {
        month_zero = '0';
    }
    if (pNode->event->hour < 10) {
        hour_zero = '0';
    }
    if (pNode->event->minute < 10) {
        minute_zero = '0';
    }

    printf("\nID: %hi\nDate: %c%hi.%c%hi.%hi - \%c%hi:%c%hi %s\nEvent: %s\nPlace: %s\nPriority: %hi\n\n", \
        pNode->id,
        day_zero,
        pNode->event->day,
        month_zero,
        pNode->event->month, 
        pNode->event->year,
        hour_zero,
        pNode->event->hour,
        minute_zero, 
        pNode->event->minute, 
        weekday, 
        pNode->event->description, 
        pNode->event->place, 
        pNode->event->priority
    );

    return;
}

bool found_substring(char* string, char* substring) {
    bool substring_found = false;
    int correct_chars = 0;
    for (int i = 0; i < (strlen(string) - strlen(substring) + 1); i++) {
        correct_chars = 0;
        for (int j = 0; j < strlen(substring); j++) {
            if (string[i+j] == substring[j]) {
                correct_chars++;
            }
        }

        if (correct_chars == strlen(substring)) {
            return true;
        }
    }

    return false;
}

void show_dateasc(struct Node* pNode) {
    if (pNode != NULL) {
        if (pNode->pLeft != NULL) {
            show_dateasc(pNode->pLeft);
        }
        show_event(pNode);
        if (pNode->pRight != NULL) {
            show_dateasc(pNode->pRight);
        }
    }
}

void show_priorityasc(struct Node* pNode, int* priority) {
    if (pNode != NULL) {
        if (pNode->pLeft != NULL) {
            show_priorityasc(pNode->pLeft, priority);
        }
        if (pNode->event->priority == *priority) {
            show_event(pNode);
        }
        if (pNode->pRight != NULL) {
            show_priorityasc(pNode->pRight, priority);
        }
    }
}

void show_substring(struct Node *pNode, char* substring) {
    if (pNode != NULL) {
        if (pNode->pLeft != NULL) {
            show_substring(pNode->pLeft, substring);
        }
        if (found_substring(pNode->event->place, substring) == true) {
            show_event(pNode);
        }
        if (pNode->pRight != NULL) {
            show_substring(pNode->pRight, substring);
        }
    }
}

void count_events(struct Node* pNode, int* count) {
    if (pNode != NULL) {
        if (pNode->pLeft != NULL) {
            count_events(pNode->pLeft, count);
        }
        *count++;
        if (pNode->pRight != NULL) {
            count_events(pNode->pRight, count);
        }
    }
}

void update_tree(struct Node* pNode, int* id) {
    if (pNode != NULL) {
        if (pNode->pLeft != NULL) {
            update_tree(pNode->pLeft, id);
        }
        pNode->id = *id;
        (*id)++;
        if (pNode->pRight != NULL) {
            update_tree(pNode->pRight, id);
        }
    }
}

void save_all(struct Node* pNode, FILE* stream) {
    if (pNode != NULL) {
        if (pNode->pLeft != NULL) {
            save_all(pNode->pLeft, stream);
        }
        
        fprintf(stream, "%hi %hi %hi %hi %hi %hi %s %s %hi\n", \
            pNode->event->day,
            pNode->event->month,
            pNode->event->year,
            pNode->event->hour,
            pNode->event->minute,
            pNode->event->weekday,
            pNode->event->description,
            pNode->event->place,
            pNode->event->priority
        );
        if (pNode->pRight != NULL) {
            save_all(pNode->pRight, stream);
        }
    }
}

void free_tree(struct Node* pNode) {
    if (pNode != NULL) {
        if (pNode->pLeft != NULL) {
            free_tree(pNode->pLeft);
        }
        if (pNode->pLeft == NULL && pNode->pRight == NULL) {
            free(pNode);
            pNode == NULL;
        }
        if (pNode->pRight != NULL) {
            free_tree(pNode->pRight);
        }
    }
}

bool is_leap(short year) {
    if (year == 2000 || year % 4 == 0) {
        return true;
    } else {
        return false;
    }
}

enum DAY define_day(short year, short month, short day) {
    short year_code = (6 + (year % 100) + (year % 100) / 4) % 7;
    short def_month_codes[13] = {0, 1, 4, 4, 0, 2, 5, 0, 3, 6, 1, 4, 6};
    short leap_month_codes[13] = {0, 0, 3, 4, 0, 2, 5, 0, 3, 6, 1, 4, 6};

    short weekday = 0;
    if (is_leap(year)) {
        weekday = (day + leap_month_codes[month] + year_code) % 7;
    } else {
        weekday = (day + def_month_codes[month] + year_code) % 7;
    }

    switch (weekday) {
        case 0:
            return SATURDAY;
        case 1:
            return SUNDAY;
        case 2:
            return MONDAY;
        case 3:
            return TUESDAY;
        case 4:
            return WEDNESDAY;
        case 5:
            return THURSDAY;
        case 6:
            return FRIDAY;
        default:
            printf("Unknown error occured in define_day()");
            break;
    }
}

bool correct(short data, short year, short month, char mode) {
    short days[13] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    switch (mode) {
        case 'y':
            return (data > 1900 && data < 2100) ? true : false;
        case 'm':
            return (data >= 1 && data <= 12) ? true : false;
        case 'd':
            if (data > 0 && data <= days[month]) {
                if (month == 2 && is_leap(year) == true) {
                    return data <= 29 ? true : false;
                } else if (month == 2 && is_leap(year) == false) {
                    return data <= 28 ? true : false;
                } else {
                    return true;
                }
            } else {
                return false;
            }
        case 'h':
            return (data >= 0 && data <= 24) ? true : false;
        case 'i':
            return (data >= 0 && data <= 60) ? true : false;
        case 'p':
            return (data >= 0 && data <= 10) ? true : false;
        default:
            printf("Unknown error occured in correct()!");
            break;
    }
}

struct Node* find_earliest(struct Node* pNode) {
    if (pNode == NULL) {
        return NULL;
    } else if (pNode->pLeft != NULL) {
        return find_earliest(pNode->pLeft);
    }
    return pNode;
}

struct Node* delete_node(struct Node* pRoot, int id) {
    if (pRoot == NULL)
        return NULL;

    if (id > pRoot->id) {
        pRoot->pRight = delete_node(pRoot->pRight, id);
    } else if (id < pRoot->id) {
        pRoot->pLeft = delete_node(pRoot->pLeft, id);
    } else {
        if (pRoot->pLeft == NULL && pRoot->pRight == NULL) {
            free(pRoot);
            return NULL;
        } else if (pRoot->pLeft == NULL || pRoot->pRight == NULL) {
            struct Node* temp_node;

            if (pRoot->pLeft == NULL) {
                temp_node = pRoot->pRight;
            } else {
                temp_node = pRoot->pLeft;
            }
            free(pRoot);
            return temp_node;
        } else {
            struct Node* temp_node = find_earliest(pRoot->pRight);
            pRoot->event = temp_node->event;
            pRoot->pRight = delete_node(pRoot->pRight, temp_node->id);
        }
    }
    
    return pRoot;
}

short get_correct_priority() {
    short priority = -1;
    while (priority == -1) {
        printf("Enter a priority of event: (it must be between 0 and 10) >>> ");
        fflush(stdin);
        scanf("%hi", &priority);
        if (correct(priority, 0, 0, 'p') == false) {
            printf("Incorrect priority!\n");
            priority = -1;
        }
    }

    return priority;
}

short get_correct_year() {
    short year = -1;
    while (year == -1) {
        printf("Enter a year of event: (it must be between 1901 and 2099) >>> ");
        fflush(stdin);
        scanf("%hi", &year);
        if (correct(year, 0, 0, 'y') == false) {
            printf("Incorrect year!\n");
            year = -1;
        }
    }

    return year;
}

short get_correct_month() {
    short month = -1;
    while (month == -1) {
        printf("Enter a month of event: (it must be correct) >>> ");
        fflush(stdin);
        scanf("%hi", &month);
        if (correct(month, 0, 0, 'm') == false) {
            printf("Incorrect month!\n");
            month = -1;
        }
    }

    return month;
}

short get_correct_day(short year, short month) {
    short day = -1;
    while (day == -1) {
        printf("Enter a day of event: (it must be correct) >>> ");
        fflush(stdin);
        scanf("%hi", &day);
        if (correct(day, year, month, 'd') == false) {
            printf("Incorrect day!\n");
            day = -1;
        }
    }

    return day;
}

short get_correct_hour() {
    short hour = -1;
    while (hour == -1) {
        printf("Enter a hour of event: (it must be correct) >>> ");
        fflush(stdin);
        scanf("%hi", &hour);
        if (correct(hour, 0, 0, 'h') == false) {
            printf("Incorrect hour!\n");
            hour = -1;
        }
    }

    return hour;
}

short get_correct_minute() {
    short minute = -1;
    while (minute == -1) {
        printf("Enter a minute of event: (it must be correct) >>> ");
        fflush(stdin);
        scanf("%hi", &minute);
        if (correct(minute, 0, 0, 'i') == false) {
            printf("Incorrect minute!\n");
            minute = -1;
        }
    }

    return minute;
}

char* get_correct_description() {
    char* description = (char*)calloc(FIELDMAXLEN, sizeof(char));
    description[0] == '\0';
    while (description[0] == '\0') {
        printf("Enter a description of event: (it must be < 100 characters long) >>> ");
        fflush(stdin);
        fgets(description, FIELDMAXLEN, stdin);
        if (strlen(description) > FIELDMAXLEN) {
            printf("Incorrect description!\n");
            description[0] == '\0';
        }
    }
    strip(description);

    return description;
}

char* get_correct_place() {
    char* place = (char*)calloc(FIELDMAXLEN, sizeof(char));
    place[0] == '\0';
    while (place[0] == '\0') {
        printf("Enter a place of event: (it must be < 100 characters long) >>> ");
        fflush(stdin);
        fgets(place, FIELDMAXLEN, stdin);
        if (strlen(place) > FIELDMAXLEN) {
            printf("Incorrect place!\n");
            place[0] == '\0';
        }
    }
    strip(place);

    return place;
}

int main(void) {
    FILE* r_count_stream = fopen("notebook.txt", "a+");
    int events_count = 0;
    char c;
    bool no_events;
    while ((c = fgetc(r_count_stream)) != EOF) {
        if (c == '\n') {
            events_count++;
        }
    }
    fclose(r_count_stream);

    FILE* r_stream = fopen("notebook.txt", "r");
    fseek(r_stream, 0L, SEEK_SET);

    struct Node* pRoot = NULL;
    int loaded = 0;

    for (int i = 0; i < events_count; i++) {
        struct Event* read_event = (struct Event*)malloc(sizeof(struct Event));
        if (read_event == NULL) {
            printf("Error occured while allocating memory");
        }

        read_event->description = (char*)malloc(sizeof(char) * FIELDMAXLEN);
        if (read_event->description == NULL) {
            printf("Error occured while allocating memory");
        }

        read_event->place = (char*)malloc(sizeof(char) * FIELDMAXLEN);
        if (read_event->place == NULL) {
            printf("Error occured while allocating memory");
        }
        
        int read = fscanf(r_stream, "%hi %hi %hi %hi %hi %hi %s %s %hi\n", \
            &(read_event->day),
            &(read_event->month),
            &(read_event->year),
            &(read_event->hour),
            &(read_event->minute),
            &(read_event->weekday),
            read_event->description,
            read_event->place,
            &(read_event->priority)
        );

        if (read == 9) {
            loaded++;
        }

        if (pRoot == NULL) {
            pRoot = add_event(pRoot, read_event);
        } else {
            add_event(pRoot, read_event);
        }
    }

    if (loaded != 0) {
        printf("successfully loaded %d events from memory\n", loaded);
    }
    
    char user_input = ' ';

    printf("Welcome to your NoteBookApp\n");
    while (1) {
        printf("Select action below:\n\t1.Show all notes\n\t2.Add note\n\t3.Delete note\n\te.Exit app\n>>> ");
        fflush(stdin);
        scanf("%c", &user_input); 
    
        switch(user_input) {
        case '1':
            if (pRoot == NULL) {
                printf("No events in NoteBook\n");
            } else {
                printf("Choose sort option:\n\t1.Show all notes in date ascending order\n\t2.Show all notes in priority ascending order\n\t3.Show all notes that contain entered substring\n>>> ");
                char sort_option = ' '; 
                fflush(stdin);
                scanf("%c", &sort_option); 

                if (sort_option == '1') {
                    show_dateasc(pRoot);
                } else if (sort_option == '2') {
                    for (int i = 0; i <= 10; i++) {
                        show_priorityasc(pRoot, &i);
                    }
                } else if (sort_option == '3') {
                    char substring[MAXSUBSTRINGLEN] = { 0 };
                    printf("Enter substring to search in place field >>> ");
                    scanf("%s", substring);
                    printf("Substring was found in these events:\n");
                    show_substring(pRoot, substring);
                } else {
                    printf("Error input detected!");
                }
            }

            break;
        case '2':

            struct Event* new_event = (struct Event*)malloc(sizeof(struct Event));
            if (new_event == NULL) {
                printf("Error occured while allocating memory");
            }
            new_event->year = get_correct_year();
            new_event->month = get_correct_month();
            new_event->day = get_correct_day(new_event->year, new_event->month);
            new_event->hour = get_correct_hour();
            new_event->minute = get_correct_minute();
            new_event->weekday = define_day(new_event->year, new_event->month, new_event->day);
            new_event->priority = get_correct_priority();

            new_event->description = (char*)malloc(sizeof(char) * FIELDMAXLEN);
            if (new_event->description == NULL) {
                printf("Error occured while allocating memory");
            }
            copy(new_event->description, get_correct_description(), FIELDMAXLEN);

            new_event->place = (char*)malloc(sizeof(char) * FIELDMAXLEN);
            if (new_event->place == NULL) {
                printf("Error occured while allocating memory");
            }
            copy(new_event->place, get_correct_place(), FIELDMAXLEN);

            if (pRoot == NULL) {
                pRoot = add_event(pRoot, new_event);
            } else {
                add_event(pRoot, new_event);
            }
            printf("Event successfully added!\n");

            break;
        case '3':
            if (pRoot == NULL) {
                printf("No events in NoteBook\n");
            } else {
                int id = 1;
                int choosen_id = 0;

                printf("All events: ");

                update_tree(pRoot, &id);
                show_dateasc(pRoot);

                printf("Select event ID that you want to delete >>> ");
                fflush(stdin);
                scanf("%d", &choosen_id);

                delete_node(pRoot, choosen_id);
            }

            break;
        case 'e':
            FILE* w_stream = fopen("notebook.txt", "w");
            printf("Saving all changes\n");
            save_all(pRoot, w_stream);

            free_tree(pRoot);
            printf("%d files was closed\n", _fcloseall());
            printf("Closing NoteBookApp...\n");
            return 0;
        default:
            printf("Detected input error!\n");
        }
    }
}