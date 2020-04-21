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
  // Decision logic. Actual movement is handled by Paddle::_move()
  while(_running) {
    // target starts at centre of paddle. If the ball is within 0.025*screen_width we change target to top or bottom of paddle
    float target = _location->Y() + (_location->H()/2), tolerance = 0.013;
    std::unique_lock<std::mutex> l(_mtx);  // make sure our ball(shared_ptr<Location>) doesn't get deleted while we're accessing member functions
    if(_ball && std::abs(_ball->X() - _location->X()) < .025 ) {
      float top =  _ball->Y() - _location->Y(),
            bottom = _ball->Y() - (_location->Y() + _location->H());
      target = std::abs(top) < std::abs(bottom) ? top : bottom;
    }

    if(target > _ball->Y()) {  // paddle low
      if(target == _location->Y() ||  target - _ball->Y() > tolerance) _moving = Direction::kUp;
      else _moving = Direction::kNone;
    } else if(target == _location->Y() + _location->H() || _ball->Y() - target > tolerance) _moving = Direction::kDown;
      else _moving = Direction::kNone;
    l.unlock();  // allows time for other threads to discard our ball
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}

void AIPaddle::Stop() {
  std::lock_guard<std::mutex> l(_mtx);
  _ball.reset();  // breaks circular ref between ball and paddle
  Interactive::Stop();
}