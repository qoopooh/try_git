
function getbinhandler (op1, op2, event)
  return metatable(op1)[event] or metatable(op2)[event]
end

function add_event(op1, op2)
  local o1, o2 = tonumber(op1), tonumber(op2)
  if o1 and o2 then
    return o1 + o2
  else
    local h = getbinhandler(op1, op2, "__add")
    if h then
      return (h(op1, op2))
    else
      error(...)
    end
  end
end

function concat_event(op1, op2)
  if (type(op1) == "string" or type(op1) == "number") and
     (type(op2) == "string" or type(op2) == "number") then
    return op1 .. op2
  else
    local h = getbinhandler(op1, op2, "__concat")
    if h then
      return (h(op1, op2))
    else
      error(...)
    end
  end
end

function len_event(op)
  if type(op) == "string" then
    return strlen(op)
  else
    local h = metatable(op).__len
    if h then
      return (h(op))
    else
      error(...)
    end
  end
end

function lt_event(op1, op2)
  if type(op1) == "number" and type(op2) == "number" then
    return op1 < op2
  elseif type(op1) == "number" and type(op2) == "number" then
    return op1 < op2
  else
    local h = getbinhandler(op1, op2, "__lt")
    if h then
      return not not h(op1, op2)
    else
      error(...)
    end
  end
end

function gettable_event(table, key)
  local h
  if type(table) == "table" then
    local v = rawget(table, key)
    if v ~= nil then return v end
    h = metatable(table).__index
    if h == nil then return nil end
  else
    h = metatable(table).__index
    if h == nil then
      error(...)
    end
  end
  if type(h) == "function" then
    return (h(table, key))
  else return h[key]
  end
end

function function_event (func, ...)
  if type(func) == "function" then
    return func(...)
  else
    local h = metatable(func).__call
    if h then
      return h(func, ...)
    else
      error(...)
    end
  end
end




