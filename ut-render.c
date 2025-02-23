/*
  demonstrate:
  - rendering objects
  - player control
  - zombie ai
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

void zombie_Update(Scene_t *scene, Thing *zombie, float d_time) {

  Thing *player = get(scene->things, 0);

  float vx = player->x - zombie->x;
  float vy = player->y - zombie->y;

  normalize_Vector(&vx, &vy);

  zombie->vx = vx * 100;
  zombie->vy = vy * 100;
  // printf("vx: %f vy: %f\n", thing->vx, thing->vy);

  if (calculate_distance_things(zombie, player) < 50) {
    zombie->color[0] = 255;
    if (is_bounding_box_collision(zombie, player)) {
      player->color[1] = 255;
    } else {
      player->color[1] = 0;
    }
  } else {
    zombie->color[0] = 0;
  }
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

void generate_zombie_coords(float *x, float *y) {
  if (rand() % 2 == 0) {
    // Left or right edge
    *x = (rand() % 2 == 0) ? (rand() % 20) : (780 + rand() % 20);
    *y = rand() % 600;
  } else {
    // Top or bottom edge
    *x = rand() % 800;
    *y = (rand() % 2 == 0) ? (rand() % 20) : (580 + rand() % 20);
  }
}

void spawn_zombies(Scene_t *scene) {
  for (int i = 0; i < 2; i++) {
    float x = 0;
    float y = 0;
    generate_zombie_coords(&x, &y);
    add_thing(scene, x, y, 10, 10, 0.0f, 0.0f, ZOMBIE_TYPE, 255, 255, 0, 255);
  }
}

int main() {

  srand(time(NULL));
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

  // add player at index 0 to access easier
  // TODO need a more dynamic alternative
  add_thing(scene, 400, 300, 10, 10, 0.0f, 0.0f, PLAYER_TYPE, 255, 0, 0, 255);

  // add zombies afterwards
  spawn_zombies(scene);

  print_thing_ids(scene);

  run_Play(play);

  destroy_Scene(scene);

  return 0;
}
