#!/usr/bin/env lua
-- https://en.wikipedia.org/wiki/Lua_(programming_language)#Object-oriented_programming

local Vector = {}
local VectorMeta = { __index = Vector }

function Vector.new(x, y, z)
  return setmetatable({x=x, y=y, z=z}, VectorMeta)
end

function Vector.magnitude(self)
  return math.sqrt(self.x^2 + self.y^2 + self.z^2)
end

local vec = Vector.new(0, 1, 0) -- Create a vector
print(vec.magnitude(vec))
print(vec.y)
