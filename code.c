#include <stdio.h>
#include <stdlib.h>



struct list_item{
    int value ;
    struct list_item * next;
};

void append(struct list_item * first, int x){ 
/* puts x at the end
of the list */
printf("yo");
    struct list_item* last = (struct list_item*) malloc(sizeof(struct list_item));
    last->value = x;
    last->next = NULL;
  //  if(first != NULL){
        while(first->next != NULL){
            first = first->next;            
        }
        first->next = last;
   // }
}


void prepend(struct list_item * first , int x){
 /* puts x at the
beginning of the list */
    struct list_item* newfirst = (struct list_item*) malloc(sizeof(struct list_item));
    newfirst->next = first;
    newfirst->value = x;

 //   struct list_item *next = first->next;
   // while(cur != NULL){
     //   cur->next = cur;
       // cur = next;
    //}
//    first->value = x;
}


void print(struct list_item * first){
/* prints all elements in
the list */
    struct list_item *cur = first;
    printf("HEJ");
    while(cur != NULL){
        printf("%d", cur->value);
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

    while(cur!=NULL){
        if(cur->value < x)
        {
            prev = cur;
            cur = cur->next;
        }
        else{
            newElem->next = cur;
            prev->next = newElem;
        }
    }
}


void clear(struct list_item *first){
 /* free everything dynamically allocated */
    struct list_item* next;
    while(first != NULL){
        next = first->next;
        free(first);
        first = next;
    }
}


int main( int argc, char ** argv) {
    printf("HELLO");
struct list_item root;
root.value = -1; /*This value is always ignored */
root.next =NULL;
    append(root.next, 5);
    print(root.next);
    clear(root.next);
}