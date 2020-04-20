#include "include/color.h"

Color& Color::operator=(const Color &&s) {
    if(this == &s) return *this;
    _r = s.R();
    _g = s.G();
    _b = s.B();
    _a = s.A();
    return *this;
  }

Color& Color::operator=(const Color &s) {
    if(this == &s) return *this;
    _r = s.R();
    _g = s.G();
    _b = s.B();
    _a = s.A();
    return *this;
  }

Color& Color::operator--() {
  _r *= 0.5;
  _g *= 0.5;
  _b *= 0.5;
  return *this;
}

Color& Color::operator++() {
  _r *= 1.2;
  _g *= 1.2;
  _b *= 1.2;
  return *this;
}