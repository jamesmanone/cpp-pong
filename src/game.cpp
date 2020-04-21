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



const Color Game::_c = Color(0x0F, 0xFF, 0x0F, 0xFF);

Game::Game(Renderer *renderer) : _renderer(renderer) {
  int w = _renderer->getScreenWidth(), h = _renderer->getScreenHeight();
  _renderer->SetGamePointer(this);  // renderer needs pointer to *this for messaging 
  _pieces.ball = std::make_shared<Ball>(_c, this);
  _pieces.aiPaddle = std::make_shared<AIPaddle>(_c, this, _pieces.ball->getLocationPtr());
  _pieces.playerPaddle = std::make_shared<PlayerPaddle>(_c, this);

  // add Paddle::_Location to ball for collision detection
  _pieces.ball->AddPaddle(_pieces.aiPaddle->getLocationPtr());
  _pieces.ball->AddPaddle(_pieces.playerPaddle->getLocationPtr());

  double n = 20, width = 0.01, middle = 0.5, height = 1/n - 0.02, y = 0.01;

  Color c = _c;
  --c;

  // create the centreline
  while(y < h) {
    _pieces.decorations.emplace_back(std::make_shared<Drawable>(c, Location(middle - (width/2), y, width, height)));
    y += 1/n;
  }
  
  std::vector<std::shared_ptr<Drawable>> d;

  // add everything drawable to a vector<shared_ptr<Drawable>> and send it to Renderer
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
  // start all update loops (multithreaded)
  _pieces.playerPaddle->Act();
  _pieces.aiPaddle->Act();
  _pieces.ball->Act();

  Monitor();  // start a thread to handle messages <SDL_Event> from the main render thread
  _renderer->Render();  // renders. Blocks main thread until Renderer::Stop() is called
}

void Game::Stop() {
  _renderer->ClearDrawable();  // calls vector::clear on the vector<shared_ptr<Drawable>> to release the shared pointers
  _renderer->Stop();

  // Interactive::Stop() kills loops started in Act(). Aditional functionality in overrides in some classes
  _pieces.ball->Stop();
  _pieces.aiPaddle->Stop();
  _pieces.playerPaddle->Stop();

  // dump pointers to the drawable objects
  _pieces.ball.reset();
  _pieces.aiPaddle.reset();
  _pieces.playerPaddle.reset();
  for(auto &d : _pieces.decorations) d.reset();
}

void Game::Score(Paddle::Type p) {
  if(p == Paddle::Type::kPlayer) ++_playerScore;
  else ++_computerScore;
  _renderer->SetScoreFlag();  // Renderer::_render checks the score flag to see if the score needs to be updated
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
    if(e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q)) {
      Stop();
      monitor = false;
      break;
    } else if(e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {  // window resized
      _renderer->SetScreenSize(e.window.data1, e.window.data2);
    }else if(e.type == SDL_KEYDOWN) {  // update direction of movement in PlayerPaddle.
      if(e.key.keysym.sym == SDLK_UP) _pieces.playerPaddle->move(Paddle::Direction::kUp);
      else if(e.key.keysym.sym == SDLK_DOWN) _pieces.playerPaddle->move(Paddle::Direction::kDown);
    } else if(e.type == SDL_KEYUP && (e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_DOWN)) _pieces.playerPaddle->move(Paddle::Direction::kNone);

  }
}

// debugging function. Prints coordinates to the console (overwriting the line to make it readable) inactive unless started on it's own thread in Monitor()
void Game::_log() {
  std::shared_ptr<Location> ball = _pieces.ball->getLocationPtr(),
                            c_p = _pieces.aiPaddle->getLocationPtr(),
                            p_p = _pieces.playerPaddle->getLocationPtr();
  while(_renderer->Running()) {

    std::cout << "\rBall(" << std::to_string(ball->X()) << ", " << std::to_string(ball->Y())
              << ") aiPaddle(" << std::to_string(c_p->X()) << ", " << std::to_string(c_p->Y())
              << ") playerPaddle(" << std::to_string(p_p->X()) << ", " << std::to_string(p_p->Y())
              << ")" << std::flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

