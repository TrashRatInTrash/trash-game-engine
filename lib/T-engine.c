#include "t-lib.c"
#include "t-vector.h"
#include "T-engine.h"
#include <SDL2/SDL_scancode.h>
#include <time.h>
#include <unistd.h>

Thing *malloc_Thing() {

  Thing *thing = malloc(sizeof(Thing));

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
  thing->custom_Properties = NULL;

  return thing;
}

Scene_t *init_Scene() {
  Scene_t *scene = malloc(sizeof(Scene_t));

  if (scene == NULL) {
    return NULL;
  }

  memset(scene, 0, sizeof(Scene_t));

  scene->things = create_Array(10);

  if (scene->things == NULL) {
    fprintf(stderr, "Failed to create things array\n");
    free(scene);
    return NULL;
  }

  scene->scene_ID = -1;

  // Initialize the available_Indices array
  scene->available_Indicies = create_Array(10); // Initial capacity of 10
  if (scene->available_Indicies == NULL) {
    fprintf(stderr, "Failed to create available_Indices array\n");
    destroy_Array(scene->things);
    free(scene);
    return NULL;
  }

  scene->on_update = NULL;
  scene->on_update_renderer = NULL;

  return scene;
}

void destroy_Scene(Scene_t *game) {
  if (game == NULL) {
    return;
  }

  for (size_t i = 0; i < game->things->ptr; i++) {
    Thing *thing = get(game->things, i);
    if (thing != NULL) {
      free(thing);
    }
  }

  destroy_Array(game->things);
  destroy_Array(game->available_Indicies);

  free(game);
}

/*
  initializes an empty Play
  with a window of heigh and width
*/
Play_t *init_Play(int Win_Width, int Win_Height) {

  Play_t *play = malloc(sizeof(Play_t));
  memset(play, 0, sizeof(Play_t));

  struct dynArray_t *scenes;

  create_Array(3);
  play->scenes = scenes;
  play->state_count = 0;
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

Thing *add_thing(Scene_t *game, int x, int y, int width, int height, float vx,
                 float vy, int tid, int r, int g, int b, int a) {

  size_t index;
  if (game->available_Indicies->ptr > 0) {
    index = (size_t)(uintptr_t)get(game->available_Indicies,
                                   game->available_Indicies->ptr - 1);
    delete_at(game->available_Indicies,
              game->available_Indicies->ptr -
                  1); 
  } else {
    index = game->things->ptr;
  }

  Thing *obj = get(game->things, index);

  if (obj == NULL) {
    obj = malloc_Thing();
    if (obj == NULL) {
      fprintf(stderr, "Failed to allocate memory for new Thing\n");
      return NULL;
    }

    if (append(&game->things, obj) != 0) {
      fprintf(stderr, "Failed to append new Thing to things array\n");
      free(obj);
      return NULL;
    }
  }

  obj->id = index;
  obj->type_id = tid;
  obj->x = x;
  obj->y = y;
  obj->vx = vx;
  obj->vy = vy;
  obj->width = width;
  obj->height = height;
  obj->color[0] = r;
  obj->color[1] = g;
  obj->color[2] = b;
  obj->color[3] = a;
  // obj->poly = NULL;

  return obj;
}
