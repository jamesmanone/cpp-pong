// Forward declarations
// class Interactive;
// class Ball;
// class Paddle;
#ifndef LOCATION_H
#define LOCATION_H

class Location {
friend class Interactive;
friend class Ball;
friend class Paddle;
public:
  Location() { }
  Location(double x, double y, double w, double h) : _x(x), _y(y), _h(h), _w(w) { }
  Location(const Location &s) : _x(s.X()), _y(s.Y()), _h(s.H()), _w(s.W()) { }
  Location &operator=(const Location &s);
  Location &operator=(const Location &&s);
  double X() const { return _x; }
  double Y() const { return _y; }
  double W() const { return _w; }
  double H() const { return _h; }
protected:
  void Move(double dx, double dy) { _x += dx; _y += dy; }
  void X(double x) { _x = x; }
  void Y(double y) { _y = y; }
private:
  double _x;
  double _y;
  double _w;
  double _h;
};

#endif