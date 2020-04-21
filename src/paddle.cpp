#include "include/paddle.h"
#include <chrono>

// Update (x, y) based on _moving and bounds checks
void Paddle::_move() {
  while(_running) {
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> d = now - _last;
    _last = now;

    double delta = delta_y * d.count();  // movement this frame
    if(_moving == Direction::kUp) {
      _location->Move(0, -delta);
      if(_location->Y() < -(_location->H()/2)) _location->Y(-(_location->H()/2));  // bounds check

    } else if(_moving == Direction::kDown) {
      _location->Move(0, delta);
      if(_location->Y() + (_location->H()/2) > maxY) _location->Y(maxY - (_location->H()/2));  // bounds check
    }

    // sleep for 62.5 Hz target update
    d = std::chrono::high_resolution_clock::now() - _last;
    if(d.count() < 16) std::this_thread::sleep_for(std::chrono::milliseconds(16 - (int)d.count()));
    
  }
}