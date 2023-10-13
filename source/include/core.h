#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef __cplusplus
typedef int bool;
#endif

#define TRUE 0x01
#define FALSE 0x00

struct arena_f{
	char* data;
	size_t used;
	size_t capacity;
	size_t type_size; 
};

struct handle_list_f{
	struct arena_f resources;
	struct arena_f slots;
	char *safe_object;
	int start_free_slot;
	int free_slot_count;
	int slot_count;
};

struct slot_f{
	int resource_index;
	int slot_index;
	int next_free;
	int version;
};

struct handle_f{
	int index;
	int version;
};

/*==================== Game Platform ====================*/

// Memory Manager
char *get_memory_p(size_t size);
char *resize_memory_p(char *data, size_t old_size, size_t new_size);
bool free_memory_p(char *data);
char *copy_memory_p(char* destiny, char* source, size_t move_size);
char *move_memory_p(char* destiny, char* source, size_t move_size);

/*==================== Game Framework ====================*/

// Arena
struct arena_f init_arena_f(size_t type_size, size_t arena_size);
char *push_size_f(struct arena_f *arena, size_t size);
void reset_arena_f(struct arena_f *arena);

// Handle
struct handle_list_f init_handle_list_f(size_t type_size, int initial_size);
void set_safe_object_handle_list_f(struct handle_list_f *handle_list, void* item);
struct handle_f add_handle_list_f(struct handle_list_f *handle_list, void* item);
char *lookup_f(struct handle_list_f *handle_list, struct handle_f handle);
bool is_valid_handle_f(struct handle_list_f *handle_list, struct handle_f handle);
bool remove_handle_list_f(struct handle_list_f *handle_list, struct handle_f handle);
void reset_handle_list_f(struct handle_list_f *handle_list, bool b_resized_arenas, int new_size);
void free_handle_list_f(struct handle_list_f *handle_list);
