#include "include/game.h"
#include "include/interactive.h"
#include "include/ball.h"
#include "include/renderer.h"
#include "include/ai_paddle.h"
#include "include/player_paddle.h"
#include <memory>
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>



const Color Game::_c = Color(0x0F, 255, 0x0F, 0xFF);

Game::Game(std::shared_ptr<Renderer> renderer) : _renderer(renderer) {
  _renderer->SetGamePointer(this);
  _pieces.ball = new Ball(200, 200, _c, this);
  _pieces.aiPaddle = new AIPaddle(_c, this, _pieces.ball->getLocationPtr());
  _pieces.playerPaddle = new PlayerPaddle(_c, this);

  _pieces.ball->AddPaddle(_pieces.aiPaddle->getLocationPtr());
  _pieces.ball->AddPaddle(_pieces.playerPaddle->getLocationPtr());

  _p.emplace_back(_pieces.ball);
  _p.emplace_back(_pieces.aiPaddle);
  _p.emplace_back(_pieces.playerPaddle);

  _renderer->SetDrawable(_p);
  Interactive::setXY(_renderer->getScreenWidth(), renderer->getScreenHeight());
}

Game::~Game() {
  // for(auto p : _pieces) delete p;
  for(auto &th : _threads) th.join();
}

void Game::Start() {
  for(Interactive *piece : _p) piece->Act();
  Monitor();
  _renderer->Render();
}

void Game::Stop() {
  _pieces.ball->Stop();
  _pieces.aiPaddle->Stop();
  _pieces.playerPaddle->Stop();
  _renderer->Stop();
}

void Game::Monitor() {
  _threads.emplace_back(std::thread(&Game::_monitor, this));
}

void Game::Send(SDL_Event &&e) {
  _q.send(std::move(e));
}

void Game::_monitor() {
  while(1) {
    SDL_Event e = _q.receive();
    if(e.type == SDL_QUIT) {
      Stop();
      break;
    } else if(e.type == SDL_KEYDOWN) {
      if(e.key.keysym.sym == SDLK_UP) _pieces.playerPaddle->move(Paddle::Direction::kUp);
      else if(e.key.keysym.sym == SDLK_DOWN) _pieces.playerPaddle->move(Paddle::Direction::kDown);
    } else if(e.type == SDL_KEYUP && (e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_DOWN)) _pieces.playerPaddle->move(Paddle::Direction::kNone);

  }
}

