
#include "t-vector.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_ttf.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  float x, y;
} point;

typedef struct {
  point *vertices;
  int vert_count;
  point center;
  double angle; // write in radian using M_PI
} Tpolygon;

void normalize_Vector(float *vx, float *vy);

point *create_point(float x, float y);

Tpolygon *create_polygon(struct dynArray_t *vertices);

int draw_filled_polygon(Tpolygon *poly, const SDL_Color color,
                        SDL_Renderer *renderer);

void rotate_points(Tpolygon *poly, double angle);

float get_angle_between_points(float x1, float y1, float x2, float y2);
