#include <stdlib.h>
#include <stdint.h>

#include "list.h"
/* 
  Double-pointer linked list implementation
*/

typedef struct node {
    uint8_t data;
    struct node *next;
} node_t;

typedef struct list {
    node_t *first;
    node_t *last;
    int32_t size;
} list_t;

node_t *make_node(uint8_t c) {
    node_t *ret = (node_t*)calloc(1, sizeof(node_t));
    ret->data = c;
    ret->next = NULL;
    return ret;
}
void free_node(node_t *n) {
    free(n);
}

list_t *make_list() {
    list_t *ret = (list_t*)calloc(1,sizeof(list_t));
    ret->first = NULL;
    ret->last = NULL;
    ret->size = 0;
    return ret;
}

void free_list(list_t *l) {
    node_t *cur = l->first;
    node_t *next;
    while (cur != NULL) {
	next = cur->next;
	free_node(cur);
	cur = next;
    }
    free(l);
}

void insert_front(list_t *l, uint8_t val) {
    node_t *n = make_node(val);
    n->next = l->first;
    l->first = n;
    if (n->next == NULL) {
	// this is the first added element
	// set the backend pointer of the list to n
	l->last = n;
    }
    l->size++;
}

void insert_back(list_t *l, uint8_t val) {
    node_t *n = make_node(val);
    l->last->next = n;
    l->last = n;
    n->next = NULL;
    if (l->first = NULL) {
	// this is the first added element
	// update the front pointer
	l->first = n;
    }
    l->size++;
}
uint8_t remove_front(list_t *l) {
    if (l->first == NULL) {return 0;}
    node_t *cur = l->first;
    uint8_t val = cur->data;
    l->first = cur->next;
    free(cur);
    l->size--;
    return val;
}

uint32_t get_size(list_t *l) {
    return l->size;
}
