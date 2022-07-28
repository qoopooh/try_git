#!/usr/bin/env lua

x = 10

do
  local x = x -- create a new local variable
  print (x)
  x = x + 1

  do
    local x = x+1 -- create another local
    print (x)
  end

  print (x)
end

print (x)
