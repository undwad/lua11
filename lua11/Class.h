/*
** Class.h 2013.09.25 09.51.46 undwad
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
			if (table.createNew() && table.setGlobal(name) && table.set("type", typeid(T).hash_code()))
			{
				set("__gc", MAKECALLBACKPTR(L, [](Table t)
				{
					if (T* ptr = get(t))
					{
						delete ptr;
						return true;
					}
					return false;
				}));
			}
		}
		virtual ~Class() { }
		 
		operator bool() { return (bool)table; }
		bool operator !() { return !table; }

		string error() { return table.error; }

		template <typename ...P> bool init(const string& name)
		{
			return table ? set(name, MAKECALLBACKPTR(L, [](Table t, P... p) { return set(t, new T(p...)); })) : false;
		}

		template <typename ...P> bool init() { return init<P...>("init"); }

		template <typename R, typename ...P> bool set(const string& name, R(T::*func)(P...))
		{
			return table ? set(name, MAKECALLBACKPTR(L, [func](Table t, P... p)
			{
				if (T* ptr = get(t))
					return (ptr->*func)(p...);
				return R();
			})) : false;
		}

		template <typename R, typename ...P> bool setStatic(const string& name, R(*func)(P...))
		{
			return table ? set(name, MAKECALLBACKPTR(L, [func](P... p) { return (*func)(p...); })) : false;
		}

		static T* get(Table& t)
		{
			void* ptr;
			size_t type;
			return t && t.get("type", &type) && typeid(T).hash_code() == type && t.get("ptr", &ptr) && ptr ? (T*)ptr : nullptr;
		}

	private:
		lua_State* L;
		Table table;
		vector<shared_ptr<CallbackRef>>* callbacks;

		static bool set(Table& t, T* ptr) { return t && ptr && t.set("type", typeid(T).hash_code()) && t.set("ptr", (void*)ptr); }

		bool set(const string& name, CallbackRef* callback)
		{
			callbacks->push_back(shared_ptr<CallbackRef>(callback));
			return table.set(name, *callback);
		}
	};
}

#endif // _LUA11_CLASS_H__