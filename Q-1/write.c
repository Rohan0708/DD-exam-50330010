#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "ioctl.h"

int main()
{
    /* code */
    int fd,result,BAUD_RATE = 9600,STP_BITS = 8;
    
    struct default_data ToSend={9600,1,0};

    char ubuff[100] = "This is from user space to producer..\n";

    fd = open("/dev/IOCTL",O_RDWR,0777);
    if(fd < 0)
    {
        printf("Error in open!!\n");
        exit(1);
    }

    write(fd,ubuff,sizeof(ubuff));
  // printf("producer o/p  : %s\n",ubuff);

    close(fd);
    return 0;
}
