#include "renderer.h"
#include "game.h"
#include <memory>
#include <iostream>
#include <thread>
#include <chrono>
#include "SDL.h"


int main() {
  std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>(800, 600);
  // Renderer *renderer = new Renderer(800, 600);
  // renderer->Render();
  auto game = Game(renderer);
  game.Start();
  // game.waitForGameOver();
  return 0;
}