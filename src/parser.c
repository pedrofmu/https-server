#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Crea un header (es un linked-list)
struct http_header *create_header() {
  struct http_header *header =
      (struct http_header *)malloc(sizeof(struct http_header));
  if (header == NULL) {
    printf("Memory allocation failed\n");
    exit(1);
  }
  header->name = NULL;
  header->value = NULL;
  header->next_header = NULL;
  return header;
}

struct http_request *parse_request(const char *raw_data) {
  // obtener el method
  size_t data_len = strlen(raw_data) + 1;
  char *data_modifiable = malloc(data_len);
  strcpy(data_modifiable, raw_data);

  char *method = strtok(data_modifiable, " ");
  if (method == NULL) {
    return NULL;
  }

  // obtener el path
  char *path = strtok(NULL, " ");
  if (path == NULL || path[0] != '/') {
    return NULL;
  }

  // saltar hasta los headers
  int i = 0;
  while (strncmp(&raw_data[i], "\r\n", 2)) {
    i++;
  }
  i += 2;

  struct http_header *first_header;
  struct http_header *working_header;
  if (i < data_len - 2 && strncmp(&raw_data[i], "\r\n", 2) != 0) {
    first_header = create_header();
    working_header = first_header;
  } else {
    first_header = NULL;
    working_header = NULL;
  }

  // parsea los headers
  while (i < data_len - 2 && strncmp(&data_modifiable[i], "\r\n", 2) != 0) {
    char *header_name = strtok(&data_modifiable[i], ":");

    char *header_value =
        strtok(&data_modifiable[i + strlen(header_name) + 2], "\r");

    working_header->name = strdup(header_name);
    working_header->value = strdup(header_value);

    // saltar hasta el siguiente header
    while (i < data_len - 2 && strncmp(&raw_data[i], "\r\n", 2) != 0) 
      i++;
    i += 2;

    if (strncmp(&data_modifiable[i], "\r\n", 2) == 0)
      break;

    working_header->next_header = create_header();
    working_header = working_header->next_header;
  }

  // body
  char *body;
  if (i + 2 > strlen(&data_modifiable[i])) {
    body = strdup(&data_modifiable[i + 2]);
  } else {
    body = "";
  }

  struct http_request *request = malloc(sizeof(struct http_request));
  request->method = strdup(method);
  request->path = strdup(path);
  request->headers = first_header;
  request->body = body;

  free(data_modifiable);

  return request;
}

void free_request(struct http_request *req) {
  if (req == NULL) {
    return;
  }

  // Liberar los headers
  struct http_header *working_header = req->headers;
  struct http_header *next_header;
  while (working_header != NULL) {
    next_header = working_header->next_header;
    free(working_header->name);
    free(working_header->value);
    free(working_header);   
    working_header = next_header;
  }

  // Liberar el resto de la estructura
  free(req->method);
  free(req->path);
  free(req->body);
  free(req);
}
