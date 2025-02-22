#include "t-lib.c"
#include <SDL2/SDL_scancode.h>
#include <time.h>
#include <unistd.h>

#define MAX_THINGS 100
#define DEFAULT_TYPE 0
#define RAYCAST_TYPE 1

typedef struct {
  int id;            // unique ID for each thing
  int type_id;       // id to identify different types of things
  float x, y;        // position
  float vx, vy;      // velocity
  int width, height; // also use for bounding box purpoess
  int color[4];
  void *custom_Properties;
} Thing;

typedef struct {
  Thing *thing;
  char text[];
} Text;

typedef struct {
  float dx, dy;
  float length;
} Raycast;

typedef struct {
  Thing **things;          // list of things
  int thing_count;         // nb of active things
  int *available_Indicies; // keep track of empty indicies in **things
  int av_i_count;          // nb of empty indicies

  // user input states
  int key_up, key_down, key_left, key_right;
  int mouse_x, mouse_y;
  int mouse_button_pressed;
  int quit_button_pressed;

  // custom hook for user defined functions
  void (*on_update)(void *game, Thing *thing, float delta_time);
  void (*on_update_renderer)(void *game, Thing *thing);
} GameState;

typedef struct {
  struct dynArray_t states; // vector storing all the game states
  int state_count;
  int active_state_i;

  SDL_Window *win;
  SDL_Renderer *renderer;

  void (*on_update_global)(void *globe);
} GlobeState;


