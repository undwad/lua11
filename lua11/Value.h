/*
** Value.h by undwad
** lua11 is a very lightweight binding lua with C++11
** https://github.com/undwad/lua11 mailto:undwad@mail.ru
** see copyright notice in lua11.h
*/

#ifndef _LUA11_VALUE_H__
#define _LUA11_VALUE_H__

namespace lua11 
{
	class Nil : public NilRef
	{
	public:
		Nil(lua_State* l) : NilRef(l) { }
		virtual ~Nil() { }
	};

	class Value : public ValueRef, protected Stack
	{
	public:
		Value() { }
		Value(lua_State* l) : ValueRef(l) {}
		Value(lua_State* l, const string& name) : ValueRef(l) { getGlobal(name); }
		Value(const Value& v) : ValueRef(v) { }
		virtual ~Value() { }

		bool getGlobal(const string& name)
		{
			lua_getglobal(L, name.c_str());
			if (Stack::is(L, this))
				return ValueRef::pop(L);
			error = string(name) + " is not value";
			return false;
		}

		bool setGlobal(const string& name)
		{
			if (ValueRef::push(L))
			{
				lua_setglobal(L, name.c_str());
				return true;
			}
			return false;
		}

		template <typename T> bool set(T v)
		{
			Stack::push(L, v);
			return ValueRef::pop(L);
		}

		template <typename T> void operator =(T v) { set(v); }

		template <typename T> bool get(T* v)
		{
			bool result = false;
			if (ValueRef::push(L))
			{
				if (Stack::is(L, v))
				{
					Stack::pop(L, v);
					result = true;
				}
				lua_pop(L, 1);
			}
			return result;
		}

		template <typename T> operator T() { T v;  get(&v); return v; }

		int type()
		{
			int result = LUA_TNONE;
			if (ValueRef::push(L))
			{
				result = lua_type(L, -1);
				lua_pop(L, 1);
			}
			return result;
		}

		string typeName() { return lua_typename(L, type()); }

		template <typename T> T get()
		{
			T result = T();
			get(&result);
			return result;
		}
	};
}

#endif // _LUA11_VALUE_H__