#pragma once
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#define _CRT_SECURE_NO_WARNINGS

#include "Exemple.h"

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
#include <optional>

#include <filesystem> // C++17 standard header file name

std::wstring replace_all(std::wstring subject, const std::wstring& search, const std::wstring& replace);
static inline void rtrim(std::wstring& s);
std::string wstr_to_u8(std::wstring uneWString);
const std::vector<std::pair<std::wstring, std::wstring>>lire_paireCleValeur_depuisFichierTxt(std::wstring const& nomFichier, std::wstring separeteur);
const std::vector<std::wstring> lire_fichierTxt(std::wstring const& nomFichier, std::vector<std::wstring> separeteurs);
const std::vector<std::wstring> lire_fichierTxt(std::wstring const& nomFichier, std::vector<std::wstring> separeteurs, bool f);
const std::wstring lire_fichierTxt(std::wstring const& nomFichier);
inline std::wstring space(int i, wchar_t espace);
//void Console_Lire(HANDLE hOut, const std::wstring& wstr, int taille_indentation, wchar_t caractere_indentation = L' ');

void test_date_tiret_sp_etc(wchar_t d);
void test_sp_et_npos_ou_pas_isblank(wchar_t sp, bool t);
void test_sp_et_npos_ou_pas_isblank(wchar_t sp, bool t);
bool checkyear(int y);
bool checkmonth(int m);
bool checkday(int m, int d, int y);

class exception_date_tiret
{
    std::wstring message;
public:
    exception_date_tiret() : message(L"CCCC-BB-AA pas de...") {}
    std::wstring get_message() const { return message; }
};

class exception_date_tiret_sp_etc
{
    std::wstring message;
public:
    exception_date_tiret_sp_etc() : message(L"bbbbbbbbbbb") {}
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

