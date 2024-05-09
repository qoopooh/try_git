#!/usr/bin/env lua
-- Defining a global variable
x = 10

-- Accessing the variable through the _G table
print(_G["x"])  -- Output: 10

-- Modifying the variable through the _G table
_G["x"] = 20
print(x)        -- Output: 20

