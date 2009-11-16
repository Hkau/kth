#include <stdlib.h>
#include <stdio.h>
#include <string.h> // fixar strcpy :)
#include "nod.h"

void writePost(Post * p) {
	printf("Namn: %s\nbmi: %f\n",p->name, p->bmi);
}

void writeList(Post * p) {
	for(;p != NULL; p = p->next)
		writePost(p);
}

Post * find(int (* compare) (Post *  a, Post * b), Post seeked, Post * list) {
	for(;list!=NULL;list=list->next)
		if(compare(&seeked, list) == 0)
			return list;
	return NULL;
}

int compare_names(Post * a, Post * b) {
	return strcmp(a->name, b->name);
}

int compare_bmi(Post * a, Post * b) {
	return b->bmi-a->bmi;
}

void insert(Post ** list) {
	Post *p = malloc(sizeof(Post));


	printf("Vad heter personen? ");
	scanf("%s", p->name);

	float m;
	printf("Hur lång är personen (m)? ");
	scanf("%f", &m);

	float kg;
	printf("Hur mycket väger personen? "); // %d om man vill, men va?
	scanf("%f", &kg);

	p->bmi = kg/(m*m);

	p->next = *list;
	*list = p;
}

int remove_person(Post **list, char *name)
{
	Post *p = *list;
	if(strcmp(p->name, name) == 0)
	{
		free(*list);
		*list = p->next;
		return 1;
	}
	for(; p->next != NULL; p = p->next)
	{
		if(strcmp(p->next->name, name) == 0)
		{
			Post *tmp = p->next;
			p->next = p->next->next;
			free(tmp);
			return 1;
		}
	}

	return 0;
}

void load_names(char * filename, Post ** list) {
    char name[NAME_LENGTH];
    float bmi;
    FILE *fil = fopen(filename, "r");
    Post * p;
    if (fil == NULL) {
	printf("Filen inte funnen.\n");
    } else {
        while (fscanf(fil, "%s %f", name, &bmi) == 2) {
            p = (Post *) malloc(sizeof(Post));
            strcpy(p->name, name);
            p->bmi = bmi;
            
            p->next = *list;
            *list = p;
        }
    }
}

