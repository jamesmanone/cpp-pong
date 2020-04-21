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
  _threads.emplace_back(std::thread(&Ball::_move, this));
}

void Ball::_move() {
  _reset(Paddle::Type::kPlayer);  // Start with a serve from the player
  while(_running) {
    std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = now - _last;

    _last = now;
    double r = _location->W()/2;
    double t = duration.count();

    _location->Move((veloX * t), (veloY * t));

    // BounceY
    if(_location->Y() + r >= maxY) {
      _location->Y(maxY - r);
      veloY *= -1;
    } else if(_location->Y() <= r) {
      _location->Y(r);
      veloY *= -1;
    }

    double p = checkPaddleCollision();
    if(std::abs(p - (double)INT_MAX) > 1 ) {  // INT_MAX is used as a stand in for false. 0, negative, or positive can all be valid
      veloX*=-1;
      veloY = p * 0.013;  // 0.013 gives a good ballance between making it impossible to get enough veloY to score and getting so much on the edge that the ball becones impossible to hit
    }

    // SCORE!
    if(_location->X() + r > maxX) {
      _game->Score(Paddle::Type::kPlayer);
      _reset(Paddle::Type::kPlayer);
    } else if(_location->X() < r) {
      _game->Score(Paddle::Type::kComputer);
      _reset(Paddle::Type::kComputer);
    }

    std::chrono::duration<double, std::milli> d = std::chrono::high_resolution_clock::now() - _last;

    if(d.count() < 16) std::this_thread::sleep_for(std::chrono::milliseconds((int)d.count() - 16));  // 62 Hz max update
  }
}

double Ball::checkPaddleCollision() {
  // Left and right edge of ball
  double rad = _location->W() / 2,
         x_r = _location->X() + rad,
         x_l = x_r - (rad * 2),
         top = _location->Y() - rad,
         bottom = top + (rad * 2);

  std::lock_guard<std::mutex> l(_mtx);  // prevent Stop() from clearing our paddles while we're accessing their methods
  for(auto p : _paddles) {
    if(!p) continue;  // nullptr check
    double l = p->X(), r = l + p->W();  // left and right side of paddle
    if((std::abs(r - x_l) > 0.0014) &&
      (std::abs(l - x_r) > 0.0014)) continue;  // away from paddle on x axis

    else if(top > p->Y() + p->H() ||                      // miss: below
            bottom < p->Y() ||                            // miss: above
            (x_l < 0.2 && veloX > 0) ||                   // already bounced off l paddle (debounced bounce‽)
            (x_r > Interactive::maxX - 0.2 && veloX < 0)  // already bounced off r paddle
        ) continue;

    // HIT! Return distance from center of paddle
    else {
      double c_y = p->Y() + (p->H()/2);
      return _location->Y() - c_y;
    }
  }
  return INT_MAX;  // no hits
}

void Ball::_reset(Paddle::Type p) {
  veloY = 0;
  veloX = 0;
  std::this_thread::sleep_for(std::chrono::milliseconds(1500));

  // casting Player::Type to int results in the correct index for that paddle
  double newX = p == Paddle::Type::kPlayer ? _paddles[(int)p]->X() + _paddles[(int)p]->W() + (_location->W()/2) : 
                                          _paddles[(int)p]->X() - (_location->W()/2);


  // place the ball on the serving paddle at y origin, inner x edge;
  _location->X(newX);
  _location->Y(_paddles[(int)p]->Y());
  veloX = p == Paddle::Type::kPlayer ? 0.0004 : -0.0004;
  _waitForPaddleMovement((int)p);
}

void Ball::_waitForPaddleMovement(int i) {
  double y = _paddles[i]->Y();
  auto t1 = std::chrono::high_resolution_clock::now();
  std::lock_guard<std::mutex> l(_mtx);
  while(_running && std::abs(y - _paddles[i]->Y()) < 0.00139) {  // wait for paddle movement or timeout
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    if(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - t1).count() > 3) break;
  }
  veloY = _paddles[i]->Y() > y ? 0.0003 : -0.0003;
  _location->Move(veloX*5.0, veloY*5.0);
  _last = std::chrono::high_resolution_clock::now();

}

void Ball::Stop() {
  Interactive::Stop();
  std::lock_guard<std::mutex> l(_mtx);
  _paddles.clear();
}