#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define bool int
#define false 0
#define true 1 
typedef unsigned long speed_t;


//Declaring a list of requesting Commands
enum request_commands \
{   Coin_In = 0x13,
    Coin_Out = 0x14, 
    Jackpot = 0x15,
    Machine_Status = 0x40,
    Configuration_Info = 0x50
}; 

/*By considering that every real pyhsical slot machine adress is mapped 
to Virtual adress in array slot_adress 
  "cid0": 5636171, "cid1": 875646994, "cid2": 540226902 }
*/
uint8_t slot_adress[3]  = {0x01,0x02,0x03};


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
    serial_fd = open(portName); //  is needed for serial communication

    if (serial_fd == -1)
    {
        perror("Error opening serial port");
        return -1;
    }

    // Set baud rate
    speed_t baud;
    if (baudRate == 9600) {
        baud = 9600;
    } else if(baudRate == 19200)
    {
        baud = 19200;
    }
    else
    {
      printf("Invalid baudrate");
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


char* checkSASResponse(char* response, int length)
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
 
 uint16_t meterValue = ((uint8_t)response[4] << 8) | (uint8_t)response[3];
  char *json_string;
    // Allocate memory for the JSON string dynamically
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


void sendSASCommand(enum request_commands request_command, uint8_t adress) {

    uint8_t request_frame [5];
    request_frame[0] = 0x2;// STX
    request_frame[1] = adress; // slot machine adress (Virtual not pyhsical)
    request_frame[2] = request_command;
    request_frame[3] = calculateChecksum(request_frame, 3); // Checksum
    request_frame[4] = 0x03; // ETX

    //  sending command over serial port
   sendData((char *)request_frame, 5);
}


// Function to send data over serial port
int sendData(const char *data, int length) {

    if (serial_fd == -1) {
        fprintf(stderr, " Serial port not initialized.\n");
        return -1; // Errorserial
    }

    ssize_t bytes_written = write(serial_fd, data, length);

     if (bytes_written == -1) {

        fprintf(stderr, "Error write to the serial port\n");
        return -1;

    } else if (bytes_written != length) {
        fprintf(stderr, "Error: Not all data bytes were written to the serial port (Expected %d, actual %d).\n", length, bytes_written);
        return -1; // Error: not all data sent
    }

     printf("Data sent: ");
    for(int i=0; i<length; i++){
      printf("%02X ",(uint8_t)data[i]);
    }

    printf("\n");

    return bytes_written;
}

// Function to receive data from serial port
int receiveData(char *buffer, int maxLength) {

    if (serial_fd == -1) {
        fprintf(stderr, "Serial port not initialized.\n");
        return -1; // Error serial 
    }


    // Clear buffer before reading
    memset(buffer, 0, maxLength);
    printf("Waiting for response...\n");

    ssize_t bytes_read = read(serial_fd, buffer, maxLength);
    if (bytes_read == -1) {
        fprintf(stderr, "Error reading serial port:\n");
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


int main(int argc, char const *argv[])
{   
    //for example  Coin_Out request command
    enum request_commands request_command = Coin_Out;
    
    //For example adress for slot machine "cid2": 540226902 mapped to 0x03 in index 2
    uint8_t adress = slot_adress[2];

    // Choose 9600 or 19200 depending on your agremment baud rate 
    speed_t baudRate = 9600; 
    //Chossing the port for ex: COM1
    const char *portName = "COM1";

    //Setting up the port
    if (setupSerialPort(portName, baudRate) != 0) {
       fprintf(stderr, "Failed to setup serial port.\n");
       return 1;
    }

    
    //Requesting Command in SAS IGT protocol
    sendSASCommand(request_command, adress);

    
     char responseBuffer[256];
    int responseLength = receiveData(responseBuffer, 256);

   if(responseLength > 0){
     char *jsonResponse = parseSASResponse(responseBuffer, responseLength);
     if(jsonResponse != NULL){
        
       //Handle JSON Response
      printf("JSON response is: %s\n", jsonResponse);
      free(jsonResponse);
      }
    }

    //Closing the port
    closeSerialPort();

    return 0;
}
