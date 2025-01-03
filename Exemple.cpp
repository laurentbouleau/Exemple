//#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
//#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING 1


#include "pch.h"
#include "film.h"
#include "serie.h"
#include "utils.h"
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
    
    //const std::wstring racine = t + L"Azertyuiop - qsdfghjklm.[2021-2023 Netflix].Manga";
    //const std::wstring racine = t + L"Azertyuiop £££.[2024].Mini-série";
     
    //const std::wstring racine = t + L"Kaleidoscope.[2023 Netflix]";
    //const std::wstring racine = t + L"Seven Deadly Sins- Four Knights of the Apocalypse.[2023].Manga";
    //const std::wstring racine = t + L"Dr. STONE.[2019-2022 Crunchyroll].Manga";
    //const std::wstring racine = t + L"Andor.[2022- Disney+]";
//    const std::wstring racine = t + L"Star Wars Rebels.[2014-2017 Disney+].Animation";
    //const std::wstring racine = t + L"Star Wars- The Clone wars.[2008-2020 Disney+].Animation";
    //const std::wstring racine = t + L"Shogun.[2024- Disney+]"; // Japon

//    const std::wstring racine = t + L"Avatar 2 - la voie de l'eau.(2022-12-14)";
    //const std::wstring racine = t + L"Kingsglaive- Final Fantasy XV.(2016-08-30 VOD).Animation";
    //const std::wstring racine = t + L"Prey.(2022-08-05 Disney+)";
    //const std::wstring racine = t + L"Sous la Seine.(2024-06-05 Netflix)";
//    const std::wstring racine = t + L"Star Wars - Episode VII - Le réveil de la force.(2015-12-16)";
    //const std::wstring racine = t + L"Star Wars- The Clone Wars.(2008-08-27).Animation";

    AffichagePersonnalisation person = getCurrentAffichagePersonnalisation();//fonction à implémenter pour récupérer la configuration de personnalisation de l'utilisateur.

    const std::vector<std::wstring> racines{
                                        L"Azertyuiop - qsdfghjklm.[2021-2023 Netflix].Manga",
                                        L"Azertyuiop £££.[2024].Mini-série",

                                        L"Kaleidoscope.[2023 Netflix]",
                                        L"Seven Deadly Sins- Four Knights of the Apocalypse.[2023].Manga",
                                        L"Dr. STONE.[2019-2022 Crunchyroll].Manga",
                                        L"Andor.[2022- Disney+]",
                                        L"Star Wars Rebels.[2014-2017 Disney+].Animation",
                                        L"Star Wars- The Clone wars.[2008-2020 Disney+].Animation",
                                        L"Shogun.[2024- Disney+]",

                                        L"Avatar 2 - la voie de l'eau.(2022-12-14)",
                                        L"Kingsglaive- Final Fantasy XV.(2016-08-30 VOD).Animation",
                                        L"Prey.(2022-08-05 Disney+)",
                                        L"Sous la Seine.(2024-06-05 Netflix)",
                                        L"Star Wars - Episode VII - Le réveil de la force.(2015-12-16)",
                                        L"Star Wars- The Clone Wars.(2008-08-27).Animation"
    };
    //auto racine = racines[1];
    auto racine = racines[2];
    //auto racine = racines[4];
    std::wcout << L"racine=[" << racine << L']' << std::endl;
    fs::path r = t + racine;
    auto nomDossier = r.filename().wstring();
    assert(nomDossier.length() > 0 && L"Nom de dossier vide");

    bool found = false;

    std::size_t pos, pos2;
    const std::wstring d_p1 = L".(";
    pos = nomDossier.find(d_p1);
    const std::wstring d_p2 = L")";
    pos2 = nomDossier.find(d_p2);
    const std::size_t c_annee_size = 4;
    const std::size_t c_mois_size = 2;
    const std::size_t c_jour_size = 2;
    const std::size_t c_tiret_size = 1;

    std::wregex nomDossier_pattern{ L"(.+?)(?:\\.\\((?:(\\d{4})\\-(\\d{2})\\-(\\d{2})\\s*([^\\)]*))\\))?(?:\\.(.+))?$" };

    std::wsmatch match;
    if (!found && pos > 2 && pos2 > (pos + c_annee_size + c_tiret_size + c_mois_size + c_tiret_size + c_jour_size) && pos != std::wstring::npos && pos2 != std::wstring::npos &&
        std::regex_match(nomDossier, match, nomDossier_pattern))
    {
        static Film film(racine);
        found = true;
        for (const auto& entry_film : fs::directory_iterator((film.getRacine())))
        {
            if (entry_film.is_regular_file())
            {
                film.initialiser_Fichier(entry_film);
            }
        }
        film.AffichagePersonnaliser(person);
        film.Print();
    }

    const std::wstring d_p3 = L".[";
    pos = nomDossier.find(d_p3);
    const std::wstring d_p4 = L"]";
    pos2 = nomDossier.find(d_p4);
    std::wregex serie_pattern{ L"^(.+?)(?:\\.\\[(\\d{4}\\-\\d{4}\\s|\\d{4}\\-\\d{4}|\\d{4}\\-\\s|\\d{4}\\s|\\d{4})([^\\]]*)\\])(?:\\.(.+))?$|^(.+)(?:\\.(.+))$" };
    std::wsmatch match2;
    if (!found && pos > 2 && pos2 > (pos + c_annee_size) && pos != std::wstring::npos && pos2 != std::wstring::npos && std::regex_match(nomDossier, match2, nomDossier_pattern))
    {
        static Serie serie(racine);
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

                serie.saisons.push_back({ entry_serie, serie });
                auto& saison = serie.saisons.back();
                for (const auto& entry_saison : fs::directory_iterator(entry_serie))
                {
                    if (entry_saison.is_regular_file())
                    {
                        saison.initialiser_Fichier(entry_saison);
                    }
                }
            }
        }
        serie.AffichagePersonnaliser(person);
        serie.Print();
    }

    std::wcout << L"\r\n";
    std::wcout << L"Ok !!!" << L"\r\n";
    return EXIT_SUCCESS;
}
