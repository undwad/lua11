/*
** example.cpp 2013.09.23 09.16.42 undwad
** lua11 is a very lightweight binding lua with C++11
** https://github.com/undwad/lua11 mailto:undwad@mail.ru
** see copyright notice in lua11.h
*/

//this file is the example of binding lua with C++11
//basic knowledge of lua language and lua c api required
//the best way to learn lua11 is to debug this example step by step

//include lua11 headers
//you can include your version of lua before this
//otherwise lua11 will include lua from lua11/lua
#include "lua11/lua11.h"

using namespace lua11;

//next macro saves current lua stack to the text file
//you can open the file (with for example far manager) to check for lua stack leaks at runtime
#define SAVESTACK lua11::Stack::save(&*L, "stack.txt");

int main(int argc, char* argv[])
{
	shared_ptr<lua_State> L(luaL_newstate(), lua_close); //create new lua state
	luaL_openlibs(&*L); //optionaly open standard lua libraries

	//EXAMPLE 1
	//shows how to execute lua scripts
	{
		Script script(&*L); //new empty lua script
		if (script.loadFile("helloworld.lua")) //load script from lua file
			script(); //execute script
		else
			cout << script.error << endl; //print error

		SAVESTACK
	}

	//EXAMPLE 2
	//shows how to execute lua functions with parameters and receive the result
	{
		Script script(&*L);
		script.loadText(R"LUA(
			return function(name)
				return 'hello world, '..name..'!'
			end
		)LUA");

		if (script) //script successfully loaded
		{
			string str; //string variable for result
			if (!script("Nick", &str)) //execute script
				cout << script.error << endl; //print error

			//script execution returned error because script returns function not string
			//so we need to do next
			Function func(&*L); //function variable for result
			if (script(&func)) //execute script
			{
				if (func("Nick", &str)) //execute function
					cout << str << endl; //print result
				else
					cout << func.error << endl; //print error
			}
			else
				cout << script.error << endl; //print error
		}
		else
			cout << script.error << endl; //print error

		SAVESTACK
	}
	//every function returns boolean result where true means ok and false means error
	//every class has string error field that should be checked on error
	//further will be no error printing

	//EXAMPLE 3
	//shows how to access lua tables and functions
	Function print(&*L, "print"); //get lua print function
	print(true, 234, "some text"); //test it
	//from now and on we will print everything with it because it's cool
	{
		if (ScriptText(&*L, R"LUA(
			array = { '!1!', '!2!', '!3!' }
			table = {
				num = 123,
				text = 'some text',
				subtable = {
					bool = true,
					func = function(a, b) return a - b, a + b end
				}
			}
			function table:func() return self.text..self.num end
		)LUA")())
		{
			Table table(&*L); //table variable
			if (table.getGlobal("array")) //get global table
			{
				string text;
				//it is posible to iterate thru array by index
				for (int i = 1; i <= table.len(); i++)
					if (table.get(i, &text)) //get element at index i
						print(i, text);

				//if table values are of the same type it is possible to iterate thru them
				vector<string> values; //vector for table values
				if (table.getValues(values)) //get table values
				for (auto value : values) //print table keys
					print(value);

				//if table keys are of the same type1
				//and table values are of the same type2
				//it is possible to iterate thru pairs of them
				map<int, string> map; //map for table key/value pairs
				if (table.getMap(map)) //get table key/value pairs
					for (auto pair : map) //print table key/value pairs
						print(pair.first, pair.second);
			}
			if (table.getGlobal("table")) //get global table
			{
				//if table keys are of the same type it is possible to iterate thru them
				vector<string> keys; //vector for table keys
				if (table.getKeys(keys)) //get table keys
				for (auto key : keys) //print table keys
					print(key);

				//print items type values
				print(table.type("num"), table.type("text"), table.type("subtable"), table.type("someparam"));

				//print items type names
				print(table.typeName("num"), table.typeName("text"), table.typeName("subtable"), table.typeName("someparam"));

				int num;
				string text;
				if (table.get("num", &num) && table.get("text", &text)) //get table fields
					print(num, text); //print result

				Table subtable(&*L); //subtable variable
				if (table.get("subtable", &subtable)) //get subtable
				{
					print(subtable); //print result
					bool b;
					Function func(&*L); //subtable function variable
					if (subtable.get("bool", &b) && subtable.get("func", &func)) //get subtable fields
					{
						print(b, func); //print result
						int r1, r2;
						if (func(1, 2, &r1, &r2)) //execute function of subtable and receive 2 results
							print(r1, r2); //print results
					}
				}

				//now execute classlike table function
				Function func(&*L);
				if (table.get("func", &func)) //get table function
				{
					if (func(table, &text)) //to execute classlike function we should pass table as a first function parameter
						print(text);
				}
			}
		}

		SAVESTACK
	}

	//EXAMPLE 4
	//shows how create lua tables and values from c++
	{
		ScriptText(&*L, "print(table)")(); //print our existing table

		Table table(&*L);
		if (table.createNew()) //create new table
			if (table.setGlobal("table")) //set global variable to table
			{
				ScriptText(&*L, "print(table)")(); //print it
				Table subtable(&*L);
				if (subtable.createNew()) //create new table
					if (subtable.set("param", 123.321)) //set subtable item
					if (table.set("subtable", subtable)) //set subtable as table item
					{
						ScriptText(&*L, "print(table.subtable.param)")(); //print subtable item
						if(subtable.set("param", Nil(&*L))) //remove subtable item
							ScriptText(&*L, "print(table.subtable.param)")(); //print subtable item
					}
			}

		Value value(&*L, "value1"); //global value
		value = 123; //set global value value
		ScriptText(&*L, "print(value1)")(); //print it
		value.name = "value2"; //set global value name
		value = true; //set global value
		ScriptText(&*L, "print(value2)")(); //print it
		print((bool)value); //print it
		value.set("value3", "some text"); //set global value name and value
		ScriptText(&*L, "print(value3)")(); //print it
		print(value.type(), value.typeName()); //print value type and name
		ScriptText(&*L, "value3 = 123.321")(); //change it
		double v;
		if (value.get(&v)) //get it
			print(v); //print it
		print(value.type(), value.typeName()); //print value type and name

		SAVESTACK
	}

	//EXAMPLE 4
	//shows how call std::function from lua and create lua metatables
	{
		auto func = [](int a, int b) { return (float)a / b; }; //define callback anon function
		auto callback1 = MAKECALLBACK(&*L, func); //callback holds std::function<float(int, int)>
		callback1.setGlobal("div"); //sets callback as global function
		ScriptText(&*L, "print(div(1, 2))")(); //test it

		auto proc = [](string s) { cout << s << endl; }; //define callback anon procedure
		auto callback2 = MAKECALLBACK(&*L, proc); //callback holds std::function<void(string)>
		callback2.setGlobal("proc"); //sets callback as global function
		ScriptText(&*L, "print(proc('some text', 'excess param'))")(); //test it
		
		ScriptText(&*L, "print(table.someparam)")(); //print our table's absent parameter

		Table table(&*L);
		if (table.getGlobal("table")) //get table
		{
			Table metatable(&*L); //define metatable for table
			if (!table.getMeta(&metatable)) //try to get metatable
				print(table.error); //print error because table yet does not have metatable

			metatable.createNew(); //create metatable
			auto __index = MAKECALLBACK(&*L, [](Table table, string key) //define callback __index metamethod
			{
				string value;
				return string(key) + "_" + (table.rawget(key, &value) ? value : "nil"); //return key_value string
			});

			if (metatable.set("__index", __index)) //set metatable's __index function
			{
				if (table.setMeta(metatable)) //set table's metatable
					ScriptText(&*L, "print(table.someparam)")(); //once again print our table's absent parameter
			}

			if (table.getMeta(&metatable)) //get metatable
				print(metatable); //print result
		}

		SAVESTACK
	}


	////EXAMPLE 5!
	//{
	//	auto func = []() { return "joder"; };
	//	auto callback = MAKECALLBACK(&*L, func);
	//	callback.setGlobal("test");
	//	ScriptText(&*L, "print(test(111, 222))")();

	//	SAVESTACK
	//}


	////EXAMPLE 4
	////shows how to bind c++ classes to lua
	//{
	//	struct Object
	//	{
	//		string str;

	//		Object(const string& s) : str(s) { }

	//		string getString() { return str; }
	//		
	//		void setString(const string& s) { str = s; }
	//	};

	//	
	//	auto constructor = MAKECALLBACK(&*L, [L](string s)
	//	{
	//		cout << "constructor" << endl;
	//		auto o = new Object(s);
	//		cout << (void*)o << endl;
	//		auto destructor = MAKECALLBACK(&*L, [o](void* ptr)
	//		{
	//			cout << "desrtuctor" << endl;
	//			delete o;
	//			return 0;
	//		});
	//		//Table mt(&*L);
	//		//mt.createNew();
	//		//mt.set("__gc", destructor);
	//		
	//		Table t(&*L);
	//		t.createNew();
	//		return t;
	//	});
	//	constructor.setGlobal("Object");

	//	if (ScriptText(&*L, R"LUA(
	//		f = function()
	//			print(Object)
	//			o = Object('some text')
	//			print(o)
	//			print('exit')
	//		end
	//		print(pcall(f))
	//	)LUA")())
	//	{
	//	}

	//	SAVESTACK
	//}

	return 0;
}



