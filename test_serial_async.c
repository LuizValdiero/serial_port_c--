#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <errno.h>
#include <termios.h>

#include <stdint.h>

void signal_handler_IO (int status);   /* definition of signal handler */

typedef struct buffer_t {
    uint32_t buffer_size;
    unsigned char * buffer;
} buffer_t;

int n;
int fd;
int connected;
struct termios termAttr;
struct sigaction saio;

int main()
{
    
     fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
     if (fd == -1)
     {
        perror("open_port: Unable to open /dev/ttyO1\n");
        exit(1);
     }

     saio.sa_handler = signal_handler_IO;
     saio.sa_flags = 0;
     saio.sa_restorer = NULL; 
     sigaction(SIGIO,&saio,NULL);

     fcntl(fd, F_SETFL, FNDELAY);
     fcntl(fd, F_SETOWN, getpid());
     fcntl(fd, F_SETFL,  O_ASYNC ); /**<<<<<<------This line made it work.**/

     tcgetattr(fd,&termAttr);
     //baudRate = B115200;          /* Not needed */
     cfsetispeed(&termAttr,B115200);
     cfsetospeed(&termAttr,B115200);
     termAttr.c_cflag &= ~PARENB;
     termAttr.c_cflag &= ~CSTOPB;
     termAttr.c_cflag &= ~CSIZE;
     termAttr.c_cflag |= CS8;
     termAttr.c_cflag |= (CLOCAL | CREAD);
     termAttr.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
     termAttr.c_iflag &= ~(IXON | IXOFF | IXANY);
     termAttr.c_oflag &= ~OPOST;
     tcsetattr(fd,TCSANOW,&termAttr);
     printf("UART1 configured....\n");

     connected = 1;
     while(connected == 1){
           // some code
     }

     close(fd);
     exit(0);             
}

void signal_handler_IO (int status)
{
    (void *)&status;
    buffer_t data_package_size = {.buffer = malloc(4), .buffer_size = 4};   
    int res = read(fd, data_package_size.buffer, data_package_size.buffer_size);
    if ( res == (int) data_package_size.buffer_size)
    {
        int size = 0;
        memcpy(&size, data_package_size.buffer, sizeof(int));
        printf("size: %d\n", size);
        buffer_t data = {.buffer = malloc(size), .buffer_size = size};
        res = read(fd, data.buffer, data.buffer_size);
        printf("%s (%d bytes)\n", data.buffer, data.buffer_size);
    }
}