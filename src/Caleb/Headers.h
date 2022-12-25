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
using TString = std::wstring;
#define tcout  wcout
#define tcin  wcin
#define to_tstring   to_wstring
#else
using TString = std::string;
#define tcout  cout
#define tcin  wcin
#define to_tstring   to_string
#endif;