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
#include <cctype>

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

std::wstring demander_nom_dossier()
{
    std::wstring nom_dossier{ L"" };
    std::wcin >> nom_dossier;
    return nom_dossier;
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
    std::vector<std::wstring> t2;

    const wchar_t* p{ L".\\Exemple.txt" };
    std::filesystem::path filePath(p);
    try
    {
        //t = lire_fichierTxt(filePath);
        t2 = lire_fichierTxt(filePath, { L"\n" });
    }
    catch (runtime_error const& exception)
    {
        std::wcout << L"Erreur : " << exception.what() << std::endl;
    }
    
    AffichagePersonnalisation person = getCurrentAffichagePersonnalisation();//fonction à implémenter pour récupérer la configuration de personnalisation de l'utilisateur.

    //std::list<std::wstring> racines{
    const std::vector<std::wstring> racines{
                                        L"/Séries.[]/++++/L/L'Attaque des Titans.[2013- Crunchyroll].Manga",
                                        L"Azertyuiop £££.[2024-].Mini-série",

                                        L"/Séries.[]/+++/K/Kaleidoscope.[2023 Netflix]",
                                        L"/Séries.[]/+++/S/Se/Seven Deadly Sins- Four Knights of the Apocalypse.[2023-].Manga",
                                        L"Dr. STONE.[2019- Crunchyroll].Manga",
                                        L"/Séries.[]/++++/A/Andor.[2022-2025 Disney+]",
                                        L"Star Wars Rebels.[2014-2017 Disney+].Animation",
                                        L"Star Wars- The Clone wars.[2008-2020 Disney+].Animation",
                                        L"Shogun.[2024- Disney+]",
                                        L"Love, Death + Robots.[2019- Netflix].Animation",
                                        L"/Séries.[]/++++/#/1883.[2021 Paramount+]",

                                        L"Avatar 2 - la voie de l'eau.(2022-12-14)",
                                        L"Kingsglaive- Final Fantasy XV.(2016-08-30 VOD).Animation",
                                        L"Prey.(2022-08-05 Disney+)",
                                        L"Ravage.(2025-04-25 Netflix)",
                                        L"Star Wars - Episode VII - Le réveil de la force.(2015-12-16)",
                                        L"Star Wars- The Clone Wars.(2008-08-27).Animation",
                                        //L"Saison.txt",
                                        //L"you"
    };
    //auto racine = racines[0];
    //auto racine = racines[1];
    //auto racine = racines[2];
    auto racine = racines[3];
    //auto racine = racines[4];
//    auto racine = racines[6];
    //auto racine = racines[9];     
    //auto racine = racines[10];

    //auto racine = racines[11];
    //auto racine = racines[16];
    //auto racine = racines.back();
    std::wcout << L"racine=[" << racine << L']' << std::endl;
    std::wstring wstr;
    fs::path r;// = L"";
    if (racine == racines.back())
    {
        //std::vector<wstring> argv_You = lire_fichierTxt(L"d:\\Users\\admin\\Documents\\You.txt", {L"\n"});

        //std::flush;
//std::cin.ignore(255, '\n');
//std::cin.clear();

//std::cin.eof()
//std::cin.fail()
//        Il existe des flux pour communiquer avec des fichiers(std::ifstream et std::ofstream), avec des chaînes de caractères(std::istringstream et std::ostringstream), avec l’entrée et la sortie standards(std::cin et std::cout).




        /*std::vector<wstring> s;// = { demander_nom_dossier() };
        std::wstring wstr;
        bool found2 = false;
        bool found_you = false;
        bool found_t = false;
        bool found_f_ou_s = false;
        bool found_dossier = false;
        std::wstring dossier = L"";
        std::wcout << "You : " << std::endl;
        while (!found2)
        {
            while (std::getline(wcin, wstr, L' '))
            {
                ;
                if (wstr.empty())
                {
                    std::wcout << L"Nom de dossier vide !!!" << std::endl;
                    std::wcout << "You : " << std::endl;
                    continue;

                }
                if (!found_you && !found_t && !found_f_ou_s && !found_dossier && (wstr == L"you" || wstr == L"You" || wstr == L"YOU")  )
                {
                    found_you = true;
                    wstr = L"";
                    continue;
                }
                if (!found_t && !found_f_ou_s && !found_dossier && (wstr[0] == L'/' || wstr[0] == L'-') && wstr[1] == L't'/)
                {
                    found_t = true;
                    wstr = L"";
                    continue;
                }
                if (!found_f_ou_s && !found_dossier && (wstr[0] == L'f' || wstr[0] == L's'))
                {
                    found_f_ou_s = true;
                    wstr = L"";
                    continue;
                }
                if (!found_dossier)// && std::is_directory(wstr))
                {
                    dossier = wstr;
                    wstr = L"";
                    
                    found_dossier = true;
                    if (found_dossier)
                        break;
                }
            }
        }

        r = dossier + L" " + std::getline(wcin, wstr, L'\n');
        */

        //std::wcin.clear();
//        std::wstring wstr;
        //std::vector<std::wstring> dossier;

        //std::vector<std::wstring> dos;

        //while (getline(std::wcin, wstr))
        //{
        //    dos.push_back(wstr);
        //}

        // Now try it with a whitespace delimiter
        /*while (std::getline(std::wcin, wstr, L' '))
        {
            dossier.push_back(wstr);
        }
        dossier.erase(dossier.begin());
        dossier.erase(dossier.begin());
        dossier.erase(dossier.begin());
        for (const auto& d : dossier)
        {
            std::wcout << d << std::endl;
        }*/

        std::getline(wcin, wstr);
        std::wcout << L"{" << wstr << L"}" << std::endl;
        if (fs::is_directory(wstr))
        {
            //faire un truc "intelligent"
            r = wstr;
        }
    }
    else
    {
        //r = t + racine;
        r = t2[0] + racine;
   }
    //r = wstr;
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
        //serie.Print();
        PrintSerieOnOutstream(serie, std::wcout);
    }

    std::wcout << L"===> Ok !!!" << L"\r\n";
    return EXIT_SUCCESS;
}
