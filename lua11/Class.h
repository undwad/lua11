/*
** Class.h 2013.09.24 16.14.41 undwad
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
			callbacks = new vector<shared_ptr<CallbackRef>>();
			if (table.createNew() && table.setGlobal(name))
			{
				auto callback = MAKECALLBACKPTR(L, [](Table t)
				{
					T* obj;
					if (t.get("instance", (void**)&obj) && obj)
					{
						delete obj;
						return true;
					}
					return false;
				});
				set("__gc", callback);
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
				auto callback = MAKECALLBACKPTR(L, [](Table t, P... p)
				{
					auto obj = new T(p...);
					return obj && t.set("instance", (void*)obj);
				});
				return set(name, callback);
			}
			return false;
		}

		template <typename ...P> bool init() { return init<P...>("init"); }

		template <typename R, typename ...P> bool set(const string& name, R(T::*func)(P...))
		{
			if (table)
			{
				auto callback = MAKECALLBACKPTR(L, [func](Table t, P... p)
				{
					T* obj;
					if (t.get("instance", (void**)&obj) && obj)
						return (obj->*func)(p...);
					return R();
				});
				return set(name, callback);
			}
			return false;
		}

	private:
		lua_State* L;
		Table table;
		vector<shared_ptr<CallbackRef>>* callbacks;

		bool set(const string& name, CallbackRef* callback)
		{
			callbacks->push_back(shared_ptr<CallbackRef>(callback));
			return table.set(name, *callback);
		}
	};
}

#endif // _LUA11_CLASS_H__