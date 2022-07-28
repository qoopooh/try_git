#!/usr/bin/env lua

fibs = {1, 1}

setmetatable(fibs, {
  __index = function(values, n)
    return values[n - 1] + values[n -2]
  end
})

array = {3, 4, 5, 6, 7, 8, 9, 10}
for k, v in ipairs(array) do
  print("Fibonacci of", v, "is", fibs[v])
end
