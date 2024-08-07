//#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
//#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING 1


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
//#include <stdio.h>
#include <locale>
#include <algorithm>
#include <codecvt>
#//include <tuple>
#include <optional>

#include <filesystem> // C++17 standard header file name

using namespace std;
namespace fs = std::filesystem;

HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
DWORD dwMode = 0;
CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
int X;

using DateVisionnage = DateRecord;

extern const std::vector<std::wstring> Audiodescription;
extern const std::vector<std::wstring> Genre;
extern const std::vector<std::wstring> Nationalite;
extern const std::vector<std::wstring> Sous_Genre;

extern std::wstring replace_all(std::wstring subject, const std::wstring& search, const std::wstring& replace);

extern const std::vector<std::wstring> lire_fichierTxt(std::wstring const& nomFichier, std::vector<std::wstring> separeteurs);
extern const std::vector<std::wstring> lire_fichierTxt(std::wstring const& nomFichier, std::vector<std::wstring> separeteurs, bool found);
extern const std::vector<std::pair<std::wstring, std::wstring>>lire_paireCleValeur_depuisFichierTxt(std::wstring const& nomFichier, std::wstring separeteur);
//extern const std::wstring lire_et_decouper_fichierTxt(std::wstring const& nomFichier);
extern const std::wstring lire_fichierTxt(std::wstring const& nomFichier);

extern void test_date_tire(wchar_t d);
extern void test_sp_et_npos_ou_pas_isblank(wchar_t sp, bool t);
extern bool checkyear(int y);
extern bool checkmonth(int m);
extern bool checkday(int m, int d, int y);


void init()
{
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    GetConsoleScreenBufferInfo(hOut, &csbiInfo);
    _wsetlocale(LC_ALL, L"fr");
    //std::locale("fr_FR.utf8");
    X = csbiInfo.srWindow.Right + 1;
    //
}

int wmain(int argc, wchar_t* argv[])
{

    //int i = 0;
    //
    init();
    //hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
    {
        return GetLastError();
    }
    //    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode))
    {
        return GetLastError();
    }
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode))
    {
        return GetLastError();
    }

    //    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    GetConsoleScreenBufferInfo(hOut, &csbiInfo);

    std::wstring t;
    const wchar_t* p{ L".\\Exemple.txt" };
    std::filesystem::path filePath(p);
    try
    {
        t = lire_fichierTxt(filePath);
    }
    catch (runtime_error const& exception)
    {
        std::wcout << L"Erreur : " << exception.what() << std::endl;
    }
    
    //const std::wstring racine = t + L"Kaleidoscope.[2023 Netflix]";
    //const std::wstring racine = t + L"Azertyuiop - qsdfghjklm.[2021-2023 Netflix].Manga";
    //const std::wstring racine = t + L"SupraCell.[2024 Netflix].Mini-série";
    //const std::wstring racine = t + L"Azertyuiop £££.[2024].Mini-série";
    //const std::wstring racine = t + L"Star Wars- The Clone wars.[2008-2020].Animation";
    const std::wstring racine = t + L"Star Wars- The Clone Wars.(2008-08-27).Animation";
    //const std::wstring racine = t + L"Seven Deadly Sins- Four Knights of the Apocalypse.[2023].Manga";
    //const std::wstring racine = t + L"Avatar 2 - la voie de l'eau.(2022-12-14)";
    //const std::wstring racine = t + L"Prey.(2022-08-05 Disney+)";
    //const std::wstring racine = t + L"La Stratégie Ender.(2013-11-06)";
    std::wcout << L"racine=[" << racine << L']' << std::endl;

    fs::path r = racine;
    std::wcout << r.filename() << std::endl;;
    auto nomDossier = r.filename().wstring();
    assert(nomDossier.length() > 0 && L"Nom de dossier vide");

    bool found = false;

    /*std::wregex film_pattern{L"(.+?)(?:\\.\\((\\d{4}\\-\\d{2}\\-\\d{2}\\s)?([^\\)]*)\\))?(?:\\.(.+))?"};
    std::wsmatch match;
    if (!found && std::regex_match(nomDossier, match, film_pattern))
    {
        Film film(racine);
        found = true;
        for (const auto& entry_film : fs::directory_iterator((film.getRacine())))
        {
            if (entry_film.is_regular_file())
            {
                film.initialiser_Fichier(entry_film);
            }
        }
        film.Print();
    }*/
    std::size_t pos, pos2;
    const std::wstring d_p1 = L".(";
    pos = nomDossier.find(d_p1);
    const std::wstring d_p2 = L")";
    pos2 = nomDossier.find(d_p2);

    if (!found && pos > 2 && pos2 > (pos + 4 + 1 + 2 + 1 + 2) && pos != std::wstring::npos && pos2 != std::wstring::npos)
    //std::wregex nomDossier_pattern{ L"(.+?)(\\s\\:\\s|\\:\\s|/|\\s\\-\\s|\\-\\s)(.+)" };
    //    std::wregex nomDossier_pattern{ L"(.+?)(?:\\.\\((\\d{4}\\-\\d{2}\\-\\d{2}\\s)?([^\\)]*)\\))?(?:\\.(.+))?" };
    //https://regex101.com/r/hu1CUo/1
    //(.+?)(?:\.\((\d{4}\-\d{2}\-\d{2}\s)?((^\))*)\))?(?:\.(.+))?
//    std::wregex nomDossier_pattern{ L"(.+?)(?:\\.\\(\\d{4}\\-\\d{2}\\-\\d{2}\\s)?((^\\))*)\\))?(?:\\.(.+))?" };
    //Avatar 2 - la voie de l'eau.(2022-12-14)
    //Prey.(2022 - 08 - 05 Disney + )
    //Star Wars - The Clone Wars.(2008 - 08 - 27).Animation
//    std::wsmatch match;
//    if (!found && std::regex_match(nomDossier, match, nomDossier_pattern))
    {
        Film film(racine);
        found = true;
        for (const auto& entry_film : fs::directory_iterator((film.getRacine())))
        {
            if (entry_film.is_regular_file())
            {
                film.initialiser_Fichier(entry_film);
            }
        }
        film.Print();
    }

    /*if (!found)
    {
        std::wregex serie_pattern{ L"(.+?)(?:\\.\\[(\\d{4}\\-\\d{4}\\s|\\d{4}\\-\\d{4}|\\d{4}\\-\\s|\\d{4}\\s|\\d{4})?([^\\]]*)\\])?(?:\\.(.+))?" };
        std::wsmatch match2;
        if (!found && std::regex_match(nomDossier, match2, serie_pattern))
        {
            Serie serie(racine);
            found = true;
            for (const auto& entry_serie : fs::directory_iterator((serie.getRacine())))
            {
                if (entry_serie.is_regular_file())
                {
                    serie.initialiser_Fichier(entry_serie);
                    continue;
                }
                if (fs::is_directory(entry_serie))
                {
                    Saison saison(entry_serie, serie);
                    for (const auto& entry_saison : fs::directory_iterator(entry_serie))
                    {
                        if (entry_saison.is_regular_file())
                        {
                            saison.initialiser_Fichier(entry_saison);
                        }
                    }
                    serie.saisons.push_back(saison);
                }
            }
            serie.Print();
        }
    }*/
    const std::wstring d_p3 = L".[";
    pos = nomDossier.find(d_p3);
    const std::wstring d_p4 = L"]";
    pos2 = nomDossier.find(d_p4);
    if (!found && pos > 2 && pos2 > (pos + 4) && pos != std::wstring::npos && pos2 != std::wstring::npos)
    {
        Serie serie(racine);
        found = true;
        for (const auto& entry_serie : fs::directory_iterator((serie.getRacine())))
        {
            if (entry_serie.is_regular_file())
            {
                serie.initialiser_Fichier(entry_serie);
                continue;
            }
            if (fs::is_directory(entry_serie))
            {
                Saison saison(entry_serie, serie);
                for (const auto& entry_saison : fs::directory_iterator(entry_serie))
                {
                    if (entry_saison.is_regular_file())
                    {
                        saison.initialiser_Fichier(entry_saison);
                    }
                }
                serie.saisons.push_back(saison);
            }
        }
        serie.Print();
    }

    std::wcout << L"\r\n";
    std::wcout << L"Ok !!!" << L"\r\n";
    return EXIT_SUCCESS;
}
