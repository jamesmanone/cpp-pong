#include "ball.h"

Ball& Ball::operator=(const Ball &&s) {
  if(this == &s) return *this;

  veloX = s.veloX;
  veloY = s.veloY;
  _location = s._location;
  _last = s._last;
  _color = std::move(s._color);
  return *this;
}

void Ball::Act() {
  _last = std::chrono::high_resolution_clock::now();
  _thread.emplace_back(std::thread(&Ball::_move, this));
}

void Ball::_move() {
  while(1) {
    std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = now - _last;

    _last = now;
    int r = _location.W()/2;
    double t = duration.count() / 33;

    _location.Move((int)(veloX * t), (int)(veloY * t));

    // BonceX
    if(_location.X() + r > maxX) {
      _location.X(maxX - r);
      veloX *= -1;
    } else if(_location.X() < r) {
      _location.X(r);
      veloX *= -1;
    }

    // BounceY
    if(_location.Y() + r > maxY) {
      _location.Y(maxY - r);
      veloY *= -1;
    } else if(_location.Y() < r) {
      _location.Y(r);
      veloY *= -1;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));  // ~100 fps is more than enough
  }
}