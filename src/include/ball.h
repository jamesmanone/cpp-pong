#include "interactive.h"
#include <vector>
#include "paddle.h"

#ifndef BALL_H
#define BALL_H

class Ball : public Interactive {
public:
  Ball() { }
  Ball(Color c, Game *g) : Interactive(c, Location(startX, startY, diameter, 0), g) { }
  Ball(int x, int y, Color c) : Interactive(c, Location(x, y, diameter, 0)) { }
  Ball(int x, int y, Color c, Game *g) : Interactive(c, Location(x, y, diameter, 0), g) { }
  Ball &operator=(Ball &s);
  Ball &operator=(const Ball &&s);

  void AddPaddle(std::shared_ptr<Location> l) { _paddles.emplace_back(l); }
  void Act() override;
  void Stop() override;
private:
  double checkPaddleCollision();
  void _move() override;
  void _reset(Paddle::Type p);
  void _waitForPaddleMovement(int i);

  std::vector<std::shared_ptr<Location>> _paddles;
  static constexpr float startX{0.5};
  static constexpr float startY{0.5};
  double veloX{0.0};
  double veloY{0.0};
  static constexpr float diameter{0.02};
};

#endif