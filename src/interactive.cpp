#include "include/interactive.h"

int Interactive::maxY = 0;
int Interactive::maxX = 0;

Interactive::~Interactive() {
  for(auto &t : _thread) t.join();
}

void Interactive::setXY(int x, int y) {
  maxX = x;
  maxY = y;
}