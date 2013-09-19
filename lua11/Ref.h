
/*
** Ref.h 2013.09.19 10.47.38 undwad
** lua11 is a very lightweight binding lua with C++11
** https://github.com/lua11 mailto:undwad@mail.ru
** see copyright notice in lua11.h 
*/

#ifndef _LUA11_REF_H__
#define _LUA11_REF_H__

namespace lua11 
{
	class Ref
	{
		friend class Stack;

	public:
		string error;

		Ref() : L(nullptr) { }
		Ref(lua_State* l) : L(l) { }
		Ref(const Ref& r) : L(r.L) { }
		virtual ~Ref() { }

		lua_State* getL() const { return L; }

	protected:
		mutable lua_State* L;

		virtual bool pop(lua_State* L) = 0;

		virtual bool pop(lua_State* L, int i)
		{
			this->L = L;
			lua_pushvalue(L, i);
			return pop(L);
		}

		virtual bool push(lua_State* L) const = 0;
	};

	class RegistryRef : public Ref
	{
		friend class Stack;

	public:
		RegistryRef() : index(0), copy(false) { }
		RegistryRef(lua_State* l) : Ref(l), index(0), copy(false) { }
		RegistryRef(const RegistryRef& r) : Ref(r), index(r.index), copy(true) { }
		virtual ~RegistryRef() { free(); }

		operator bool() { return index; }
		bool operator !() { return !index; }

	protected:
		virtual bool pop(lua_State* L) 
		{ 
			free();
			index = luaL_ref(L, LUA_REGISTRYINDEX);
			copy = false;
			return index;
		}

		virtual bool push(lua_State* L) const
		{
			if(index)
			{
				lua_rawgeti(L, LUA_REGISTRYINDEX, index);
				return true;
			}
			return false;
		}

	private:
		int index;
		bool copy;

		void free()
		{
			if(index && !copy)
				luaL_unref(L, LUA_REGISTRYINDEX, index);
			index = 0;
		}
	};

	class FunctionRef : public RegistryRef
	{
	public:
		FunctionRef() { }
		FunctionRef(lua_State* l) : RegistryRef(l) { }
		FunctionRef(const FunctionRef& r) : RegistryRef(r) { }
		virtual ~FunctionRef() { }
	};

	class TableRef : public RegistryRef
	{
	public:
		TableRef() { }
		TableRef(lua_State* l) : RegistryRef(l) { }
		TableRef(const TableRef& r) : RegistryRef(r) { }
		virtual ~TableRef() { }
	};

	class CallbackRef : public Ref
	{
	public:
		CallbackRef(lua_State* l) : Ref(l) { }
		virtual ~CallbackRef() { }
	};

	class NilRef : public Ref
	{
		friend class Stack;

	public:
		NilRef(lua_State* l) : Ref(l) { }
		virtual ~NilRef() { }

	protected:
		virtual bool pop(lua_State* L) 
		{ 
			lua_pop(L, 1); 
			return true;
		}

		virtual bool push(lua_State* L) const 
		{
			lua_pushnil(L);
			return true;
		}
	};
}

#endif // _LUA11_REF_H__