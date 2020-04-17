#include "include/game.h"
#include "include/interactive.h"
#include "include/ball.h"
#include "include/renderer.h"
#include <memory>
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>



const Color Game::_c = Color(0x0F, 255, 0x0F, 0xFF);

Game::Game(std::shared_ptr<Renderer> renderer) : _renderer(renderer) {
  _pieces.ball = new Ball(200, 200, _c, this);
  _p.emplace_back(_pieces.ball);
  _renderer->SetDrawable(_p);
  Interactive::setXY(_renderer->getScreenWidth(), renderer->getScreenHeight());
}

Game::~Game() {
  // for(auto p : _pieces) delete p;
  for(auto &th : _threads) th.join();
}

void Game::Start() {
  for(Interactive *piece : _p) piece->Act();
  _renderer->Render();
  // while(1) {
  //   SDL_Event e;
  //   if(SDL_PollEvent(&e) && e.type == SDL_QUIT) break;
  //   std::this_thread::sleep_for(std::chrono::milliseconds(1));
  // }
  // _renderer->Stop();
}

void Game::waitForGameOver() {
  while(_q.receive()->type != SDL_QUIT) {}
}

