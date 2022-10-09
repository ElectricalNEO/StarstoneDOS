#include "intr.h"

void start() {
    
    int10h_teletype_output('X', 0, 0);
    while(1);
    
}
