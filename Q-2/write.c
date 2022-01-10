// WRITER .........

#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc,const char *argv[])
{
    int fd;
    
    char ubuff[60]="HIII WORK DONE..\n";
    fd=open("/dev/NAME",O_RDWR,0777);
    if(fd<0)
    {
        printf("ERROR IN OPENING..\n");
        exit(1);
    }
    write(fd,ubuff,sizeof(ubuff));
    
    close(fd);
    return 0;
}