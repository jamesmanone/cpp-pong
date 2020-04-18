#include "include/interactive.h"

int Interactive::maxY = 0;
int Interactive::maxX = 0;

Interactive::~Interactive() {
  Stop();
  for(auto &t : _threads) t.join();
}

void Interactive::setXY(int x, int y) {
  maxX = x;
  maxY = y;
}