#include "include/renderer.h"
#include "include/game.h"
#include <iostream>


int main() {
  Renderer renderer(1024, 768);
  Game game(&renderer);
  game.Start();  // blocks main thread until quit event
  std::cout << std::endl;
  return 0;
}