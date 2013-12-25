#!/usr/bin/env lua

t = {}
s = 'from=world, t=Lua'
for k, v in string.gmatch(s, "(%w+)=(%w+)") do
  t[k] =v
end
print (t['from'], t['t'])

