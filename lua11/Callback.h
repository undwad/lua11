/*
** Callback.h by undwad
** lua11 is a very lightweight binding lua with C++11
** https://github.com/undwad/lua11 mailto:undwad@mail.ru
** see copyright notice in lua11.h
*/

#ifndef _LUA11_CALLBACK_H__
#define _LUA11_CALLBACK_H__

namespace lua11
{
	template <typename R, typename ...T> class Callback : public CallbackRef, protected Stack
	{
	public:
		Callback(lua_State* l, function<R(T...)> f) : CallbackRef(l), func(f) { }
		virtual ~Callback() { }

		bool setGlobal(const string& name)
		{
			push(L);
			lua_setglobal(L, name.c_str());
			return true;
		}

	protected:
		virtual bool pop(lua_State* L)
		{
			lua_pop(L, 1);
			return false;
		}

		virtual bool push(lua_State* L) const
		{
			lua_pushlightuserdata(L, (void*)this);
			lua_pushcclosure(L, &callback, 1);
			return true;
		}

	private:
		function<R(T...)> func;

		bool param(lua_State* L, int i) { return true; }
		template <typename P0, typename ...P> bool param(lua_State* L, int i, P0& p0, P&... p)
		{
			if (is(L, &p0, i))
			{
				get(L, &p0, i);
				return param(L, i + 1, p...);
			}
			return false;
		}

		int texterror(const string& text)
		{
			lua_pushnil(L);
			lua_pushstring(L, text.c_str());
			return 2;
		}
		int paramerror() { return texterror("invalid callback parameters"); }
		int resulterror() { return texterror("invalid callback result"); }

		template<typename Q> int callfunc(lua_State *L, function<void()>)
		{
			func();
			return 0;
		}

		template<typename Q> int callfunc(lua_State *L, function<Q()>)
		{
			if(Stack::push(L, func()))
				return 1;
			else return resulterror();
		}

		template<typename ...P> int callfunc(lua_State *L, function<void(P...)>)
		{
			tuple<P...> p;
			auto params = misc::make_function([this, L](P&... p) { return param(L, 1, p...); });
			if (misc::forwardref(p, params))
			{
				misc::forward(p, func);
				return 0;
			}
			else return paramerror();
		}

		template<typename Q, typename ...P> int callfunc(lua_State *L, function<Q(P...)>)
		{
			tuple<P...> p;
			auto params = misc::make_function([this, L](P&... p) { return param(L, 1, p...); });
			if (misc::forwardref(p, params))
			{
				if(Stack::push(L, misc::forward(p, func)))
					return 1;
				else return resulterror();
			}
			else return paramerror();
		}

		static int callback(lua_State *L) { return ((Callback<R, T...>*)lua_topointer(L, lua_upvalueindex(1)))->callfunc(L, function<R(T...)>()); }
	};

	struct CallbackFactory
	{
		template <typename R, typename ...T> static Callback<R, T...> make(lua_State* l, function<R(T...)> f) { return Callback<R, T...>(l, f); }
		template <typename R, typename ...T> static Callback<R, T...>* makeptr(lua_State* l, function<R(T...)> f) { return new Callback<R, T...>(l, f); }
	};

#	define MAKECALLBACK(l, f) CallbackFactory::make(l, misc::make_function(f))
#	define MAKECALLBACKPTR(l, f) CallbackFactory::makeptr(l, misc::make_function(f))

}

#endif // _LUA11_CALLBACK_H__
