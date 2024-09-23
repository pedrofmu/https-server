#include "create_respons.h"
#include "parser.h"

#include <netinet/in.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

char *working_directory = "/srv/";

volatile sig_atomic_t stop = 0;
void handle_sigint(int sig) { stop = 1; }

SSL_CTX *ctx;

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

// Crea un contexto SSL
SSL_CTX *create_context() {
  const SSL_METHOD *method;
  SSL_CTX *ctx;

  method = TLS_server_method();

  ctx = SSL_CTX_new(method);
  if (!ctx) {
    perror("Unable to create SSL context");
    ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
  }

  return ctx;
}

// Añade las claves al contexto ssl (debes incluir las claves en la misma
// carpeta en la que se encuentra el binario)
void configure_context(SSL_CTX *ctx) {
  /* Set the key and cert */
  if (SSL_CTX_use_certificate_file(ctx, "cert.pem", SSL_FILETYPE_PEM) <= 0) {
    ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
  }

  if (SSL_CTX_use_PrivateKey_file(ctx, "key.pem", SSL_FILETYPE_PEM) <= 0) {
    ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
  }
}

// gestiona el nuevo socket de comunciacion creado para el cliente
void *client_handler(void *arg) {
  int *client_socket = (int *)arg;

  SSL *ssl = SSL_new(ctx);

  char request_buffer[1024] = {0};
  int r_len = 0;

  char *response;

  SSL_set_fd(ssl, *client_socket);

  if (SSL_accept(ssl) <= 0)
    return NULL;

  r_len = SSL_read(ssl, request_buffer, sizeof(request_buffer));
  if (r_len <= 0) {
    SSL_free(ssl);
    close(*client_socket);
    free(client_socket);
    return NULL;
  }

  response = create_http_response(request_buffer);
  SSL_write(ssl, response, strlen(response));

  SSL_shutdown(ssl);
  SSL_free(ssl);

  close(*client_socket);
  free(client_socket);

  return NULL;
}

// Funcion main
int main(int argc, char *argv[]) {
  int opt;
  char *port = "6890";

  // Maneja la señal SIGINT
  struct sigaction sa;
  sa.sa_handler = handle_sigint;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction(SIGINT, &sa, NULL);

  // Procesa los argumentos de línea de comandos
  while ((opt = getopt(argc, argv, "p:f:")) != -1) {
    switch (opt) {
    case 'p':
      port = optarg;
      break;
    case 'f':
      working_directory = optarg;
      break;
    default: /* '?' */
      fprintf(stderr, "Uso: %s [-p puerto] [-f carpeta]\n", argv[0]);
      exit(EXIT_FAILURE);
    }
  }

  ctx = create_context();
  configure_context(ctx);

  int srv_socket = create_srv_socket(port);

  // aceptar conexiones entrantes
  listen(srv_socket, 100);

  while (!stop) {
    int *client_socket = malloc(sizeof(int));

    *client_socket = accept(srv_socket, (struct sockaddr *)NULL, NULL);
    if (*client_socket < 0) {
      free(client_socket);
      continue;
    }

    pthread_t client_thread;
    pthread_create(&client_thread, NULL, client_handler, (void *)client_socket);
    pthread_detach(client_thread);
  }

  printf("Servidor detendido de forma satisfactoria :)\n");
  close(srv_socket);

  return EXIT_SUCCESS;
}
