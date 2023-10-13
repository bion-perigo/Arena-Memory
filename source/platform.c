#include "core.h"

char *get_memory_p(size_t size){
    return (char*)malloc(size);
}

char *resize_memory_p(char *data, size_t old_size, size_t new_size){
    char* new_data = get_memory_p(new_size);
    return (char*)move_memory_p(new_data, data, old_size);
}

bool free_memory_p(char *data){
    if(data == NULL){
        return FALSE;
    }
    free(data);
    return TRUE;
}

char *copy_memory_p(char* destiny, char* source, size_t move_size){
    return  (char*)memcpy(destiny, source, move_size);
}

char *move_memory_p(char* destiny, char* source, size_t move_size){
    char* ret_val = (char*)memcpy((void*)destiny, (void*)source, move_size);
    free_memory_p(source);
    return ret_val;
}
