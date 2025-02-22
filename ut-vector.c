

#include "lib/t-vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct test_struct {
  int id;
  char *name;
  size_t index;
};

void print_Array(struct dynArray_t *varray) {
  printf("capacity = %d\n", (int)varray->capacity);
  for (int i = 0; i < varray->ptr; i++) {
    printf("x: %i \n", ((struct test_struct *)get(varray, i))->id);
  }
}

int main() {

  struct dynArray_t *arr = create_Array(2);
  if (arr == NULL) {
    fprintf(stderr, "Failed to create array\n");
    return 1;
  }

  char *name = "name-test";
  struct test_struct a = {5, name, 0};
  struct test_struct b = {23, name, 0};
  struct test_struct c = {60, name, 0};

  append(&arr, &a);
  append(&arr, &b);
  append(&arr, &c);

  printf("Before deletion:\n");
  print_Array(arr);

  delete_at(arr, 1);

  printf("\nAfter deletion:\n");

  print_Array(arr);

  struct test_struct d = {66, name, 0};
  append(&arr, &d);

  printf("\nAfter appending new object:\n");
  print_Array(arr);

  struct test_struct *e = (struct test_struct *)get(arr, 5);
  if (e != NULL) {
    printf("index 5: %d\n", e->id);
  }

  destroy_Array(arr);

  return 0;
}
