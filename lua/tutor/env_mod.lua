#!/usr/bin/env lua
local myModuleEnv = {}
myModuleEnv._ENV = myModuleEnv

local function myModuleFunction()
  _ENV = myModuleEnv
  moduleName = "MyModule"
end

myModuleFunction()
print(myModuleEnv.moduleName)  -- Output: MyModule
print(moduleName)              -- Error: nil (since moduleName is not a global)
