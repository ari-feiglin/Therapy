#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE (1024)

typedef struct snippet_s{
    char * user_input;
    char * therapist_talk;
    int following_snippets;
}snippet_t;

typedef enum bool_s {false=0, true=1}bool;

int get_next_string(int fd, char ** string);
int get_raw_input(char * prompt, char ** input);
int new_segment(int fd);
int conversation_part(int fd, char * first_input);
int lower(char * s1, char ** s2);
int therapize();
