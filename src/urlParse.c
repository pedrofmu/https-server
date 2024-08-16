#include "include/urlParse.h"

#include <stdio.h>
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

int get_query(char *values, char *data) {
  printf("%s\n", values);
  if (strlen(values) <= 3)
    return -1;

  int i = 0;
  while (strncmp(&values[i], "?q=", 3) != 0) {
    i++;
    if (i >= strlen(values) - 3)
      return -1;
  }

  strncpy(data, &values[i + 3], strlen(values) - i - 3);
  data[strlen(values) - i - 3] = '\0';

  return 0;
}

int is_proxy(char *values) {
  if (strncmp(values, "http://", 7) != 0)
    return -1;

  return 0;
}

char *create_http_response(char *request) {
  printf("%s\n", request);
  struct http_request req = parse_request(request);
  char *response = NULL;
  char *data = malloc(strlen(req.params) + 1);

  // Si es el metodo GET devolver datos, si no dar error 501
  if (strcmp(req.method, "GET") == 0) {
    // Pagina principal
    if (strcmp(req.params, "/") == 0) {
      response = "HTTP/1.1 200 OK\r\n"
                 "Content-Type: text/html; charset=UTF-8\r\n\r\n"
                 "<!DOCTYPE html>\r\n"
                 "<html>\r\n"
                 "<head>\r\n"
                 "<title>Testing Basic HTTP-SERVER</title>\r\n"
                 "</head>\r\n"
                 "<body>\r\n"
                 "Bichota C\r\n"
                 "</body>\r\n"
                 "</html>\r\n";
    } else {
      response = "HTTP/1.1 418 I'm a teapot\r\n";
    }
  } else {
    response = "HTTP/1.1 404 Not found\r\n";
  }

  free(data);
  return response;
}
