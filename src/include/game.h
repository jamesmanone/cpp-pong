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
  std::shared_ptr<Ball> ball;
  std::shared_ptr<AIPaddle> aiPaddle;
  std::shared_ptr<PlayerPaddle> playerPaddle;
  std::vector<std::shared_ptr<Drawable>> decorations;
};

class Game {
public:
  Game(Renderer *renderer);
  ~Game();

  void Monitor();
  void Send(SDL_Event &&e);
  void Start();
  void Stop();
  void Score(Paddle::Type p);
  int PlayerScore() { return _playerScore; }
  int ComputerScore() { return _computerScore; }
  
private:
  void _monitor();
  void _log();

  Renderer *_renderer;
  pieces _pieces;
  MessageQueue<SDL_Event> _q;
  std::vector<std::thread> _threads;

  int _playerScore{0};
  int _computerScore{0};
  static const Color _c;

};

#endif