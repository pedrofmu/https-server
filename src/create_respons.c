#include "include/create_respons.h"
#include <bits/types/struct_iovec.h>
#include <include/parser.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char *working_directory;

char *get_file(char *path) {
  size_t buffer_len = strlen(path) + strlen(working_directory) + 1;
  char *full_path = malloc(buffer_len);
  if (full_path == NULL) {
    return NULL; 
  }

  snprintf(full_path, buffer_len, "%s%s", working_directory, path);

  FILE *file = fopen(full_path, "r");
  free(full_path); 

  if (file == NULL) {
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  rewind(file);

  char *file_content = (char *)malloc((file_size + 1) * sizeof(char));
  if (file_content == NULL) {
    fclose(file); 
    return NULL;
  }

  size_t read_size = fread(file_content, sizeof(char), file_size, file);
  fclose(file); 
  file_content[read_size] = '\0';

  return file_content;
}

char *get_method(struct http_request req) {
  // obtiene el archivo basandose en el path
  char *file_content = get_file(req.params);
  if (file_content == NULL) {
    return strdup("HTTP/1.1 404 Page not found\r\n");
  }

  const char *header = "HTTP/1.1 200 OK\r\n"
                       "Content-Type: text/html; charset=UTF-8\r\n\r\n";
  size_t response_size = strlen(header) + strlen(file_content) + 1;

  char *response = (char *)malloc(response_size);
  if (response == NULL) {
    free(file_content);
    return strdup("HTTP/1.1 404 Page not found\r\n");
  }

  // Copia y concatena las cadenas
  snprintf(response, response_size, "%s%s", header, file_content);
  free(file_content);

  return response;
}

char *create_http_response(char *request) {
  printf("%s\n", request);
  struct http_request req = parse_request(request);

  // Si es el metodo GET devolver datos, si no dar error 404
  if (strcmp(req.method, "GET") == 0) {
    return get_method(req);
  } else {
    return strdup("HTTP/1.1 404 Not found\r\n");
  }
}

