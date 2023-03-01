#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
    int p1[2],p2[2];
    char buf[10];

    pipe(p1);
    pipe(p2);

    int pid;

    if((pid = fork()) < 0) {
        fprintf(2,"fork error\n");
        exit(0);
    }
    else if(pid > 0) {   //father
        close(p1[0]);       //  0:read    1:write
        close(p2[1]);
        write(p1[1],"ping",4);      //p2 child write  p1 father write
        close(p1[1]);               //p2 father read  p1 child read
        read(p2[0],buf,4);
        fprintf(1,"%d: received %s\n", getpid(), buf);
    }
    else {   //child
        close(p1[1]);
        close(p2[0]);
        read(p1[0],buf,4);
        fprintf(1,"%d: received %s\n", getpid(), buf);
        write(p2[1],"pong",4);
        close(p2[1]);
    }



    exit(0);
}
