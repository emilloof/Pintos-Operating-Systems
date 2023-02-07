#include <stdio.h>
#include <stdlib.h>



struct list_item{
    int value ;
    struct list_item * next;
};

void append(struct list_item * first, int x){ 
/* puts x at the end
of the list */
    struct list_item* last = (struct list_item*) malloc(sizeof(struct list_item));
    last->value = x;
    last->next = NULL;
    struct list_item* cur;
    struct list_item* prev;
    cur = first;
        while(cur->next != NULL){
        cur = cur->next;
        }
    cur->next = last;
}



void prepend(struct list_item * first , int x){
 /* puts x at the
beginning of the list */
    struct list_item* newfirst = (struct list_item*) malloc(sizeof(struct list_item));
    struct list_item* temp = first->next;
    first->next = newfirst;
    newfirst->next = temp;
    newfirst->value = x;
}


void print(struct list_item * first){
/* prints all elements in
the list */
    struct list_item *cur = first->next;
    while(cur != NULL){
        printf("%d \n", cur->value);
        cur = cur->next;
    }
}


void input_sorted(struct list_item *first , int x){
/* input sorted : find the first element in the list larger than x
and input x right before that element */
    struct list_item *cur = first;
    struct list_item* newElem = (struct list_item*) malloc(sizeof(struct list_item));
    newElem->value = x;
    struct list_item*  prev = first;

    while(cur->next !=NULL){
        if(cur->value < x)
        {
            prev = cur; 
            cur = cur->next;
        }
        else{
            newElem->next = cur;
            prev->next = newElem;
            break;
        }
    }
}


void clear(struct list_item *first){
 /* free everything dynamically allocated */
    struct list_item* next;
    while(first->next != NULL){
        next = first->next;
        first->next = next->next;
        free(next);
    }
}


int main( int argc, char ** argv) {
struct list_item root;
root.value = -1; /*This value is always ignored */
root.next =NULL;
    append(&root, 1);
    append(&root, 2);
    append(&root, 3);
    append(&root, 4);
    append(&root, 6);
    append(&root, 7);
    prepend(&root, -10);
    input_sorted(&root,5);
    print(&root);
    clear(&root);
    append(&root, 100);
    print(&root);
    print(&root);
}