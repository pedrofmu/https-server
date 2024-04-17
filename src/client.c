#include "include/client.h"
#include "include/urlParse.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_BUF_SIZE 1024

char *send_get(char *url) {
  char server_ip[16];
  char server_port[6];

  parse_url(url, server_ip, server_port);

  int sockfd;
  struct sockaddr_in server_addr;
  char request[MAX_BUF_SIZE];
  char *response = NULL;
  ssize_t total_bytes_received = 0;

  // Crear un socket
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("Error al crear el socket");
    exit(EXIT_FAILURE);
  }

  // Especificar la dirección del servidor
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(strtoul(server_port, NULL, 10));
  server_addr.sin_addr.s_addr = inet_addr(server_ip);

  // Conectar al servidor
  if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <
      0) {
    perror("Error al conectar con el servidor");
    exit(EXIT_FAILURE);
  }

  // Construir la solicitud HTTP (por ejemplo, una solicitud GET)
  snprintf(request, sizeof(request),
           "GET / HTTP/1.1\r\n"
           "Host: %s:%s\r\n"
           "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:109.0) "
           "Accept: "
           "text/html,application/xhtml+xml,application/xml;q=0.9,image/"
           "avif,image/webp,*/*;q=0.8\r\n"
           "Accept-Language: en-US,en;q=0.5\r\n"
           "Accept-Encoding: gzip, deflate\r\n"
           "Connection: keep-alive\r\n"
           "Upgrade-Insecure-Requests: 1\r\n"
           "Sec-GPC: 1\r\n"
           "Cache-Control: no-cache\r\n"
           "Pragma: no-cache\r\n"
           "\r\n", server_ip, server_port);

  printf("Paquete enviado: %s\n", request);

  // Enviar la solicitud al servidor
  if (write(sockfd, request, strlen(request)) < 0) {
    perror("Error al enviar la solicitud");
    exit(EXIT_FAILURE);
  }

  // Recibir la respuesta del servidor
  // Recibir la respuesta del servidor incrementalmente
  ssize_t bytes_received;
  while ((bytes_received = recv(sockfd, request, MAX_BUF_SIZE, 0)) > 0) {
    response = realloc(response, total_bytes_received + bytes_received + 1);
    if (response == NULL) {
      perror("Error al asignar memoria");
      exit(EXIT_FAILURE);
    }
    memcpy(response + total_bytes_received, request, bytes_received);
    total_bytes_received += bytes_received;
  }
  if (bytes_received < 0) {
    perror("Error al recibir la respuesta");
    exit(EXIT_FAILURE);
  }

  // Agregar terminador nulo al final de la cadena
  response[total_bytes_received] = '\0';

  // Cerrar la conexión
  close(sockfd);

  return response;
}
