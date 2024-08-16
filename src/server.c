#include "include/urlParse.h"

#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

// crea el socket fd que representa al servidor y que hace lisen
int create_srv_socket(char port[]) {
  // Crea el fd del socket
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (socket_fd < 0) {
    printf("b");
    return EXIT_FAILURE;
  }

  // Declara la ip y port del socket
  struct sockaddr_in addr;

  memset(&addr, 0, sizeof(addr));

  addr.sin_family = AF_INET;
  addr.sin_port = htons(atoi(port));
  addr.sin_addr.s_addr = INADDR_ANY;

  // Bindea el puerto
  if ((bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr))) < 0) {
    return EXIT_FAILURE;
  }

  return socket_fd;
}

// gestiona el nuevo socket de comunciacion creado para el cliente
void *client_handler(void *arg) {
  int *client_socket = (int *)arg;

  char request_buffer[1024] = {0};
  int r_len = 0;

  char *response;

  r_len = read(*client_socket, request_buffer, sizeof(request_buffer));

  if (r_len < 0)
    return NULL;

  response = create_http_response(request_buffer);

  write(*client_socket, response, strlen(response));

  close(*client_socket);

  return NULL;
}

// Funcion main
int main(int argc, char *argv[]) {
  int srv_socket = create_srv_socket("6890");

  // aceptar conexiones entrantes
  listen(srv_socket, 100);

  // acepta conexiones entrantes
  while (1) {
    int *client_socket = malloc(sizeof(int));

    *client_socket = accept(srv_socket, (struct sockaddr *)NULL, NULL);

    pthread_t client_thread;
    pthread_create(&client_thread, NULL, client_handler, (void *)client_socket);
  }

  close(srv_socket);

  return EXIT_SUCCESS;
}
