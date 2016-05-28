// Chung, Po-Chien: 63533832
// Gupta, Nikki: 11391554
// Dubon, Jeremy: 19812840

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "csapp.h"

#define BUFFER_SIZE 1024

// Prototypes
bool validate_port(int argc, char** argv);
void run_proxy(int fd, FILE* log_file);

/*** function main ***

Run the proxy as outlined in lab 4:
https://eee.uci.edu/16s/36680/labs/lab4.pdf

***/
int main(int argc, char** argv) {

  bool validPort = validate_port(argc, argv); // Bool for validating the port.
  char* port = argv[1]; // The port provided to the command line.
  int listenfd, connfd; // Socket file descriptors.
  socklen_t client_len; // Length of client response.
  struct sockaddr_in client_addr; // Address of client.
  FILE* log_file = NULL; // The log file.

  // Make sure the port is valid.
  if(validPort == false) {
    exit(EXIT_FAILURE);
  }

  // Open the log file.
  log_file = fopen("proxy.log", "a");
  if(!log_file) {
    puts("Unable to open the log file.");
    puts("The proxy will run but requests will not be logged.");
  }

  // Begin listening on the specified port.
  listenfd = Open_listenfd(port);
  while(true) {
    client_len = sizeof(client_addr);
    connfd = Accept(listenfd, (SA*) &client_addr, &client_len);
    run_proxy(connfd, log_file);
    Close(connfd);
  }

  fclose(log_file);
  return 0;

}

/*** function run_proxy ***

Run a proxy for the provided connection.

Input:
# fd : The file descriptor for the active connection.

Returns: void

***/
void run_proxy(int clientfd, FILE* log_file) {

  rio_t client_rio; // The decriptor for the buffer associated with "clientfd". See csapp.c
  char buffer[BUFFER_SIZE] = {'\0'}; // A string for a single line read in from a file.
  char method[BUFFER_SIZE] = {'\0'}; // Method read in from "buffer"
  char uri[BUFFER_SIZE] = {'\0'}; // URI read in from "buffer"
  char version[BUFFER_SIZE] = {'\0'}; // Version read in from "buffer"

  Rio_readinitb(&client_rio, clientfd); // Associate clientfd with a read buffer.
  Rio_readlineb(&client_rio, buffer, BUFFER_SIZE); // Read in a line from clientfd.
  sscanf(buffer, "%s %s %s", method, uri, version); // Read request data.

  /** Note: (TODO - Delete this comment block before submitting)

    When connecting to: http://www.ics.uci.edu/~harris/test.html
    method = GET
    uri = http://www.ics.uci.edu/~harris/test.html
    version = HTTP/1.1

  ***/

  printf("%s %s %s\n", method, uri, version);

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
