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
    char kbuff[60];

    fd = open("/dev/IOCTL",O_RDWR,0777);
    if(fd < 0)
    {
        printf("Error in open!!\n");
        exit(1);
    }

    read(fd,kbuff,sizeof(kbuff));
    printf("Kernel data read o/p\n  :...%s...\n",kbuff);

    close(fd);
    return 0;
}
