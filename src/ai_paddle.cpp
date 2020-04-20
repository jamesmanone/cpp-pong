#include "include/ai_paddle.h"
#include <chrono>
#include <thread>
#include <cmath>
#include <memory>

void AIPaddle::Act() {
  _last = std::chrono::high_resolution_clock::now();
  _threads.emplace_back(std::thread(&AIPaddle::_move, this));
  _threads.emplace_back(std::thread(&AIPaddle::_ai, this));
}

void AIPaddle::_ai() {
  
  auto t1 = std::chrono::high_resolution_clock::now();
  while(_running) {
    float target = _location->Y() + (_location->H()/2), tolerance = 0.013;
    std::unique_lock<std::mutex> l(_mtx);
    if(_ball && std::abs(_ball->X() - _location->X()) < .025 ) {
      float top =  _ball->Y() - _location->Y(),
            bottom = _ball->Y() - (_location->Y() + _location->H());
      target = std::abs(top) < std::abs(bottom) ? top : bottom;
    } else target = _location->Y() + (_location->H()/2);

    if(target > _ball->Y()) {  // paddle low
      if(target == _location->Y() ||  target - _ball->Y() > tolerance) _moving = Direction::kUp;
      else _moving = Direction::kNone;
    } else if(target == _location->Y() + _location->H() || _ball->Y() - target > tolerance) _moving = Direction::kDown;
      else _moving = Direction::kNone;
    l.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}

void AIPaddle::Stop() {
  std::lock_guard<std::mutex> l(_mtx);
  _ball.reset();
  Interactive::Stop();
}