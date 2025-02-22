#include "t-vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct dynArray_t *create_Array(size_t capacity) {
  struct dynArray_t *varray = malloc(sizeof(struct dynArray_t));
  if (varray == NULL) {
    return NULL;
  }

  varray->capacity = capacity;
  varray->ptr = 0;

  varray->data = malloc(sizeof(void *) * capacity);
  if (varray->data == NULL) {
    free(varray);
    return NULL;
  }

  return varray;
}

struct dynArray_t *resize_Array(struct dynArray_t *p_varray) {
  printf("resizing\n");
  size_t new_capacity = p_varray->capacity * RESIZE_CONSTANT;

  struct dynArray_t *n_varray = create_Array(new_capacity);
  if (n_varray == NULL) {
    return NULL;
  }

  memcpy(n_varray->data, p_varray->data, sizeof(void *) * p_varray->ptr);
  n_varray->ptr = p_varray->ptr;
  free(p_varray->data);
  free(p_varray);

  return n_varray;
}

int append(struct dynArray_t **varray, void *element) {
  if ((*varray)->ptr >= (*varray)->capacity) {
    *varray = resize_Array(*varray);
    if (*varray == NULL) {
      return -1;
    }
  }
  (*varray)->data[(*varray)->ptr] = element;
  (*varray)->ptr++;
  return 0;
}

void *get(struct dynArray_t *varray, size_t index) {
  if (index >= varray->ptr) {
    return NULL;
  }
  return varray->data[index];
}

void delete_at(struct dynArray_t *varray, size_t index) {
  if (index >= varray->ptr) {
    return;
  }

  varray->data[index] = varray->data[varray->ptr - 1];
  varray->ptr--;
}

void destroy_Array(struct dynArray_t *varray) {
  if (varray != NULL) {
    if (varray->data != NULL) {
      free(varray->data);
    }
    free(varray);
  }
}

