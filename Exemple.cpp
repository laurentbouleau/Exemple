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
#include <list>
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

void rechercher_saison()
{
    //std::list<std::wstring> racines = { L"e:\\Séries.[]\\+++++\\" };
    //std::vector<std::wstring> racines = {
    std::vector<std::filesystem::path> racines
    {
        L"h:\\Séries.[]\\",
        L"h:\\Séries.[]\\-\\",
        L"h:\\Séries.[]\\+\\",
        L"h:\\Séries.[]\\++\\",
        L"h:\\Séries.[]\\+++\\",
        L"h:\\Séries.[]\\+++\\",
        L"h:\\Séries.[]\\+++++\\"
    };
    while (racines.size() != 0)
    {
        auto racine = racines[0];
        auto nomDossier = racine.wstring();
        //assert(nomDossier.length() > 0 && L"Nom de dossier vide");
        std::wcout << L"nomDossier=[" << nomDossier << L"]" << std::endl;;
        //            for(auto& sous_dossier : fs::directory_iterator(nomDossier))
        for (const auto& sous_dossier : nomDossier)
        {
            /*if (!nomDossier.empty())
            {
                std::wcout << L"Ok --->" << sous_dossier << L", " << nomDossier << std::endl;
                //if (racine.filename().wstring() == L"Only Murders in the Building.[2021- Disney+]")
//                    if(sous_dossier.filename().wstring() == L"Only Murders in the Building.[2021- Disney+]")
                    if (nomDossier == L"Only Murders in the Building.[2021- Disney+]")
                        std::wcout << L"bla : " << sous_dossier << std::endl;

                }
                else
                    std::wcout << L"pas ok ! --->" << sous_dossier << L", " << nomDossier << std::endl;
                    */

            if (nomDossier == L"Only Murders in the Building.[2021- Disney+]")
                std::wcout << L"bla : " << sous_dossier << std::endl;


            //std::wcout << L"{" << sous_dossier << L"}" << std::endl;
            //if(L"#" == sous_dossier)
        //    if (sous_dossier.is_regular_file())
 //           if (nomDossier.is_regular_file())
            /* {
                std::wcout << L"    ---> [" << sous_dossier << L']' << std::endl;
                std::wcout << L"    ---> [" << nomDossier << L']' << std::endl;
            }*/
        }
        /*if (fs::is_directory(nomDossier))
        {
            std::wcout << L"    ===> [" << nomDossier << L']' << std::endl;
            std::wcout << L"    ===> [" << racine << L']' << std::endl;
            for (const auto& entry_saison : fs::directory_iterator(nomDossier))
            {
                if (nomDossier == L"Only Murders in the Building.[2021- Disney+]")
                    std::wcout << L"Only Murders in the Building.[2021- Disney+]" << std::endl;

            }
            if (nomDossier == L"Only Murders in the Building.[2021- Disney+]")
                std::wcout << L"Only Murders in the Building.[2021- Disney+]" << std::endl;
        }*/
        std::wcout << L"===> ok !!! (" << racine << L")" << std::endl;
        racines.erase(racines.begin());
    }

}

/*int wmain(int argc, wchar_t* argv[])
{
    init();

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

    AffichagePersonnalisation person = getCurrentAffichagePersonnalisation();//fonction à implémenter pour récupérer la configuration de personnalisation de l'utilisateur.

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

    std::list<std::wstring> racines{ t };

    while (racines.size() != 0)
    {
        auto racine = *racines.begin();

        for (const auto& entry : fs::directory_iterator(racine))
        {

            if (entry.is_directory())
            {
                std::wcout << L"racine=[" << entry << L']' << std::endl;
                fs::path r = entry;
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
                    static Film film(entry);
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
                    static Serie serie(entry);
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

                //ajouter ici tout le code nécessaire à vos cas particuliers
                if (!found)racines.push_back(entry.path())
                    ;
            }
        }
        racines.pop_front();
    }


    std::wcout << L"\r\n";
    std::wcout << L"Ok !!!" << L"\r\n";
    return EXIT_SUCCESS;
}*/

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
    
    AffichagePersonnalisation person = getCurrentAffichagePersonnalisation();//fonction à implémenter pour récupérer la configuration de personnalisation de l'utilisateur.

    //std::list<std::wstring> racines{
    const std::vector<std::wstring> racines{
                                        L"Azertyuiop - qsdfghjklm.[2021-2023 Netflix].Manga",
                                        L"Azertyuiop £££.[2024].Mini-série",

                                        L"Kaleidoscope.[2023 Netflix]",
                                        L"Seven Deadly Sins- Four Knights of the Apocalypse.[2023].Manga",
                                        L"Dr. STONE.[2019- Crunchyroll].Manga",
                                        L"Andor.[2022- Disney+]",
                                        L"Star Wars Rebels.[2014-2017 Disney+].Animation",
                                        L"Star Wars- The Clone wars.[2008-2020 Disney+].Animation",
                                        L"Shogun.[2024- Disney+]",
                                        L"Love, Death + Robots.[2019- Netflix].Animation",

                                        L"Avatar 2 - la voie de l'eau.(2022-12-14)",
                                        L"Kingsglaive- Final Fantasy XV.(2016-08-30 VOD).Animation",
                                        L"Prey.(2022-08-05 Disney+)",
                                        L"Sous la Seine.(2024-06-05 Netflix)",
                                        L"Star Wars - Episode VII - Le réveil de la force.(2015-12-16)",
                                        L"Star Wars- The Clone Wars.(2008-08-27).Animation",
                                        L"Saison.txt"
    };
    //auto racine = racines[1];
    auto racine = racines[2];
    //auto racine = racines[4];
    //auto racine = racines[6];
    //auto racine = racines[9]; 
    //auto racine = racines[10];
    //auto racine = racines[16];
    std::wcout << L"racine=[" << racine << L']' << std::endl;
    fs::path r = t + racine;
    auto nomDossier = r.filename().wstring();
    assert(nomDossier.length() > 0 && L"Nom de dossier vide");

    bool found = false;
    if (L"Saison.txt" == racine)
    {
        found = true;
        rechercher_saison();
    }

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

    std::wcout << L"===> Ok !!!" << L"\r\n";
    return EXIT_SUCCESS;
}
