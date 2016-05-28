// Chung, Po-Chien: 63533832
// Gupta, Nikki: 11391554
// Dubon, Jeremy: 19812840

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "csapp.h"

// Prototypes
bool validate_port(int argc, char** argv);
void run_proxy(char* port);

int main(int argc, char** argv) {

  bool validPort = validate_port(argc, argv);

  if(validPort == true) {
    run_proxy(argv[1]);
  } else {
    exit(EXIT_FAILURE);
  }

  return 0;

}

/*** function run_proxy ***

Run the proxy as outlined in lab 4:
https://eee.uci.edu/16s/36680/labs/lab4.pdf

***/
void run_proxy(char* port) {

  // TODO

}

/*** function validate_port ***

Read in command line arguments and validate the port number.

Input:
# int argc: "argc" provided to main.
# char* argv: "argv" provided to main.

Returns:
# Success: true
# Failure: false

***/
bool validate_port(int argc, char** argv) {

  FILE* servicesFile = NULL; // Pointer to /etc/services.
  int port = -1; // The port number entered into the command line.

  char serviceName[255] = {'\0'}; // The name of a single service.
  char serviceType[255] = {'\0'}; // The type of connection used.
  int servicePort = 0; // The port number used.

  // Validate that the port number exists.
  if(argc != 2) {
    fprintf(stderr, "Invalid arguments\n");
    return false;
  }

  // Read the port as an integer.
  port = atoi(argv[1]);

  // Ensure the port number is in range.
  if(port < 1024 || port > 65536) {
    fprintf(stderr, "Port number out of range 1024 < port number < 65536\n");
    return false;
  }

  // Open /etc/services.
  servicesFile = fopen("/etc/services", "r");
  if(!servicesFile) {
    fprintf(stderr, "Unable to read /etc/services\n");
    return false;
  }

  // Loop through /etc/services and ensure that the port number isn't in use.
  while(fscanf(servicesFile, "%s %d/%s\n", serviceName, &servicePort, serviceType) != EOF) {
    if(servicePort == port) {
      fprintf(stderr, "Port in use by: %s\n", serviceName);
      fclose(servicesFile);
      return false;
    }
  }

  fclose(servicesFile);
  return true;

}
