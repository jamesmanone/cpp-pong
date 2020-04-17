#include "location.h"


Location& Location::operator=(const Location &s) {
  if(&s == this) return *this;

  _x = s._x;
  _y = s._y;
  _w = s._w;
  _h = s._h;
  return *this;
}

Location& Location::operator=(const Location &&s) {
  if(&s == this) return *this;

  _x = s._x;
  _y = s._y;
  _w = s._w;
  _h = s._h;
  return *this;
}
