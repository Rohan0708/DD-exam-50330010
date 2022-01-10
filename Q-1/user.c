#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "ioctl.h"

struct stats
{
    int size;
    int kbuff[90];
    int r_w;
};

int main()
{
   

    fd = open("/dev/IOCTL",O_RDWR,0777);
    if(fd < 0)
    {
        printf("Error in opening device!!!\n");
        exit(1);
    }

    result = ioctl(fd, SET_BAUD_RATE, &BAUD_RATE);
    if(result < 0)
    {
        printf("\n Error in IOCTL\n");
        return -1;
    }

    ioctl(fd,GETSTATS,(struct stats*)&data);
    printf("size of r_w: %d",data->r_w);
    printf("size : %d",data->size);

    close(fd);
}