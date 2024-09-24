#include "file_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern char *working_directory;

char *obtain_file_content(char *path) {
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

char *get_mime_type(char *filepath) {
  int dot_point = -1;
  int i = 0;

  // Encuentra la posición del último punto en el filepath
  while (filepath[i] != '\0') {
    if (filepath[i] == '.')
      dot_point = i;

    if (filepath[i] == '/')
      dot_point = -1;

    i++;
  }

  // Si no se encuentra un punto, devolver un tipo MIME genérico
  if (dot_point == -1) {
    return "application/octet-stream";
  }

  // Obtén un puntero al comienzo de la extensión
  char *extension = &filepath[dot_point];

  // Comparar la extensión
  if (strcmp(extension, ".html") == 0) {
    return "text/html";
  } else if (strcmp(extension, ".css") == 0) {
    return "text/css";
  } else if (strcmp(extension, ".js") == 0) {
    return "application/javascript";
  } else if (strcmp(extension, ".json") == 0) {
    return "application/json";
  } else if (strcmp(extension, ".png") == 0) {
    return "image/png";
  } else if (strcmp(extension, ".jpg") == 0 ||
             strcmp(extension, ".jpeg") == 0) {
    return "image/jpeg";
  } else if (strcmp(extension, ".gif") == 0) {
    return "image/gif";
  } else if (strcmp(extension, ".txt") == 0) {
    return "text/plain";
  } else if (strcmp(extension, ".pdf") == 0) {
    return "application/pdf";
  } else {
    return "application/octet-stream";
  }
}

int write_file(char *path, char *data) {
  size_t buffer_len = strlen(path) + strlen(working_directory) + 1;
  char *full_path = malloc(buffer_len);
  if (full_path == NULL) {
    return -1;
  }

  snprintf(full_path, buffer_len, "%s%s", working_directory, path);

  FILE *file = fopen(full_path, "w+");

  fprintf(file, "%s", data);
  return 1;
}
