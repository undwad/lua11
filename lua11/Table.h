
/*
** Table.h 2013.09.19 14.50.26 undwad
** lua11 is a very lightweight binding lua with C++11
** https://github.com/lua11 mailto:undwad@mail.ru
** see copyright notice in lua11.h
*/

#ifndef _LUA11_TABLE_H__
#define _LUA11_TABLE_H__

namespace lua11 
{
	class Table : public TableRef, protected Stack
	{
	public:
		Table() { }
		Table(lua_State* l) : TableRef(l) {}
		Table(lua_State* l, const string& name) : TableRef(l) { getGlobal(name); }
		Table(const Table& t) : TableRef(t) { }
		virtual ~Table() { }

		bool getGlobal(const string& name)
		{
			lua_getglobal(L, name.c_str());
			if(lua_istable(L, -1))
				return TableRef::pop(L);
			error = string(name) + " is not table";
			return false;
		}

		bool createNew() 
		{
			lua_newtable(L); 
			return TableRef::pop(L);
		}

		bool setGlobal(const string& name)
		{
			if(TableRef::push(L))
			{
				lua_setglobal(L, name.c_str());
				return true;
			}
			return false;
		}

		bool getMeta(TableRef* mt)
		{
			bool result = false;
			if(TableRef::push(L))
			{
				if(lua_getmetatable(L, -1))
				{
					if(is(L, mt, -1))
					{
						Stack::pop(L, mt, -1);
						result = true;
					}
				}
				else error = "can not get metatable";
				lua_pop(L, 1);
			}
			return result;
		}

		bool setMeta(const TableRef& mt)
		{
			bool result = false;
			if(TableRef::push(L))
			{
				Stack::push(L, mt);
				if(lua_setmetatable(L, -2))
					result = true;
				else error = "can not set metatable";
				lua_pop(L, 1);
			}
			return result;
		}

		int len() 
		{ 
			int result = -1;
			if(TableRef::push(L))
			{
				result = lua_rawlen(L, -1);  
				lua_pop(L, 1);
			}
			return result;
		}

		template <typename T> bool getKeys(vector<T>&  keys)
		{
			keys.clear();
			return iterate([this, &keys]()
			{
				T key;
				lua_pop(L, 1);
				if(Stack::is(L, &key))
				{
					Stack::get(L, &key);
					keys.push_back(key);
					return true;
				}
				return false;
			});
		}

		template <typename T> bool getValues(vector<T>&  values)
		{
			values.clear();
			return iterate([this, &values]()
			{
				T value;
				if(Stack::is(L, &value))
				{
					Stack::pop(L, &value);
					values.push_back(value);
					return true;
				}
				return false;
			});
		}

		template <typename K, typename V> bool getMap(map<K, V>& map)
		{
			map.clear();
			return iterate([this, &map]()
			{
				pair<K, V> pair;
				if(Stack::is(L, &pair.second))
				{
					Stack::pop(L, &pair.second);
					if(Stack::is(L, &pair.first))
					{
						Stack::get(L, &pair.first);
						map.insert(pair);
						return true;
					}
				}
				return false;
			});
		}

#		define _SET(T, N, K, F, KV) \
			bool N(K, T v) \
			{ \
				bool result = false; \
				if(TableRef::push(L)) \
				{ \
					Stack::push(L, KV); \
					Stack::push(L, v); \
					F(L, -3); \
					result = true; \
				} \
				lua_pop(L, 1); \
				return result; \
			} 

#		define SET(T) \
			_SET(T, set, const string& key, lua_settable, key.c_str()) \
			_SET(T, set, int key, lua_settable, key) \
			_SET(T, rawset, const string& key, lua_rawset, key.c_str()) \
			_SET(T, rawset, int key, lua_rawset, key)

		SET(bool)
		SET(unsigned char)
		SET(char)
		SET(short int)
		SET(unsigned short int)
		SET(int)
		SET(unsigned int)
		SET(long int)
		SET(unsigned long)
		SET(long long)
		SET(unsigned long long)
		SET(float)
		SET(double)
		SET(const char*)
		SET(const void*)
		SET(void*)
		SET(const string&)
		SET(const FunctionRef&)
		SET(const TableRef&)
		SET(const CallbackRef&)
		SET(const NilRef&)

#		undef SET
#		undef _SET

#		define _GET(T, N, K, F, KV)	\
			bool N(K, T* p) \
			{ \
				bool result = false; \
				if(TableRef::push(L)) \
				{ \
					Stack::push(L, KV); \
					F(L, -2); \
					if(is(L, p)) \
					{ \
						Stack::pop(L, p); \
						result = true; \
					} \
					else \
					{ \
						error = string("type mismatch "#T" <> ") + typeName(L); \
						lua_pop(L, 1); \
					} \
				} \
				lua_pop(L, 1); \
				return result; \
			} 

#		define GET(T) \
			_GET(T, get, const string& key, lua_gettable, key.c_str()) \
			_GET(T, get, int key, lua_gettable, key) \
			_GET(T, rawget, const string& key, lua_rawget, key.c_str()) \
			_GET(T, rawget, int key, lua_rawget, key)

		GET(bool)
		GET(unsigned char)
		GET(char)
		GET(short int)
		GET(unsigned short int)
		GET(int)
		GET(unsigned int)
		GET(long int)
		GET(unsigned long)
		GET(long long)
		GET(unsigned long long)
		GET(float)
		GET(double)
		GET(void*)
		GET(string)
		GET(FunctionRef)
		GET(TableRef)
		GET(CallbackRef)
		GET(NilRef)

#		undef GET
#		undef _GET

	private:
		bool iterate(function<bool()> callback)
		{
			bool result = false;
			if(TableRef::push(L))
			{
				result = true;
				lua_pushnil(L);
				while(lua_next(L, -2)) 
					if(!callback())
					{
						result = false;
						break;
					}
			}
			lua_pop(L, 1);
			return result;
		}
	};
}

#endif // _LUA11_TABLE_H__


