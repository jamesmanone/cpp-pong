#include "include/player_paddle.h"
#include <thread>

void PlayerPaddle::Act() {
  _threads.emplace_back(std::thread(&PlayerPaddle::_move, this));
}