-- class.lua
-- Compatible with Lua 5.1 (not 5.0).
function class(base, init)
   local c = {}    -- a new class instance
   if not init and type(base) == 'function' then
      init = base
      base = nil
   elseif type(base) == 'table' then
    -- our new class is a shallow copy of the base class!
      for i,v in pairs(base) do
         c[i] = v
      end
      c._base = base
   end
   -- the class will be the metatable for all its objects,
   -- and they will look up their methods in it.
   c.__index = c
   -- expose a constructor which can be called by <classname>(<args>)
   local mt = {}
   mt.__call = function(class_tbl, ...)
	   local obj = {}
	   setmetatable(obj,c)
	   if init then
		  init(obj, ...)
	   else 
		  -- make sure that any stuff from the base class is initialized!
		  if base and base.init then
			base.init(obj, ...)
		  end
	   end
--by undwad begin
		local mc = {}
		mc.__index = function(t, k) 
			local r = rawget(c, k)
			if r then return r end
			local f = rawget(c, 'get'..k)
			if 'function' == type(f) then return f(t) end
		end
		mc.__newindex = function(t, k, v)
			local f = rawget(c, 'set'..k)
			if 'function' == type(f) then f(t, v)
			else rawset(t, k, v) end
		end
		mc.__tostring = c.__tostring
		setmetatable(obj, mc)
--by undwad end
	   return obj
   end
   c.init = init
   c.is_a = function(self, klass)
      local m = getmetatable(self)
      while m do 
         if m == klass then return true end
         m = m._base
      end
      return false
   end
   setmetatable(c, mt)
   return c
end
