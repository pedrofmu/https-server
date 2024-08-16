struct http_request {
  char* method;
  char* params; 
};

struct http_request parse_request(char* request);

int parse_url(const char* url, char* server_ip, char* server_port);
