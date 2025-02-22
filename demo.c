#include "lib/T-engine.h"

int main() {

  Scene_t *game = init_Scene();
  if (game == NULL) {
    return 1;
  }

  Thing *thing =
      add_thing(game, 100, 200, 50, 50, 1.0f, 1.0f, 1, 255, 0, 0, 255);
  if (thing == NULL) {
    destroy_Scene(game);
    return 1;
  }

  destroy_Scene(game);

  return 0;
}
