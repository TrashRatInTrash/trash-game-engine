
#include "t-lib.h"

void normalize_Vector(float *vx, float *vy) {
  double magnitude = sqrt((double)(*vx * *vx + *vy * *vy));
  if (magnitude != 0) {
    *vx = (*vx / magnitude);
    *vy = (*vy / magnitude);
  } else {
    *vx = 0;
    *vy = 0;
  }
}
