#pragma once
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#define _CRT_SECURE_NO_WARNINGS

//#define TXT_                             0
//#define JGP_                             1
//#define PNG_                             2
//#define WEBP_                            3

#include "Exemple.h"
#include "film_serie.h"

// C :
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
// C++
#include <windows.h>
#include <cassert>
//#include <cstddef>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
//#include <stdio.h>
#include <locale>
#include <algorithm>
#include <codecvt>
#include <sstream>
//#include <tuple>
#include <regex>
#include <numeric>
#include <string_view>

#include <filesystem> // C++17 standard header file name

extern const std::vector<std::wstring> Genre;
extern const std::vector<std::wstring> Sur;
extern const std::vector<std::wstring> Sous_Genre;
extern const std::vector<std::wstring> Nationalite;

struct DateRecord;
