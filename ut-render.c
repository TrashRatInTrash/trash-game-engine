
#include "lib/T-engine.h"

/*
  test the object (Thing) management
  of the game engine.
  can add and remove dynamically
*/

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

  Play_t *play = init_Play(800, 600);
  if (play == NULL) {
    return 1;
  }

  Scene_t *scene = init_Scene();
  if (scene == NULL) {
    return 1;
  }

  add_Scene_to_Play(scene, play);

  set_Active_State(play, scene);

  add_thing(scene, 100, 200, 50, 50, 1.0f, 1.0f, 1, 255, 0, 0, 255);

  run_Play(play);

  destroy_Scene(scene);

  return 0;
}
