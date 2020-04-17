// Forward declarations
class Interactive;
class Ball;
#ifndef LOCATION_H
#define LOCATION_H

class Location {
friend class Interactive;
friend class Ball;
public:
  Location() { }
  Location(int x, int y, int w, int h) : _x(x), _y(y), _h(h), _w(w) { }
  Location(const Location &s) : _x(s.X()), _y(s.Y()), _h(s.H()), _w(s.W()) { }
  Location &operator=(const Location &s);
  Location &operator=(const Location &&s);
  int X() const { return _x; }
  int Y() const { return _y; }
  int W() const { return _w; }
  int H() const { return _h; }
protected:
  void Move(int dx, int dy) { _x += dx; _y += dy; }
  void X(int x) { _x = x; }
  void Y(int y) { _y = y; }
private:
  int _x;
  int _y;
  int _w;
  int _h;
};

#endif