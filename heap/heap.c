#include <assert.h>
#include <string.h>

#include "heap.h"

static inline void swap(int *a, int *b)
{
	int t = *a;
	*a = *b;
	*b = t;
}

static inline size_t parenti(size_t i)
{
	return i>0 ? (i-1)>>1 : i;
}
static inline size_t lefti(size_t i)
{
	return (i<<1)+1;
}
static inline size_t righti(size_t i)
{
	return (i<<1)+2;
}

struct heap
{
	int *ptr;
	size_t size, capacity;
};

size_t heap_size(struct heap *h)
{
	return h->size;
}
size_t heap_capacity(struct heap *h)
{
	return h->capacity;
}
int *heap_ptr(struct heap *h)
{
	return h->ptr;
}

static inline size_t num_leaves(struct heap *h)
{
	assert(h);
	return (h->size+1)>>1;
}
static inline size_t num_inner(struct heap *h)
{
	assert(h);
	return h->size>>1;
}

static void floyd_heapify_iter(struct heap *h, size_t i)
{
	for (size_t j = i; j < num_inner(h); )
	{
		const size_t l = lefti(j), r = righti(j);
		size_t next = j;
		if (l < h->size && h->ptr[l] > h->ptr[j])
			next = l;
		if (r < h->size && h->ptr[r] > h->ptr[next])
			next = r;
		if (next != j)
		{
			swap(h->ptr+j, h->ptr+next);
			j = next;
		} else break;
	}
}

static void floyd_build_heap(struct heap *h)
{
	for (ssize_t i = num_inner(h) - 1; i >= 0; --i)
		floyd_heapify_iter(h, i);
}

static void naive_build_heap(struct heap *h)
{
	assert(h);
	for (size_t i = 1; i < h->size; ++i)
		for (size_t k = i, p; k > 0 && h->ptr[p = parenti(k)] < h->ptr[k]; k = p)
			swap(h->ptr+k, h->ptr+p);
}

static inline void nonalloc_heap_init(struct heap *h, int *p, size_t s)
{
	h->ptr = p;
	h->capacity = h->size = s;
	floyd_build_heap(h);
}

struct heap *heap_init(int *p, size_t s)
{
  struct heap *h = malloc(sizeof(struct heap));
	nonalloc_heap_init(h, p, s);
	return h;
}

void heap_destroy(struct heap *h)
{
	free(h);
}

static inline void resize(struct heap *h)
{
	int *newp = realloc(h->ptr, h->capacity + h->capacity / 2);
	if (!newp)
		abort();
	if (h->ptr == newp)
		return;
	memcpy(newp, h->ptr, sizeof(int) * h->size);
	h->ptr = newp;
	h->capacity += h->capacity / 2;
}

void heap_insert(struct heap *h, int item)
{
	if (h->size + 1 > h->capacity)
		resize(h);
	h->ptr[h->size] = item;
	for (size_t i = h->size, p; i > 0 && h->ptr[p = parenti(i)] < h->ptr[i]; i = p)
		swap(h->ptr+i, h->ptr+p);
	++h->size;
}

bool heap_extract_max(struct heap *h, int *res)
{
	assert(h);
	if (!h->size)
		return false;
	*res = *h->ptr;
	swap(h->ptr, h->ptr + h->size - 1);
	--h->size;
	floyd_heapify_iter(h, 0);
	return true;
}

bool heap_max(struct heap *h, int *res)
{
	assert(h);
	if (!h->size)
		return false;
	*res = *h->ptr;
	return true;
}

void heap_sort(int *p, size_t s)
{
	struct heap h;
	nonalloc_heap_init(&h, p, s);
	for (ssize_t i = s - 1; i > 0; --i)
	{
		swap(h.ptr, h.ptr+i);
		--h.size;
		floyd_heapify_iter(&h, 0);
	}
}
