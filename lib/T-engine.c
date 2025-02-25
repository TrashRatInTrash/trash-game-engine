#include "T-engine.h"
#include "t-vector.h"
#include <SDL2/SDL_scancode.h>
#include <time.h>
#include <unistd.h>

Thing_t *malloc_Thing() {

  Thing_t *thing = malloc(sizeof(Thing_t));

  if (thing == NULL) {
    return NULL;
  }

  thing->id = -1;
  thing->type_id = -1;
  thing->x = 0.0f;
  thing->y = 0.0f;
  thing->vx = 0.0f;
  thing->vy = 0.0f;
  thing->width = 10;
  thing->height = 10;
  thing->index = -1;
  thing->poly = NULL;
  thing->custom_Properties = NULL;

  return thing;
}

Scene_t *init_Scene() {
  Scene_t *scene = malloc(sizeof(Scene_t));

  if (scene == NULL) {
    return NULL;
  }

  memset(scene, 0, sizeof(Scene_t));

  scene->things = create_Array(5);

  if (scene->things == NULL) {
    fprintf(stderr, "Failed to create things array\n");
    free(scene);
    return NULL;
  }

  scene->scene_ID = -1;

  scene->on_update = NULL;
  scene->on_update_renderer = NULL;

  return scene;
}

void destroy_Scene(Scene_t *play) {
  if (play == NULL) {
    return;
  }

  for (size_t i = 0; i < play->things->ptr; i++) {
    Thing_t *thing = get(play->things, i);
    if (thing != NULL) {
      free(thing);
    }
  }
  destroy_Array(play->things);
  free(play);
}

/*
  initializes an empty Play
  with a window of heigh and width
*/
Play_t *init_Play(int Win_Width, int Win_Height) {

  Play_t *play = malloc(sizeof(Play_t));
  memset(play, 0, sizeof(Play_t));

  struct dynArray_t *scenes;

  scenes = create_Array(3);
  play->scenes = scenes;
  play->active_state_i = -1;

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("SDL_Init Error: %s\n", SDL_GetError());
    return NULL;
  }

  SDL_Window *win = SDL_CreateWindow("2D Game Engine", 100, 100, Win_Width,
                                     Win_Height, SDL_WINDOW_SHOWN);
  if (win == NULL) {
    printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
    SDL_Quit();
    return NULL;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(
      win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == NULL) {
    printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
    SDL_DestroyWindow(win);
    SDL_Quit();
    return NULL;
  }

  play->renderer = renderer;
  play->win = win;

  return play;
}

void set_Active_State(Play_t *play, Scene_t *scene) {
  play->active_state_i = scene->scene_ID;
}

void add_Scene_to_Play(Scene_t *scene, Play_t *play) {
  append(&play->scenes, scene);
  scene->scene_ID = play->scene_count++;
}

Thing_t *add_thing(Scene_t *scene, int x, int y, int width, int height, float vx,
                 float vy, int tid, int r, int g, int b, int a) {

  Thing_t *thing = malloc_Thing();

  if (thing == NULL) {
    fprintf(stderr, "Failed to allocate memory for new Thing\n");
    return NULL;
  }
  if (append(&scene->things, thing) != 0) {
    fprintf(stderr, "Failed to append new Thing to things array\n");
    free(thing);
    return NULL;
  }

  thing->id = scene->things->ptr;
  thing->type_id = tid;
  thing->x = x;
  thing->y = y;
  thing->vx = vx;
  thing->vy = vy;
  thing->width = width;
  thing->height = height;
  thing->color[0] = r;
  thing->color[1] = g;
  thing->color[2] = b;
  thing->color[3] = a;
  thing->index = scene->things->ptr;
  // thing->poly = NULL;

  return thing;
}

int destroy_Thing(Scene_t *scene, int thing_index) {

  printf("destroying Thing at index: %d\n", thing_index);

  // delete thing from index, swaps last element to its place then deletes
  delete_at(scene->things, thing_index);
  // update index of the moved thing (previously at end of array), to new index
  Thing_t *moved_thing = get(scene->things, thing_index); //
  moved_thing->index = thing_index;

  return 0;
}

void draw_rectangle(SDL_Renderer *renderer, float x, float y, int width,
                    int height, int color[4]) {
  SDL_Rect rect;
  rect.x = (int)x - width / 2;
  rect.y = (int)y - height / 2;
  rect.w = width;
  rect.h = height;

  SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], color[3]);
  SDL_RenderFillRect(renderer, &rect);
}

void render_things(Scene_t *scene, SDL_Renderer *renderer) {

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  for (int i = 0; i < scene->things->ptr; i++) {

    Thing_t *thing = get(scene->things, i);

    if (thing == NULL || thing->id == -1) {
      continue;
    }

    if (thing->poly == NULL) {
      draw_rectangle(renderer, thing->x, thing->y, thing->width, thing->height,
                     thing->color);
    } else {
      thing->poly->center.x = thing->x;
      thing->poly->center.y = thing->y;

      // printf("polygon center: %f, %f\n", thing->poly->center.x,
      //        thing->poly->center.y);

      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

      SDL_Color color = {.r = thing->color[0],
                         .g = thing->color[1],
                         .b = thing->color[1],
                         .a = thing->color[3]};

      draw_filled_polygon(thing->poly, color, renderer);

      if (scene->on_update_renderer != NULL) {
        scene->on_update_renderer(scene, thing);
      } else {
        printf("no user render func found\n");
      }
    }
  }
  SDL_RenderPresent(renderer);
}

int is_bounding_box_collision(Thing_t *a, Thing_t *b) {
  return (a->x < b->x + b->width && a->x + a->width > b->x &&
          a->y < b->y + b->height && a->y + a->height > b->y);
}

float calculate_distance_things(Thing_t *a, Thing_t *b) {

  float distance = -1;

  distance =
      sqrt(((a->x - b->x) * (a->x - b->x)) + ((a->y - b->y) * (a->y - b->y)));

  return distance;
}

void update_things(Scene_t *scene, float d_time) {
  /*
    go upto `ptr` only, empty void pointers
    aren't set to NULL
  */
  for (int i = 0; i < scene->things->ptr; i++) {
    Thing_t *thing = get(scene->things, i);
    thing->x += thing->vx * d_time;
    thing->y += thing->vy * d_time;
    if (scene->on_update) {
      scene->on_update(scene, thing, d_time);
    }
  }
}

void handle_input(Play_t *play) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      exit(0);
    }

    // printf("\n~~~~~~~~~~~~~~~~\nInputs:");
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    play->key_up = state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W];
    // printf("key_up:%d\n", play->key_up);
    play->key_down = state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S];
    // printf("key_down:%d\n", play->key_down);
    play->key_left = state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A];
    // printf("key_left:%d\n", play->key_left);
    play->key_right = state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D];
    // printf("key_right:%d\n", play->key_right);
    play->quit_button_pressed = state[SDL_SCANCODE_R];

    if (event.type == SDL_MOUSEMOTION) {
      play->mouse_x = event.motion.x;
      play->mouse_y = event.motion.y;
    }
    if (event.type == SDL_MOUSEBUTTONDOWN) {
      play->mouse_button_pressed = 1;
    }
    if (event.type == SDL_MOUSEBUTTONUP) {
      play->mouse_button_pressed = 0;
    }
  }
}

void run_Play(Play_t *play) {
  while (1) {
    if (play->active_state_i < 0) {
      return;
    }
    // printf("active state index: %d\n", play->active_state_i);
    Scene_t *active_scene = get(play->scenes, play->active_state_i);
    float delta_time = 0.016f;

    // printf("active state id: %d\n", active_scene->scene_ID);

    handle_input(play);
    update_things(active_scene, delta_time);
    render_things(active_scene, play->renderer);

    SDL_Delay(16);
  }
}
