#ifndef HCQ_H
#define HCQ_H

// Students are kept in order by time with newest arrival at the end 
struct student{
    char *name;
    time_t *arrival_time;
    struct course *course;
    struct student *next_overall;
    struct student *next_course;
};

/* TAs are kept in reverse order of their time of addition. Newest
   TAs are kept at the head of the list. */ 
struct ta{
    char *name;
    struct student *current_student;
    struct ta *next;
};

struct course{
    char code[7];
    char *description;
    struct student *head;
    struct student *tail;
    int helped;          // number of students who saw a TA
    int bailed;          // number of students who gave up waiting
    double wait_time;    // total elapsed waiting time for students 
    double help_time;    //  total time spent helping students 
};


typedef struct student Student;
typedef struct course Course;
typedef struct ta Ta;

// helper functions not directly related to only one command in the API
Student *find_student(Student *stu_list, char *student_name);
Ta *find_ta(Ta *ta_list, char *ta_name);
Course *find_course(Course *courses, int num_courses, char *course_code);
void release_current_student(Ta *ta);


// functions provided as the API to a help-centre queue
int add_student(Student **stu_list_ptr, char *student_name, char *course_num,
    Course *courses, int num_courses);
int give_up_waiting(Student **stu_list_ptr, char *student_name);

void add_ta(Ta **ta_list_ptr, char *ta_name);
int remove_ta(Ta **ta_list_ptr, char *ta_name);

//  if student is currently being served then this finishes this student
//    if there is no-one else waiting then the currently being served gets
//    set to null 
int take_next_overall(char *ta_name, Ta *ta_list, Student **stu_list_ptr);
int take_next_course(char *ta_name, Ta *ta_list, Student **stu_list_ptr, char *course_num, Course *courses, int num_courses);


// how many students are waiting in the queue -- report by course and overall
// could return dynamically allocated strings rather than print
//   or could leave this for A4 changes
void print_all_queues(Student *stu_list, Course *courses, int num_courses);

// list currently being served by current Tas
void print_currently_serving(Ta *ta_list);

// list all students in queue (for your own testing and debugging)
void print_full_queue(Student *stu_list);

// show stats by course 
int stats_by_course(Student *stu_list, char *course_code, Course *courses, int num_courses, Ta *ta_list);


/* 
Dynamically allocate space for the array course list and populate it
according to information in the configuration file config_filename
Return the number of courses in the array.
*/
int config_course_list(Course **courselist_ptr, char *config_filename);
#endif
