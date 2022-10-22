#include "string.h"

char* strtok_buf;

char* strtok(char* str, char* delim) {
    
    char* start;
    uint8_t i = 0;
    
    if(str) strtok_buf = str;
    start = strtok_buf;
    
    if(!*strtok_buf) return 0;
    
    while(1) {
        
        for(i = 0; delim[i]; i++) if(*strtok_buf == delim[i]) goto end;
        if(!*strtok_buf) return start;
        strtok_buf++;
        
    }
    
    end:
    
    *strtok_buf = 0;
    strtok_buf++;
    
    return start;
    
}

size_t strlen(char* str) {
    
    size_t ret = 0;
    while(*str++) ret++;
    return ret;
    
}

void to_uppercase(char* str) {
    
    while(*str) {
        
        if(*str >= 'a' && *str <= 'z') *str -= ('a' - 'A');
        str++;
        
    }
    
}

void to_lowercase(char* str) {
    
    while(*str) {
        
        if(*str >= 'A' && *str <= 'Z') *str += ('a' - 'A');
        str++;
        
    }
    
}
