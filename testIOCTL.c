#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<sys/ioctl.h>
 
#define WR_VAL _IOW('a',1,int32_t*)
#define RD_VAL _IOR('a',2,int32_t*)
 
int main()
{
        int fd;
        int32_t value, number;        
 
        printf("\n Opening Driver\n");
        fd = open("/dev/device1", O_RDWR);
        if(fd < 0) {
                printf("Cannot open device file...\n");
                return 0;
        }
 
        printf("Enter the Value to send\n");
        scanf("%d",&number);
        printf("Writing Value to Driver\n");
        ioctl(fd, WR_VAL, (int32_t*) &number); 
 
        printf("Reading Value from Driver\n");
        ioctl(fd, RD_VAL, (int32_t*) &value);
        printf("Value is %d\n", value);
 
        printf("Closing Driver\n");
        close(fd);
}