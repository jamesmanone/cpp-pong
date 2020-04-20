
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

  static constexpr int _xstart{1024 - _width - 10};
  static  constexpr int _ystart{600 - _height - 100};

};

#endif