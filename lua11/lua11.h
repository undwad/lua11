/*
** lua11.h by undwad
** lua11 is a very lightweight binding lua with C++11
** https://github.com/undwad/lua11 mailto:undwad@mail.ru
** see copyright notice at the end of this file
*/

#ifndef _LUA11_H__
#define _LUA11_H__

#define _VARIADIC_MAX 10

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <tuple>
#include <memory>
#include <vector>
#include <list>
#include <map>

using namespace std;
using namespace placeholders;

#ifndef LUA_VERSION
#	include "lua/lua.hpp"
#endif

static string error;

#pragma warning(disable : 4800)
#pragma warning(disable : 4244)

#include "stdmisc.h"
#include "Debug.h"
#include "Ref.h"
#include "Stack.h"
#include "Table.h"
#include "Value.h"
#include "Function.h"
#include "Callback.h"
#include "Class.h"

#pragma warning(default : 4800)
#pragma warning(default : 4244)

#endif // _LUA11_H__

/******************************************************************************
* Copyright (C) 2013 Undwad, Samara, Russia
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
******************************************************************************/

