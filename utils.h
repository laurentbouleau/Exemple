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
#include <cctype>

#include <filesystem> // C++17 standard header file name

std::wstring replace_all(std::wstring subject, const std::wstring& search, const std::wstring& replace);

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # inline void ltrim(std::wstring& s)                                                                                                                 #
// #                                                                                                                                                    #
// ######################################################################################################################################################
// trim from start (in place)
inline void ltrim(std::wstring& s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](wchar_t ch) {
        return !std::isspace(ch);
        }));
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # inline void trim(std::wstring& s)                                                                                                                  #
// #                                                                                                                                                    #
// ######################################################################################################################################################
// trim from end (in place)
inline void rtrim(std::wstring& s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](wchar_t ch) {
        return !std::isspace(ch);
        }).base(), s.end());
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # inline void trim(std::wstring& s)                                                                                                                  #
// #                                                                                                                                                    #
// ######################################################################################################################################################
// trim from both ends (in place)
inline void trim(std::wstring& s)
{
    rtrim(s);
    ltrim(s);
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # inline std::wstring ltrim_copy(std::wstring s)                                                                                                     #
// #                                                                                                                                                    #
// ######################################################################################################################################################
// trim from start (copying)
inline std::wstring ltrim_copy(std::wstring s)
{
    ltrim(s);
    return s;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # inline std::wstring rtrim_copy(std::wstring s)                                                                                                     #
// #                                                                                                                                                    #
// ######################################################################################################################################################
// trim from end (copying)
inline std::wstring rtrim_copy(std::wstring s)
{
    rtrim(s);
    return s;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # inline std::wstring trim_copy(std::wstring s)                                                                                                      #
// #                                                                                                                                                    #
// ######################################################################################################################################################
// trim from both ends (copying)
inline std::wstring trim_copy(std::wstring s)
{
    trim(s);
    return s;
}

// 

std::string wstr_to_u8(std::wstring uneWString);
const std::vector<std::pair<std::wstring, std::wstring>>lire_paireCleValeur_depuisFichierTxt(std::wstring const& nomFichier);
const std::vector<std::pair<std::wstring, std::wstring>>lire_paireCleValeur_depuisFichierTxt(std::wstring const& nomFichier, std::wstring separeteur_pattern);
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

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # Classes                                                                                                                                            #
// #                                                                                                                                                    #
// ######################################################################################################################################################

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

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # Structs                                                                                                                                            #
// #                                                                                                                                                    #
// ######################################################################################################################################################

struct DateRecord
{
    std::tm date{ 0 };
    bool someFlag{ false };
};

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # Template<typename ... Args>                                                                                                                        #
// #                                                                                                                                                    #
// ######################################################################################################################################################

template<typename ... Args>
std::wstring wstring_format(const std::wstring& format, Args ... args)
{
    int size_s = _snwprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
    if (size_s <= 0) { throw std::runtime_error("Error during formatting."); }
    auto size = static_cast<size_t>(size_s);
    std::unique_ptr<wchar_t[]> buf(new wchar_t[size]);
    _snwprintf(buf.get(), size, format.c_str(), args ...);
    return std::wstring(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}


