#include "location.h"
#include "color.h"
#include <memory>

#ifndef DRAWABLE_H
#define DRAWABLE_H

class Drawable {
public:
  Drawable();
  Drawable(Color c) : _color(c) { }
  Drawable(Color c, Location l) : 
          _location(std::make_shared<Location>(l)), _color(c) { }
  std::shared_ptr<Location> getLocationPtr() { return _location; }
  Location getLocation() { return Location(*_location); }
  Color getColor() { return Color(_color); }
protected:

  std::shared_ptr<Location> _location;
  Color _color;
};

#endif