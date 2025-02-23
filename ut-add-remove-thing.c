
/*
  test the object (Thing) management
  of the game engine.
  can add and remove dynamically
*/

#include "lib/T-engine.h"

void print_thing_ids(Scene_t *scene) {
  /*
    go upto `ptr` only, empty void pointers aren't to
    be NULL
  */
  printf("capacity = %d\n", (int)scene->things->capacity);
  for (int i = 0; i < scene->things->ptr; i++) {
    Thing *thing = scene->things->data[i];
    if (thing == NULL) {
      printf("NULL thing");
      continue;
    }
    printf("x: %f \n", thing->x);
  }
}

int main() {

  // Play_t *play = init_Play(800, 600);
  // if (play == NULL) {
  //   return 1;
  // }

  Scene_t *scene = init_Scene();
  if (scene == NULL) {
    return 1;
  }

  // add_Scene_to_Play(scene, play);

  add_thing(scene, 100, 200, 50, 50, 1.0f, 1.0f, 1, 255, 0, 0, 255);
  add_thing(scene, 101, 200, 50, 50, 1.0f, 1.0f, 1, 255, 0, 0, 255);
  add_thing(scene, 102, 200, 50, 50, 1.0f, 1.0f, 1, 255, 0, 0, 255);
  add_thing(scene, 103, 200, 50, 50, 1.0f, 1.0f, 1, 255, 0, 0, 255);
  add_thing(scene, 104, 200, 50, 50, 1.0f, 1.0f, 1, 255, 0, 0, 255);
  add_thing(scene, 105, 200, 50, 50, 1.0f, 1.0f, 1, 255, 0, 0, 255);

  print_thing_ids(scene);

  destroy_Thing(scene, 3);
  destroy_Thing(scene, 2);

  add_thing(scene, 106, 200, 50, 50, 1.0f, 1.0f, 1, 255, 0, 0, 255);
  add_thing(scene, 107, 200, 50, 50, 1.0f, 1.0f, 1, 255, 0, 0, 255);

  destroy_Thing(scene, 4);

  print_thing_ids(scene);

  destroy_Scene(scene);

  return 0;
}
