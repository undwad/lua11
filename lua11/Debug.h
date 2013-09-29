/*
** Debug.h by undwad
** lua11 is a very lightweight binding lua with C++11
** https://github.com/undwad/lua11 mailto:undwad@mail.ru
** see copyright notice in lua11.h
*/

#ifndef _LUA11_DEBUG_H__
#define _LUA11_DEBUG_H__

namespace lua11 
{
	class Debug
	{
	public:
		static void print(lua_State* L, ostream& stream)
		{
			stream << "STACK" << endl;
			for (int i = 1; i <= lua_gettop(L); i++)
			{
				stream << i << ": ";
				printvalue(L, stream, i);
			}
			stream << endl;
			stream << "REGISTRY" << endl;
			for (int i = 1; i <= lua_rawlen(L, LUA_REGISTRYINDEX); i++)
			{
				stream << i << ": ";
				lua_rawgeti(L, LUA_REGISTRYINDEX, i);
				printvalue(L, stream);
				lua_pop(L, 1);
			}
		}

		static void save(lua_State* L, const string& path)
		{ 
			ofstream file(path);
			print(L, file); 
			file.close();
		}

	private:
		static void printvalue(lua_State* L, ostream& stream, int index = -1)
		{
			int t = lua_type(L, index);
			stream << lua_typename(L, t) << " ";
			switch (t)
			{
			case LUA_TNIL: stream << "nil"; break;
			case LUA_TBOOLEAN: stream << lua_toboolean(L, index); break;
			case LUA_TNUMBER: stream << lua_tonumber(L, index); break;
			case LUA_TSTRING: stream << lua_tostring(L, index); break;
			case LUA_TTABLE:
			case LUA_TFUNCTION:
			case LUA_TTHREAD:
			case LUA_TLIGHTUSERDATA:
			case LUA_TUSERDATA: stream << lua_topointer(L, index); break;
			}
			stream << endl;
		}
	};

#	define LUA11DUMP(L) Debug::save(L, "lua11dump.txt");
}

#endif // _LUA11_DEBUG_H__