#!/usr/bin/env lua

Point = {}

Point.new = function(x, y)
  return {x = x, y = y}
end

Point.set_x = function(point, x)
  point.x = x
end

p = Point.new(10, 20)
print(p.x)

Point.set_x(p, 30)
print(p["x"])
