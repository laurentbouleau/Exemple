#pragma once

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include "pch.h"
//#include "film_serie.h"
#include "film.h"
#include "serie.h"

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
//#include <stdio.h>
#include <locale>
#include <algorithm>
#include <codecvt>
#include <tuple>
#include <optional>
#include <filesystem> // C++17 standard header file name

struct Person
{
    std::wstring m_h = L"h";
    std::wstring m_min = L"min";
    //                                    (bleu)         (vert)                 (rouge)                (jaune)
    std::vector<std::wstring> m_keyColor{ L"\x1b[94;1m", L"\x1b[38;2;0;255;0m", L"\x1b[38;2;255;0;0m", L"\x1b[38;2;255;255;0m"};
    std::wstring m_valuesColor = L"\x1b[38;2;255;255;255m"; // Blanc
};