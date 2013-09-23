/*
** Value.h 2013.09.23 09.16.42 undwad
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

	//class Value : public ValueRef, protected Stack
	//{
	//public:
	//	string error;
	//	string name;

	//	Value(lua_State* l) : L(l) {}
	//	Value(lua_State* l, const string& n) : L(l), name(n) {}
	//	virtual ~Value() { }

	//	bool getGlobal(const string& name)
	//	{
	//		lua_getglobal(L, name.c_str());
	//		if (lua_istable(L, -1))
	//			return TableRef::pop(L);
	//		error = string(name) + " is not table";
	//		return false;
	//	}

	//	bool createNew()
	//	{
	//		lua_newtable(L);
	//		return TableRef::pop(L);
	//	}

	//	bool setGlobal(const string& name)
	//	{
	//		if (TableRef::push(L))
	//		{
	//			lua_setglobal(L, name.c_str());
	//			return true;
	//		}
	//		return false;
	//	}

	//	template <typename T> void set(T v)
	//	{
	//		Stack::push(L, v);
	//		lua_setglobal(L, name.c_str());
	//	}

	//	template <typename T> void operator =(T v) { set(v); }

	//	template <typename T> void set(const string& n, T v) { name = n; set(v); }

	//	template <typename T> bool get(T* v)
	//	{
	//		lua_getglobal(L, name.c_str());
	//		if (Stack::is(L, v))
	//		{
	//			Stack::pop(L, v);
	//			return true;
	//		}
	//		lua_pop(L, 1);
	//		return false;
	//	}

	//	template <typename T> operator T() { T v;  get(&v); return v; }

	//	template <typename T> bool get(const string& n, T* v) { name = n; return get(v); }

	//	int type()
	//	{
	//		lua_getglobal(L, name.c_str());
	//		int result = lua_type(L, -1);
	//		lua_pop(L, 1);
	//		return result;
	//	}

	//	string typeName() { return lua_typename(L, type()); }

	//protected:
	//	mutable lua_State* L;
	//};

	class Value : protected Stack
	{
	public:
		string error;
		string name;

		Value(lua_State* l) : L(l) {}
		Value(lua_State* l, const string& n) : L(l), name(n) {}
		virtual ~Value() { }

		template <typename T> void set(T v)
		{
			Stack::push(L, v);
			lua_setglobal(L, name.c_str());
		}

		template <typename T> void operator =(T v) { set(v); }

		template <typename T> void set(const string& n, T v) { name = n; set(v); }

		template <typename T> bool get(T* v)
		{
			lua_getglobal(L, name.c_str());
			if (Stack::is(L, v))
			{
				Stack::pop(L, v);
				return true;
			}
			lua_pop(L, 1);
			return false;
		}

		template <typename T> operator T() { T v;  get(&v); return v; }

		template <typename T> bool get(const string& n, T* v) { name = n; return get(v); }

		int type()
		{
			lua_getglobal(L, name.c_str());
			int result = lua_type(L, -1);
			lua_pop(L, 1); 
			return result; 
		}

		string typeName() { return lua_typename(L, type()); }

	protected:
		mutable lua_State* L;
	};
}

#endif // _LUA11_VALUE_H__