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

using std::cout;


const Color Game::_c = Color(0x0F, 255, 0x0F, 0xFF);

Game::Game(Renderer *renderer) : _renderer(renderer) {
  int w = _renderer->getScreenWidth(), h = _renderer->getScreenHeight();
  Interactive::setXY(w, h);
  _renderer->SetGamePointer(this);
  _pieces.ball = std::make_shared<Ball>(200, 200, _c, this);
  _pieces.aiPaddle = std::make_shared<AIPaddle>(_c, this, _pieces.ball->getLocationPtr());
  _pieces.playerPaddle = std::make_shared<PlayerPaddle>(_c, this);

  _pieces.ball->AddPaddle(_pieces.aiPaddle->getLocationPtr());
  _pieces.ball->AddPaddle(_pieces.playerPaddle->getLocationPtr());

  int n = 7, middle = w/2, height = h/n - 10, y = 5;

  Color c = _c;
  --c;

  while(y < h) {
    _pieces.decorations.emplace_back(std::make_shared<Drawable>(c, Location(middle, y, 10, height)));
    y += h/n;
  }
  
  std::vector<std::shared_ptr<Drawable>> d;

  for(auto decoration : _pieces.decorations) d.emplace_back(decoration);
  d.emplace_back(_pieces.ball);
  d.emplace_back(_pieces.playerPaddle);
  d.emplace_back(_pieces.aiPaddle);

  _renderer->SetDrawable(d);
}

Game::~Game() {
  for(auto &th : _threads) th.join();
}

void Game::Start() {
  _pieces.playerPaddle->Act();
  _pieces.aiPaddle->Act();
  _pieces.ball->Act();
  Monitor();
  _renderer->Render();
}

void Game::Stop() {
  _renderer->ClearDrawable();
  _renderer->Stop();
  _pieces.ball->Stop();
  _pieces.aiPaddle->Stop();
  _pieces.playerPaddle->Stop();

  _pieces.ball.reset();
  _pieces.aiPaddle.reset();
  _pieces.playerPaddle.reset();
}

void Game::Score(Paddle::Type p) {
  if(p == Paddle::Type::kPlayer) ++_playerScore;
  else ++_computerScore;
  _renderer->SetScoreFlag();

  std::cout << "\rPlayer: " << _playerScore << " Computer: " << _computerScore << std::flush;
}

void Game::Monitor() {
  _threads.emplace_back(std::thread(&Game::_monitor, this));
}

void Game::Send(SDL_Event &&e) {
  _q.send(std::move(e));
}

void Game::_monitor() {
  bool monitor{true};
  while(monitor) {
    SDL_Event e = _q.receive();
    if(e.type == SDL_QUIT) {
      Stop();
      monitor = false;
      break;
    } else if(e.type == SDL_KEYDOWN) {
      if(e.key.keysym.sym == SDLK_UP) _pieces.playerPaddle->move(Paddle::Direction::kUp);
      else if(e.key.keysym.sym == SDLK_DOWN) _pieces.playerPaddle->move(Paddle::Direction::kDown);
    } else if(e.type == SDL_KEYUP && (e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_DOWN)) _pieces.playerPaddle->move(Paddle::Direction::kNone);

  }
}

