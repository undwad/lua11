/*
* * example.cpp by undwad
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
				list<string> values; //vector for table values
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
				list<string> keys; //vector for table keys
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

		Value value(&*L); //new value
		value = 123; //set value's value
		value.setGlobal("value1"); //save value to global table
		ScriptText(&*L, "print(value1)")(); //print it
		value = true; //set value's value
		value.setGlobal("value2"); //save value to global table
		ScriptText(&*L, "print(value2)")(); //print it
		print((bool)value); //print it
		value.set("some text"); //set value's value
		value.setGlobal("value3"); //save value to global table
		ScriptText(&*L, "print(value3)")(); //print it
		print(value.type(), value.typeName()); //print value type and name
		ScriptText(&*L, "value3 = 123.321")(); //change it
		value.getGlobal("value3"); //get value from global table
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

			auto __newindex = MAKECALLBACK(&*L, [](Table table, string key, Value value) //define callback __newindex metamethod
			{
				table.rawset(key, value); //raw set table value at given key
			});

			if (metatable.set("__index", __index) && metatable.set("__newindex", __newindex)) //set metatable's __index and __newindex functions
			{
				if (table.setMeta(metatable)) //set table's metatable
				{
					//and test metamethods
					ScriptText(&*L, R"LUA(
						print(table.someparam)
						table.someparam = 0123456789
						print(table.someparam)
						print(table.anotherparam)	
						table.anotherparam = function() end
						print(table.anotherparam)	
					)LUA")();
				}
			}

			if (table.getMeta(&metatable)) //get metatable
				print(metatable); //print result
		}

		SAVESTACK
	}

	//EXAMPLE 5
	//shows how to bind C++ classes to lua
	{
		struct Test //test class
		{
			Test() { cout << "Test()" << endl; } 
			Test(lua_State* l) { cout << "Test(" << (void*)l << ")" << endl; }
			Test(lua_State* l, string s) { cout << "Test(" << (void*)l << ", " << s << ")" << endl; }
			~Test() { cout << "~Test()" << endl; }
			void print(string s) { cout << "print " << s << endl; } //test function
			int add(int a, int b) //test function
			{ 
				cout << a << " + " << b << " = " << (a + b) << endl; 
				return a + b; 
			} 
			static int mul(int a, int b) //test static function
			{
				cout << a << " * " << b << " = " << (a * b) << endl;
				return a * b;
			}
			static string getstr() { return "some text"; } //test function
		};

		auto test = Class<Test>(&*L, "Test") //define lua class in C++
			.init() //define constructor "init"
			.initL("__initl") //define alternative constructor "inits"
			.initL<string>("__inits") //define alternative constructor "inits"
			.set("print", &Test::print) //define function
			.set("add", &Test::add) //define function
			.set("mul", &Test::mul) //define static function
			.set("getstr", &Test::getstr) //define static function
			.set("typeinfo", "test c++ type"); //define static field
		if (!test) //check for error
			print(test.error()); //print error

		{
			ScriptText s(&*L, R"LUA(
			print(Test, Test.type, Test.typeinfo) --print type info
			print(Test:mul(100,6)) --test static function
			print(Test:getstr()) --test static function
			require 'class' --import class module
			Test = class(Test) --define class Test descendant of defined in C++ Test
			function Test:__init() --constructor of new Test class
				Test.__base.__init(self) --call base constrcuctor
				--Test.__base.__initl(self) --call base constrcuctor
				--Test.__base.__inits(self, 'some text') --call base constrcuctor
				self.a = 'A' --initialize field
			end
			function Test:__gc() --define destructor
				Test.base.__gc(self) --callbase destructor
			end
			function Test:print2(s) --define function
				print('print2', s)
			end
			t = Test() --create instance
			print(t, t.__ptr, t.__type, t:__is(Test)) --print instance info
			t:print2(123) --test lua function
			t:print(123) --test c++ function
			print(t:add(123, 321)) --test c++ function
			print(t.a) --print field
		)LUA");
			s();
			cout << s.error << endl;
		}

		SAVESTACK
	}

	cout << "EXIT" << endl;

	return 0;
}



