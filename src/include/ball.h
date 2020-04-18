#include "interactive.h"
#include <vector>
// Forward Declaration
class Game;

#ifndef BALL_H
#define BALL_H

class Ball : public Interactive {
public:
  Ball() { }
  Ball(int x, int y, Color c) : Interactive(c, Location(x, y, diameter, 0)) { }
  Ball(int x, int y, Color c, Game *g) : Interactive(c, Location(x, y, diameter, 0), g) { }
  Ball &operator=(Ball &s);
  Ball &operator=(const Ball &&s);

  void AddPaddle(Location *l) { _paddles.emplace_back(l); }
  void Act() override;
private:
  int checkPaddleCollision();
  void _move() override;

  std::vector<Location*> _paddles;
  int veloX{10};
  int veloY{10};
  static constexpr int diameter{20};
};

#endif