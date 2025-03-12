#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>


uint8_t calculateChecksum(uint8_t *data, int length) {
    uint8_t checksum = 0;
    for (int i = 0; i < length; i++) {
        checksum ^= data[i];
    }
    return checksum;
}


int serial_fd = -1;

// Setup the serial port
int setupSerialPort(const char *portName, int baudRate)
{
    serial_fd = open(portName, O_RDWR | O_NOCTTY | O_SYNC); 

    if (serial_fd == -1)
    {
        perror("Error opening serial port");
        return -1;
    }

    struct termios tty;
    if (tcgetattr(serial_fd, &tty) != 0)
    {
        perror("Error getting termios settings");
        close(serial_fd);
        serial_fd = -1;
        return -1;
    }

    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity
    tty.c_cflag &= ~CSTOPB; // Clear stop bit, using one stop bit
    tty.c_cflag &= ~CSIZE;  // Clear data size
    tty.c_cflag |= CS8;     // Set data size to 8 bits
    tty.c_cflag &= ~CRTSCTS; // Disable hardware flow control
    tty.c_cflag |= CREAD | CLOCAL; // Enable receiver and ignore modem lines
    tty.c_lflag &= ~ICANON; //Disable canonical mode
    tty.c_lflag &= ~ECHO;
    tty.c_lflag &= ~ECHOE;
    tty.c_lflag &= ~ECHONL;
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);
    tty.c_oflag &= ~OPOST;

    // Set baud rate
    speed_t baud;
    if (baudRate == 9600) {
        baud = B9600;
    } else if(baudRate == 19200)
    {
        baud = B19200;
    }
    else
    {
      printf("Invalid baudrate");
      close(serial_fd);
      serial_fd = -1;
      return -1;
    }

    cfsetospeed(&tty, baud);
    cfsetispeed(&tty, baud);
    if (tcsetattr(serial_fd, TCSANOW, &tty) != 0)
    {
        perror("Error setting termios settings");
        close(serial_fd);
        serial_fd = -1;
        return -1;
    }

    return 0;
}

void closeSerialPort() {
    if (serial_fd != -1) {
        close(serial_fd);
        serial_fd = -1;
    }
}


// Function to send data over serial port using RS-232 settings
int sendData(const char *data, int length) {
    if (serial_fd == -1) {
        fprintf(stderr, "Error: Serial port not initialized.\n");
        return -1; // Error: serial not initiliazed
    }
    ssize_t bytes_written = write(serial_fd, data, length);
     if (bytes_written == -1) {
        fprintf(stderr, "Error writing to serial port: %s\n", strerror(errno));
        return -1;
    } else if (bytes_written != length) {
        fprintf(stderr, "Error: Not all data bytes were written to the serial port (Expected %d, actual %zd).\n", length, bytes_written);
        return -1; // Error: not all data sent
    }

     printf("Data sent: ");
    for(int i=0; i<length; i++){
      printf("%02X ",(uint8_t)data[i]);
    }
    printf("\n");
    return bytes_written;
}

// Function to receive data from serial port using RS-232 settings
int receiveData(char *buffer, int maxLength) {
    if (serial_fd == -1) {
        fprintf(stderr, "Error: Serial port not initialized.\n");
        return -1; // Error: serial not initiliazed
    }
    // Clear the buffer
    memset(buffer, 0, maxLength);
    printf("Waiting for response...\n");
    ssize_t bytes_read = read(serial_fd, buffer, maxLength);
    if (bytes_read == -1) {
        fprintf(stderr, "Error reading from serial port: %s\n", strerror(errno));
        return -1; // Error reading from serial
    }
    if(bytes_read == 0){
      printf("No data received from serial port.\n");
      return 0; // No data received
    }


    printf("Data Received: ");
    for(int i=0; i<bytes_read; i++){
      printf("%02X ", (uint8_t)buffer[i]);
    }
    printf("\n");
    return (int)bytes_read;
}


void sendSASCommand() {
    uint8_t command[5];
    command[0] = 0x02;  // STX
    command[1] = 0x01;  // Address
    command[2] = 0x13;  // Command

    command[3] = calculateChecksum(command, 3); // Checksum
    command[4] = 0x03;  // ETX

    printf("SAS Command: ");
    for (int i = 0; i < 5; i++) {
        printf("%02X ", command[i]);
    }
    printf("\n");

    sendData((char *)command, 5);
}


char* parseSASResponse(char* response, int length)
{
    if(length < 6){
      printf("Response message is too short \n");
      return NULL; // Response is too short to be a valid response
    }

    if(response[0] != 0x02){
      printf("Response message does not start with STX \n");
      return NULL; // No STX
    }

    if(response[length - 1] != 0x03){
      printf("Response message does not end with ETX \n");
      return NULL; // No ETX
    }
    uint8_t checksum = calculateChecksum((uint8_t *)response, length - 2);

    if(checksum != (uint8_t)response[length - 2]){
       printf("Invalid checksum \n");
        return NULL;
    }

    uint16_t meterValue = ((uint8_t)response[4] << 8) | (uint8_t)response[3];

    char *json_string;
    // Allocate memory for  JSON y
    int jsonLength = snprintf(NULL, 0, "{\"address\": %u, \"command\": %u, \"meterValue\": %u}", (uint8_t)response[1], (uint8_t)response[2], meterValue);
    json_string = (char*) malloc(jsonLength + 1);

    if(json_string == NULL){
        perror("Error allocating memory for JSON string");
         return NULL;
    }
    snprintf(json_string, jsonLength + 1,  "{\"address\": %u, \"command\": %u, \"meterValue\": %u}", (uint8_t)response[1], (uint8_t)response[2], meterValue);

  printf("Valid response received! \n");
    return json_string;
}


int main(){
    const char *portName = "/dev/ttyS0"; //  This is COM1 in windows.
    int baudRate = 9600; // Choose 9600 or 19200 depending on your machine
    if (setupSerialPort(portName, baudRate) != 0) {
       fprintf(stderr, "Failed to setup serial port.\n");
       return 1;
    }
    sendSASCommand();
    char responseBuffer[256];
    int responseLength = receiveData(responseBuffer, 256);

   if(responseLength > 0){
     char *jsonResponse = parseSASResponse(responseBuffer, responseLength);
     if(jsonResponse != NULL){
       //Handle JSON 
      printf("JSON response is: %s\n", jsonResponse);
      free(jsonResponse); // Free  memory
      }
    }
    closeSerialPort();
    return 0;
}