#include "include/parser.h"

#include <stdlib.h>
#include <string.h>

int parse_url(const char *url, char *server_ip, char *server_port) {
  int i = 7;
  strcpy(server_port, "80"); 

  while (url[i] != '/') {
    if (url[i] == ':') {
      int j = i + 1; 
      strncpy(server_ip, &url[7], i - 7); 
      server_ip[i - 7] = '\0'; 
      
      while (url[j] != '/' && url[j] != '\0') {
        j++;
      }

      strncpy(server_port, &url[i + 1], j - i - 1); 
      server_port[j - i - 1] = '\0'; 
      return 0;
    }

    i++;
  }

  strncpy(server_ip, &url[7], i - 7); 
  server_ip[i - 7] = '\0';

  return 0;
}

struct http_request parse_request(char *request) {
  // Primer parametro
  int i = 0;
  while (request[i] != ' ') {
    i++;
  }

  char *method = (char *)malloc(i + 1);
  strncpy(method, request, i);
  method[i] = '\0';

  // Segundo parametro
  int j = i + 1;
  while (request[j] != ' ') {
    j++;
  }

  char *params = (char *)malloc(j - i - 1);
  strncpy(params, &request[i + 1], j - i - 1);
  params[j - i - 1] = '\0';

  // Crear la extructura
  struct http_request parsed_request = {method, params};

  return parsed_request;
};
