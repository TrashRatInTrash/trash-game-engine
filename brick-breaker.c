
#include "lib/T-engine.h"
#include "lib/t-vector.h"

#define WIDTH 600
#define HEIGHT 900

#define PLAYER_TYPE 5
#define BULLET_TYPE 6
#define BRICK_TYPE 7

#define NUMBER_OF_COLUMNS 5
#define NUMBER_OF_ROWS 6

#define BRICK_WIDTH 100
#define BRICK_HEIGHT 50

Play_t *play;

void malloc_Brick() {}

void update_renderer(void *pgame, Thing_t *thing) {}

void player_Update(Scene_t *scene, Thing_t *player, float d_time) {

  if (play->key_right) {
    player->vx = 500;
  }
  if (play->key_left) {
    player->vx = -500;
  } else {
    player->vx /= 1.5;
  }
}

void bullet_Update(Scene_t *scene, Thing_t *bullet, float d_time) {

  if (bullet->x >= WIDTH - bullet->width / 2.0f) {
    bullet->vx *= -1;
  }
  if (bullet->x <= bullet->width / 2.0f) {
    bullet->vx *= -1;
  }

  for (int i = 0; i < scene->things->ptr; i++) {
    Thing_t *other = get(scene->things, i);
    if (other == NULL) {
      continue;
    }
    if (other->type_id != 6) {
      if (is_bounding_box_collision(bullet, other)) {
        printf("collision with tid %d\n", other->type_id);
        bullet->vy *= -1;
        if (other->type_id == BRICK_TYPE) {
          destroy_Thing(scene, other);
        }
        break;
      }
    }
  }
}

void update(void *pgame, Thing_t *thing, float d_time) {

  Scene_t *scene = (Scene_t *)pgame;
  switch (thing->type_id) {
  case PLAYER_TYPE:
    player_Update(scene, thing, d_time);
    break;
  case BRICK_TYPE:
    break;
  case BULLET_TYPE:
    bullet_Update(scene, thing, d_time);
    break;
  }
}

void init_scene(Scene_t *scene) {

  Thing_t *player = add_thing(scene, 175, 850, 70, 20, 0.0f, 0.0f, PLAYER_TYPE,
                              255, 0, 0, 255);

  Thing_t *bricks[NUMBER_OF_COLUMNS][NUMBER_OF_ROWS];

  int hor_offset = BRICK_WIDTH + 15;
  int vert_offset = BRICK_HEIGHT + 15;
  int x_start = (WIDTH - ((BRICK_WIDTH + 15) * NUMBER_OF_COLUMNS)) / 2 +
                ((BRICK_WIDTH + 15) / 2);

  for (int i = 0; i < NUMBER_OF_COLUMNS; i++) {
    for (int j = 0; j < NUMBER_OF_ROWS; j++) {
      bricks[i][j] = add_thing(
          scene, x_start + (i * hor_offset), 50 + (j * vert_offset),
          BRICK_WIDTH, BRICK_HEIGHT, 0.0f, 0.0f, BRICK_TYPE, 0, 255, 0, 255);
    }
  }

  Thing_t *ball = add_thing(scene, 175, 700, 20, 20, -100, -100, BULLET_TYPE, 0,
                            0, 255, 255);

  printf("Ball initial velocity: vx = %f, vy = %f\n", ball->vx,
         ball->vy); // Debug print
}

int main() {

  srand(time(NULL));
  play = init_Play(WIDTH, HEIGHT);
  if (play == NULL) {
    return 1;
  }

  Scene_t *scene = init_Scene();
  if (scene == NULL) {
    return 1;
  }
  scene->on_update = update;
  scene->on_update_renderer = update_renderer;

  add_Scene_to_Play(scene, play);

  set_Active_State(play, scene);

  init_scene(scene);

  run_Play(play);

  destroy_Scene(scene);

  return 0;
}
