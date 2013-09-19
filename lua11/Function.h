
/*
** Function.h 2013.09.19 10.47.38 undwad
** lua11 is a very lightweight binding lua with C++11
** https://github.com/lua11 mailto:undwad@mail.ru
** see copyright notice in lua11.h
*/

#ifndef _LUA11_FUNCTION_H__
#define _LUA11_FUNCTION_H__

namespace lua11 
{
	class Function : public FunctionRef, protected Stack
	{
	public:
		Function() { }
		Function(lua_State* l) : FunctionRef(l) { }
		Function(lua_State* l, const string& name) : FunctionRef(l) { getGlobal(name); }
		Function(const Function& f) : FunctionRef(f) { }
		virtual ~Function() { }

		bool getGlobal(const string& name)
		{
			lua_getglobal(L, name.c_str());
			if(lua_isfunction(L, -1))
				return FunctionRef::pop(L);
			lua_pop(L, 1);
			error = string(name) + " is not function";
			return false;
		}

		bool setGlobal(const string& name)
		{
			if(FunctionRef::push(L))
			{
				lua_setglobal(L, name.c_str());
				return true;
			}
			return false;
		}

		template <typename ...T> bool operator()(T... p)
		{
			if(FunctionRef::push(L))
			{
				dir = Dir::In;
				count = index = 0;
				if(params(L, p...))
				{
					if(lua_pcall(L, count, -(index), 0))
					{
						error = lua_tostring(L, -1);
						lua_pop(L, 1);
						return false;
					}
					dir = Dir::Out;
					return params(L, p...);
				}
			}
			lua_pop(L, 1);
			return false;
		}

	private:
		enum Dir { In, Out } dir;
		int count, index;

		bool params(lua_State* L) { return true; }
		template <typename T0, typename ...T> bool params(lua_State* L, T0 p0, T... p) { return param(L, p0) ? params(L, p...) : false; }

#		define PARAM(T) \
			bool param(lua_State* L, T p) \
			{ \
				if(Dir::In == dir) \
				{ \
					count++; \
					Stack::push(L, p); \
				} \
				return true; \
			}

		PARAM(bool)
		PARAM(unsigned char)
		PARAM(char)
		PARAM(short int)
		PARAM(unsigned short int)
		PARAM(int)
		PARAM(unsigned int)
		PARAM(long int)
		PARAM(unsigned long)
		PARAM(long long)
		PARAM(unsigned long long)
		PARAM(float)
		PARAM(double)
		PARAM(const char*)
		PARAM(const void*)
		PARAM(void*)
		PARAM(const string&)
		PARAM(const FunctionRef&)
		PARAM(const TableRef&)
		PARAM(const CallbackRef&)
		PARAM(const NilRef&)

#		undef PARAM

#		define PARAM(T) \
			bool param(lua_State* L, T* p) \
			{ \
				if(Dir::Out == dir) \
				{ \
					if(is(L, p, index)) \
					{ \
						Stack::pop(L, p, index); \
						index++; \
					} \
					else \
					{ \
						error = string("type mismatch "#T" <> ") + typeName(L, index); \
						lua_settop(L, index - 1); \
						return false; \
					} \
				} \
				else index--; \
				return true; \
			}

		PARAM(bool)
		PARAM(unsigned char)
		PARAM(char)
		PARAM(short int)
		PARAM(unsigned short int)
		PARAM(int)
		PARAM(unsigned int)
		PARAM(long int)
		PARAM(unsigned long)
		PARAM(long long)
		PARAM(unsigned long long)
		PARAM(float)
		PARAM(double)
		PARAM(void*)
		PARAM(string)
		PARAM(FunctionRef)
		PARAM(TableRef)
		PARAM(CallbackRef)
		PARAM(NilRef)

#		undef PARAM
	};

	class Script : public Function
	{
	public:
		Script(lua_State* l) : Function(l) { }
		virtual ~Script() { }

#		define LOAD(N, F) \
			bool load##N(const string& s) \
			{ \
				if(F(L, s.c_str())) \
				{ \
					error = lua_tostring(L, -1); \
					lua_pop(L, 0); \
					return false; \
				} \
				FunctionRef::pop(L); \
				return true; \
			}

		LOAD(File, luaL_loadfile)
		LOAD(Text, luaL_loadstring)

#		undef LOAD

	private:
		Script(const Script& s) : Function(s) { }
	};

	class ScriptFile : public Script
	{
	public:
		ScriptFile(lua_State* l, const string& file) : Script(l) { loadFile(file); }
		virtual ~ScriptFile() { }
	};

	class ScriptText : public Script
	{
	public:
		ScriptText(lua_State* l, const string& text) : Script(l) { loadText(text); }
		virtual ~ScriptText() { }
	};

}

#endif // _LUA11_FUNCTION_H__