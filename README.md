# Servidor https simple 

Este es un servidor HTTPS simple. Es un proyecto personal que he realizado para aprender cómo funciona un servidor HTTPS y otros aspectos de la programación de red a bajo nivel.

Se implementan los protocolos GET y PUT; sin embargo, no está preparado para un entorno de producción.


## Tecnologías Utilizadas

- OpenSSL 
- C 
- librerias de C como `sys/socket.h` 

## Funcionamiento

En la función `main()` se crea un socket (`srv_socket`) que se pone a escuchar peticiones entrantes.

Si un cliente se conecta, se crea un nuevo socket que se pasa a un nuevo thread para permitir que haya más de un cliente de forma simultánea.

En el `client_handler()`, se realiza el handshake de SSL y se lee la request. Una vez leída, se hace un parse de dicha request y, posteriormente, se procesan los datos.

Por último, la función `create_http_response()` devuelve un `char*` que es lo que se envía al socket del cliente.

## Instalación

Para instalarlo y probarlo tu mismo sigue los siguientes pasos (estan basados en Fedora Linux)

1. Clona el repositorio:
   ```bash
   git clone https://github.com/pedrofmu/https-server.git
   ```

2. Muevete a la carpeta
   ```bash
   cd https-server 
   ```
3. Crea la carpeta de la build  
   ```bash
   mkdir build 
   ```
4. Asegurate de tener las dependencias necesarias  
   ```bash
   sudo dnf install gcc cmake openssl 
   ```
5. Muevete a la carpeta de build  
   ```bash
   cd build
   ```
6. Muevete a la carpeta de build  
   ```bash
   cd build
   ```
7. Complia el codigo  
   ```bash
   cmake .. & make 
   ```

## Uso

Para utilizarlo simplemente ejecuta el binario.
   ```bash
   (carpeta-de-build)/https-server 
   ```
Le debes pasar como parametros el puerto "-p" y la carpeta en la que se va a ejecutar "-f"

Para testearlo puedes acceder desde tu navegador https://(tu ip o 127.0.0.1):(el puerto que hallas seleccionado/(el archivo que quiera acceder).

## Contacto

Si quieres contactarme puedes enviarme un correo a:

> contacto@pedrofm.dev

O ver mi pagina web:

> pedrfm.dev
