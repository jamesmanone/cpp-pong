#ifndef GAME_H
#define GAME_H

#include "interactive.h"
#include "ball.h"
#include "ai_paddle.h"
#include "player_paddle.h"
#include "renderer.h"
#include "color.h"
#include "message_queue.h"
#include <memory>
#include <vector>
#include <thread>

struct pieces {
  Ball *ball;
  AIPaddle *aiPaddle;
  PlayerPaddle *playerPaddle;
};

class Game : std::enable_shared_from_this<Game> {
public:
  Game(std::shared_ptr<Renderer> renderer);
  ~Game();

  void Monitor();
  void Send(SDL_Event &&e);
  void Start();
  void Stop();
  
private:
  void _monitor();
  std::shared_ptr<Renderer> _renderer;
  pieces _pieces;
  std::vector<Interactive*> _p;
  MessageQueue<SDL_Event> _q;
  static const Color _c;
  std::vector<std::thread> _threads;

};

#endif