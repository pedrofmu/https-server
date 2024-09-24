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

struct http_request parse_request(char *raw_data) {
  // method
  int i = 0;
  while (raw_data[i] != ' ') {
    i++;
  }

  char *method = (char *)malloc(i + 1);
  strncpy(method, raw_data, i);
  method[i] = '\0';

  // path
  int j = i + 1;
  while (raw_data[j] != ' ') {
    j++;
  }

  char *path = (char *)malloc(j - i - 1);
  strncpy(path, &raw_data[i + 1], j - i - 1);
  path[j - i - 1] = '\0';

  // header
  struct http_header *first_header = create_header();
  while (raw_data[i] != '\r') {
    i++;
  }

  i += 1;
  // itinera los valores y crea una linked list con los valores
  struct http_header *working_header = first_header;
  while (raw_data[i] != '\r' || raw_data[i + 1] != '\n') {
    j = i;
    while (raw_data[j] != ':') {
      j++;
    }

    int k = j;

    while (raw_data[k] != '\n') {
      k++;
    }

    working_header->name = malloc((j - i + 1) * sizeof(char));
    strncpy(working_header->name, &raw_data[i], j - i);
    working_header->name[j] = '\0';

    working_header->value = malloc((k - (j + 2) + 1) * sizeof(char));
    strncpy(working_header->value, &raw_data[j + 2], k - (j + 2));
    working_header->value[k] = '\0';

    i = k + 1;

    // crear un nuevo http header y cambiar los punteros
    working_header->next_header = create_header();
    working_header = working_header->next_header;
  }

  // Crear la extructura
  struct http_request parsed = {method, path, first_header, &raw_data[i + 2]};

  return parsed;
}
