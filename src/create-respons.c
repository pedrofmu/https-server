#include "include/create-respons.h"
#include <include/urlParse.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
