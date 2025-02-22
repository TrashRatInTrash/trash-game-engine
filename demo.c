#include "lib/T-engine.h"

void print_thing_ids(Scene_t *scene) {
  /*
    go upto `ptr` only, empty void pointers aren't to
    be NULL
  */
  printf("capacity = %d\n", scene->things->capacity);
  for (int i = 0; i < scene->things->ptr; i++) {
    Thing *thing = scene->things->data[i];
    if (thing == NULL) {
      printf("NULL thing");
    }
    printf("x: %f \n", thing->x);
  }
}

int main() {

  Scene_t *scene = init_Scene();
  if (scene == NULL) {
    return 1;
  }

  Thing *things[10];

  add_thing(scene, 100, 200, 50, 50, 1.0f, 1.0f, 1, 255, 0, 0, 255);
  add_thing(scene, 101, 200, 50, 50, 1.0f, 1.0f, 1, 255, 0, 0, 255);
  add_thing(scene, 102, 200, 50, 50, 1.0f, 1.0f, 1, 255, 0, 0, 255);
  add_thing(scene, 103, 200, 50, 50, 1.0f, 1.0f, 1, 255, 0, 0, 255);
  add_thing(scene, 104, 200, 50, 50, 1.0f, 1.0f, 1, 255, 0, 0, 255);
  add_thing(scene, 105, 200, 50, 50, 1.0f, 1.0f, 1, 255, 0, 0, 255);

  print_thing_ids(scene);

  destroy_Scene(scene);

  return 0;
}
