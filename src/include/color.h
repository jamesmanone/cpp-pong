#ifndef COLOR_H
#define COLOR_H
#include <cstdint> 

class Color {
public:
  Color() { }
  Color(int r, int g, int b, int a) : _r(r), _g(g), _b(b), _a(a) { }
  Color(const Color &s) : _r(s.R()), _g(s.G()), _b(s.B()), _a(s.A()) { }
  Color(const Color &&s) : _r(s.R()), _g(s.G()), _b(s.B()), _a(s.A()) { }
  Color &operator=(const Color &&s);
  Color &operator=(const Color &s);
  Color &operator--();
  Color &operator++();
  uint8_t R() const { return _r; }
  uint8_t G() const { return _g; }
  uint8_t B() const { return _b; }
  uint8_t A() const { return _a; }
private:
  uint8_t _r;
  uint8_t _g;
  uint8_t _b;
  uint8_t _a;
};

#endif