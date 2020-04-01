// tutorial
//https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/ 

// C library headers
#include <stdio.h>
#include <string.h>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()


int config_serial_port(int * serial_port);

int main() {
    // file descriptor - /dev/ttyUSB0
    int serial_port = open("/dev/ttyUSB0", O_RDWR);

    // Check for errors
    if(serial_port < 0) {
        printf("Error %i from open: %s\n", errno, strerror(errno));
        return errno;
    }

    if (config_serial_port(&serial_port)) {
        printf("Error - Serial port config");
        return errno;
    }

    //unsigned char msg[] = { 'H', 'e', 'l', 'l', 'o', '\r' };
    //write(serial_port, "Hello, world!", sizeof(msg));

    char read_buf [256];
    memset(&read_buf, '\0', sizeof(read_buf));

    int num_bytes = read(serial_port, &read_buf, sizeof(read_buf));

    if (num_bytes < 0) {
        printf("Error reading: %s", strerror(errno));
    }
    printf("Read %i bytes. Received message: %s", num_bytes, read_buf);

    close(serial_port);

    return 0;
}

int config_serial_port(int * serial_port){

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
    // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT IN LINUX)
    // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT IN LINUX)

    /**
     * VMIN and VTIME (c_cc)
     **/

    /*
    VMIN = 0, VTIME = 0: 
    No blocking, return immediately with what is available

    VMIN > 0, VTIME = 0: 
    This will make read() always wait for bytes (exactly how many is \
    determined by VMIN), so read() could block indefinitely.

    VMIN = 0, VTIME > 0: 
    This is a blocking read of any number chars with a maximum \
    timeout (given by VTIME). read() will block until either any \
    amount of data is available, or the timeout occurs.

    VMIN > 0, VTIME > 0: 
    Block until either VMIN characters have been received, \
    or VTIME after first character has elapsed. Note that the \
    timeout for VTIME does not begin until the first character is received.
    */
    tty.c_cc[VTIME] = 5;    // Wait for up to 0.5s (5 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 0;

    /**
     * Baud Rate
     **/

    // Set in/out baud rate to be 9600
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