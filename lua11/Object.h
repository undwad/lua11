/*
** Value.h 2013.09.23 11.10.57 undwad
** lua11 is a very lightweight binding lua with C++11
** https://github.com/undwad/lua11 mailto:undwad@mail.ru
** see copyright notice in lua11.h
*/

#ifndef _LUA11_OBJECT_H__
#define _LUA11_OBJECT_H__

namespace lua11 
{
	template <class T> class Object
	{
	public:
		Object() { }
		virtual ~Object() { }

		template <typename ...P> Object& constructor(const string& name)
		{

			return *this;
		}

	};
}

#endif // _LUA11_OBJECT_H__