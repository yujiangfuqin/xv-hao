#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
    static char buf[DIRSIZ+1];
    char *p;

    for(p=path+strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;


    if(strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    //memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));  会在buf后面添置空格来实现对齐，导致比较字符串失败
    buf[strlen(p)] = 0;     //把后缀搞掉
    return buf;
}

void
find(char *path, char *file){
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path,0)) < 0){
        fprintf(2,"find: cannot open %s\n",path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2,"find: cannot stat %s\n",path);
        close(fd);
        return;
    }


    switch (st.type) {
        case T_FILE:
            if (strcmp(fmtname(path), file) == 0)
                printf("%s\n", path);
            break;
        case T_DIR:
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
                printf("find: path too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';             //这三步用于将path改为目录

            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                if(de.inum == 0)    //inum == 0 表明目录入口无效
                    continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                if (!strcmp(de.name, ".") || !strcmp(de.name, ".."))
                    continue;
                //不会重复在 . ..里去find
                find(buf, file);
            }
            break;
    }
    close(fd);
}

int
main(int argc, char *argv[])
{
    if(argc != 3){
        fprintf(2, "find: format error!\n");
        exit(0);
    }
    find(argv[1], argv[2]);
    exit(0);
}