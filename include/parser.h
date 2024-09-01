struct http_request {
  char* method;
  char* params; 
};

struct http_request parse_request(char* request);
