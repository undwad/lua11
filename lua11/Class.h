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
		Class(lua_State* l) : L(l), table(L)
		{ 
			if (table.createNew() && table.setGlobal(typeid(T).name()))
			{
				auto gc = new Callback<void, Table>(L, [this](Table t) 
				{ 
					T* obj;
					if (t.get("instance", (void**)&obj) && obj)
						delete obj;
				});
				callbacks.push_back(shared_ptr<CallbackRef>(gc));
				table.set("__gc", gc);
			}
		}
		virtual ~Class() { }

		operator bool() { return (bool)table; }
		bool operator !() { return !table; }

		string error() { return table.error; }

		template <typename ...P> Class& init(const string& name)
		{
			if (table)
			{
				auto lambda = [this](Table t, P... p)
				{ 
					auto obj = new T(p...);
					t.set("instance", (void*)obj);
				};
				auto init = new Callback<void, Table, P...>(L, lambda);
				callbacks.push_back(shared_ptr<CallbackRef>(init));
				table.set(name, init);
			}
			return *this;
		}

		template <typename ...P> Class& init() { return init<P...>("name"); }

	private:
		lua_State* L;
		Table table;
		vector<shared_ptr<CallbackRef>> callbacks;
	};
}

#endif // _LUA11_CLASS_H__