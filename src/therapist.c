#include "therapist.h"

int get_next_string(int fd, char ** string){
    char buffer[BUFFER_SIZE] = {0};
    int error_check = 1;
    int i = 0;
    int bytes_read = 0;

    for(i=0; 1 == error_check; i++){
        error_check = read(fd, &buffer[i], sizeof(buffer[i]));
        if(-1 == error_check){
            perror("GET_NEXT_STRING: Read error");
            printf("(Errno: %i)\n", errno);
            bytes_read = -1;
            goto cleanup;
        }

        bytes_read += error_check;

        if(0 == buffer[i]){
            break;
        }
    }

    *string = malloc(bytes_read);
    if(NULL == string){
        perror("GET_NEXT_STRING: Malloc error");
        printf("(Errno: %i)\n", errno);
        bytes_read = -1;
        goto cleanup;
    }

    memcpy(*string, buffer, bytes_read + 1);

cleanup:
    return bytes_read;
}

int get_next_snippet(int fd, snippet_t * snippet){
    int error_check = 0;
    int bytes_read = 0;

    error_check = get_next_string(fd, &snippet->user_input);
    if(-1 == error_check){
        bytes_read = -1;
        goto cleanup;
    }
    bytes_read += error_check;

    error_check = get_next_string(fd, &snippet->therapist_talk);
    if(-1 == error_check){
        bytes_read = -1;
        goto cleanup;
    }
    bytes_read += error_check;

    error_check = read(fd, &snippet->following_snippets, sizeof(snippet->following_snippets));
    if(-1 == error_check){
        bytes_read = -1;
        goto cleanup;
    }
    bytes_read += error_check;

cleanup:
    return bytes_read;
}

int pass_over_segment(int fd, int num_of_subsegments){
    int i = 0;
    snippet_t snippet = {0};
    int error_check = 0;

    for(i=0; i<num_of_subsegments; i++){
        error_check = get_next_snippet(fd, &snippet);
        if(-1 == error_check){
            goto cleanup;
        }

        if(snippet.following_snippets > 0){
            pass_over_segment(fd, snippet.following_snippets);
        }
    }

cleanup:
    return error_check;
}

int conversation_part(int fd, char * first_input){
    char * input = NULL;
    int error_check = 0;
    int difference = 0;
    int num_of_snippets = -1;
    int curr_snippet = 0;
    bool found = true;
    snippet_t snippet = {0};

    error_check = lseek(fd, 0, SEEK_SET);
    if(-1 == error_check){
        perror("CONVERSATION_PART: Lseek error");
        printf("(Errno: %i)\n", errno);
        goto cleanup;
    }

    input = first_input;

    while(true){
        do{
            error_check = get_next_snippet(fd, &snippet);
            if(-1 == error_check){
                goto cleanup;
            }
            else if(0 == error_check){
                found = false;
                break;
            }

            error_check = lower(snippet.user_input, &snippet.user_input);
            if(-1 == error_check){
                goto cleanup;
            }
            error_check = lower(input, &input);
            if(-1 == error_check){
                goto cleanup;
            }
            
            difference = strncmp(input, snippet.user_input, BUFFER_SIZE);
            
            if(0 != difference && snippet.following_snippets > 0){
                pass_over_segment(fd, snippet.following_snippets);
            }

            curr_snippet++;
        }while(difference != 0 && error_check > 0 && (curr_snippet < num_of_snippets || num_of_snippets == -1));

        if(found && difference == 0){
            printf("%s\n", snippet.therapist_talk);
            if(NULL != input){
                free(input);
                input = NULL;
            }
            if(snippet.following_snippets > 0){
                error_check = get_raw_input(">> ", &input);
                if(-1 == error_check){
                    goto cleanup;
                }
            }
            else{
                break;
            }

            curr_snippet = 0;
            num_of_snippets = snippet.following_snippets;
        }
        else{
            puts("I don't know what to say to that");
            break;
        }
    }

cleanup:
    if(NULL != input){
        free(input);
    }

    return error_check;
}

int write_snippet(int fd, snippet_t snippet){
    int error_check = 0;

    error_check = write(fd, snippet.user_input, strnlen(snippet.user_input, BUFFER_SIZE) + 1);
    if(-1 == error_check){
        perror("WRITE_SNIPPET: Write error");
        printf("(Errno: %i)\n", errno);
        goto cleanup;
    }

    error_check = write(fd, snippet.therapist_talk, strnlen(snippet.therapist_talk, BUFFER_SIZE) + 1);
    if(-1 == error_check){
        perror("WRITE_SNIPPET: Write error");
        printf("(Errno: %i)\n", errno);
        goto cleanup;
    }

    error_check = write(fd, &snippet.following_snippets, sizeof(snippet.following_snippets));
    if(-1 == error_check){
        perror("WRITE_SNIPPET: Write error");
        printf("(Errno: %i)\n", errno);
        goto cleanup;
    }

cleanup:
    return error_check;
}

int new_segment(int fd){
    char * user_prompt = NULL;
    char * therapist_reponse = NULL;
    char * num_of_responses = NULL;
    int error_check = 0;
    int i = 0;
    snippet_t snippet = {0};

    error_check = get_raw_input("Original user prompt: ", &user_prompt);
    if(-1 == error_check){
        goto cleanup;
    }

    error_check = get_raw_input("Therapist response: ", &therapist_reponse);
    if(-1 == error_check){
        goto cleanup;
    }

    error_check = get_raw_input("Number of responses: ", &num_of_responses);
    if(-1 == error_check){
        goto cleanup;
    }

    snippet.therapist_talk = therapist_reponse;
    snippet.user_input = user_prompt;
    snippet.following_snippets = (int)strtol(num_of_responses, NULL, 10);

    error_check = lseek(fd, 0, SEEK_END);
    if(-1 == error_check){
        perror("CONVERSATION_PART: Lseek error");
        printf("(Errno: %i)\n", errno);
        goto cleanup;
    }

    error_check = write_snippet(fd, snippet);
    if(-1 == error_check){
        goto cleanup;
    }

    if(NULL != user_prompt){
        free(user_prompt);
    }
    if(NULL != therapist_reponse){
        free(therapist_reponse);
    }
    if(NULL != num_of_responses){
        free(num_of_responses);
    }

    for(i=0; i<snippet.following_snippets; i++){
        new_segment(fd);
    }

cleanup:
    return error_check;
}

int therapize(){
    int fd = 0;
    int error_check = -1;
    int difference = 0;
    char * input = NULL;

    fd = open("therapy", O_RDWR | O_CREAT, 0666);
    if(-1 == fd){
        perror("THERAPIZE: Open error");
        printf("(Errno: %i)\n", errno);
        goto cleanup;
    }

    while(true){
        error_check = get_raw_input(">> ", &input);
        if(-1 == error_check){
            goto cleanup;
        }

        difference = strncmp("dev mode", input, BUFFER_SIZE);
        if(0 == difference){
            new_segment(fd);
        }
        else{
            conversation_part(fd, input);
        }
    }



cleanup:
    return error_check;
}
