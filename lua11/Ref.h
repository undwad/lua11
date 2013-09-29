/*
** Ref.h by undwad
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
		RegistryRef() { }
		RegistryRef(lua_State* l) : Ref(l) { }
		RegistryRef(const RegistryRef& r) : Ref(r), index(r.index) { }
		virtual ~RegistryRef() { }

		operator bool() { return index && LUA_NOREF != *index; }
		bool operator !() { return index && LUA_NOREF == *index; }

		void copy(const RegistryRef& r) { index = r.index; }

	protected:
		virtual bool pop(lua_State* L) 
		{ 
			index = shared_ptr<int>(new int(luaL_ref(L, LUA_REGISTRYINDEX)), bind(free, L, _1));
			return LUA_NOREF != *index;
		}

		virtual bool push(lua_State* L) const
		{
			if (index && LUA_NOREF != *index)
			{
				lua_rawgeti(L, LUA_REGISTRYINDEX, *index);
				return true;
			}
			return false;
		}

	private:
		shared_ptr<int> index;

		static void free(lua_State* L, int* index)
		{
			if (index)
			{
				if (LUA_NOREF != *index)
					luaL_unref(L, LUA_REGISTRYINDEX, *index);
				delete index;
			}
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
		NilRef() { }
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