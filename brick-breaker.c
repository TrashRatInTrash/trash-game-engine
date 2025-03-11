
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

void update(void *pgame, Thing_t *thing, float d_time) {}

void init_scene(Scene_t *scene) {

  Thing_t *player = add_thing(scene, 175, 800, 50, 10, 0.0f, 0.0f, PLAYER_TYPE,
                              255, 0, 0, 255);

  Thing_t *bricks[NUMBER_OF_COLUMNS][NUMBER_OF_ROWS];

  int hor_offset = BRICK_WIDTH + 15;
  int vert_offset = BRICK_HEIGHT + 15;
  int x_start = (WIDTH - ((BRICK_WIDTH + 15) * NUMBER_OF_COLUMNS)) / 2;

  for (int i = 0; i < NUMBER_OF_COLUMNS; i++) {
    for (int j = 0; j < NUMBER_OF_ROWS; j++) {
      bricks[i][j] = add_thing(
          scene, x_start + (i * hor_offset), 50 + (j * vert_offset),
          BRICK_WIDTH, BRICK_HEIGHT, 0.0f, 0.0f, BRICK_TYPE, 0, 255, 0, 255);
    }
  }
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
