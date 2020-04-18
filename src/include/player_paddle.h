#include "paddle.h"
#ifndef PLAYER_PADDLE_H
#define PLAYER_PADDLE_H

class PlayerPaddle : public Paddle {
public:
  PlayerPaddle(Color c, Game *g) : Paddle(c, Location(_xstart, _ystart, _width, _height), g) { }
  void Act() override;
private:
  static constexpr int _xstart{10};
  static constexpr int _ystart{100};
};

#endif