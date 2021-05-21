#include <vector.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

static_assert(MIN_VECTOR_SIZE, "MIN_VECTOR_SIZE must be positive");

/* Helper functions */
static inline int min(int a, int b) {
    return a > b ? b : a;
}

static inline int max(int a, int b) {
    return a > b ? a : b;
}

static inline void VectorDoubleData(Vector restrict vector) {
    vector->data = realloc(vector->data, sizeof(void*) * (vector->alloc_size *= 2));
}

static inline void* VectorUnsafeGet(Vector restrict vector, int itemNum) {
    return vector->data[itemNum];
}

static inline void VectorUnsafeSet(Vector restrict vector, int itemNum, void* newItem) {
    vector->data[itemNum] = newItem;
}

/* Function definitions */
Vector NewVector(int size) {
    Vector restrict out = malloc(sizeof(VectorRepr));
    const int newAllocSize = max(size, MIN_VECTOR_SIZE);
    out -> data = malloc(sizeof(void*) * newAllocSize);
    out -> size = size;
    out -> alloc_size = newAllocSize;
    return out;
}

Vector NewEmptyVector() {
    Vector restrict out = malloc(sizeof(VectorRepr));
    out -> data = malloc(sizeof(void*) * MIN_VECTOR_SIZE);
    out -> size = 0;
    out -> alloc_size = MIN_VECTOR_SIZE;
    return out;
}

Vector NewFilledVector(int size, void* contents[]) {
    Vector out = NewVector(size);
    memcpy(out -> data, contents, sizeof(void*) * size);
    return out;
}

void DeleteVector(Vector vector) {
    free(vector->data);
    free(vector);
}

void* VectorGet(Vector restrict vector, int itemNum) {
    assert(itemNum < vector->size && itemNum >= 0);
    return VectorUnsafeGet(vector, itemNum);
}

void VectorSet(Vector restrict vector, int itemNum, void* newItem) {
    assert(itemNum < vector->size && itemNum >= 0);
    VectorUnsafeSet(vector, itemNum, newItem);
}

int VectorSize(Vector restrict vector) {
    return vector->size;
}

bool VectorEmpty(Vector restrict vector) {
    return VectorSize(vector) == 0;
}

int VectorAllocSize(Vector restrict vector) {
    return vector->alloc_size;
}

void VectorResize(Vector restrict vector, int size) {
    assert(size >= 0);
    vector->size = size;
    if(vector->alloc_size >= size)
        return;
    if(vector->alloc_size * 2 >= size) {
        VectorDoubleData(vector);
        return;
    }
    vector->data = realloc(vector->data, sizeof(void*) * (vector->alloc_size = size));
}

void VectorClear(Vector restrict vector) {
    VectorResize(vector, 0);
}

void VectorForcedResize(Vector restrict vector, int size) {
    size = max(size, MIN_VECTOR_SIZE);
    vector->data = realloc(vector->data, sizeof(void*) * (vector->alloc_size = size));
}

void VectorForcedClear(Vector restrict vector) {
    VectorForcedResize(vector, 0);
}

void VectorPushBack(Vector restrict vector, void* newItem) {
    vector->size++;
    if(vector->size > vector->alloc_size) {
        VectorDoubleData(vector);
    }
    VectorUnsafeSet(vector, vector->size - 1, newItem);
}

void* VectorPopBack(Vector restrict vector) {
    assert(vector->size);
    vector->size--;
    return VectorUnsafeGet(vector, vector->size);
}

void* VectorBack(Vector restrict vector) {
    return VectorUnsafeGet(vector, vector->size - 1);
}