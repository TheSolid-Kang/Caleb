#pragma once
#include <iostream>

#include <string>

#include <array>
#include <vector>

#include <list>

#include <map>

#include <set>

#include <functional>
#include <algorithm>

#include <type_traits>

#include "Values.h"

#include "CIO.h"

#if UNICODE 
using MyString = std::wstring;
#define tcout  wcout
#define tcin  wcin
#define to_tstring   to_wstring
#else
using MyString = std::string;
#define tcout  cout
#define tcin  wcin
#define to_tstring   to_string
#endif;