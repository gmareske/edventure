#pragma once

#include <stdint.h>

typedef struct node node_t;

typedef struct list list_t;

list_t *make_list();
void free_list(list_t *l);
void insert_front(list_t *l, uint8_t val);
void insert_back(list_t *l, uint8_t val);
uint8_t remove_front(list_t *l);
uint32_t get_size(list_t *l);
