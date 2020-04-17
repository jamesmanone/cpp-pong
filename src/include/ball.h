#include "interactive.h"

// Forward Declaration
class Game;

#ifndef BALL_H
#define BALL_H

class Ball : public Interactive {
public:
  Ball() { }
  Ball(int x, int y, Color c) : Interactive(c, Location(x, y, 10, 0)) { }
  Ball(int x, int y, Color c, Game *g) : Interactive(c, Location(x, y, 10, 0), g) { }
  Ball &operator=(Ball &s);
  Ball &operator=(const Ball &&s);

  void Act() override;
protected:
  void _move() override;
  int veloX{10};
  int veloY{10};
};

#endif