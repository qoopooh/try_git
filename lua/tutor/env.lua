#!/usr/bin/env lua
-- The default global environment
a = 5
print(a)  -- Output: 5

-- A new environment table
local newEnv = {}

-- A function with a different environment
local function testFunc()
  _ENV = newEnv
  a = 10  -- This 'a' is different from the global 'a'
end

testFunc()
print(a)                 -- Output: 5 (global 'a' remains unchanged)
print(newEnv.a)          -- Output: 10 ('a' in the new environment)

