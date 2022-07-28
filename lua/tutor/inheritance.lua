#!/usr/bin/env lua
-- https://en.wikipedia.org/wiki/Lua_(programming_language)

local Vector = {}
Vector.__index = Vector

function Vector:new(x, y, z)
  return setmetatable({x=x, y=y, z=z}, self)
end

function Vector:magnitude()
  return math.sqrt(self.x^2 + self.y^2 + self.z^2)
end

-- Example of class inheritance
local VectorMult = {}
VectorMult.__index = VectorMult
setmetatable(VectorMult, Vector) -- Make VectorMult a child of Vector

function VectorMult:multiply(val)
  self.x = self.x * val
  self.y = self.y * val
  self.z = self.z * val
  return self
end

local vec = VectorMult:new(0, 1, 0) -- Create a vector
print(vec.magnitude(vec))
print(vec.y)

vec:multiply(3)
print(vec.y)
print(vec.magnitude(vec))
