/* Remember that a header file contains
 *   - type definitions
 *   - function signatures
 *   - defined constants or macros  (We haven't talked about macros)
 *	 - extern variables
 * A header file must not include global variable definitions, function bodies
 * or anything else that requires memory to be allocated.
 */

struct node {
    int x;
    struct node *next;
}; 

struct node *insert_head(struct node *head, int val);
int delete_head(struct node **head_p);
struct node *insert_rear(struct node *head, int val);
int delete_rear(struct node **head_p);
void print_list(struct node *head);
