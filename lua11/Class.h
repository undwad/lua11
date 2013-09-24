/*
** Class.h 2013.09.24 10.24.37 undwad
** lua11 is a very lightweight binding lua with C++11
** https://github.com/undwad/lua11 mailto:undwad@mail.ru
** see copyright notice in lua11.h
*/

#ifndef _LUA11_CLASS_H__
#define _LUA11_CLASS_H__

namespace lua11 
{
	template <class T> class Class
	{
	public:
		Class(lua_State* l, const string& name) : L(l), table(L)
		{ 
			if (table.createNew() && table.setGlobal(name))
			{
				auto gc = new Callback<bool, Table>(L, _gc);
				callbacks.push_back(shared_ptr<CallbackRef>(gc));
				table.set("__gc", *gc);
			}
		}
		virtual ~Class() { }

		operator bool() { return (bool)table; }
		bool operator !() { return !table; }

		string error() { return table.error; }

		template <typename ...P> bool init(const string& name)
		{
			if (table)
			{
				auto init = new Callback<bool, Table, P...>(L, _init<P...>);
				callbacks.push_back(shared_ptr<CallbackRef>(init));
				return table.set(name, *init);
			}
			return false;
		}

		template <typename ...P> bool init() { return init<P...>("init"); }

	private:
		lua_State* L;
		Table table;
		vector<shared_ptr<CallbackRef>> callbacks;

		static bool _gc(Table t)
		{
			T* obj;
			if (t.get("instance", (void**)&obj) && obj)
			{
				delete obj;
				return true;
			}
			return false;
		}

		template <typename ...P> static bool _init(Table t, P... p)
		{
			auto obj = new T(p...);
			return obj && t.set("instance", (void*)obj);
		}
	};
}

#endif // _LUA11_CLASS_H__