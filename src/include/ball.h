#include "interactive.h"
#include <vector>
#include "paddle.h"

#ifndef BALL_H
#define BALL_H

class Ball : public Interactive {
public:
  Ball() { }
  Ball(int x, int y, Color c) : Interactive(c, Location(x, y, diameter, 0)) { }
  Ball(int x, int y, Color c, Game *g) : Interactive(c, Location(x, y, diameter, 0), g) { }
  Ball &operator=(Ball &s);
  Ball &operator=(const Ball &&s);

  void AddPaddle(std::shared_ptr<Location> l) { _paddles.emplace_back(l); }
  void Act() override;
  void Stop() override;
private:
  int checkPaddleCollision();
  void _move() override;
  void _reset(Paddle::Type p);
  void _waitForPaddleMovement(int i);

  std::vector<std::shared_ptr<Location>> _paddles;
  double veloX{0.3};
  double veloY{0.25};
  static constexpr int diameter{20};
};

#endif