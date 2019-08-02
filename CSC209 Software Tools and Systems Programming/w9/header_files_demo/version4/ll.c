/* This code is presented as an example of separate compilation and
 * header file usage. This version uses the header files correctly.
 * This file implements several operations on a linked list. 
 * In this version, we avoid the use of a global variable by passing
 * in the head of the list as an argument.
 * Pay special attention to when the pointer to the head of this 
 * may change and how that is handled by each function.
 */

#include <stdio.h>
#include <stdlib.h>
// Remember to NEVER EVER include a ".c" file. Even if you are getting
// compilation errors
#include "ll.h"



struct node *insert_head(struct node *head, int val) {
    struct node *new_node = malloc(sizeof(struct node));
    new_node->x = val;
    new_node->next = head;
    head = new_node;
    return head;
}

int delete_head(struct node **head_p) {
    struct node *temp = *head_p;
    int val = -1;
    if(*head_p != NULL) {
        *head_p = (*head_p)->next;
        val = temp->x;
        free(temp);
    }
    return val;
}

struct node *insert_rear(struct node *head, int val) {
    struct node *new_node = malloc(sizeof(struct node));
    new_node->x = val;
    new_node->next = NULL;

    if(head == NULL) {
        return new_node;
    } else {
        struct node *current = head;
        while(current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
        return head;
    }
}

int delete_rear(struct node **head_p) {
    int val = -1;
    if(*head_p == NULL) {
        return val;
    } else if((*head_p)->next == NULL) {
        val = (*head_p)->x;
        free(*head_p);
        *head_p = NULL;
    } else {
        struct node *current = (*head_p)->next;
        struct node *prev = *head_p;
        while(current->next != NULL) {
            prev = current;
            current = current->next;
        }
        val = current->x;
        free(prev->next);
        prev->next = NULL;
    }
    return val;
}

void print_list(struct node *head) {
    struct node *current = head;
    while(current != NULL) {
        printf("%d ", current->x);
        current = current->next;
    }
    printf("\n");
}
