#include <stdio.h>      // include functions for input and output
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    int data;
    struct Node *prev;  // * points to another place in memory
    struct Node *next;  
} Node;

typedef struct {
    Node *head;  // don't need to specify struct since it is declared after struct Node typedef
    int size;
} ClinkedList;

// forward declaration so functions can all access each other
Node *new_node(int data, Node *prev, Node *next);
int add_to_end(ClinkedList *list, int data);
void print_list(const ClinkedList *list);
int init_list(ClinkedList *list);
// TODO 
void free_list(ClinkedList *list);
void insert_at_pos(ClinkedList *list, int pos, int data);
int set(ClinkedList *list, int pos, int data);  // return old val we just changed
int get(const ClinkedList *list, int pos);
Node *_get_node(const ClinkedList *list, int pos);     // underscore in front signifies "private" use
int delete(ClinkedList *list, int pos);     // return val of node removed
bool remove_first_of(ClinkedList *list, int data);
int *get_sub_list (const ClinkedList *list, int start, int stop);
int index_of(const ClinkedList *list, int data);  // return pos of first occurrence of data
int index_after(const ClinkedList *list, int start, int data);    // return pos of first occurrence after start
int clear_list(ClinkedList *list);
void remove_range(ClinkedList *list, int start, int stop);
void equals(const ClinkedList *this, const ClinkedList *other);

int main(int argc, char **argv) {
    ClinkedList list;
    if (!init_list(&list)) {    // when sending a memory address of a variable in, use &
        fprintf(stderr, "couldn't create a new list :(");
        return 1;
    }
    add_to_end(&list, 10);
    add_to_end(&list, 11);
    print_list(&list);
    free_list(&list);


}

// make methods + constructor + deconstructor outside of structs
// structs are meant only to store data
int init_list(ClinkedList *list) {
    if (!list) {
        return 0;
    }
    // create the header node and adjust references
    list->head = new_node(0, NULL, NULL);
    list->head->next = list->head;
    list->head->prev = list->head;
    list->size = 0;
    return 1;
}

Node *new_node(int data, Node *prev, Node *next) {
    Node *n = (Node *)(malloc(sizeof(Node)));
    if (!n) {    // n is a null pointer
        fprintf(stderr, "failed to allocate memory for new node");
        return NULL;
    }
    n->data = data;
    n->prev = prev;
    n->next = next;
    return n;
}

int add_to_end(ClinkedList *list, int data) {
    if (!list->head) {
        fprintf(stderr, "this list header is null meow!! >:3\n");
        return 0;   // 0 = failure, 1 = success
    }
    Node *last = list->head->prev;
    Node *n = new_node(data, last, list->head);
    last->next = n;
    list->head->prev = n;
    list->size++;
    return 1;
}

void print_list(const ClinkedList *list) {   // like before, * = points to mem address, but const allows us to not accidentally change datue
    /* pass by pointer so we can save memory, const doesn't need memory
     * if we didn't have *, would make a local shallow copy of list that takes unnecessary memory
     * since we aren't altering the list, we can just look at the original copy */

    // don't start at head if nonempty
    Node *temp = list->head->next;
    putchar('{');
    // address hanging comma by just printing first if nonempty
    if (list->size != 0) {
        printf("%d", temp->data);
        temp = temp->next;
    }
    for (int i = 1; i < list->size; i++) {  // if empty, won't run
        printf(", %d", temp->data);
        temp = temp->next;
    }
    puts("}");
}

// free the memory the list is taking up
void free_list(ClinkedList *list) {
    clear_list(list);
    free(list->head);
    list = NULL;
}

int clear_list(ClinkedList *list) {
    if (!list->head) {
        fprintf(stderr, "you didn't initialize the list silly 8_8");
        return 0;
    }
    Node *current = list->head->next->next;
    while (current != list->head) {
        free(current->prev);
        current = current->next;
    }
    free(current->prev);    // address picket fence issue

    list->head->next = list->head;
    list->head->prev = list->head;

    return 1;
}
