#include "therapist.h"

int main(){
    int therapist_fd = -1;
    char * thing = "Hello";
    char * t = NULL;

    therapize();

cleanup:
    if(-1 != therapist_fd){
        close(therapist_fd);
    }

    return(0);
}