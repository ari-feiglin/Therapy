#include "therapist.h"

int get_raw_input(char * prompt, char ** input){
    ssize_t size = 0;
    int error_check = 0;

    if(NULL != prompt){
        printf("%s", prompt);
    }

    error_check = getline(input, &size, stdin);
    if(-1 == error_check){
        perror("GET_RAW_INPUT: getline error");
        printf("(Errno: %i)\n", errno);
        goto cleanup;
    }

    (*input)[error_check-1] = 0;

cleanup:
    return error_check;
}

int lower(char * s1, char ** s2){
    int bytes_read = 0;
    int i = 0;
    int s1_len = 0;
    char temp_s2[BUFFER_SIZE] = {0};

    s1_len = strnlen(s1, BUFFER_SIZE);

    for(i=0; i<s1_len; i++){
        if('A' <= s1[i] && s1[i] <= 'Z'){
            temp_s2[i] = s1[i] + 'a' - 'A';
        }
        else{
            temp_s2[i] = s1[i];
        }

        bytes_read++;
    }

    *s2 = malloc(s1_len + 1);
    if(NULL == *s2){
        perror("LOWER: Malloc error");
        printf("(Errno: %i)\n", errno);
        bytes_read = -1;
        goto cleanup;
    }
    
    memcpy(*s2, temp_s2, bytes_read+1);

cleanup:
    return bytes_read;
}