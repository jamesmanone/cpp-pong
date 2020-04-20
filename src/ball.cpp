#include "include/ball.h"
#include "include/game.h"
#include <cmath>
#include <iostream>

Ball& Ball::operator=(const Ball &&s) {
  if(this == &s) return *this;

  veloX = s.veloX;
  veloY = s.veloY;
  _location = std::move(s._location);
  _last = s._last;
  _color = std::move(s._color);
  return *this;
}

void Ball::Act() {

  _last = std::chrono::high_resolution_clock::now();
  _threads.emplace_back(std::thread(&Ball::_move, this));
}

void Ball::_move() {
  while(_running) {
    std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = now - _last;

    _last = now;
    int r = _location->W()/2;
    double t = duration.count();

    _location->Move((veloX * t), (veloY * t));

    // BounceY
    if(_location->Y() + r > maxY) {
      _location->Y(maxY - r);
      veloY *= -1;
    } else if(_location->Y() < r) {
      _location->Y(r);
      veloY *= -1;
    }

    int p = checkPaddleCollision();
    if(p != INT_MAX) {
      veloX*=-1;
      veloY = p * 0.01;
    }

    // SCORE!
    if(_location->X() + r > maxX) {
      _game->Score(Paddle::Type::kPlayer);
      _reset(Paddle::Type::kPlayer);
    } else if(_location->X() < r) {
      _game->Score(Paddle::Type::kComputer);
      _reset(Paddle::Type::kComputer);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
}

int Ball::checkPaddleCollision() {
  // Left and right edge of ball
  double rad = _location->W() / 2,
         x_r = _location->X() + rad,
         x_l = x_r - (rad * 2),
         top = _location->Y() - rad,
         bottom = top + (rad * 2);

  for(auto p : _paddles) {
    if(p == nullptr) continue;
    float l = p->X(), r = l + p->W();  // left and right side of paddle
    if((std::abs(r - x_l) > 1.5) &&
      (std::abs(l - x_r) > 1.5)) continue;  // away from paddle on x axis

    else if( top > p->Y() + p->H() ||        // miss: below
        bottom < p->Y()) continue;              // miss: above

    // HIT! Return distance from center of paddle
    else {
      double c_y = p->Y() + (p->H()/2);
      return _location->Y() - c_y;
    }
  }
  return INT_MAX;
}

void Ball::_reset(Paddle::Type p) {
  veloY = 0;
  veloX = 0;
  int i = p == Paddle::Type::kPlayer ? 1 : 0;
  std::this_thread::sleep_for(std::chrono::milliseconds(1500));

  int newX = p == Paddle::Type::kPlayer ? _paddles[i]->X() + _paddles[i]->W() + (_location->W()/2) : 
                                          _paddles[i]->X() - (_location->W()/2);


  _location->X(newX);
  _location->Y(_paddles[i]->Y());
  veloX = p == Paddle::Type::kPlayer ? 0.3 : -0.3;
  _waitForPaddleMovement(i);
}

void Ball::_waitForPaddleMovement(int i) {
  int y = _paddles[i]->Y();
  auto t1 = std::chrono::high_resolution_clock::now();
  std::lock_guard<std::mutex> l(_mtx);
  while(_running && std::abs(y - _paddles[i]->Y()) < 3) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    if(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - t1).count() > 3) break;
  }
  if(!_paddles[i]) return;
  veloY = _paddles[i]->Y() > y ? 0.25 : -0.25;
  _location->Move(veloX*5, veloY*5);
  _last = std::chrono::high_resolution_clock::now();

}

void Ball::Stop() {
  Interactive::Stop();
  std::lock_guard<std::mutex> l(_mtx);
  _paddles.clear();
}