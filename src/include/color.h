#ifndef COLOR_H
#define COLOR_H

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
  int R() const { return _r; }
  int G() const { return _g; }
  int B() const { return _b; }
  int A() const { return _a; }
private:
  int _r;
  int _g;
  int _b;
  int _a;
};

#endif