#!/usr/bin/env lua
--[[
-- https://www.tutorialspoint.com/lua/lua_tables.htm
--]]

--
-- Concatenation
--
fruits = {"banana","orange","apple"}

-- concatenate with a character
print(table.concat(fruits," -- "))
-- concatenate fruits based on index
print(table.concat(fruits," -- ", 2, 3))

--
-- Insert and Remove
--
table.insert(fruits, "mango")
print("Fruit at index 4 is", fruits[4])

table.insert(fruits, 2, "grapes")
print("Fruit at index 2 is", fruits[2])

--print("The maximum elements in table is", table.maxn(fruits))

print("The last element is", fruits[5])

table.remove(fruits)
print("The previous last element is", fruits[5])


--
-- Sorting table
--
for k, v in ipairs(fruits) do
  print(k, v)
end

table.sort(fruits)
print("sorted table")

for k, v in ipairs(fruits) do
  print(k, v)
end
