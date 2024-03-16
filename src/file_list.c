#include "voom_main.h"


Handle *alloc_handle(Buffer buf) {
    Handle *new_node = malloc(sizeof(Handle));
    if (new_node) {
        *new_node = (Handle){buf, new_node, new_node};
    }
    return new_node;
}

void free_handle(Handle *node) {
    if (node) {
        free(node->data.lines);
        free(node);
    }
}

Handle *create_buffer_list(Buffer buf) {
    Handle *new_node = alloc_handle(buf);
    if (!new_node) {
        fprintf(stderr, "Failed to allocate memory\n");
    }
    return new_node;
}

void insert_after(Handle *node, Buffer buf) {
    Handle *new_node = alloc_handle(buf);
    if (new_node) {
        new_node->prev = node;
        new_node->next = node->next;
        node->next->prev = new_node;
        node->next = new_node;
    } else {
        fprintf(stderr, "Failed to allocate memory\n");
    }
}

void remove_node(Handle *node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
    free_handle(node);
}

void free_buffer_list(Handle *head) {
    if (head) {
        Handle *current = head->next;
        while (current != head) {
            Handle *next = current->next;
            free_handle(current);
            current = next;
        }
        free_handle(head);
    }
}