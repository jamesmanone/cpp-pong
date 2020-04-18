#include "include/paddle.h"
#include <chrono>

void Paddle::_move() {
  while(_running) {
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> d = now - _last;
    _last = now;

    double delta = delta_y * d.count(); 
    // double delta = delta_y;
    if(_moving == kUp) {
      _location.Move(0, -delta);
      if(_location.Y() < -(_location.H()/2)) _location.Y(_location.H()/2);

    } else if(_moving == kDown) {
      _location.Move(0, delta);
      if(_location.Y() + (_location.H()/2) > maxY) _location.Y(maxY - (_location.H()/2));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    
  }
}