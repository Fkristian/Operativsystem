#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <mqueue.h>
#include <fcntl.h>
#include <string.h>

int main(void){

    int MAX_SIZE = 100;
    int MAX_NUM_MSG = 10;
    char *my_mq = "/mq";
    char buf[MAX_SIZE];
    
    mqd_t mqd;
    struct mq_attr attr;

    // Form the queue attributes
    attr.mq_maxmsg = MAX_NUM_MSG;
    attr.mq_msgsize = MAX_SIZE;

    // Create message queue
    mqd = mq_open(my_mq, O_RDONLY | O_CREAT, 0644, &attr);
    
    printf("Hi I'm Luke, i just got a message:\n");

    // Read the message from the message queue
    mq_receive(mqd, buf, MAX_SIZE, NULL);
    printf("Message: %s\n", buf);

    // Close the message queue
    mq_unlink(my_mq);
    mq_close(mqd);
  

    exit(0);
}