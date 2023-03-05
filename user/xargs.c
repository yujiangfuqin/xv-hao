#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"


int
main(int argc, char *argv[])
{   
    char line[1024];
    char *param[MAXARG]; //prarm存储执行所需的参数：最大32
    char *cmd = argv[1];  //存储要执行的指令
    int cnt, argc_p = 0;    //argc_p记录param的参数数量(一定别忘了初始化)
    for (int i = 1; i < argc; i++) 
        param[argc_p++] = argv[i];

    while((cnt = read(0, line, 1024)) > 0){    //把命令读到line里
        
        if(fork() == 0){    //子进程执行exec
            
            char *temp = (char*)malloc(sizeof(line));
            int index = 0;

            for(int i = 0; i < cnt; i++){
                if(line[i] == ' ' || line[i] == '\n'){      //分隔出一个参数；
                    temp[index] = 0;
                    param[argc_p++] = temp;
                    index = 0;
                    temp = (char*)malloc(sizeof(line));                              //易错点，需要重新为temp分配一块内存，因为类型都是char*，不能接着用这块内存
                }
                else { temp[index++] = line[i]; }
            }
            temp[index] = 0;
            param[argc_p] = 0;
            exec(cmd, param);
        }

        // wait(0);   //父进程等待子进程结束
    }
    exit(0);
}