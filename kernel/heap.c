#include "heap.h"
#include "stdio.h"

memory_chunk_t* heap = 0;

void combine_with_next(memory_chunk_t* chunk) {
    
    if(!chunk) return;
    
    chunk->next = chunk->next->next;
    chunk->next->prev = chunk;
    
}

void init_heap(void* addr) {
    
    if(!addr) return;
    
    heap = addr;
    heap->prev = 0;
    heap->next = 0;
    heap->used = 0;
    
}

void* kmalloc(size_t size) {
    
    memory_chunk_t* chunk;
    memory_chunk_t* next;
    
    if(!size || !heap) return 0;
    
    chunk = heap;
    
    while(chunk->next) {
        
        if((size_t)chunk->next - (size_t)chunk - sizeof(memory_chunk_t) >= size && !chunk->used) {
            
            /// TODO: split the chunk if its too big
            
            if((size_t)chunk->next - (size_t)chunk - sizeof(memory_chunk_t) - size > sizeof(memory_chunk_t)) {
                
                memory_chunk_t* new_chunk = (memory_chunk_t*)((size_t)chunk + size + sizeof(memory_chunk_t));
                
                new_chunk->prev = chunk;
                new_chunk->next = chunk->next;
                new_chunk->used = 0;
                
                chunk->next = new_chunk;
                
                if(!new_chunk->next->used) combine_with_next(new_chunk);
                
            }
            
            chunk->used = 1;
            return (void*)((size_t)chunk + sizeof(memory_chunk_t));
            
        }
        
        chunk = chunk->next;
        
    }
    
    next = (memory_chunk_t*)((size_t)chunk + sizeof(memory_chunk_t) + size);
    chunk->next = next;
    chunk->used = 1;
    
    next->prev = chunk;
    next->next = 0;
    next->used = 0;
    
    return (void*)((size_t)chunk + sizeof(memory_chunk_t));
    
}

void kfree(void* addr) {
    
    memory_chunk_t* chunk;
    
    if(!addr) return;
    
    chunk = (memory_chunk_t*)((size_t)addr - sizeof(memory_chunk_t));
    
    chunk->used = 0;
    
    if(!chunk->next->used) {
        
        combine_with_next(chunk);
        
    }
    if(chunk->prev && !chunk->prev->used) combine_with_next(chunk->prev);
    
}
