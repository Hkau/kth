#include <stdlib.h>
#include <stdio.h>

struct post {
    char name[29];
    float bmi;
    struct post * next;
};

typedef struct post Post;

void writePost(Post * p) {
    printf("Namn: %s\nbmi: %f\n", p->name, p->bmi); 
}

int main(int argc, char * argv[]) {
    int vikt;
    float lengd;
    Post * p = (Post *) malloc(sizeof(Post));
    p -> next = (Post *) malloc(sizeof(Post));

    printf("Vad heter du? ");
    fscanf(stdin, "%s", p->name);

    printf("Hur lång är du (m)? ");
    fscanf(stdin, "%f", &lengd);

    printf("Vad väger du (kg)? ");
    fscanf(stdin, "%d", &vikt);

    p -> bmi = vikt / (lengd * lengd);

    writePost(p);

    free(p->next);
    free(p);

    return 0;
}
