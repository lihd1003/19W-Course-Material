#include <stdio.h>
#include <stdlib.h>
#include "ll.h"


void insert_head(int val) {
    struct node *new_node = malloc(sizeof(struct node));
    new_node->x = val;
    new_node->next = head;
    head = new_node;
}

int delete_head() {
    struct node *temp = head;
    int val = -1;
    if(head != NULL) {
        head = head->next;
        val = temp->x;
        free(temp);
    }
    return val;
}

void insert_rear(int val) {
    struct node *new_node = malloc(sizeof(struct node));
    new_node->x = val;
    new_node->next = NULL;

    if(head == NULL) {
        head = new_node;
    } else {
        struct node *current = head;
        while(current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
}

int delete_rear() {
    int val = -1;
    if(head == NULL) {
        return val;
    } else if(head->next == NULL) {
        val = head->x;
        free(head);
        head = NULL;
    } else {
        struct node *current = head->next;
        struct node *prev = head;
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

void print_list() {
    struct node *current = head;
    while(current != NULL) {
        printf("%d ", current->x);
        current = current->next;
    }
    printf("\n");
}
