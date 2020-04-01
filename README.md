# Serial Port

- In Linux: https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/ 

##

esp01: 
read text of the serial and rewrite in the serial

pc:
```
$ ./serial <port serial>
```

connect in serial port and write text
while (1)
    read serial port, invert text and rewrite in serial port