struct http_header {
  char *name;
  char *value;

  struct http_header *next_header;
};

struct http_request {
  char *method;
  char *path;
  struct http_header *headers;
  char *body;
};

struct http_request parse_request(char* request);
