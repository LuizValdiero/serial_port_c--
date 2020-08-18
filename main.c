
// C library headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "ca_serial_handler.h"

typedef struct buffer_t {
    uint32_t buffer_size;
    unsigned char * buffer;
} buffer_t;

#define MAX_BUFFER_SIZE 100

static void get_args(int argc, char *argv[], char **dev)
{
	if (argc != 2) {
        //printf("Unexpected number of arguments %d (expected 1)", argc-1);
		*dev = "/dev/ttyUSB0";
        return;
	}
	*dev = argv[1];
}


int main(int argc, char *argv[]) {

	char * dev;
	//char * plaintext;
	//char * inverted;
	//int text_size = MAX_BUFFER_SIZE;

    get_args(argc, argv, &dev);

    // file descriptor - /dev/ttyUSB0
    int serial_port;
    init_serial_port(&serial_port, &dev);
    if (serial_port < 0)
        exit(1);

/*    plaintext = (char *) malloc(text_size);
    if (!plaintext) {
        printf("Error %i - malloc: %s\n", errno, strerror(errno));
        return errno;
    }
*/

    buffer_t output = {.buffer = malloc(3), .buffer_size = 3};
    do {
    output.buffer_size = read(serial_port, output.buffer, output.buffer_size);
    //printf(" output (%d bytes): %s\n", output.buffer_size, output.buffer);
    } while (output.buffer_size <= 0);
    printf(" output (%d bytes): %s\n", output.buffer_size, output.buffer);
    
    memcpy(output.buffer, "ok", 3);
    output.buffer_size = 3;
    printf(" output (%d bytes): %s\n", output.buffer_size, output.buffer);
    output.buffer_size = write(serial_port, output.buffer, output.buffer_size);
    
    //write(serial_port, output.buffer, output.buffer_size);
    
    /*
    text_size = strlen("aabb");
    memcpy( plaintext, "aabb", text_size);
    while (1)
    {   
        printf("write %s\n", plaintext);
        text_size = write(serial_port, plaintext, text_size);
        printf("writed %d bytes\n", text_size);
        memset(plaintext,0x00, MAX_BUFFER_SIZE);
        // int num_bytes = read(serial_port, &plaintext, sizeof(plaintext));
        while (1)
        {
            int num_bytes = read(serial_port, plaintext, MAX_BUFFER_SIZE);
            if (num_bytes < 0) {
                printf("Error reading: %s", strerror(errno));
                break;
            }
            if (num_bytes > 0)
            {
                printf("read\n");
                printf("Read %i bytes. \nReceived message:\n%s\n", num_bytes, plaintext);
                int m = num_bytes / 2;
                int i = 0;
                num_bytes --;
                char aux = 0;
                for(; i < m; i++)
                {
                    aux = plaintext[i];
                    plaintext[i] = plaintext[num_bytes - i];
                    plaintext[num_bytes - i] = aux;
                }
                printf("Invert message:\n%s\n", plaintext);
                break;
            }
        }
    }
    free(plaintext);
    */



    close(serial_port);

    return 0;
}
