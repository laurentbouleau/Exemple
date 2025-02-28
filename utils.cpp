#include "pch.h"

//#include "Exemple.h"
#include "utils.h"

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

//#include <algorithm> 
#include <cctype>
//#include <locale>


#include <filesystem> // C++17 standard header file name

// Hors saison : ???
// Saison 1 : exemple : 2023/02/15
// Saison 2 : exemple : 2024/09/10
// etc...

using namespace std;
namespace fs = std::filesystem;

using DateVisionnage = DateRecord;

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::wstring replace_all(std::wstring subject, const std::wstring& search, const std::wstring& replace)                                            #
// #                                                                                                                                                    #
// ######################################################################################################################################################

std::wstring replace_all(std::wstring subject, const std::wstring& search, const std::wstring& replace)
{
    std::size_t pos = 0;
    // function find return string::npos if not found.
    while ((pos = subject.find(search, pos)) != wstring::npos)
    {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
    return subject;
}

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
// # inline std::wstring ltrim_copy(std::wstring s)                                                                                                      #
// #                                                                                                                                                    #
// ######################################################################################################################################################
// trim from start (copying)
inline std::wstring ltrim_copy(std::wstring s)
{
    ltrim(s);
    return s;
}

// trim from end (copying)
inline std::wstring rtrim_copy(std::wstring s)
{
    rtrim(s);
    return s;
}

// trim from both ends (copying)
inline std::wstring trim_copy(std::wstring s) {
    trim(s);
    return s;
}




// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::string wstr_to_u8(std::wstring uneWString)                                                                                                    #
// #                                                                                                                                                    #
// ######################################################################################################################################################

std::string wstr_to_u8(std::wstring uneWString)
{
    return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(uneWString);
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const std::vector<std::pair<std::wstring, std::wstring>>lire_paireCleValeur_depuisFichierTxt(std::wstring const& nomFichier)                       #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const std::vector<std::pair<std::wstring, std::wstring>>lire_paireCleValeur_depuisFichierTxt(std::wstring const& nomFichier)
{
    std::string contenuFichier{ u8"" };
    std::vector<std::pair<std::wstring, std::wstring>> clevaleurs;

    ifstream fichier{ nomFichier };
    if (!fichier)
    {
        throw std::runtime_error("Fichier impossible à ouvrir.");
    }

    contenuFichier = std::string(istreambuf_iterator<char>{fichier}, {});

    if (contenuFichier == u8"")
    {
        throw std::runtime_error("Le fichier '" + wstr_to_u8(nomFichier) + "' est vide.");
    }

    wstring_convert<codecvt_utf8<wchar_t>, wchar_t> convertiseur;
    std::wstring converti = convertiseur.from_bytes(contenuFichier);
    rtrim(converti);

    std::wregex line_format_rg{ L"^(?:(?:(.+?) : (.+))|(?:(.+?) : )|(?: : (.+))|(\\.\\.\\.|…)|(.+))$" };
    const auto etc = L"…";
    const auto empty_string = L"";
    for (std::wsregex_iterator it{ converti.begin(), converti.end(), line_format_rg }, end{}; it != end; ++it) 
    {
        auto entry = *it;
        if (entry[5].matched)
        {
            clevaleurs.push_back(std::make_pair(etc, empty_string));
            break;
        }
        else if (entry[1].matched)
        {
            std::wstring nom = entry[1].str();
            std::wstring role = entry[2].str();
            clevaleurs.push_back(std::make_pair(nom, role));
        }
        else if (entry[3].matched)
        {
            std::wstring nom = entry[3].str();
            clevaleurs.push_back(std::make_pair(nom, empty_string));
        }
        else if (entry[4].matched)
        {
            std::wstring role = entry[4].str();
            clevaleurs.push_back(std::make_pair(empty_string, role));
        }
        else if (entry[6].matched)
        {
            std::wstring nom = entry[6].str();
            clevaleurs.push_back(std::make_pair(nom, empty_string));
        }
    }
    return clevaleurs;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const std::vector<std::pair<std::wstring, std::wstring>>lire_paireCleValeur_depuisFichierTxt(std::wstring const& nomFichier,                       #
// #                                                                                              std::wstring separeteur_pattern)                      #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const std::vector<std::pair<std::wstring, std::wstring>>lire_paireCleValeur_depuisFichierTxt(std::wstring const& nomFichier, std::wstring separeteur_pattern)
{
    std::string contenuFichier{ u8"" };
    std::vector<std::pair<std::wstring, std::wstring>> clevaleurs;

    ifstream fichier{ nomFichier };
    if (!fichier)
    {
        throw std::runtime_error("Fichier impossible à ouvrir.");
    }

    contenuFichier = std::string(istreambuf_iterator<char>{fichier}, {});

    if (contenuFichier == u8"")
    {
        throw std::runtime_error("Le fichier '" + wstr_to_u8(nomFichier) + "' est vide.");
    }

    wstring_convert<codecvt_utf8<wchar_t>, wchar_t> convertiseur;
    std::wstring converti = convertiseur.from_bytes(contenuFichier);
    rtrim(converti);
    converti += L'\n';

    std::size_t pos = converti.length();

    if (pos == std::wstring::npos)
        return clevaleurs;

    const std::wregex line_format_rg{ L"^(.+?)?((" + separeteur_pattern + L")(.*))|(…|\\.\\.\\.)|(.+)$" };

    for (std::wsregex_iterator it{ converti.begin(), converti.end(), line_format_rg }, end{}; it != end; ++it) {
        auto entry = *it;
        if (entry[5].matched)
        {
            clevaleurs.push_back(std::make_pair(L"…", L""));
        }
        else if (entry[1].matched)
        {
            if (entry[4].matched)
            {
                clevaleurs.push_back(std::make_pair(entry[1].str(), entry[4].str()));
            }
            else
            {
                clevaleurs.push_back(std::make_pair(entry[1].str(), L""));
            }
        }
        else
        {
            if (entry[4].matched)
            {
                clevaleurs.push_back(std::make_pair(L"", entry[4].str()));
            }
            else
            {
                if (entry[6].matched)
                {
                    clevaleurs.push_back(std::make_pair(entry[6].str(), L""));
                }
            }
        }
    }

    return clevaleurs;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const std::vector<std::wstring> lire_fichierTxt(std::wstring const& nomFichier, std::vector<std::wstring> separeteurs)                             #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const std::vector<std::wstring> lire_fichierTxt(std::wstring const& nomFichier, std::vector<std::wstring> separeteurs)
{
    std::string contenuFichier{ u8"" };
    std::vector<std::wstring> retVal{};

    ifstream fichier{ nomFichier };
    if (!fichier)
    {
        throw std::runtime_error("Fichier impossible à ouvrir.");
    }

    contenuFichier = std::string(istreambuf_iterator<char>{fichier}, {});

    if (contenuFichier == u8"")
    {
        throw std::runtime_error("Le fichier '" + wstr_to_u8(nomFichier) + "' est vide.");
    }

    wstring_convert<codecvt_utf8<wchar_t>, wchar_t> convertiseur;
    std::wstring converti = convertiseur.from_bytes(contenuFichier);
    rtrim(converti);

    bool found = false;

    do
    {
        found = false;
        size_t pos_found = std::string::npos;
        std::wstring sep_found = L"";

        for (auto&& sep : separeteurs)
        {
            std::size_t pos = converti.find(sep);
            if (pos != std::wstring::npos && (!found || pos_found > pos))
            {
                pos_found = pos;
                found = true;
                sep_found = sep;
            }
        }

        if (found)
        {
            retVal.push_back(converti.substr(0, pos_found));
            converti = converti.substr(pos_found + sep_found.length());
        }
    } while (found);

    if (converti.length() > 0)
    {
        rtrim(converti);
        retVal.push_back(converti);
    }
    return retVal;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const std::vector<std::wstring> lire_fichierTxt(std::wstring const& nomFichier, std::vector<std::wstring> separeteurs, bool f)                     #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const std::vector<std::wstring> lire_fichierTxt(std::wstring const& nomFichier, std::vector<std::wstring> separeteurs, bool f)
{
    std::string contenuFichier{ u8"" };
    //std::string ligneCourante{ u8"" };
    std::vector<std::wstring> retVal{};

    ifstream fichier{ nomFichier };
    if (!fichier)
    {
        throw std::runtime_error("Fichier impossible à ouvrir.");
    }

    contenuFichier = std::string(istreambuf_iterator<char>{fichier}, {});

    if (contenuFichier == u8"")
    {
//        throw std::runtime_error("Le fichier '" + wstr_to_u8(nomFichier) + "' est vide.");
        return { L"" };
    }

    wstring_convert<codecvt_utf8<wchar_t>, wchar_t> convertiseur;
    std::wstring converti = convertiseur.from_bytes(contenuFichier);
    rtrim(converti);

    bool found = false;

    do
    {
        found = false;
        size_t pos_found = std::string::npos;
        std::wstring sep_found = L"";

        for (auto&& sep : separeteurs)
        {
            std::size_t pos = converti.find(sep);
            if (pos != std::wstring::npos && (!found || pos_found > pos))
            {
                pos_found = pos;
                found = true;
                sep_found = sep;
            }
        }

        if (found)
        {
            retVal.push_back(converti.substr(0, pos_found));
            converti = converti.substr(pos_found + sep_found.length());
        }
    } while (found);

    if (converti.length() > 0)
        retVal.push_back(converti);
    return retVal;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const std::wstring lire_fichierTxt(std::wstring const& nomFichier)                                                                                 #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const std::wstring lire_fichierTxt(std::wstring const& nomFichier)
{
    std::string contenuFichier{ u8"" };
    std::ifstream fichier{ nomFichier };

    contenuFichier = std::string(istreambuf_iterator<char>{fichier}, {});

    std::wstring_convert<codecvt_utf8<wchar_t>, wchar_t> convertiseur;
    std::wstring converti = convertiseur.from_bytes(contenuFichier);
    rtrim(converti);
    return converti;
}


// ######################################################################################################################################################
// #                                                                                                                                                    #
// # inline std::wstring space(int i, wchar_t espace)                                                                                                   #
// #                                                                                                                                                    #
// ######################################################################################################################################################

inline std::wstring space(int i, wchar_t espace)
{
    std::wstring wstr = L"";
    //for (auto ===> Pas trouvé !!!)
    for (int j = 0; j < i; j++)
        wstr += espace;
    return wstr;
}

/*void Console_Lire(HANDLE hOut, const std::wstring& wstr, int taille_indentation, wchar_t caractere_indentation = L' ')
{
    DWORD numberOfBytesWritten = 0;

    std::wstring indentation(taille_indentation, caractere_indentation);

    BOOL res;

    if (taille_indentation > 0)
    {
        res = WriteFile(hOut, &indentation[0], (DWORD)(indentation.size() * sizeof(wchar_t)), &numberOfBytesWritten, NULL);
        assert(res == TRUE);
    }

    res = WriteFile(hOut, &wstr[0], (DWORD)(wstr.size() * sizeof(wchar_t)), &numberOfBytesWritten, NULL);
    assert(res == TRUE);
}*/

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void test_date_tire(wchar_t d)                                                                                                                     #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void test_date_tire(wchar_t d)
{
    if (d != L'-')
    {
        throw exception_date_tiret();
    }
    return;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void test_date_tiret_sp_etc(wchar_t d)                                                                                                             #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void test_date_tiret_sp_etc(wchar_t d)
{
    if (d != L'-' && d != L'/' && d != L'.' && d != L' ')
    {
        throw exception_date_tiret_sp_etc();
    }
    return;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void test_sp_et_npos_ou_pas_isblank(wchar_t sp, bool t)                                                                                            #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void test_sp_et_npos_ou_pas_isblank(wchar_t sp, bool t)
{
    if (sp != L' ' && t)
    {
        throw exception_test_sp_et_npos_ou_pas_isblank();
    }
    return;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # bool checkyear(int y)                                                                                                                              #
// #                                                                                                                                                    #
// ######################################################################################################################################################

bool checkyear(int y)
{
    return 1582 <= y;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # bool checkmonth(int m)                                                                                                                             #
// #                                                                                                                                                    #
// ######################################################################################################################################################

bool checkmonth(int m)
{
    return (1 <= m && m <= 12);
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # bool checkday(int m, int d, int y)                                                                                                                 #
// #                                                                                                                                                    #
// ######################################################################################################################################################

bool checkday(int m, int d, int y)
{
    bool retVal = false;

    if (checkyear(y))
    {
        if (checkmonth(m))
        {
            if (d > 0)
            {
                if (d <= 28)
                {
                    retVal = true;
                }
                else if (d == 29)
                {
                    if (m != 2)
                    {
                        retVal = true;
                    }
                    else
                    {
                        retVal = (y % 4 == 0) && ((y % 100 != 0) || (y % 400 == 0));
                    }
                }
                else if (d == 30)
                {
                    retVal = m != 2;
                }
                else if (d == 31)
                {
                    retVal = (m != 2 && m != 4 && m != 6 && m != 9 && m != 11);
                }
            }
        }
    }

    return retVal;
}
