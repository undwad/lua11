/*
** Ref.h 2013.09.23 12.36.06 undwad
** lua11 is a very lightweight binding lua with C++11
** https://github.com/undwad/lua11 mailto:undwad@mail.ru
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
		RegistryRef() : index(LUA_NOREF) { }
		RegistryRef(lua_State* l) : Ref(l), index(LUA_NOREF) { }
		RegistryRef(const RegistryRef& r) : Ref(r), index(LUA_NOREF)
		{ 
			if(r.push(L))
				pop(L); 
		}
		virtual ~RegistryRef() { free(); }

		operator bool() { return LUA_NOREF != index; }
		bool operator !() { return LUA_NOREF == index; }

	protected:
		virtual bool pop(lua_State* L) 
		{ 
			free();
			index = luaL_ref(L, LUA_REGISTRYINDEX);
			return LUA_NOREF != index;
		}

		virtual bool push(lua_State* L) const
		{
			if (LUA_NOREF != index)
			{
				lua_rawgeti(L, LUA_REGISTRYINDEX, index);
				return true;
			}
			return false;
		}

	private:
		int index;

		void free()
		{
			if (LUA_NOREF != index)
				luaL_unref(L, LUA_REGISTRYINDEX, index);
			index = LUA_NOREF;
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

	class ValueRef : public RegistryRef
	{
	public:
		ValueRef() { }
		ValueRef(lua_State* l) : RegistryRef(l) { }
		ValueRef(const ValueRef& r) : RegistryRef(r) { }
		virtual ~ValueRef() { }
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