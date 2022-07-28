#!/usr/bin/env lua

co = coroutine.create(function (val1, val2)
  local tempvar3 = 10
  print("coroutine section 1", val1, val2, tempvar3)

  local tempvar1 = coroutine.yield(val1+1,val2+1)
  tempvar3 = tempvar3 + val1
  print("coroutine section 2", tempvar1, tempvar2, tempvar3)

  local tempvar1, tempvar2 = coroutine.yield(val1+val2, val1-val2)
  tempvar3 = tempvar3 + val1
  print("coroutine section 3", tempvar1, tempvar2, tempvar3)

  return val2, "end"
end)

print("main", coroutine.resume(co, 3, 2))
print("main", coroutine.resume(co, 12, 14))
print("main", coroutine.resume(co, 5, 6))
print("main", coroutine.resume(co, 10, 20))

--[[ output
coroutine section 1     3       2       10
main    true    4       3
coroutine section 2     12      nil     13
main    true    5       1
coroutine section 3     5       6       16
main    true    2       end
main    false   cannot resume dead coroutine
--]]
