function class(base, init)
	local c = {}
	if not init and type(base) == 'function' then
		init = base
		base = nil
	elseif type(base) == 'table' then
		for i,v in pairs(base) do
			c[i] = v
		end
		c.__base = base
	end
	local __index = c.__index
	c.__index = function(t, k)
		local r = rawget(c, k)
		if r then return r end
		local f = rawget(c, 'get'..k)
		if 'function' == type(f) then return f(t) end
	end
	c.__newindex = function(t, k, v)
		local f = rawget(c, 'set'..k)
		if 'function' == type(f) then f(t, v)
		else rawset(t, k, v) end
	end
	local mt = {}
	mt.__call = function(class_tbl, ...)
		local obj = {}
		setmetatable(obj,c)
		if class_tbl.__init then
			class_tbl.__init(obj, ...)
		elseif base and base.__init then
			base.__init(obj, ...)
		end
		return obj
	end
	c.__init = init
	c.__is = function(self, klass)
		local m = getmetatable(self)
		while m do 
			if m == klass then return true end
				m = m.__base
			end
		return false
	end
	setmetatable(c, mt)
	return c
end
