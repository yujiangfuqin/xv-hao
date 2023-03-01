#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int
main(int argc, char *argv[])
{
    if(argc <=1) {
        fprintf(2, "sleep: you should input an argument!\n");
    }
    else {
        int i = atoi(argv[1]);
        sleep(i);
    }
    exit(0);
}