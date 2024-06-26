#pragma once

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#define TXT_                             0
#define JGP_                             1
#define PNG_                             2
#define WEBP_                            3

#include "film.h"
#include "film_serie.h"
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
#include <filesystem> // C++17 standard header file name

inline std::wstring space(int i, wchar_t espace);
//const void Console_Lire(std::wstring wstr, int x1, int x2);

//
//void Console_Lire(HANDLE hOut, const std::wstring& wstr, int taille_indentation, wchar_t caractere_indentation = L' ');
//


class exception_date_tiret
{
    std::wstring message;
public:
    exception_date_tiret() : message(L"CCCC-BB-AA pas de...") {}
    std::wstring get_message() const { return message; }
};


class exception_test_sp_et_npos_ou_pas_isblank
{
    std::wstring message;
public:
    exception_test_sp_et_npos_ou_pas_isblank() : message(L"Espace invalide !!!") {}
    std::wstring get_message() const { return message; }
};

struct DateRecord
{
    std::tm date{ 0 };
    bool someFlag{ false };
};

