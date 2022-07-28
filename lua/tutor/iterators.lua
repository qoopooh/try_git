#!/usr/bin/env lua
-- https://www.tutorialspoint.com/lua/lua_iterators.htm
--
-- Stateless Iterators
--
function square(iteratorMaxCount, currentNumber)
  if currentNumber < iteratorMaxCount then
    currentNumber = currentNumber + 1
    return currentNumber, currentNumber * currentNumber
  end
end

function squares(iteratorMaxCount)
  return square, iteratorMaxCount, 0
end

for i, n in squares(3) do
  print(i, n)
end

--
-- Stateful Iterators
--
array = {"Lua", "Tutorial"}

function elementIterator (collection)

  local index = 0
  local count = #collection

  -- The closure funciton is returned

  return function ()
    index = index + 1

    if index <= count then
      -- return the current element of the iterator
      return collection[index]
    end
  end
end

for element in elementIterator(array) do
  print(element)
end
