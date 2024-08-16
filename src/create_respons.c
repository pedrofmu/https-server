#include "include/create_respons.h"
#include <bits/types/struct_iovec.h>
#include <include/urlParse.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char *working_directory;

char *get_file(char *path) {
  size_t buffer_len = strlen(path) + strlen(working_directory) + 1;
  char *full_path = malloc(buffer_len);
  strcat(full_path, working_directory);
  strcat(full_path, path);

  FILE *file;
  file = fopen(full_path, "r");

  if (file == NULL)
    return NULL;

  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  rewind(file);

  char *file_content = (char *)malloc((file_size + 1) * sizeof(char));
  if (file_content == NULL) {
    return NULL;
  }

  size_t read_size = fread(file_content, sizeof(char), file_size, file);
  file_content[read_size] = '\0';

  return file_content;
};

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
      // obtiene el archivo basandose en el path
      char *file_content = get_file(req.params);
      if (file_content == NULL) {
        free(data);
        return "HTTP/1.1 500 Server error\r\n";
      }

      size_t response_size =
          strlen("HTTP/1.1 200 OK\r\n"
                 "Content-Type: text/html; charset=UTF-8\r\n\r\n") +
          strlen(file_content) + 1;
      response = (char *)malloc(response_size);

      if (response == NULL) {
        free(data);
        return "HTTP/1.1 500 Server error\r\n";
      }

      // Copia y concatena las cadenas
      strcpy(response, "HTTP/1.1 200 OK\r\n"
                       "Content-Type: text/html; charset=UTF-8\r\n\r\n");
      strcat(response, file_content);

      free(file_content);
    }
  } else {
    response = "HTTP/1.1 404 Not found\r\n";
  }

  free(data);
  return response;
}
