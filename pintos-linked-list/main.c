#include "list.h"
#include "list.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct student {
	char *name;
	struct list_elem elem;
};


void insert (struct list *student_list) {
	struct student *stu =  (struct student*) malloc(sizeof(struct student));
	printf("Type the name of the student: ");
	char *name = (char*) malloc(20*sizeof(char));
	scanf("%s", name);
	stu->name = name;
	struct list_elem *elem = &(stu->elem);
	list_push_back(student_list, &stu->elem);
}

void delete (struct list *student_list) {
	struct list_elem *e = list_head(student_list);
	struct student *stu;
	printf("Type the name of the student: ");
	char name[20];
	scanf("%s", name);
	while((e = list_next(e)) != list_end(student_list)) 
    	{
			stu = list_entry(e, struct student, elem);			
			if(strcmp(stu->name, name)== 0){	
				list_remove(e);
				free(stu);
			}
     	}


}

void list (struct list *student_list) {
	struct list_elem *elem;
	struct student *stu;
	elem = list_head(student_list);	
	while((elem = list_next(elem)) != list_end(student_list)) 
    	{
			stu = list_entry(elem, struct student, elem);
			printf("%s\n", stu->name);
     	}
}

void quit (struct list *student_list) {
	struct list_elem *e;
	struct student *stu;
	while (!list_empty (student_list))
     {
    	e = list_pop_front (student_list);
	   	stu = list_entry(e, struct student, elem);
        list_remove(e);
	    free(stu);
     }
	exit(0);
}

int main() {
	struct list student_list;
	list_init(&student_list);
	int opt;

	do {
		printf("Menu:\n");
		printf("1 - Insert student\n");
		printf("2 - Delete student\n");
		printf("3 - List students\n");
		printf("4 - Exit\n");
		scanf("%d", &opt);
		switch (opt) {
			case 1:
				{
					insert(&student_list);
					break;
				}
			case 2:
				{
					delete(&student_list);
					break;
				}
			case 3:
				{
					list(&student_list);
					break;
				}

			case 4:
				{
					quit(&student_list);
					break;
				}
			default:
				{
					printf("Quit? (1/0):\n");
					scanf("%d", &opt);
					if (opt)
						quit(&student_list);
					break;
				}
		}
	} while(1);
		
	return 0;
}
