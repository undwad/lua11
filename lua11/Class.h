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
	template <class T> class Object
	{
	public:
		Object(lua_State* l) : L(l) { }
		virtual ~Object() { }

		template <typename ...P> Object& constructor(const string& name)
		{
			Table table(L);
			auto init = new Callback<Table, P...>(L, [L](P... p)
			{
			});
			callbacks.push_back(shared_ptr<CallbackRef>(init));
			//table.set(name)

			return *this;
		}

	private:
		lua_State* L;
		vector<shared_ptr<CallbackRef>> callbacks;
	};
}

#endif // _LUA11_CLASS_H__