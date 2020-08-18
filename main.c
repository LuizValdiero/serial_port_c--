
// C library headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "defines.h"
#include "ca_serial_handler.h"

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
    get_args(argc, argv, &dev);

    int serial_port;
    init_serial_port(&serial_port, &dev);
    if (serial_port < 0)
        exit(1);

    printf("\ninicializou a serial");
    unsigned char buffer_in_str[MAX_BUFFER_SIZE];
    buffer_t buffer_in = {.buffer_size = MAX_BUFFER_SIZE, .buffer = buffer_in_str};
    for(int i = 0; i < 10; i++) {
        read_serial_port(&serial_port, &buffer_in);
        printf("\n(%d bytes): %s", buffer_in.buffer_size, buffer_in.buffer);
    }

    close(serial_port);

    return 0;
}
