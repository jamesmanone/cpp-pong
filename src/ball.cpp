#include "include/ball.h"
#include <cmath>
#include <iostream>

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
  for( auto p : _paddles) std::cout << "X: " << p->X() << " Y: " << p->Y() << std::endl;

  _last = std::chrono::high_resolution_clock::now();
  _threads.emplace_back(std::thread(&Ball::_move, this));
}

void Ball::_move() {
  while(1) {
    std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = now - _last;

    _last = now;
    int r = _location.W()/2;
    double t = duration.count() / 33;

    _location.Move((veloX * t), (veloY * t));

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

    int p = checkPaddleCollision();
    if(p != INT_MAX) {
      veloX*=-1;
      veloY = p * 0.25;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
}

int Ball::checkPaddleCollision() {
  int checked = 0;
  // Left and right edge of ball
  double x_r = _location.X() + (_location.W()/2),
         x_l = _location.X() - (_location.W()/2);

  for(auto p : _paddles) {
    ++checked;
    float l = p->X(), r = l + p->W();  // left and right side of paddle
    if((std::abs(r - x_l) > 1.5) &&
      (std::abs(l - x_r) > 1.5)) { }  // away from paddle on x axis

    else if( (_location.Y() - (_location.W()/2) > p->Y() + p->H()) ||                                             // miss: below
        (_location.Y() + (_location.W()/2) < p->Y())) { if(checked == 2) std::cout << "miss Y\n"; }               // miss: above

    // distance from center of paddle
    else {
      double c_y = p->Y() + (p->H()/2);
      return _location.Y() - c_y;
    }
  }
  if(checked != 2) std::cout << "not all paddles checked\n";
  return INT_MAX;
}