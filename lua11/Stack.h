/*
** Stack.h 2013.09.23 09.16.42 undwad
** lua11 is a very lightweight binding lua with C++11
** https://github.com/undwad/lua11 mailto:undwad@mail.ru
** see copyright notice in lua11.h
*/

#ifndef _LUA11_STACK_H__
#define _LUA11_STACK_H__

namespace lua11 
{
	class Stack
	{
	public:
		Stack() { }
		virtual ~Stack() { }

		static void print(lua_State* L, ostream& stream)
		{
			for(int i = 1; i <= lua_gettop(L); i++) 
			{  
				int t = lua_type(L, i);
				stream << lua_typename(L, t) << " ";
				switch (t) 
				{
				case LUA_TNIL: stream << "nil"; break;
				case LUA_TBOOLEAN: stream << lua_toboolean(L, i); break;
				case LUA_TNUMBER: stream << lua_tonumber(L, i); break;
				case LUA_TSTRING: stream << lua_tostring(L, i); break;
				case LUA_TTABLE: 
				case LUA_TFUNCTION: 
				case LUA_TTHREAD: 
				case LUA_TLIGHTUSERDATA: 
				case LUA_TUSERDATA: stream << lua_topointer(L, i); break;
				}
				stream << endl;
			}
		}

		static void save(lua_State* L, const string& path)
		{ 
			ofstream file(path);
			print(L, file); 
			file.close();
		}

		void push(lua_State* L, bool p) { lua_pushboolean(L, p); }
		void push(lua_State* L, unsigned char p) { lua_pushunsigned(L, p); }
		void push(lua_State* L, char p) { lua_pushinteger(L, p); }
		void push(lua_State* L, short int p) { lua_pushinteger(L, p); }
		void push(lua_State* L, unsigned short int p) { lua_pushunsigned(L, p); }
		void push(lua_State* L, int p) { lua_pushinteger(L, p); }
		void push(lua_State* L, unsigned int p) { lua_pushunsigned(L, p); }
		void push(lua_State* L, long int p) { lua_pushinteger(L, p); }
		void push(lua_State* L, unsigned long p) { lua_pushunsigned(L, p); }
		void push(lua_State* L, long long p) { lua_pushinteger(L, p); }
		void push(lua_State* L, unsigned long long p) { lua_pushunsigned(L, p); }
		void push(lua_State* L, float p) { lua_pushnumber(L, p); }
		void push(lua_State* L, double p) { lua_pushnumber(L, p); }
		void push(lua_State* L, const char* p) { lua_pushstring(L, p); }
		void push(lua_State* L, const void* p) { lua_pushlightuserdata(L, (void*)p); }
		void push(lua_State* L, void* p) { lua_pushlightuserdata(L, p); }
		void push(lua_State* L, const string& p) { lua_pushlstring(L, p.c_str(), p.size()); }
		void push(lua_State* L, const FunctionRef& p) { p.push(L); }
		void push(lua_State* L, const TableRef& p) { p.push(L); }
		void push(lua_State* L, const ValueRef& p) { p.push(L); }
		void push(lua_State* L, const NilRef& p) { p.push(L); }
		void push(lua_State* L, const CallbackRef& p) { p.push(L); }

		bool is(lua_State* L, bool* p, int i = -1) { return lua_isboolean(L, i); }
		bool is(lua_State* L, unsigned char* p, int i = -1) { return lua_isnumber(L, i); }
		bool is(lua_State* L, char* p, int i = -1) { return lua_isnumber(L, i); }
		bool is(lua_State* L, short int* p, int i = -1) { return lua_isnumber(L, i); }
		bool is(lua_State* L, unsigned short int* p, int i = -1) { return lua_isnumber(L, i); }
		bool is(lua_State* L, int* p, int i = -1) { return lua_isnumber(L, i); }
		bool is(lua_State* L, unsigned int* p, int i = -1) { return lua_isnumber(L, i); }
		bool is(lua_State* L, long int* p, int i = -1) { return lua_isnumber(L, i); }
		bool is(lua_State* L, unsigned long* p, int i = -1) { return lua_isnumber(L, i); }
		bool is(lua_State* L, long long* p, int i = -1) { return lua_isnumber(L, i); }
		bool is(lua_State* L, unsigned long long* p, int i = -1) { return lua_isnumber(L, i); }
		bool is(lua_State* L, float* p, int i = -1) { return lua_isnumber(L, i); }
		bool is(lua_State* L, double* p, int i = -1) { return lua_isnumber(L, i); }
		bool is(lua_State* L, void** p, int i = -1) { return lua_islightuserdata(L, i); }
		bool is(lua_State* L, string* p, int i = -1) { return lua_isstring(L, i); }
		bool is(lua_State* L, FunctionRef* p, int i = -1) { return lua_isfunction(L, i); }
		bool is(lua_State* L, TableRef* p, int i = -1) { return lua_istable(L, i); }
		bool is(lua_State* L, ValueRef* p, int i = -1) { return !lua_isnil(L, i); }
		bool is(lua_State* L, NilRef* p, int i = -1) { return lua_isnil(L, i); }
		bool is(lua_State* L, CallbackRef* p, int i = -1) { return lua_iscfunction(L, i); }

		void get(lua_State* L, bool* p, int i = -1) { *p = lua_toboolean(L, i); }
		void get(lua_State* L, unsigned char* p, int i = -1) { *p = lua_tounsigned(L, i); }
		void get(lua_State* L, char* p, int i = -1) { *p = lua_tointeger(L, i); }
		void get(lua_State* L, short int* p, int i = -1) { *p = lua_tointeger(L, i); }
		void get(lua_State* L, unsigned short int* p, int i = -1) { *p = lua_tounsigned(L, i); }
		void get(lua_State* L, int* p, int i = -1) { *p = lua_tointeger(L, i); }
		void get(lua_State* L, unsigned int* p, int i = -1) { *p = lua_tounsigned(L, i); }
		void get(lua_State* L, long int* p, int i = -1) { *p = lua_tointeger(L, i); }
		void get(lua_State* L, unsigned long* p, int i = -1) { *p = lua_tounsigned(L, i); }
		void get(lua_State* L, long long* p, int i = -1) { *p = lua_tointeger(L, i); }
		void get(lua_State* L, unsigned long long* p, int i = -1) { *p = lua_tounsigned(L, i); }
		void get(lua_State* L, float* p, int i = -1) { *p = lua_tonumber(L, i); }
		void get(lua_State* L, double* p, int i = -1) { *p = lua_tonumber(L, i); }
		void get(lua_State* L, void** p, int i = -1) { *p = lua_touserdata(L, i); }
		void get(lua_State* L, string* p, int i = -1) { size_t len; lua_tolstring(L, i, &len); *p = string(lua_tolstring(L, i, nullptr), len); }
		void get(lua_State* L, FunctionRef* p, int i = -1) { p->Ref::pop(L, i); }
		void get(lua_State* L, TableRef* p, int i = -1) { p->Ref::pop(L, i); }
		void get(lua_State* L, ValueRef* p, int i = -1) { p->Ref::pop(L, i); }
		void get(lua_State* L, NilRef* p, int i = -1) { p->Ref::pop(L, i); }
		void get(lua_State* L, CallbackRef* p, int i = -1) { p->Ref::pop(L, i); }

#		define POP(T) void pop(lua_State* L, T* p, int i = -1) { get(L, p, i); lua_remove(L, i); }

		POP(bool)
		POP(unsigned char)
		POP(char)
		POP(short int)
		POP(unsigned short int)
		POP(int)
		POP(unsigned int)
		POP(long int)
		POP(unsigned long)
		POP(long long)
		POP(unsigned long long)
		POP(float)
		POP(double)
		POP(void*)
		POP(string)
		POP(FunctionRef)
		POP(TableRef)
		POP(ValueRef)
		POP(NilRef)
		POP(CallbackRef)

#		undef POP

	};
}

#endif // _LUA11_STACK_H__