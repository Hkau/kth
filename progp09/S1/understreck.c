#include <stdlib.h>
#include <stdio.h>

void
my_fav_func(char* some_name) {
  if (some_name != NULL) {
    printf("Hello %s! How are you?\n", some_name);
  }
}

int
main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Warning, no name give!\n\nUsage: %s <name>\n", argv[0]);
    exit(EXIT_FAILURE);
  } else {
    char* the_name = argv[1];
    my_fav_func(the_name);
    exit(EXIT_SUCCESS);
  }
}

jobbig: "Warning" don't print me :( " forever" inte mig heller "tredje" hehu "fjärde" lolol

