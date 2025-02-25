
#ifndef T_ENGINE_H
#define T_ENGINE_H

#include "t-lib.h"
#include "t-vector.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>
#include <unistd.h>

#define DEFAULT_TYPE 0
#define RAYCAST_TYPE 1

typedef struct {
  int id;            // unique ID for each thing
  int type_id;       // id to identify different types of things
  float x, y;        // position
  float vx, vy;      // velocity
  int width, height; // also use for bounding box purposes
  int color[4];
  Tpolygon *poly;
  int index; // index in object array, can't be same as ID else we have to
             // remove its references when deleting, and idk how yet

  void *custom_Properties;
} Thing_t;

typedef struct {
  Thing_t *thing;
  char text[];
} Text;

typedef struct {
  float dx, dy;
  float length;
} Raycast;

typedef struct {
  int scene_ID;
  struct dynArray_t *things; // list of things
  int thing_count;           // nb of active things

  // custom hook for user-defined functions
  void (*on_update)(void *game, Thing_t *thing, float delta_time);
  void (*on_update_renderer)(void *game, Thing_t *thing);
} Scene_t;

typedef struct {
  struct dynArray_t *scenes; // vector storing all the game states
  int scene_count;
  int active_state_i;

  // user input states
  int key_up, key_down, key_left, key_right;
  int mouse_x, mouse_y;
  int mouse_button_pressed;
  int quit_button_pressed;

  SDL_Window *win;
  SDL_Renderer *renderer;

  void (*on_update_global)(void *globe);
} Play_t;

// Function prototypes

Thing_t *malloc_Thing();
Scene_t *init_Scene();
void destroy_Scene(Scene_t *game);
Play_t *init_Play(int Win_Width, int Win_Height);
Thing_t *add_thing(Scene_t *game, int x, int y, int width, int height, float vx,
                   float vy, int tid, int r, int g, int b, int a);
int destroy_Thing(Scene_t *scene, int thing_index);
void add_Scene_to_Play(Scene_t *scene, Play_t *play);

void run_Play(Play_t *play);

void set_Active_State(Play_t *play, Scene_t *scene);

float calculate_distance_things(Thing_t *a, Thing_t *b);

int is_bounding_box_collision(Thing_t *a, Thing_t *b);

#endif // T_ENGINE_H
