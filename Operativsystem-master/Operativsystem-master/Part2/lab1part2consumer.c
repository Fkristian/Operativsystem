#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <mqueue.h>
#include <fcntl.h>
#include <string.h>

int main(void){
    printf("Luke, I have a message for you\n");
    /*Extract the content of the text file to a string*/
    char *buffer;
    int length;
    FILE * f = fopen ("Loremipsum.txt", "r");
    
    fseek (f, 0, SEEK_END);
    length = ftell (f);
   
    fseek (f, 0, SEEK_SET);
    buffer = malloc (length);
    fread (buffer, 1, length, f);
    fclose (f);

   printf("%d", length);

    char *my_mq = "/mq";
    mqd_t mqd;
    mqd = mq_open(my_mq, O_WRONLY);

    mq_send(mqd, buffer, strlen(buffer), 0);

    mq_close(mqd);

    exit(0);
}