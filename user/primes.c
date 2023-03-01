#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main()
{
    int number[36],p[2];
    int index = 1;
    int pid;
    for(int i = 2; i <= 35; i++){
        number[index] = i;
        index++;
    }

    while(index > 1){       // 当number里为空或是只有上一轮的prime时退出循环
        pipe(p);    //init pipe p    0 read   1 write
        if((pid = fork()) < 0){
            fprintf(2, "fork error!\n");
            exit(0);
        }
        else if(pid > 0){      //parent
            close(p[0]);
            for(int i = 1; i < index; i++){     //从1开始读，0存着本次素数
                write(p[1], &number[i], sizeof(number[i]));    //write 的大小是字节，所以要sizeof，之前用的都是字符，所以无所谓的
            }                   //write 第二个参数是指针变量，因此要传地址
            close(p[1]);
            wait(0);      //等子进程结束才行
            exit(0);
        }
        else {          //child
            int temp;
            index = 0;
            close(p[1]);
            while(read(p[0], &temp, sizeof(temp)) != 0){
                if(index == 0){
                    number[0] = temp;  //number[0]存着本次要输出的prime
                    index++;
                }
                else if(index > 0 && (temp % number[0]) != 0){
                    number[index] = temp;//存储不能被本次prime整除的数传给下一个child
                    index++;
                }
            }
            close(p[0]);
            fprintf(1,"prime %d\n",number[0]);
        }
    }

    exit(0);
}