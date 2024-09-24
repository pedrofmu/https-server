#include "create_respons.h"
#include "file_manager.h"
#include "parser.h" 

#include <bits/types/struct_iovec.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char *working_directory;

char *get_method(struct http_request req) {
  // Obtiene el archivo basándose en el path
  char *file_content = obtain_file_content(req.path);
  if (file_content == NULL) {
    return strdup("HTTP/1.1 404 Page not found\r\n");
  }

  // Obtiene el MIME type basado en el archivo
  char *mime_type = get_mime_type(req.path);
  if (mime_type == NULL) {
    free(file_content);
    return strdup("HTTP/1.1 500 Internal Server Error\r\n");
  }

  // Construye la cabecera dinámicamente usando el mime_type
  const char *header_format =
      "HTTP/1.1 200 OK\r\nContent-Type: %s; charset=UTF-8\r\n\r\n";
  size_t header_size = snprintf(NULL, 0, header_format, mime_type) + 1;
  char *header = (char *)malloc(header_size);
  if (header == NULL) {
    free(file_content);
    free(mime_type);
    return strdup("HTTP/1.1 500 Internal Server Error\r\n");
  }

  snprintf(header, header_size, header_format, mime_type);

  // Calcula el tamaño total de la respuesta
  size_t response_size = strlen(header) + strlen(file_content) + 1;
  char *response = (char *)malloc(response_size);
  if (response == NULL) {
    free(file_content);
    free(header);
    return strdup("HTTP/1.1 500 Internal Server Error\r\n");
  }

  // Copia y concatena las cadenas
  snprintf(response, response_size, "%s%s", header, file_content);

  // Liberar la memoria
  free(file_content);
  free(header);

  return response;
}

char *put_method(struct http_request req) {
  char *respons = malloc(1023 * sizeof(char));

  if (write_file(req.path, req.body) > 0) {
    // Sucess
    snprintf(respons, 1023 * sizeof(char),
             "HTTP/1.1 201 Created\r\n"
             "Content-Location: /new.html\r\n\r\n");
  } else {
    // Fail
    snprintf(respons, 1023 * sizeof(char),
             "HTTP/1.1 204 No Content\r\n"
             "Content-Location: /new.html\r\n\r\n");
  }

  return respons;
};

char *create_http_response(char *request) {
  printf("%s\n", request);
  struct http_request req = parse_request(request);

  // Si es el metodo GET devolver datos, si no dar error 404
  if (strcmp(req.method, "GET") == 0) {
    return get_method(req);
  } else if (strcmp(req.method, "PUT") == 0) {
    return put_method(req);
  } else {
    return strdup("HTTP/1.1 404 Not found\r\n");
  }
}
