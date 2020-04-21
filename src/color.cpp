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

// Overload prefix operators to lighten or darken the color. Should roughly equate to luminance
Color& Color::operator--() {
  _r *= 0.5;
  _g *= 0.5;
  _b *= 0.5;
  return *this;
}

Color& Color::operator++() {
  int r = _r * 1.2, g = _g * 1.2, b = _b * 1.2;
  // overflow check
  _r = r <= 0xFF ? r : 0xFF;
  _g = g <= 0xFF ? g : 0xFF;
  _b = b <= 0xFF ? b : 0xFF;
  return *this;
}