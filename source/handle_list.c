#include "core.h"

struct handle_list_f init_handle_list_f(size_t type_size, int initial_size){
    struct handle_list_f handle_list;
    handle_list.resources = init_arena_f(type_size, type_size * initial_size);
    handle_list.slots = init_arena_f(sizeof(struct slot_f), initial_size * sizeof(struct slot_f));
    handle_list.slot_count = 0;
    handle_list.free_slot_count = 0;
    handle_list.start_free_slot = -1;
    handle_list.safe_object = NULL;
    printf("HANDLE_LIST: Initialize => Slots Count:%d, Free Slots Count:%d, Start Free Slot:%d\n", handle_list.slot_count, handle_list.free_slot_count, handle_list.start_free_slot);
    return handle_list;
}

void set_safe_object_handle_list_f(struct handle_list_f *handle_list, void* item){
    handle_list->safe_object = (char*)malloc(handle_list->resources.type_size);
    copy_memory_p(handle_list->safe_object, (char*)item, handle_list->resources.type_size);
}

struct handle_f add_handle_list_f(struct handle_list_f *handle_list, void* item){
    struct slot_f *slot_info = NULL;
    char *resource = NULL;
    ++handle_list->slot_count;

    if(handle_list->free_slot_count > 0){
        --handle_list->free_slot_count;
        char* slot_addr = (char*)handle_list->slots.data + handle_list->start_free_slot * handle_list->slots.type_size;
        slot_info = (struct slot_f*)slot_addr;
        handle_list->start_free_slot = slot_info->next_free;
    }else{
        char* slot_addr = push_size_f(&handle_list->slots, handle_list->slots.type_size);
        slot_info = (struct slot_f*)slot_addr;
        slot_info->version = 0;
        slot_info->slot_index = handle_list->slot_count - 1;
        push_size_f(&handle_list->resources, handle_list->resources.type_size);
    }

    slot_info->resource_index = handle_list->slot_count - 1;
    resource = (char*)handle_list->resources.data + slot_info->resource_index * handle_list->resources.type_size;
    copy_memory_p(resource, (char*)item, handle_list->resources.type_size);

    struct handle_f handle;
    handle.index = slot_info->slot_index;
    handle.version = slot_info->version;

    return handle;
}

char *lookup_f(struct handle_list_f *handle_list, struct handle_f handle){
    char *ret_val = (handle_list->safe_object != NULL) ? handle_list->safe_object : (char*)"Safe Object not found";

    if(handle.index >= handle_list->slot_count){
        printf("LOOKUP: Index not found => Handle Index:%d, Slot Count:%d\n", handle.index, handle_list->slot_count);
        if(handle_list->safe_object == NULL) printf("LOOKUP FATAL:%s, for index:%d\n", ret_val, handle.index);
        return handle_list->safe_object;
    }
    
    char* slot_add = (char*)handle_list->slots.data + (handle.index * handle_list->slots.type_size);
    struct slot_f *slot = (struct slot_f*)slot_add;

    if(slot == NULL || slot->version != handle.version){
        printf("LOOKUP: Different handler version => Handle Version:%d - Slot Version:%d\n", handle.version, ((slot != NULL) ? slot->version : -1));
        if(handle_list->safe_object == NULL) printf("LOOKUP FATAL:%s, for index:%d\n", ret_val, handle.index);
        return handle_list->safe_object;
    }

    return (char*)handle_list->resources.data + slot->resource_index * handle_list->resources.type_size;
}

bool is_valid_handle_f(struct handle_list_f *handle_list, struct handle_f handle){
    if(handle.index + 1 > handle_list->slot_count || handle.index == -1){
        return FALSE;
    }

    char *slot_addr = (char*)handle_list->slots.data + handle.index * handle_list->slots.type_size;
    struct slot_f *slot = (struct slot_f*)slot_addr;

    if(slot == NULL || slot->version != handle.version){
        return FALSE;
    }

    return TRUE;
}

bool remove_handle_list_f(struct handle_list_f *handle_list, struct handle_f handle){
    if(handle.index + 1 > handle_list->slot_count || handle.index < 0){
        return FALSE;
    }

    // You need to get the address in char* and then convert it, Bug!!
    char *last_addr = (char*)handle_list->slots.data + (handle_list->slot_count -1) * handle_list->slots.type_size;
    char *removed_addr = (char*)handle_list->slots.data + handle.index * handle_list->slots.type_size;
    struct slot_f *last_slot = (struct slot_f*)last_addr;
    struct slot_f *removed_slot = (struct slot_f*)removed_addr;
    ++removed_slot->version;

    if(last_slot->resource_index != removed_slot->resource_index){
        char* last_resource = handle_list->resources.data + last_slot->resource_index * handle_list->resources.type_size;
        char* removed_resource = handle_list->resources.data + removed_slot->resource_index * handle_list->resources.type_size;
        copy_memory_p(removed_resource, last_resource, handle_list->resources.type_size);
    }

    int old = removed_slot->resource_index;
    removed_slot->resource_index = last_slot->resource_index;
    last_slot->resource_index = old;

    removed_slot->next_free = handle_list->start_free_slot;
    handle_list->start_free_slot = handle.index;
    ++handle_list->free_slot_count;
    --handle_list->slot_count;

    return FALSE;
}

void reset_handle_list_f(struct handle_list_f *handle_list, bool b_resized_arenas, int new_size){
    handle_list->free_slot_count = 0;
    handle_list->slot_count = 0;
    handle_list->start_free_slot = -1;

    if(b_resized_arenas){ // handle_list->resources.data <=> Bug na hora de copiar a memoria maior pra menor
        free_memory_p(handle_list->slots.data);
        free_memory_p(handle_list->resources.data);
        handle_list->slots.data = get_memory_p(new_size * handle_list->slots.type_size);
        handle_list->resources.data = get_memory_p(new_size * handle_list->resources.type_size);
    }
    reset_arena_f(&handle_list->slots);
    reset_arena_f(&handle_list->resources);
}

void free_handle_list_f(struct handle_list_f *handle_list){
    handle_list->free_slot_count = 0;
    handle_list->slot_count = 0;
    handle_list->start_free_slot = -1;
    free_memory_p(handle_list->slots.data);
    free_memory_p(handle_list->resources.data);
    free_memory_p(handle_list->safe_object);

    handle_list->slots.data = NULL;
    handle_list->resources.data = NULL;
    handle_list->safe_object = NULL;
}