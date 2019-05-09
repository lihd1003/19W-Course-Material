#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "hcq.h"
#define INPUT_BUFFER_SIZE 256

/*
 * Return a pointer to the struct student with name stu_name
 * or NULL if no student with this name exists in the stu_list
 */
Student *find_student(Student *stu_list, char *student_name) {
	
    while (stu_list != NULL){
		if (strcmp(student_name, stu_list->name) == 0){
			return stu_list;
		}
		stu_list = stu_list->next_overall;
	}
	return NULL;
}



/*   Return a pointer to the ta with name ta_name or NULL
 *   if no such TA exists in ta_list. 
 */
Ta *find_ta(Ta *ta_list, char *ta_name) {
	
	while (ta_list != NULL){
		if (strcmp(ta_name, ta_list->name) == 0){
			return ta_list;
		}
		ta_list = ta_list->next;
	}
    return NULL;
}


/*  Return a pointer to the course with this code in the course list
 *  or NULL if there is no course in the list with this code.
 */
Course *find_course(Course *courses, int num_courses, char *course_code) {
	
    for (int i = 0; i < num_courses; i++){
		if (strcmp(course_code, courses[i].code) == 0){
			return &(courses[i]);
		}
	}
	return NULL;
}
    

/* Add a student to the queue with student_name and a question about course_code.
 * if a student with this name already has a question in the queue (for any
   course), return 1 and do not create the student.
 * If course_code does not exist in the list, return 2 and do not create
 * the student struct.
 * For the purposes of this assignment, don't check anything about the 
 * uniqueness of the name. 
 */
int add_student(Student **stu_list_ptr, char *student_name, char *course_code,
    Course *course_array, int num_courses) {
		
	// exceptions handling
	if (find_student(*stu_list_ptr, student_name) != NULL){
		return 1;
	}
	Course *course = find_course(course_array, num_courses, course_code);
	if (course == NULL){
		return 2;
	}
	
	
	// init a new student
	Student *new_stu = malloc(sizeof(Student));
	if (new_stu == NULL){
		perror("malloc for add new student");
		exit(1);
	}
	new_stu->name = malloc(strlen(student_name)+1);
	if (new_stu->name == NULL){
		perror("malloc for student name");
		exit(1);
	}
	strcpy(new_stu->name, student_name);
	new_stu->course = course;
	new_stu->arrival_time = malloc(sizeof(time_t *));
	if (new_stu->arrival_time == NULL){
		perror("malloc for student arrival_time");
		exit(1);
	}
	*(new_stu->arrival_time) = time(NULL);
	
	// update overall queue
	Student *curr = *stu_list_ptr;
	if (curr == NULL){
		*stu_list_ptr = new_stu;
	} else {
		while (curr->next_overall != NULL){
			curr = curr->next_overall;
		}
		curr->next_overall = new_stu;
	}
	
	// update next_course queue;
	if (course->tail == NULL){
		course->head = new_stu;
		course->tail = new_stu;
	} else {
		course->tail->next_course = new_stu;
		course->tail = new_stu;
	}
	
	return 0;
}


/* Student student_name has given up waiting and left the help centre
 * before being called by a Ta. Record the appropriate statistics, remove
 * the student from the queues and clean up any no-longer-needed memory.
 *
 * If there is no student by this name in the stu_list, return 1.
 */
int give_up_waiting(Student **stu_list_ptr, char *student_name) {
	
	// find student
    Student *student = find_student(*stu_list_ptr, student_name);
	if (student == NULL){
		return 1;
	}
	
	// update overall queue
	Student *curr = *stu_list_ptr;
	// if it is the head;
	if (curr == student){
		*stu_list_ptr = student->next_overall;
	} else {
		while (curr != NULL){
			if (curr->next_overall == student){
				curr->next_overall = student->next_overall;
				break;
			}
			curr = curr->next_overall;
		}
	}
	
	// update next_course queue
	Course *stu_course = student->course;
	stu_course->wait_time += (time(NULL) - *(student->arrival_time));
	stu_course->bailed++;
	Student *course_curr = stu_course->head;
	
	// if it is the head
	if (course_curr == student){
		stu_course->head = student->next_course;
	} else {
		while (course_curr != NULL){
			if (course_curr->next_course == student){
				course_curr->next_course = student->next_course;
				// if it is the tail
				if (course_curr->next_course == NULL){
					stu_course->tail = course_curr;
				}
				break;
			}
		}
	}
	
	// free malloc
	free(student->name);
	free(student->arrival_time);
	free(student);
	
	return 0;
}

/* Create and prepend Ta with ta_name to the head of ta_list. 
 * For the purposes of this assignment, assume that ta_name is unique
 * to the help centre and don't check it.
 */
void add_ta(Ta **ta_list_ptr, char *ta_name) {
	
    // create the new Ta
    Ta *new_ta = malloc(sizeof(Ta));
    if (new_ta == NULL) {
       perror("malloc for TA");
       exit(1);
    }
    new_ta->name = malloc(strlen(ta_name)+1);
    if (new_ta->name  == NULL) {
       perror("malloc for TA name");
       exit(1);
    }
    strcpy(new_ta->name, ta_name);
    new_ta->current_student = NULL;

    // insert into front of list
    new_ta->next = *ta_list_ptr;
    *ta_list_ptr = new_ta;
}

/* The TA ta is done with their current student. 
 * Calculate the stats (the times etc.) and then 
 * free the memory for the student. 
 * If the TA has no current student, do nothing.
 */
void release_current_student(Ta *ta) {
	
	// exceptions handling
	Student *stu = ta->current_student;
	if (stu == NULL){
		return;
	}
	
	// update course stats
	Course *cour = stu->course;
	(cour->helped)++;
	cour->help_time += (time(NULL) - *(stu->arrival_time));
	
	// free malloc
	free(stu->arrival_time);
	free(stu->name);
	free(stu);
	
	// empty the ta for the next student
	ta->current_student = NULL;
}

/* Remove this Ta from the ta_list and free the associated memory with
 * both the Ta we are removing and the current student (if any).
 * Return 0 on success or 1 if this ta_name is not found in the list
 */
int remove_ta(Ta **ta_list_ptr, char *ta_name) {
    Ta *head = *ta_list_ptr;
    if (head == NULL) {
        return 1;
    } else if (strcmp(head->name, ta_name) == 0) {
        // TA is at the head so special case
        *ta_list_ptr = head->next;
        release_current_student(head);
        // memory for the student has been freed. Now free memory for the TA.
        free(head->name);
        free(head);
        return 0;
    }
    while (head->next != NULL) {
        if (strcmp(head->next->name, ta_name) == 0) {
            Ta *ta_tofree = head->next;
            //  We have found the ta to remove, but before we do that 
            //  we need to finish with the student and free the student.
            //  You need to complete this helper function
            release_current_student(ta_tofree);

            head->next = head->next->next;
            // memory for the student has been freed. Now free memory for the TA.
            free(ta_tofree->name);
            free(ta_tofree);
            return 0;
        }
        head = head->next;
    }
    // if we reach here, the ta_name was not in the list
    return 1;
}


/* TA ta_name is finished with the student they are currently helping (if any)
 * and are assigned to the next student in the full queue. 
 * If the queue is empty, then TA ta_name simply finishes with the student 
 * they are currently helping, records appropriate statistics, 
 * and sets current_student for this TA to NULL.
 * If ta_name is not in ta_list, return 1 and do nothing.
 */
int take_next_overall(char *ta_name, Ta *ta_list, Student **stu_list_ptr) {
	// find ta
	Ta *ta = find_ta(ta_list, ta_name);
	if (ta == NULL){
		return 1;
	}

	release_current_student(ta);
	
	// update overall queue
	Student *taken_stu = *stu_list_ptr;
	if (taken_stu == NULL){
		return 0;
	}
	*stu_list_ptr = taken_stu->next_overall;
	
	// update course stats and student timing
	Course *stu_course = taken_stu->course;
	stu_course->wait_time += (time(NULL) - *(taken_stu->arrival_time));
	*(taken_stu->arrival_time) = time(NULL);
	
	// update course queue
	stu_course->head = taken_stu->next_course;
	if (stu_course->tail == taken_stu){
		stu_course->tail = NULL;
	}
	
	// clear student queue info 
	taken_stu->next_course = NULL;
	taken_stu->next_overall = NULL;
	
	
	// add it to the ta
	ta->current_student = taken_stu;
	
    return 0;
}



/* TA ta_name is finished with the student they are currently helping (if any)
 * and are assigned to the next student in the course with this course_code. 
 * If no student is waiting for this course, then TA ta_name simply finishes 
 * with the student they are currently helping, records appropriate statistics,
 * and sets current_student for this TA to NULL.
 * If ta_name is not in ta_list, return 1 and do nothing.
 * If course is invalid return 2, but finish with any current student. 
 */
int take_next_course(char *ta_name, Ta *ta_list, Student **stu_list_ptr, char *course_code, Course *courses, int num_courses) {
    
	Ta *ta = find_ta(ta_list, ta_name);
	if (ta == NULL){
		return 1;
	}
	release_current_student(ta);
	
	Course *taken_course = find_course(courses, num_courses, course_code);
	if (taken_course == NULL){
		return 2;
	}
	
	Student *taken_stu = taken_course->head;
	if (taken_stu == NULL){
		return 0;
	}
	
	// update course stats and student timing
	taken_course->wait_time += (time(NULL) - *(taken_stu->arrival_time));
	*(taken_stu->arrival_time) = time(NULL);
	
	// update course queue
	if (taken_course->tail == taken_stu){
		taken_course->tail = NULL;
	}
	taken_course->head = taken_stu->next_course;
	
	// update overall queue
	if (taken_stu == *stu_list_ptr){
		*stu_list_ptr = taken_stu->next_overall;
	} else {
		Student *curr = *stu_list_ptr;
		while (curr->next_overall != taken_stu){
			curr = curr->next_overall;
		}
		curr->next_overall = taken_stu->next_overall;
	}
	
	// clear student queue info
	taken_stu->next_overall = NULL;
	taken_stu->next_course = NULL;
	
	ta->current_student = taken_stu;
	
    return 0;
}


/* For each course (in the same order as in the config file), print
 * the <course code>: <number of students waiting> "in queue\n" followed by
 * one line per student waiting with the format "\t%s\n" (tab name newline)
 * Uncomment and use the printf statements below. Only change the variable
 * names.
 */
void print_all_queues(Student *stu_list, Course *courses, int num_courses) {
	
	for (int i = 0; i < num_courses; i++){
		Course curr_course = courses[i];
		int num_waiting = 0;
		Student *curr_stu = curr_course.head;
		while (curr_stu != NULL){
			curr_stu = curr_stu->next_course;
			num_waiting++;
		}
        printf("%s: %d in queue\n", curr_course.code, num_waiting);
		Student *curr_stut = curr_course.head;
		while (curr_stut != NULL){
			printf("\t%s\n",curr_stut->name); 
			curr_stut = curr_stut->next_course;
		}
	}
}


/*
 * Print to stdout, a list of each TA, who they are serving at from what course
 * Uncomment and use the printf statements 
 */
void print_currently_serving(Ta *ta_list) {
	
	if (ta_list == NULL){
		printf("No TAs are in the help centre.\n");
	}
	
	Ta *curr_ta = ta_list;
	while (curr_ta != NULL){
		if (curr_ta->current_student != NULL){
			printf("TA: %s is serving %s from %s\n",curr_ta->name, 
			curr_ta->current_student->name, curr_ta->current_student->course->code);
		}
		else {
			printf("TA: %s has no student\n", curr_ta->name);
		}
		curr_ta = curr_ta->next;
	}
}


/*  list all students in queue (for testing and debugging)
 *   maybe suggest it is useful for debugging but not included in marking? 
 */ 
void print_full_queue(Student *stu_list) {
	Student *curr = stu_list;
	while (curr != NULL){
		printf("%s %s\n", curr->name, curr->course->code);
		curr = curr->next_overall;
	}
}

/* Prints statistics to stdout for course with this course_code
 * See example output from assignment handout for formatting.
 *
 */
int stats_by_course(Student *stu_list, char *course_code, Course *courses, int num_courses, Ta *ta_list) {
	
	// find the course
	Course *found = find_course(courses, num_courses, course_code);
	if (found == NULL){
		return 1;
	}
	
	// count how many students are waiting for the course
	Student *waiting_stu = found->head;
	int students_waiting = 0;
	while (waiting_stu != NULL){
		students_waiting++;
		waiting_stu = waiting_stu->next_course;
	}
		
	// count how many students are being helped 
	int students_being_helped = 0;
	Ta *cur_ta = ta_list;
	while (cur_ta != NULL){
		if (cur_ta->current_student != NULL && cur_ta->current_student->course == found){
			students_being_helped++;
		}
		cur_ta = cur_ta->next;
	}
	
	printf("%s:%s \n", found->code, found->description);
	printf("\t%d: waiting\n", students_waiting);
	printf("\t%d: being helped currently\n", students_being_helped);
	printf("\t%d: already helped\n", found->helped);
	printf("\t%d: gave_up\n", found->bailed);
	printf("\t%f: total time waiting\n", found->wait_time);
	printf("\t%f: total time helping\n", found->help_time);
	
	return 0;
}


/* Dynamically allocate space for the array course list and populate it
 * according to information in the configuration file config_filename
 * Return the number of courses in the array.
 * If the configuration file can not be opened, call perror() and exit.
 */
int config_course_list(Course **courselist_ptr, char *config_filename) {
	
    FILE *config_file = fopen(config_filename, "r");
	if (config_file == NULL){
		perror("Config file not found");
		exit(1);
	}
	
	char cur_read[INPUT_BUFFER_SIZE + 1];
	fgets(cur_read, INPUT_BUFFER_SIZE + 1, config_file);
	int num_courses = atol(cur_read);
	 
	*courselist_ptr = malloc(sizeof(Course) * num_courses);
	if (courselist_ptr == NULL){
		perror("malloc for course list pointer");
		exit(1);
	}
	
	int index = 0;
	while (fgets(cur_read, INPUT_BUFFER_SIZE, config_file)){

		char *new_descrip = malloc(sizeof(char) * (INPUT_BUFFER_SIZE-6));
		if (new_descrip == NULL){
			perror("malloc for course description");
			exit(1);
		}
		
		sscanf(cur_read, "%[A-Z0-9] %[^\n]", (*courselist_ptr)[index].code, new_descrip);
		(*courselist_ptr)[index].description = new_descrip;
		(*courselist_ptr)[index].helped = 0;
		(*courselist_ptr)[index].bailed = 0;
		(*courselist_ptr)[index].wait_time = 0;
		(*courselist_ptr)[index].help_time = 0;
		index++;
	}
	
	fclose(config_file);
	
	return num_courses;
}
