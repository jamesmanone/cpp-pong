#include "paddle.h"
#ifndef PLAYER_PADDLE_H
#define PLAYER_PADDLE_H

class PlayerPaddle : public Paddle {
public:
  PlayerPaddle(Color c, Game *g) : Paddle(c, Location(_xstart, _ystart, _width, _height), g) { }
  void Act() override;
private:
  static constexpr float _xstart{.01};
  static constexpr float _ystart{0.33};
};

#endif