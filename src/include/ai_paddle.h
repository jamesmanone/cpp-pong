
#include <mutex>

#ifndef AI_PADDLE_H
#define AI_PADDLE_H
#include "paddle.h"

class AIPaddle : public Paddle {
public:
  AIPaddle(Color c, Game *g, Location *b) :
          Paddle(c, Location(_xstart, _ystart, _width, _height), g), _ball(b) { }

  void Act() override;
private:
  void _ai();
  Location *_ball;
  std::mutex _mtx;

  static constexpr int _xstart{800 - _width - 10};
  static  constexpr int _ystart{600 - _height};

};

#endif