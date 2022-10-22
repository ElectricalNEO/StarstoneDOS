
#define LINKED_LIST(name, type) typedef struct name {\
    struct name* next;\
    type data; \
} name;
