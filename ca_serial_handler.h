#ifndef CA_SERIAL_HANDLER_H
#define CA_SERIAL_HANDLER_H

//https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/ 
//http://tldp.org/HOWTO/Serial-Programming-HOWTO/x115.html#AEN144

//http://www.linusakesson.net/programming/tty/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
//#include <sys/signal.h>
#include <errno.h>
#include <termios.h>

#include "defines.h"

int init_serial_port(int * serial_port, char ** dev);
int config_serial_port(int * serial_port);

void ready_serial_port(int * serial_port);
int read_serial_port(int * serial_port, buffer_t * output);

void ready_serial_port(int * serial_port) {
    unsigned char output[] = STRING_SYNCHRONIZE;
    write(*serial_port, output, STRING_SYNCHRONIZE_SIZE);
}

int read_serial_port(int * serial_port, buffer_t * output) {
    unsigned char synchronize[STRING_SYNCHRONIZE_SIZE];
    uint8_t len;
    
    ready_serial_port(serial_port);
    memset(output->buffer, 0, output->buffer_size);
    
    read(*serial_port, &len, 1);
    if (len > output->buffer_size) {
        read(*serial_port, NULL, len);
        return 1;
    }

    output->buffer_size = read(*serial_port, output->buffer, len);
    
    read(*serial_port, synchronize, STRING_SYNCHRONIZE_SIZE);
    if (memcmp(STRING_SYNCHRONIZE, synchronize, STRING_SYNCHRONIZE_SIZE))
        return 1;
    return 0;
}


int init_serial_port(int * serial_port, char ** dev) {
    *serial_port = open(*dev, O_RDWR);

    // Check for errors
    if(*serial_port < 0) {
        printf("Error %i from open: %s\n", errno, strerror(errno));
        return errno;
    }

    if (config_serial_port(serial_port)) {
        printf("Error - Serial port config");
        return errno;
    }
    return 0;
}



int config_serial_port(int * serial_port) {

    struct termios tty;
    memset(&tty, 0, sizeof(tty));

    // Read in existing settings, and handle any error
    if(tcgetattr(*serial_port, &tty) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        return 1;
    }

    /**
     * Control Modes (c_cflags)
     **/
    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity
    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication
    tty.c_cflag |= CS8; //bits per byte; CS5:5; CS6:6; CS7:7; CS8:8;
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    /**
     * Local Modes (c_lflag)
     **/
    tty.c_lflag &= ~ICANON; // Disable canonical mode
	// desabilitados por precaução
    // evita receber caracteres repetidos por causa do modo nao canonico
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo

    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP

    /**
     * Input Modes (c_iflag)
     **/
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

    /**
     * Output Modes (c_oflag)
     **/
    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
    tty.c_cc[VTIME] = 5;    // Wait for up to 0.5s (5 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 0;

    /**
     * Baud Rate
     **/
	cfsetispeed(&tty, B115200);
    cfsetospeed(&tty, B115200);

    // ------------
    // Save configuration - ttyUSB0 termios struct
    // Save tty settings, also checking for error
    if(tcsetattr(*serial_port, TCSANOW, &tty) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        return errno;
    }

    return 0;
}

#endif // CA_SERIAL_HANDLER_H