#include "pch.h"

#include "utils.h"
#include "Exemple.h"
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

// Hors saison : ???
// Saison 1 : exemple : 2023/02/15
// Saison 2 : exemple : 2024/09/10
// etc...

using namespace std;
namespace fs = std::filesystem;

using DateVisionnage = DateRecord;

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

static inline void rtrim(std::wstring& s)
{
    s.erase(find_if(s.rbegin(), s.rend(), [](int ch)
        {
            return !isspace(ch);
        }).base(), s.end());
}

std::string wstr_to_u8(std::wstring uneWString)
{
    return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(uneWString);
}

const std::vector<std::pair<std::wstring, std::wstring>>lire_paireCleValeur_depuisFichierTxt(std::wstring const& nomFichier, std::wstring separeteur)
{
    std::string contenuFichier{ u8"" };
    //std::string ligneCourante{ u8"" };
    std::vector<std::pair<std::wstring, std::wstring>> clevaleurs;

    ifstream fichier{ nomFichier };
    if (!fichier)
    {
        throw std::runtime_error("Fichier impossible à ouvrir.");
    }

    /*while (getline(fichier, ligneCourante, u8'\n'))
    {
        contenuFichier += ligneCourante + u8'\n';
    }*/
    contenuFichier = std::string(istreambuf_iterator<char>{fichier}, {});

    if (contenuFichier == u8"")
    {
        throw std::runtime_error("Le fichier '" + wstr_to_u8(nomFichier) + "' est vide.");
    }

    wstring_convert<codecvt_utf8<wchar_t>, wchar_t> convertiseur;
    std::wstring converti = convertiseur.from_bytes(contenuFichier);
    rtrim(converti);
    converti += L"\r\n";

    std::size_t pos = converti.length();

    if (pos == std::wstring::npos)
        return clevaleurs;

    std::size_t pos2 = 0;
    //while (pos = converti.find(L"\r\n"))
    while (pos = converti.find(L"\n"))
    {
        if (converti[0] != converti.length())
        {
            pos2 = converti.find(L"...");
            if (pos2 != std::wstring::npos && converti[0] == L'.' && converti[1] == L'.' && converti[2] == L'.')
            {
                clevaleurs.push_back(std::make_pair(L"…", L""));
                return clevaleurs;
            }
            pos2 = converti.find(L"…");
            if (pos2 != std::wstring::npos && converti[0] == L'…')
            {
                clevaleurs.push_back(std::make_pair(L"…", L""));
                return clevaleurs;
            }
            pos2 = converti.find(L".");
            if (pos2 != std::wstring::npos && converti[0] == L'…')
            {
                clevaleurs.push_back(std::make_pair(L"…", L""));
                return clevaleurs;
            }
            pos2 = converti.find(separeteur);
            if (pos2 == std::wstring::npos && pos != std::wstring::npos)
            {
                clevaleurs.push_back(std::make_pair(converti.substr(0, pos - 1), L""));
            }
            else if (pos2 == 0)
            {
                clevaleurs.push_back(std::make_pair(L"", converti.substr(3, (pos - 1) - 2)));
            }
            else if (pos > pos2)
            {
                clevaleurs.push_back(std::make_pair(converti.substr(0, pos2), converti.substr(pos2 + 3, pos - (pos2 + 3))));
            }
            else
            {
                break;
            }
            converti = converti.substr(pos + 1);
        }
        else
            break;
    }
    return clevaleurs;
}

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
        //throw std::runtime_error("Le fichier '" + wstr_to_u8(nomFichier) + "' est vide.");
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

/*std::vector<std::wstring> lire_et_decouper_fichierTxt(std::wstring const& nomFichier, std::vector<std::wstring> separeteurs)
{
    //...
    std::string contenuFichier{ u8"" };

    auto contenu_fichier = lire_fichierTxt(nomFichier);

    std::vector<std::wstring> retVal{};

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
}*/


/*std::wstring lire_fichierTxt(std::wstring const& nomFichier)
{
    return lire_fichierTxt(nomFichier, {})[0];
}*/

inline std::wstring space(int i, wchar_t espace)
{
    std::wstring wstr = L"";
    //for (auto ===> Pas trouvé !!!)
    for (int j = 0; j < i; j++)
        wstr += espace;
    return wstr;
}

void Console_Lire(HANDLE hOut, const std::wstring& wstr, int taille_indentation, wchar_t caractere_indentation = L' ')
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
}

void test_date_tire(wchar_t d)
{
    if (d != L'-')
    {
        throw exception_date_tiret();
    }
    return;
}

void test_date_tiret_sp_etc(wchar_t d)
{
    if (d != L'-' && d != L'/' && d != L'.' && d != L' ')
    {
        throw exception_date_tiret_sp_etc();
    }
    return;
}

void test_sp_et_npos_ou_pas_isblank(wchar_t sp, bool t)
{
    if (sp != L' ' && t)
    {
        throw exception_test_sp_et_npos_ou_pas_isblank();
    }
    return;
}

bool checkyear(int y)
{
    return 1582 <= y;
}

bool checkmonth(int m)
{
    return (1 <= m && m <= 12);
}

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
