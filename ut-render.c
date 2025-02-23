

/*
  demonstrate:
  - rendering objects
  - player control
  !- zombie ai
  !- bullets
  !- collisions
*/

#include "lib/T-engine.h"

#define PLAYER_TYPE 5
#define BULLET_TYPE 6
#define ZOMBIE_TYPE 7

Play_t *play;

void print_thing_ids(Scene_t *scene) {
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

void player_Update(Scene_t *scene, Thing *thing, float d_time) {

  if (play->key_up) {
    thing->vy = -500;
  }
  if (play->key_down) {
    thing->vy = 500;
  } else {
    thing->vy /= 1.1;
  }

  if (play->key_right) {
    thing->vx = 500;
  }
  if (play->key_left) {
    thing->vx = -500;
  } else {
    thing->vx /= 1.1;
  }
}

void bullet_Update(Scene_t *scene, Thing *thing, float d_time) {}

void zombie_Update(Scene_t *scene, Thing *thing, float d_time) {

  Thing *player = get(scene->things, 0);

  float vx = player->x - thing->x;
  float vy = player->y - thing->y;

  normalize_Vector(&vx, &vy);

  thing->vx = vx * 100;
  thing->vy = vy * 100;
  // printf("vx: %f vy: %f\n", thing->vx, thing->vy);
}

void update(void *pscene, Thing *thing, float d_time) {

  Scene_t *scene = (Scene_t *)pscene;

  switch (thing->type_id) {
  case PLAYER_TYPE:
    player_Update(scene, thing, d_time);
    break;
  case ZOMBIE_TYPE:
    zombie_Update(scene, thing, d_time);
    break;
  case BULLET_TYPE:
    bullet_Update(scene, thing, d_time);
    break;
  }
}

int main() {

  play = init_Play(800, 600);
  if (play == NULL) {
    return 1;
  }

  Scene_t *scene = init_Scene();
  if (scene == NULL) {
    return 1;
  }
  scene->on_update = update;

  add_Scene_to_Play(scene, play);

  set_Active_State(play, scene);

  add_thing(scene, 100, 200, 10, 10, 0.0f, 0.0f, PLAYER_TYPE, 255, 0, 0, 255);
  Thing *zombie =
      add_thing(scene, 500, 500, 10, 10, 0.0f, 0.0f, ZOMBIE_TYPE, 255, 255, 0, 255);

  print_thing_ids(scene);

  run_Play(play);

  destroy_Scene(scene);

  return 0;
}
