#include "include/ai_paddle.h"
#include <chrono>
#include <thread>
#include <cmath>

void AIPaddle::Act() {
  _last = std::chrono::high_resolution_clock::now();
  _threads.emplace_back(std::thread(&AIPaddle::_move, this));
  _threads.emplace_back(std::thread(&AIPaddle::_ai, this));
}

void AIPaddle::_ai() {
  
  auto t1 = std::chrono::high_resolution_clock::now();
  while(_running) {
    float target;
    if(std::abs(_ball->X() - _location.X()) < 25 ) {
      float top =  _ball->Y() - _location.Y(),
            bottom = _ball->Y() - _location.Y() + _location.H();
      target = std::abs(top) < std::abs(bottom) ? top : bottom;
    } else target = _location.Y() + (_location.H()/2);

    if(target > _ball->Y()) {  // paddle low
      if(target == _location.Y() || target - _ball->Y() > 10) _moving = kUp;
      else _moving = kNone;
    } else if(target == _location.Y() + _location.H() || _ball->Y() - target > 10) _moving = kDown;
      else _moving = kNone;

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}