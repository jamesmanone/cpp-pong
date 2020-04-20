
#include <mutex>

#ifndef AI_PADDLE_H
#define AI_PADDLE_H
#include "paddle.h"

class AIPaddle : public Paddle {
public:
  AIPaddle(Color c, Game *g, std::shared_ptr<Location> b) :
          Paddle(c, Location(_xstart, _ystart, _width, _height), g), _ball(b) { }

  void Act() override;
  void Stop() override;
private:
  void _ai();
  std::shared_ptr<Location> _ball;

  static constexpr double _xstart{0.99 - _width};
  static  constexpr double _ystart{0.66};

};

#endif