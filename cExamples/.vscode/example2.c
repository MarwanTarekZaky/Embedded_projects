#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h> // For usleep

enum request_commands { Coin_In = 0x13, Coin_Out = 0x14, Jackpot = 0x15, Machine_Status = 0x40, Configuration_Info = 0x50}; 
// Assuming you still have the calculateChecksum function available
uint8_t calculateChecksum(uint8_t *data, int length) {
    uint8_t checksum = 0;
    for (int i = 0; i < length; i++) {
        checksum ^= data[i];
    }
    return checksum;
}

// Global file descriptor for serial port
int serial_fd = -1;

// Setup the serial port
int setupSerialPort(const char *portName, int baudRate)
{
    serial_fd = open(portName, O_RDWR | O_NOCTTY); // O_NOCTTY is needed for serial communication

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


// Replace this with your actual sendData function
void sendData(char *data, int length){
  if (serial_fd == -1)
    {
      printf("Serial port not initialized\n");
      return;
    }
    // Example using write to serial port
    write(serial_fd, data, length);
    printf("Data sent: ");
    for(int i=0; i<length; i++){
      printf("%02X ",(uint8_t)data[i]);
    }
    printf("\n");
}

// Replace this with your actual receiveData function
// This function will receive a response from the machine
// and return the length of the data received.
int receiveData(char *buffer, int maxLength){
  if (serial_fd == -1)
    {
      printf("Serial port not initialized\n");
      return 0;
    }

    printf("Waiting for response... \n");
   // Attempt to read the data from the serial port
    ssize_t bytes_read = read(serial_fd, buffer, maxLength);
  if(bytes_read == -1){
    perror("Error reading from serial port");
  }

  return (int)bytes_read;
}

void sendSASCommand(uint8_t command_code) {
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

bool parseSASResponse(char* response, int length, uint8_t *meterValue)
{
    if(length < 6){
      printf("Response message is too short \n");
      return false; // Response is too short to be a valid response
    }

    if(response[0] != 0x02){
      printf("Response message does not start with STX \n");
      return false; // No STX
    }

    if(response[length - 1] != 0x03){
      printf("Response message does not end with ETX \n");
      return false; // No ETX
    }
    uint8_t checksum = calculateChecksum((uint8_t *)response, length - 2);

    if(checksum != (uint8_t)response[length - 2]){
       printf("Invalid checksum \n");
        return false;
    }

  *meterValue = ((uint8_t)response[4] << 8) | (uint8_t)response[3];

  printf("Valid response received! \n");
  printf("Address: %02X \n", (uint8_t)response[1]);
  printf("Command Echo: %02X \n", (uint8_t)response[2]);
  printf("Meter value: %d\n", *meterValue);

    return true;
}


int main(){
    const char *portName = "/dev/ttyS0"; // Replace with your serial port. This is COM1 in windows.
    int baudRate = 9600; // Choose 9600 or 19200 depending on your machine
    if (setupSerialPort(portName, baudRate) != 0) {
       fprintf(stderr, "Failed to setup serial port.\n");
       return 1;
    }
    sendSASCommand();
    char responseBuffer[256];
    int responseLength = receiveData(responseBuffer, 256);

   if(responseLength > 0){
    uint8_t meterValue;
     if(parseSASResponse(responseBuffer, responseLength, &meterValue)){
      //Handle meter value
      printf("Meter value is %u\n", meterValue);
      }
    }
    closeSerialPort();
    return 0;
}