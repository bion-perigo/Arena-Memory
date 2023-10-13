#include "core.h"

struct arena_f init_arena_f(size_t type_size, size_t arena_size){
    struct arena_f arena;
    arena.capacity = arena_size;
    arena.type_size = type_size;
    arena.used = 0;
    arena.data = get_memory_p(arena_size);
    printf("ARENA: Initialize => Type Size:%llu Bytes, Size:%llu/%llu Bytes\n", arena.type_size, arena.used, arena.capacity);
    return arena;
}

char *push_size_f(struct arena_f *arena, size_t size){
    if(arena->used + size >= arena->capacity){
        printf("ARENA: Resized => Old Size:%llu, New Size:%llu\n", arena->capacity, arena->capacity * 2);
        arena->data = resize_memory_p(arena->data, arena->used, arena->capacity * 2);
        arena->capacity = arena->capacity * 2;
    }
    char *address = arena->data + arena->used;
    arena->used += size;

    return address;
}

void reset_arena_f(struct arena_f *arena){
    arena->used = 0;
}