/*
** Class.h 2013.09.25 15.47.35 undwad
** lua11 is a very lightweight binding lua with C++11
** https://github.com/undwad/lua11 mailto:undwad@mail.ru
** see copyright notice in lua11.h
*/

#ifndef _LUA11_CLASS_H__
#define _LUA11_CLASS_H__

namespace lua11 
{
	template <class T> struct ClassUtils
	{
		static T* get(Table& t)
		{
			void* ptr;
			string type;
			return t && t.get("type", &type) && typeid(T).name() == type && t.get("ptr", &ptr) && ptr ? (T*)ptr : nullptr;
		}

		static bool set(Table& t, T* ptr) { return t && ptr && t.set("type", typeid(T).name()) && t.set("ptr", (void*)ptr); }
	};

	template <class T> class Class
	{
	public:
		Class(lua_State* l, const string& name) : L(l), table(L)
		{ 
			callbacks = new vector<shared_ptr<CallbackRef>>();
			if (table.createNew() && table.setGlobal(name) && table.set("type", typeid(T).hash_code()))
			{
				auto callback = MAKECALLBACKPTR(L, [](Table t)
				{
					if (T* ptr = ClassUtils<T>::get(t))
					{
						delete ptr;
						return true;
					}
					return false;
				});
				store("__gc", callback);
			}
		}
		virtual ~Class() { }
		 
		operator bool() { return (bool)table; }
		bool operator !() { return !table; }

		string error() { return table.error; }

		template <typename ...P> Class<T>& init(const string& name)
		{
			if (table)
			{
				auto callback = MAKECALLBACKPTR(L, [](Table t, P... p) { return ClassUtils<T>::set(t, new T(p...)); });
				store(name, callback);
			}
				
			return *this;
		}

		template <typename ...P> Class<T>& init() { return init<P...>("init"); }

		template <typename R, typename ...P> Class<T>& set(const string& name, R(T::*func)(P...))
		{
			if (table)
			{
				auto callback = MAKECALLBACKPTR(L, [func](Table t, P... p)
				{
					if (T* ptr = ClassUtils<T>::get(t))
						return (ptr->*func)(p...);
					return R();
				});
				store(name, callback);
			}
			return *this;
		}

		template <typename R, typename ...P> Class<T>& set(const string& name, R(*func)(P...))
		{
			if (table)
			{
				auto callback = MAKECALLBACKPTR(L, [func](Table t, P... p) { return (*func)(p...); });
				store(name, callback);
			}
			return *this;
		}

		template <typename P> Class<T>& set(const string& name, P p)
		{
			if (table)
				table.set(name, p);
			return *this;
		}

	private:
		lua_State* L;
		Table table;
		vector<shared_ptr<CallbackRef>>* callbacks;

		bool store(const string& name, CallbackRef* callback)
		{
			callbacks->push_back(shared_ptr<CallbackRef>(callback));
			return table.set(name, *callback);
		}
	};
}

#endif // _LUA11_CLASS_H__