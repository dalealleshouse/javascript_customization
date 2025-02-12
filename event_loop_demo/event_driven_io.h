typedef enum {
    STATE_CONNECTING,
    STATE_WRITING,
    STATE_READING,
    STATE_DONE,
    STATE_ERROR
} IOState;

typedef struct {
    int sockfd;
    IOState state;

    // The target hostname or IP, or the resolved address
    // Buffers for partial read/write

    // If you're returning a Promise to JS:
    JSContext *ctx;
    JSValue resolveFunc;
    JSValue rejectFunc;

    // For storing response data
    char *response;
    size_t responseSize;
} SocketIOItem;

