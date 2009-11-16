#ifndef NOD_H
#define NOD_H

#ifndef NAME_LENGTH
	#define NAME_LENGTH 42
#endif

struct post {
    char name[NAME_LENGTH];
    float bmi;
    struct post * next;
};

typedef struct post Post;

void writePost(Post * p);

void writeList(Post * p);

/* find tar en funktionspekare som första parameter */
Post * find(int (* compare) (Post * a, Post * b), Post seeked, Post * list);

int compare_names(Post * a, Post * b);

int compare_bmi(Post * a, Post * b);

void insert(Post ** list);

void load_names(char * filename, Post ** list);

int remove_person(Post **list, char *name);

#endif

