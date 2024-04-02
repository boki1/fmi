#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#pragma once

struct heap;

struct heap *heap_init(int *p, size_t s);
void heap_destroy(struct heap *h);

void heap_insert(struct heap *h, int item);
bool heap_extract_max(struct heap *h, int *res);
bool heap_max(struct heap *h, int *res);

size_t heap_size(struct heap *h);
size_t heap_capacity(struct heap *h);
int *heap_ptr(struct heap *h);

void heap_sort(int *p, size_t s);
