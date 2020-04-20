#include "include/renderer.h"
#include "include/game.h"
#include <memory>
#include <iostream>
#include <thread>
#include <chrono>
#include "SDL.h"


int main() {
  Renderer renderer(1024, 768);
  Game game(&renderer);
  game.Start();
  std::cout << std::endl;
  return 0;
}