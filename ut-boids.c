
/*
  Implementation of boids' 3 rules
  using trash game engine
  
  algorithm done according to:
  https://people.ece.cornell.edu/land/courses/ece4760/labs/s2021/Boids/Boids.html

  with an added rule for boundry avoidance
*/

#include "lib/T-engine.h"
#include "lib/t-vector.h"

#define WIDTH 1000
#define HEIGHT 700

#define BIRD_TYPE 5

#define SEPARATION_WEIGHT 2
#define SEPARATION_RANGE 50

#define NEIGHBORHOOD_RANGE 200
#define ALIGNMENT_WEIGHT 0.1

#define COHESION_WEIGHT 0.5

#define BOUNDARY_MARGIN 50
#define BOUNDARY_REPULSION_STRENGTH 5.0

#define SPEED_MIN 100
#define SPEED_MAX 500

Play_t *play;

void separation(Scene_t *scene, Thing_t *bird, float d_time) {
  float close_dx = 0;
  float close_dy = 0;
  int count = 0;

  for (int i = 0; i < scene->things->ptr; i++) {
    Thing_t *other = get(scene->things, i);
    if (other->id == bird->id)
      continue;

    float dist = calculate_distance_things(bird, other);
    if (dist > SEPARATION_RANGE)
      continue;

    float inv_dist = 1.0f - (dist / SEPARATION_RANGE);
    close_dx += (bird->x - other->x) * inv_dist;
    close_dy += (bird->y - other->y) * inv_dist;
    count++;
  }

  if (count > 0) {
    close_dx /= count;
    close_dy /= count;
    float mag = sqrt(close_dx * close_dx + close_dy * close_dy);
    if (mag > 0) {
      close_dx = (close_dx / mag) * SEPARATION_WEIGHT;
      close_dy = (close_dy / mag) * SEPARATION_WEIGHT;
    }

    bird->vx += close_dx;
    bird->vy += close_dy;
  }
}

void alignment(Scene_t *scene, Thing_t *bird, float d_time) {
  float xvel_avg = 0;
  float yvel_avg = 0;
  int neighboring_boids = 0;

  for (int i = 0; i < scene->things->ptr; i++) {
    Thing_t *other = get(scene->things, i);
    if (other->id == bird->id)
      continue;

    float dist = calculate_distance_things(bird, other);
    if (dist > NEIGHBORHOOD_RANGE)
      continue;

    float inv_dist = 1.0f - (dist / NEIGHBORHOOD_RANGE);
    xvel_avg += other->vx * inv_dist;
    yvel_avg += other->vy * inv_dist;
    neighboring_boids++;
  }

  if (neighboring_boids > 0) {
    xvel_avg /= neighboring_boids;
    yvel_avg /= neighboring_boids;

    bird->vx += (xvel_avg - bird->vx) * ALIGNMENT_WEIGHT;
    bird->vy += (yvel_avg - bird->vy) * ALIGNMENT_WEIGHT;
  }
}

void cohesion(Scene_t *scene, Thing_t *bird, float d_time) {
  float xpos_avg = 0;
  float ypos_avg = 0;
  int neighboring_boids = 0;

  for (int i = 0; i < scene->things->ptr; i++) {
    Thing_t *other = get(scene->things, i);
    if (other->id == bird->id)
      continue;

    float dist = calculate_distance_things(bird, other);
    if (dist > NEIGHBORHOOD_RANGE)
      continue;

    xpos_avg += other->x;
    ypos_avg += other->y;
    neighboring_boids++;
  }

  if (neighboring_boids > 0) {
    xpos_avg /= neighboring_boids;
    ypos_avg /= neighboring_boids;

    // Move toward center (but not too strongly)
    bird->vx += (xpos_avg - bird->x) * COHESION_WEIGHT * 0.01f;
    bird->vy += (ypos_avg - bird->y) * COHESION_WEIGHT * 0.01f;
  }
}

void boundary_Avoidance(Thing_t *bird) {
  float repulse_x = 0;
  float repulse_y = 0;

  if (bird->x < BOUNDARY_MARGIN) {
    float factor = 1.0 - (bird->x / BOUNDARY_MARGIN);
    repulse_x += factor * BOUNDARY_REPULSION_STRENGTH;
  }
  else if (bird->x > WIDTH - BOUNDARY_MARGIN) {
    float factor = (bird->x - (WIDTH - BOUNDARY_MARGIN)) / BOUNDARY_MARGIN;
    repulse_x -= factor * BOUNDARY_REPULSION_STRENGTH;
  }

  if (bird->y < BOUNDARY_MARGIN) {
    float factor = 1.0 - (bird->y / BOUNDARY_MARGIN);
    repulse_y += factor * BOUNDARY_REPULSION_STRENGTH;
  }
  else if (bird->y > HEIGHT - BOUNDARY_MARGIN) {
    float factor = (bird->y - (HEIGHT - BOUNDARY_MARGIN)) / BOUNDARY_MARGIN;
    repulse_y -= factor * BOUNDARY_REPULSION_STRENGTH;
  }

  bird->vx += repulse_x;
  bird->vy += repulse_y;
}

void bird_Update(Scene_t *scene, Thing_t *bird, float d_time) {

  separation(scene, bird, d_time);
  alignment(scene, bird, d_time);
  cohesion(scene, bird, d_time);
  boundary_Avoidance(bird);

  float speed = sqrt(bird->vx * bird->vx + bird->vy * bird->vy);

  if (speed > SPEED_MAX) {
    bird->color[2] = 255;
    bird->vx = (bird->vx / speed) * SPEED_MAX;
    bird->vy = (bird->vy / speed) * SPEED_MAX;
  } else if (speed < SPEED_MIN) {
    bird->color[2] = 255;
    bird->vx = (bird->vx / speed) * SPEED_MIN;
    bird->vy = (bird->vy / speed) * SPEED_MIN;
  } else {
    bird->color[2] = 0;
    // bird->vx = (rand() % 5) - 2;
    // bird->vy = (rand() % 5) - 2;
  }

  if (bird->x < 0) {
    bird->x = 0;
    bird->vx = -bird->vx;
  } else if (bird->x > WIDTH) {
    bird->x = WIDTH;
    bird->vx = -bird->vx;
  }

  if (bird->y < 0) {
    bird->y = 0;
    bird->vy = -bird->vy;
  } else if (bird->y > HEIGHT) {
    bird->y = HEIGHT;
    bird->vy = -bird->vy;
  }
}

void bird_Update_Renderer(Scene_t *scene, Thing_t *bird) {

  if (bird == NULL || bird->poly == NULL) {
    return;
  }
  double target_angle = get_angle_between_points(0, 0, bird->vx, bird->vy);

  double angle_diff = target_angle - bird->poly->angle;

  rotate_points(bird->poly, angle_diff);
  bird->poly->angle = target_angle;
}

void update_renderer(void *pgame, Thing_t *thing) {
  switch (thing->type_id) {
  case BIRD_TYPE:
    bird_Update_Renderer(pgame, thing);
    break;
  }
}

void update(void *pscene, Thing_t *thing, float d_time) {

  Scene_t *scene = (Scene_t *)pscene;

  switch (thing->type_id) {
  case BIRD_TYPE:
    bird_Update(scene, thing, d_time);
    break;
  }
}

void generate_bird_coords(float *x, float *y) {
  if (rand() % 2 == 0) {
    // Left or right edge
    *x = (rand() % 2 == 0) ? (rand() % 20) : (WIDTH - 20 + rand() % 20);
    *y = rand() % HEIGHT;
  } else {
    // Top or bottom edge
    *x = rand() % WIDTH;
    *y = (rand() % 2 == 0) ? (rand() % 20) : (HEIGHT - 20 + rand() % 20);
  }
}

void spawn_Birds(Scene_t *scene, int number_Of_Birds) {
  for (int i = 0; i < number_Of_Birds; i++) {
    float x = 0;
    float y = 0;
    generate_bird_coords(&x, &y);
    Thing_t *bird = add_thing(scene, x, y, 10, 10, 50.0f, 50.0f, BIRD_TYPE, 255,
                              255, 0, 255);

    struct dynArray_t *vertices;
    vertices = create_Array(2);

    append(&vertices, create_point(0, -3));
    append(&vertices, create_point(1, 5));
    append(&vertices, create_point(-1, 5));
    Tpolygon *poly = create_polygon(vertices);
    poly->angle = 0.0;

    poly->center.x = 50;
    poly->center.y = 50;

    rotate_points(poly, 90 * M_PI / 180.0);

    bird->poly = poly;
  }
}

void init_scene(Scene_t *scene) {
  //
  spawn_Birds(scene, 50);
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
