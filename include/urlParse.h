struct http_request {
  char* method;
  char* params; 
};

struct http_request parse_request(char* request);

char* create_http_response(char* request);

//parse the url and includes the server ip and server port
int parse_url(const char* url, char* server_ip, char* server_port);

//Return 0 in succes, the values param and data that return the querry 
int get_query(char* values, char* data);
