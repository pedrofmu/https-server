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

struct http_request *parse_request(const char* request);

void free_request(struct http_request* req);
