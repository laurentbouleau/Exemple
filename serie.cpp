#include "pch.h"

#include "Exemple.h"
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
#include <sstream>
//#include <tuple>
#include <regex>

#include <filesystem> // C++17 standard header file name
//#include <experimental/filesystem> // Header file for pre-standard implementation
//using namespace std::experimental::filesystem::v1;
using namespace std;
namespace fs = std::filesystem;

extern std::wstring replace_all(std::wstring subject, const std::wstring& search, const std::wstring& replace);

extern const std::vector<std::wstring> lire_fichierTxt(std::wstring const& nomFichier, std::vector<std::wstring> separeteurs);
extern const std::vector<std::wstring> lire_fichierTxt(std::wstring const& nomFichier, std::vector<std::wstring> separeteurs, bool found);
extern const std::vector<std::pair<std::wstring, std::wstring>>lire_paireCleValeur_depuisFichierTxt(std::wstring const& nomFichier, std::wstring separeteur);
extern const std::wstring lire_fichierTxt(std::wstring const& nomFichier);

extern void test_date_tire(wchar_t d);
extern void test_sp_et_npos_ou_pas_isblank(wchar_t sp, bool t);
extern bool checkyear(int y);
extern bool checkmonth(int m);
extern bool checkday(int m, int d, int y);

extern const void afficher_Avec(fs::path const& cheminFichier, std::vector<std::pair<std::wstring, std::wstring>>& avec);
extern const void afficher_Image(fs::path const& cheminFichier, std::vector<std::wstring>& images);

//Episode::Episode(void)
//{}

//Episode::~Episode()
//{}

Saison::Saison()
{}

Saison::~Saison()
{}

Serie::Serie(std::filesystem::path racine)
{
    this->racine = racine;
}

Serie::~Serie()
{}

/*const void PrintDate_ou_Dates(const std::vector<std::pair<std::vector<DateRecord>, std::wstring>>& dates)
{
    bool affichage_Date_ou_Dates = true;
    if (affichage_Date_ou_Dates && dates.size() > 0)
    {
        std::vector<std::wstring>keyColor{ L"\x1b[94;1m", L"\x1b[38;2;0;255;0m" }; // keyColor[0] (bleu) et keyColor[1] (vert)
        std::wstring valuesColor = L"\x1b[38;2;255;255;255m"; // Blanc
        std::size_t taille, taille2;
        wchar_t date_string[15];
        taille = std::size(dates);
        std::wstring wstr;
        for (int i = 0; i < taille; i++)
        {
            taille2 = std::size(dates[i].first);
            if (taille2 == 1)
            {
                wcsftime(date_string, 15, L"%d/%m/%Y", &dates[i].first[0].date);
                wstr = date_string;
                wstr = wstr.substr(0, 2) + keyColor[1] + L'/' + valuesColor + wstr.substr(3, 2) + keyColor[1] + L'/' + valuesColor + wstr.substr(6, 4);
                if(dates[i].second != L"")
                    wstr += keyColor[1] + L" : " + valuesColor + dates[i].second;
                if (dates[i].first[0].someFlag)
                    wstr += keyColor[1] + L" (" + valuesColor + L"préquel ou pas !" + keyColor[1] + L')' + valuesColor;
                std::wcout << wstr << std::endl;
            }
            else
            {
                int j;
                wstr = L"";
                std::wstring wstr2;
                std::size_t pos = 0;
                std::vector<wstring>k(taille2);
                std::tm temp{ 0 };
                int temp2 = 1;
                for (j = 0; j < taille2; j++)
                {
                    if (dates[i].first[j].date.tm_year == temp.tm_year && dates[i].first[j].date.tm_mon == temp.tm_mon && dates[i].first[j].date.tm_mday == temp.tm_mday)
                        // dates[i].first[j].date == temp : Marche pas !!!
                    {
                        k[j] = keyColor[1] + L'(' + valuesColor + std::to_wstring(temp2 + 1) + keyColor[1] + L')' + valuesColor;
                        if (temp2 == 1)
                            k[j - 1] += L' ' + keyColor[1] + L'(' + valuesColor + std::to_wstring(temp2) + keyColor[1] + L')' + valuesColor;
                        temp2++;
                    }
                    else
                    {
                        wcsftime(date_string, 15, L"%d/%m/%Y", &dates[i].first[j].date);
                        wstr2 = date_string;
                        k[j] = wstr2.substr(0, 2) + keyColor[1] + L'/' + valuesColor + wstr2.substr(3, 2) + keyColor[1] + L'/' + valuesColor + wstr2.substr(6, 4);
                        temp.tm_year = dates[i].first[j].date.tm_year;
                        temp.tm_mon = dates[i].first[j].date.tm_mon;
                        temp.tm_mday = dates[i].first[j].date.tm_mday;
                        temp2 = 1;
                    }

                }
                wstr2 = L"";
                for (j = 1; j < taille2 - 1; j++)
                {
                    pos = k[j].find(L"/");
                    if (pos != string::npos)
                        k[j] = keyColor[1] + L", " + valuesColor + k[j];
                }
                k.back() = L" et " + k.back();
                for (j = 0; j < taille2; j++)
                    wstr2 += k[j];
                wstr += wstr2;
                wstr += L' ' + keyColor[1] + L'[' + valuesColor + L"pas-à-pas" + keyColor[1] + L']' + valuesColor;
                if (dates[i].first.back().someFlag == true)
                    wstr += keyColor[1] + L" (" + valuesColor + L"préquel" + keyColor[1] + L')' + valuesColor;
                std::wcout << wstr << std::endl;
            }
        }
    }
}*/

/*
Episode::Episode(fs::path const& m_cheminFichier)
//{ m_cheminFichier = cheminFichier; };
{
    auto nomFichier = m_cheminFichier.filename().wstring();
    std::size_t pos = 0;
    auto strRestant = nomFichier.substr(pos);
    pos = strRestant.length();
    strRestant = strRestant.substr(0, pos - 4);
    assert((strRestant[0] != L'0') && L"Pas de zero !!!");
    try
    {
        if (!(
            (std::isdigit(strRestant[0]) && strRestant[1] == L'x')
            ||
            (std::isdigit(strRestant[0]) && std::isdigit(strRestant[1]) && strRestant[2] == L'x')
            ||
            (std::isdigit(strRestant[0]) && std::isdigit(strRestant[1]) && std::isdigit(strRestant[2]) && strRestant[3] == L'x')
            ))
        {
        }
    }
    catch (std::runtime_error const& exception)
    {
        std::cout << "Erreur : " << exception.what() << std::endl;
    }
    saison = std::stoi(strRestant, &pos);
    pos++;
    strRestant = strRestant.substr(pos);
    episode = stoi(strRestant.substr(0, pos + 1));


    pos = strRestant.find(L'.');
    strRestant = strRestant.substr(pos);
    assert((strRestant[0] == L'.') && L"Pas de . (poing) !!!");
    strRestant = strRestant.substr(1);
    assert((strRestant.length() > 9) && L"Nom de fichier trop court pour avoir au moins une date");
    std::vector<DateRecord> dr;
    std::wstring streaming = L"";

    wchar_t sp = L' ', tiret = L'-', tiret_bas = L'_';
    int y, m, d;

    int firstYear = 0, firstMon = 0, firstDay = 0;
    int i = 0;
    do
    {
        if (strRestant[0] == sp)
        {
            if (strRestant[1] == std::wstring::npos || strRestant[1] == sp)
            {
                // try cath !!!
                // Explique-moi ??? 
                // ou : 
                exit(1);
            }
            strRestant = strRestant.substr(1);
            // try cath !!!
            // Explique-moi ??? 
            // ou : 
            try // Erreur !!!
            {
                test_sp_et_npos_ou_pas_isblank(strRestant[0], isblank(strRestant[1]));
            }
            catch (exception_test_sp_et_npos_ou_pas_isblank e)
            {
                exit(1);
            }
            if (strRestant.length() > 0)
            {
                streaming = strRestant;
            }
            strRestant = L"";
            break;
        }
        if (!isdigit(strRestant[0]))
        {
            // try cath !!!
            // Explique-moi ??? 
            // ou : 
            exit(1);
        }
        // year + mon + mday
        if ((y = stoi(strRestant.substr(0, 4))) && checkyear(y)
            &&
            strRestant[4] == tiret
            &&
            (m = std::stoi(strRestant.substr(5, 2))) && checkmonth(m)
            &&
            strRestant[7] == tiret
            &&
            (d = std::stoi(strRestant.substr(8, 2))) && checkday(m, d, y)
            &&
            firstYear < y)
        {
            assert(firstYear < y && L"L'année aaaaa");
            firstYear = y;
            assert(firstMon < m && L"Le mois aaaaa");
            firstMon = m;
            assert(firstDay <= d && L"Le jours aaaaa");
            firstDay = d;
            dr.push_back(DateRecord{ 0 });
            dr[i].date.tm_year = y - 1900;
            dr[i].date.tm_mon = m - 1;
            dr[i].date.tm_mday = d;
            strRestant = strRestant.substr(10);
            if (strRestant[0] == tiret_bas)
            {
                dr[i].someFlag = true;
                strRestant = strRestant.substr(1);
            }
            i++;
            continue;
        }
        // mon + mday
        if ((m = std::stoi(strRestant.substr(0, 2))) && checkmonth(m)
            &&
            strRestant[2] == tiret
            &&
            (d = std::stoi(strRestant.substr(3, 2))) && checkday(m, d, firstYear)
            &&
            firstMon < m)
        {
            assert(firstMon < m && L"Le mois aaaaa");
            firstMon = m;
            dr.push_back(DateRecord{ 0 });
            dr[i].date.tm_year = firstYear - 1900;
            dr[i].date.tm_mon = m - 1;
            dr[i].date.tm_mday = d;
            assert(firstDay <= d && L"Le jours aaaaa");
            firstDay = d;
            strRestant = strRestant.substr(5);
            if (strRestant[0] == tiret_bas)
            {
                dr[i].someFlag = true;
                strRestant = strRestant.substr(1);
            }
            i++;
            continue;
        }
        // mday
        if ((d = std::stoi(strRestant.substr(0, 2))) && checkday(firstMon, d, firstYear)
            &&
            firstDay <= d)
        {
            firstDay = d;
            assert(firstDay <= d && L"Le jours aaaaa");
            dr.push_back(DateRecord{ 0 });
            dr[i].date.tm_year = firstYear - 1900;
            dr[i].date.tm_mon = firstMon - 1;
            dr[i].date.tm_mday = d;
            strRestant = strRestant.substr(2);
            if (strRestant[0] == tiret_bas)
            {
                dr[i].someFlag = true;
                strRestant = strRestant.substr(1);
            }
            i++;
            continue;
        }
        // try cath !!!
        // Explique-moi ???
        // ou : 
        exit(1);
    } while (strRestant.length() > 0);

    bool found;
    std::vector<std::wstring> t = lire_fichierTxt(m_cheminFichier.wstring(), { L"\n" }, false);
    pos = 0;
    if (t[0] == L"")
    {
        found = false;
        return;
    }
    pos = t[0].find(L". ");
    if (pos == std::wstring::npos || t[0][3] == L'.')
    {
        saison = 0;
        episode = 0;
    }
    else
    {
        unsigned int x = std::stoi(t[0], &pos);
        t[0] = t[0].substr(pos + 2);
    }
    bool found2 = false;
    pos = t[0].find(L" - ");
    if (pos != std::wstring::npos && !found2)
    {
        titre = t[0].substr(0, pos);
        deux_points = L" : ";
        sous_titre = t[0].substr(pos + 3);
        found2 = true;
    }
    pos = t[0].find(L" : ");
    if (pos != std::wstring::npos && !found2)
    {
        titre = t[0].substr(0, pos);
        deux_points = L" : ";
        sous_titre = t[0].substr(pos + 3);
        found2 = true;
    }
    pos = t[0].find(L": ");
    if (pos != std::wstring::npos && !found2)
    {
        titre = t[0].substr(0, pos);
        deux_points = L": ";
        sous_titre = t[0].substr(pos + 2);
        found2 = true;
    }
    pos = t[0].find(L'/');
    if (pos != std::wstring::npos && !found2)
    {
        titre = t[0].substr(0, pos);
        deux_points = L"/";
        sous_titre = t[0].substr(pos + 1);
        found2 = true;
    }
    if (pos == std::wstring::npos && !found2)
    {
        titre = t[0];
        deux_points = L"";
        sous_titre = L"";
        found2 = true;
    }
    bool temps = afficher_Temps(t[1]);
    pos = 0;
    tm.tm_min = std::stoi(t[1], &pos);
    phrases = L"";
    for (auto j = 2; j < t.size(); j++)
    {
        phrases += t[j];
    }
    found = true;
}
*/

Episode::Episode(fs::path const& cheminFichier)
{
    // ([[:digit:]]+)x([[:digit:]]{1,3})\\.(((([[:digit:]]{4})-([[:digit:]]{2})-([[:digit:]]{2})|([[:digit:]]{2})-([[:digit:]]{2})|([[:digit:]]{2})))(_?))+)(\\s(.+))?
    const std::wstring numero_saison_format = L"([[:digit:]]{1,2})"; // saison
    const std::wstring sep_numero_saison = L"x"; // x
    const std::wstring numero_episode_format = L"([[:digit:]]{1,3})"; // episode
    const std::wstring sep_episode_saison = L"\\."; //.

    const std::wstring date_year_month_day_format = L"([[:digit:]]{4})-([[:digit:]]{2})-([[:digit:]]{2})";
    const std::wstring date_month_day_format = L"([[:digit:]]{2})-([[:digit:]]{2})";
    const std::wstring date_day_format = L"([[:digit:]]{2})";
    const std::wstring stream_format = L"(\\s(.+))?";
    const std::wstring dates_format = L"((" + date_year_month_day_format + L"|" + date_month_day_format + L"|" + date_day_format + L")(_?))";

    const int dates_full_match_index = 0;
    const int dates_date_year_month_day_year_index = dates_full_match_index + 3;
    const int dates_date_year_month_day_month_index = dates_date_year_month_day_year_index + 1;
    const int dates_date_year_month_day_day_index = dates_date_year_month_day_month_index + 1;
    const int dates_date_month_day_month_index = dates_date_year_month_day_day_index + 1;
    const int dates_date_month_day_day_index = dates_date_month_day_month_index + 1;
    const int dates_date_day_day_index = dates_date_month_day_day_index + 1;
    const int dates_fucking_someFlag_index = dates_date_day_day_index + 2;

    const std::wregex filename_format_rg{ numero_saison_format + sep_numero_saison + numero_episode_format + sep_episode_saison + L"(" + dates_format + L"+)" + stream_format };

    const int filename_full_match_index = 0;
    const int filename_numero_saison_index = filename_full_match_index + 1;
    const int filename_numero_episode_index = filename_numero_saison_index + 1;
    const int filename_dates_index = filename_numero_episode_index + 1;
    const int filename_date_year_month_day_year_index = filename_dates_index + 2;
    const int filename_date_year_month_day_month_index = filename_date_year_month_day_year_index + 1;
    const int filename_date_year_month_day_day_index = filename_date_year_month_day_month_index + 1;
    const int filename_date_month_day_month_index = filename_date_year_month_day_day_index + 1;
    const int filename_date_month_day_day_index = filename_date_month_day_month_index + 1;
    const int filename_date_day_day_index = filename_date_month_day_day_index + 1;
    const int filename_fucking_someFlag_index = filename_date_day_day_index + 2;
    const int filename_stream_index = filename_fucking_someFlag_index + 2;


    auto nomFichier = cheminFichier.filename().wstring();
 
    assert(nomFichier.length() > 0 && L"Nom de fichier Episode vide");

    auto stem = cheminFichier.stem().wstring();
    // prefixe ???
    //assert((stem.length() > (9 + std::to_wstring(prefixe).length() + sep_numero_saison.length())) && L"Nom de fichier Episode trop court pour avoir au moins une date");
    assert((stem.length() > 9) && L"Nom de fichier Episode trop court pour avoir au moins une date");

    assert(std::isdigit(stem[0]) && L"Nom de fichier Episode ne commençant pas par un nombre");
    saison = std::stoi(stem);
    assert((saison <= 1000) && L"x <= 1000 !!!");
    assert((stem.find(L"x", 0) != std::wstring::npos) && L"Saison::afficher_Episode() :  x !!!");
    //assert(((fucking_x >= prefixe)) && L"saison.first != x"); // prefixe ???
    assert(std::regex_match(stem, filename_format_rg) && L"Le nom du fichier n'est pas valide");

    //std::vector<DateRecord> dates_de_diffusion;
    //std::wstring streaming = L"";

    std::wsmatch match;
    auto str = stem;
    //Exemple assez complexe de nom de fichier
    //str = L"1x01.2024-02-01_2024-02-02_02-03_0405 Netflix";
    std::regex_match(str, match, filename_format_rg);

    std::wsmatch dates_match;
    auto dates_str = match[filename_dates_index].str();
    while (std::regex_search(dates_str, dates_match, std::wregex{ dates_format }))
    {
        if (dates_match[dates_date_year_month_day_year_index].matched)
        {
            auto year = std::stoi(dates_match[dates_date_year_month_day_year_index]);
            auto month = std::stoi(dates_match[dates_date_year_month_day_month_index]);
            auto day = std::stoi(dates_match[dates_date_year_month_day_day_index]);

            assert(checkyear(year));
            assert(checkmonth(month));
            assert(checkday(month, day, year));

            DateRecord dr{ {0,0,0,day,month - 1,year - 1900} };

            dates_de_diffusion.emplace_back(dr);
        }
        else if (dates_match[dates_date_month_day_month_index].matched)
        {
            assert(dates_de_diffusion.size() > 0 && L"Utilisation d'un format mois-jour sans avoir d'année déduite.");

            auto month = std::stoi(dates_match[dates_date_month_day_month_index]);
            auto day = std::stoi(dates_match[dates_date_month_day_day_index]);

            auto lastDateRecord = dates_de_diffusion.back();
            auto last_year = lastDateRecord.date.tm_year + 1900;

            assert(checkmonth(month));
            assert(checkday(month, day, last_year));

            DateRecord dr{ {0,0,0,day,month - 1,last_year - 1900} };

            dates_de_diffusion.emplace_back(dr);
        }
        else if (dates_match[dates_date_day_day_index].matched)
        {
            assert(dates_de_diffusion.size() > 0 && L"Utilisation d'un format jour sans avoir de mois et d'années déduits.");

            auto day = std::stoi(dates_match[dates_date_day_day_index]);

            auto lastDateRecord = dates_de_diffusion.back();
            auto last_year = lastDateRecord.date.tm_year + 1900;
            auto last_month = lastDateRecord.date.tm_mon + 1;

            assert(checkday(last_month, day, last_year));

            DateRecord dr{ {0,0,0,day,last_month - 1,last_year - 1900} };

            dates_de_diffusion.emplace_back(dr);
        }
        else
        {
            assert(true && L"format de date d'épisode inconnu.");
        }

        if (dates_match[dates_fucking_someFlag_index].matched)
        {
            dates_de_diffusion.back().someFlag = true;
        }

        dates_str = dates_match.suffix().str();
    }

    if (match[filename_stream_index].matched)
    {
        streaming = match[filename_stream_index];
    }

    episode = std::stoi(match[filename_numero_episode_index]);
    bool found;
    std::vector<std::wstring> t = lire_fichierTxt(cheminFichier.wstring(), {L"\n"}, false);
    size_t pos = 0;
    if (t[0] == L"")
    {
        found = false;
        return;
    }
 
    pos = t[0].find(L". ");
    if (pos == std::wstring::npos || t[0][3] == L'.')
    {
        //saison = 0;
        episode = 0;
    }
    else
    {
        episode = std::stoi(t[0]);
        t[0] = t[0].substr(pos + 2);
    }
    // ben non !!!
    // episode = std::stoi(t[0]);

    bool found2 = false;
    pos = t[0].find(L" - ");
    if (pos != std::wstring::npos && !found2)
    {
        titre = t[0].substr(0, pos);
        deux_points = L" : ";
        sous_titre = t[0].substr(pos + 3);
        found2 = true;
    }
    pos = t[0].find(L" : ");
    if (pos != std::wstring::npos && !found2)
    {
        titre = t[0].substr(0, pos);
        deux_points = L" : ";
        sous_titre = t[0].substr(pos + 3);
        found2 = true;
    }
    pos = t[0].find(L": ");
    if (pos != std::wstring::npos && !found2)
    {
        titre = t[0].substr(0, pos);
        deux_points = L": ";
        sous_titre = t[0].substr(pos + 2);
        found2 = true;
    }
    pos = t[0].find(L'/');
    if (pos != std::wstring::npos && !found2)
    {
        titre = t[0].substr(0, pos);
        deux_points = L"/";
        sous_titre = t[0].substr(pos + 1);
        found2 = true;
    }
    if (pos == std::wstring::npos && !found2)
    {
        titre = t[0];
        deux_points = L"";
        sous_titre = L"";
        found2 = true;
    }
    //bool temps = afficher_Temps(t[1]);
    //pos = 0;
    //tm.tm_min = std::stoi(t[1], &pos);

    //afficher_temps_min(t[1]);
    initialiser_duree(t[1]);
    phrases = L"";
    for (auto j = 2; j < t.size(); j++)
        phrases += t[j];
    found = true;
    //system("PAUSE");
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Episode::afficher()                                                                                                                           #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Episode::afficher()
{
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Episode::initialiser_duree(std::wstring& m)                                                                                                   #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Episode::initialiser_duree(std::wstring& m)
{
    const std::wregex duree_format_rg{ L"([[:digit:]]+)\\s?(min|MIN|Min)" };

    std::wsmatch match;

    if (std::regex_match(m, match, duree_format_rg))
    {
        auto duree_en_minute = std::stoi(match[1]);
        duree_en_seconde = duree_en_minute * 60;
    }
    else
    {
        throw std::invalid_argument("'" + std::string{ m.begin(),m.end() } + "' n'est pas un format de durée valide.");
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Episode:Print()                                                                                                                               #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Episode::Print()
{
    if (affichage_Print_actif)
    {
        std::wstring wstr;
        bool chiffre_et_point_ou_pas = Print_Titre_chiffre_et_point_ou_pas(episode);
        if (chiffre_et_point_ou_pas)
        {
            wstr = std::to_wstring(saison);
            wstr += keyColor[1] + L'x' + valuesColor;
            wstr += std::to_wstring(episode);
            wstr += keyColor[1] + L" : " + valuesColor;
        }
        wstr += keyColor[1] + titre + valuesColor;
        if (deux_points != L"")
            wstr += deux_points + keyColor[1] + sous_titre + valuesColor;
        wstr += keyColor[1] + L" (" + valuesColor + std::to_wstring(duree_en_seconde/60) + keyColor[1] + min + L')' + valuesColor;
        wstr += keyColor[1] + L" : " + valuesColor + Print_Date_ou_Dates(dates_de_diffusion);
        std::wcout << wstr << std::endl;
        // phrases
        if (titre != L"")
            std::wcout << phrases << std::endl;
        else
            ;

    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::wstring Episode::Print_Date_ou_Dates(std::vector<DateRecord>& dates_de_diffusion)                                                             #
// #                                                                                                                                                    #
// ######################################################################################################################################################

std::wstring Episode::Print_Date_ou_Dates(std::vector<DateRecord>& dates_de_diffusion)
{
    if (affichage_Date_ou_dates && dates_de_diffusion.size() > 0)
    //    std::tm date{ 0 };
    //    bool someFlag{ false };
    {
        std::wstring date_ou_dates_wstr = L"";
        if (dates_de_diffusion.size() > 0)
        {
            std::vector<std::wstring> v_wstr;
            //for (const auto& y : x)
            for (auto i = 0; i < dates_de_diffusion.size(); i++)
            {
                std::time_t t = std::mktime(&dates_de_diffusion[i].date);
                std::tm local = *std::localtime(&t);
                std::wstringstream target_stream;
                target_stream << std::put_time(&local, L"%d/%m/%Y");
                std::wstring date = target_stream.str();
                //date = date.substr(0, 2) + L'/' + date.substr(3, 2) + L'/' + date.substr(6, 4);
                //date = date.substr(0, 2) + keyColor[1] + L'/' + valuesColor + date.substr(3, 2) + keyColor[1] + L'/' + valuesColor + date.substr(6, 4);
                v_wstr.push_back(date);
                date = L"";
                target_stream.str() = L"";
            }
            if (v_wstr.size() == 1)
            {
                date_ou_dates_wstr = v_wstr[0].substr(0, 2) + keyColor[1] + L'/' + valuesColor + v_wstr[0].substr(3, 2) + keyColor[1] + L'/' + valuesColor + v_wstr[0].substr(6, 4);
                if(dates_de_diffusion[0].someFlag)
                    date_ou_dates_wstr += keyColor[1] + L" (" + valuesColor + L"préquel ou pas !" + keyColor[1] + L')' + valuesColor;
                if (streaming != L"")
                    date_ou_dates_wstr += keyColor[1] + L" : " + valuesColor + streaming;
                return date_ou_dates_wstr;
            }
            //system("PAUSE");
            std::wstring date2 = v_wstr[0];
            int j = 1;
            for (size_t i = 0; i < v_wstr.size(); i++)
            {
                if (i > 0 && date2 == v_wstr[i])
                {
                    if (v_wstr[i - 1] == v_wstr[i])
                    {
                        v_wstr[i - 1] += L"(1)";
                        v_wstr[i] = L"(2)";
                        j += 2;
                    }
                    else
                    {
                        v_wstr[i] = L'(' + std::to_wstring(j) + L')';
                        j++;
                    }
                }
                else
                {
                    j = 1;
                    date2 = v_wstr[i];
                }
            }
            bool first = true;
            for (auto i = 0; i < v_wstr.size(); i++)
                //for (auto& v : v_wstr)
            {
                if (!first && v_wstr[i][0] != L'(')
                    date_ou_dates_wstr += L", ";
                date_ou_dates_wstr += v_wstr[i];
                first = false;
            }

            //bool last = false;
            if (dates_de_diffusion.back().someFlag)
            {
                date_ou_dates_wstr += keyColor[1] + L" (" + valuesColor + L"préquel" + keyColor[1] + L')' + valuesColor;
            }
            size_t pos;
            if (pos = date_ou_dates_wstr.find(L" et "))
                date_ou_dates_wstr = replace_all(date_ou_dates_wstr, L" et ", keyColor[1] + L" et " + valuesColor);
            if (pos = date_ou_dates_wstr.find(L")("))
                date_ou_dates_wstr = replace_all(date_ou_dates_wstr, L")(", keyColor[1] + L")(" + valuesColor);
            if (pos = date_ou_dates_wstr.find(L"("))
                date_ou_dates_wstr = replace_all(date_ou_dates_wstr, L"(", keyColor[1] + L"(" + valuesColor);
            if (pos = date_ou_dates_wstr.find(L")"))
                date_ou_dates_wstr = replace_all(date_ou_dates_wstr, L")", keyColor[1] + L")" + valuesColor);
            if (pos = date_ou_dates_wstr.find(L", "))
                date_ou_dates_wstr = replace_all(date_ou_dates_wstr, L", ", keyColor[1] + L", " + valuesColor);
            date_ou_dates_wstr += L' ' + keyColor[1] + L'[' + valuesColor + L"pas-à-pas" + keyColor[1] + L']' + valuesColor;
            if (streaming != L"")
                date_ou_dates_wstr += keyColor[1] + L" : " + valuesColor + streaming;

            return date_ou_dates_wstr;
        }
    }
    return L"";
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # bool Episode::Print_Titre_chiffre_et_point_ou_pas(unsigned short int epi                                                                           #
// #                                                                                                                                                    #
// ######################################################################################################################################################

bool Episode::Print_Titre_chiffre_et_point_ou_pas(unsigned short int episode)
{
    if (episode == 0)
        return false;
    return true;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::afficher_Dossier(fs::path const& cheminFichier)                                                                                       #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::afficher_Dossier(fs::path const& cheminDossier)
{
    auto nomDossier = cheminDossier.filename().wstring();
    assert(nomDossier.length() > 0 && L"Nom de dossier vide");
    assert(nomDossier.length() > 9 && L"Nom de fichier trop court pour avoir au moins une date");
    std::size_t pos = 0;
    std::wstring wstr = nomDossier.substr(pos);

    wchar_t sp = L' ', tiret = L'-';
    //int y;
    auto y = std::stoi(wstr, &pos);
    assert(1582 <= y && L"L'année aaaaa");
    wstr = wstr.substr(4);
    try
    {
        test_date_tire(wstr[0]);
    }
    catch (exception_date_tiret e2)
    {
        std::wcout << L"Exception a été capturée : " << e2.get_message() << std::endl;
        exit(1);
    }
    wstr = wstr.substr(1);
    auto m = std::stoi(wstr, &pos);
    assert((1 <= m && m <= 12) && L"Le mois aaaaa");
    wstr = wstr.substr(2);
    try
    {
        test_date_tire(wstr[0]);
    }
    catch (exception_date_tiret e2)
    {
        std::wcout << L"Exception a été capturée : " << e2.get_message() << std::endl;
        exit(1);
    }
    wstr = wstr.substr(1);
    auto d = std::stoi(wstr, &pos);
    assert((1 <= d && d <= 31) && L"Le jour aaaaa");
    if (!checkday(m, d, y))
    {
        std::wcout << L"Le jour aaaa !!!" << std::endl;
        exit(1);
    }
    std::tm tm;
    tm.tm_year = y - 1900;
    tm.tm_mon = m - 1;
    tm.tm_mday = d;
    wstr = wstr.substr(2);
    dossier.first = tm;
    dossier.second = wstr;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::afficher_Fichier(fs::path const& cheminFichier)                                                                                       #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::afficher_Fichier(fs::path const& cheminFichier)
{
    auto nomFichier = cheminFichier.filename().wstring();
    auto nomImage = cheminFichier.extension().wstring();
    if (nomImage == L".txt")
    {
        if (nomFichier == L"_you_.txt")
        {
            return;
        }
        if (!(std::isdigit(nomFichier[0])))
        {
            // Avec
            if (nomFichier == L"Avec.txt")
            {
                afficher_Avec(cheminFichier, avec);
                return;
            }
            // Chaîne d'origine -
            if (nomFichier == L"Chaîne d'origine.txt")
            {
                //afficher_Chaine(filename, nomFichier, d_chaine[I]);
                return;
            }
            // DVD
            if (nomFichier == L"DVD.txt")
            {
                //D_DVD[I] = true;
                return;
            }
            // Netflix
            if (nomFichier == L"Netflix.txt")
            {
                afficher_Netflix(cheminFichier);
                return;
            }
            // Note
            if (nomFichier == L"Note.txt")
            {
                afficher_Note(cheminFichier);
                return;
            }
            // Titre
            if (nomFichier == L"Titre.txt")
            {
                afficher_Titre(cheminFichier);
                return;
            }
        }
        //
        if (
            (nomFichier[0] == L'1' || nomFichier[0] == L'2' || nomFichier[0] == L'3' || nomFichier[0] == L'4' || nomFichier[0] == L'5' || nomFichier[0] == L'6' || nomFichier[0] == L'7' || nomFichier[0] == L'8' || nomFichier[0] == L'9')
            && nomFichier[1] == L'x'
            )
        {
            Creer_Episode(cheminFichier);
            return;
        }
        if (
            (nomFichier[0] == L'1' || nomFichier[0] == L'2' || nomFichier[0] == L'3' || nomFichier[0] == L'4' || nomFichier[0] == L'5' || nomFichier[0] == L'6' || nomFichier[0] == L'7' || nomFichier[0] == L'8' || nomFichier[0] == L'9')
            && (std::isdigit(nomFichier[1]))
            && nomFichier[2] == L'x'
            )
        {
            Creer_Episode(cheminFichier);
            return;
        }
        if (int j = std::stoi(nomFichier))
        {
            afficher(cheminFichier);
            return;
        }
        // Erreur !
        if (nomFichier != L"")
        {
            //E.afficher_X(-1, nomFichier, L'{' + t + L".txt} !!!");
            return;// EXIT_FAILURE;
        }
    }
    else if(nomImage == L".jpg" || nomImage == L".png" || nomImage == L".webp")
        // Image
    {
        afficher_Image(cheminFichier, image);
        return;
    }
    else
    {
    }
    return;
 }

// ######################################################################################################################################################
// ######################################################################################################################################################

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::afficher(fs::path const& cheminFichier)                                                                                               #
// #                                                                                                                                                    #
// ######################################################################################################################################################

 void Saison::afficher(fs::path const& cheminFichier)
 {
     auto nomFichier = cheminFichier.filename().wstring();
     assert(nomFichier.length() > 0 && L"Nom de fichier vide");
     std::size_t pos;
     saison.first = std::stoi(nomFichier, &pos);
     try
     {
         saison.second = lire_fichierTxt(cheminFichier.wstring());
     }
     catch (runtime_error const& exception)
     {
         std::wcout << L"Erreur : " << exception.what() << std::endl;
     }
     assert((saison.second.size() != 0));
 }

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::Creer_Episode(fs::path const& cheminFichier)                                                                                          #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::Creer_Episode(fs::path const& cheminFichier)
{
    auto nomFichier = cheminFichier.filename().wstring();

    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    Episode episode(cheminFichier);
    episodes.push_back(episode); // Episode
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::afficher_Netflix(fs::path const& cheminFichier)                                                                                       #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::afficher_Netflix(fs::path const& cheminFichier)
{
    auto nomFichier = cheminFichier.filename().wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    netflix = true;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::afficher_Note(fs::path const& cheminFichier)                                                                                          #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::afficher_Note(fs::path const& cheminFichier)
{ // 0...5 ou -1
    auto nomFichier = cheminFichier.filename().wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");

    //assert((n_filename == createdBy_filename) && L"Erreur !!! Note... !");
    std::wstring n = lire_fichierTxt(cheminFichier.wstring());
    std::size_t pos = n.length();
    if (n == L"")
    {
        note = -1.0;
        return;
    }
    if (pos != 1 && pos != 3)
    {
        //E.afficher_X(-1, n, L"Pas de [a-z] et/ou [0-9] et/ou etc…");
        return;
    }
    if (!iswdigit(n[0]))
    {
        //E.afficher_X(-1, n, L"Pas de [a-z] et/ou etc…");
        return;
    }
    if (!(n[0] == L'0' || n[0] == L'1' || n[0] == L'2' || n[0] == L'3' || n[0] == L'4' || n[0] == L'5'))
    {
        //E.afficher_X(-1, n, L"Pas de [0-5]");
        return;
    }
    if (pos == 1)
    {
        note = std::stod(n);
    }
    else
    {
        if ((n[1] == L',' || n[1] == L'.') && iswdigit(n[2]) && pos == 3)
        {
            //
            n[1] = L',';
            //
            // Ok !!!
            note = std::stod(n);
        }
        else
        {
            //E.afficher_X(-1, n, L"Pas de…");
        }
    }
    return;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::afficher_Titre(fs::path const& cheminFichier)                                                                                         #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::afficher_Titre(std::filesystem::path const& cheminFichier)
{
    auto nomFichier = cheminFichier.filename().wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    titre = lire_fichierTxt(cheminFichier.wstring());
    assert((titre.size() != 0));
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::Print()                                                                                                                               #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::Print()
{
    //Print_Date_etc();
    wchar_t date_string[15];
    wcsftime(date_string, 15, L"%d/%m/%Y", &dossier.first);
    std::wstring wstr;
    wstr = date_string;
    wstr = wstr.substr(0, 2) + keyColor[1] + L'/' + valuesColor + wstr.substr(3, 2) + keyColor[1] + L'/' + valuesColor + wstr.substr(6, 4);
    if (dossier.second != L"")
        wstr += keyColor[0] + dossier.second + valuesColor + L' ';
    if (titre != L"")
        wstr += keyColor[1] + L" : " + valuesColor + keyColor[0] + titre + valuesColor;
    wstr += keyColor[1] + L" : " + valuesColor;
    wstr += saison.second;
    wstr += L' ' + keyColor[1] + L'(' + valuesColor + std::to_wstring(saison.first) + keyColor[1] + L')' + valuesColor;
    std::wcout << wstr << std::endl;

    std::size_t taille;
    taille = std::size(episodes);
    for (auto i = 0; i < taille; i++)
    {
        episodes[i].Print();
    }
    // Netflix
    Print_Netflix();
    // Avec
    Print_Avec();
    // Images(s)
    Print_Images();
    // Saison ok !
    std::wcout << std::endl;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::Print_Avec()                                                                                                                          #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::Print_Avec()
{
    if (affichage_Avec_actif && avec.size())
    {
        std::wstring avec_str = keyColor[1] + L"Avec : " + valuesColor;
        bool found = false;
        for (auto&& [nom, role] : avec)
        {
            if (nom == L"…" || nom == L"..." || nom == L".")
            {
                found = true;
                break;
            }
            if (nom != L"" && role != L"")
            {
                avec_str += nom + L' ' + keyColor[1] + L'(' + valuesColor + role + keyColor[1] + L')' + valuesColor;
            }
            else if (nom == L"" && role != L"")
            {
                avec_str += keyColor[1] + L'(' + valuesColor + role + keyColor[1] + L')' + valuesColor;
            }
            else
            {
                avec_str += nom;
            }
            if (avec.back().first != nom)
                avec_str += keyColor[1] + L", " + valuesColor;
            else
                avec_str += keyColor[1] + L'.' + valuesColor;
        }
        if (found)
            avec_str += L"...";
        std::wcout << avec_str << std::endl;
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::Print_Date_etc()                                                                                                                      #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const void Saison::Print_Date_etc()
{
    if (affichage_Date_etc_actif)
    {
        wchar_t date_string[15];
        //std::wstring date_string;
        wcsftime(date_string, 15, L"%d/%m/%Y", &dossier.first);
        std::wstring wstr;
        wstr = date_string;
        wstr = wstr.substr(0, 2) + keyColor[1] + L'/' + valuesColor + wstr.substr(3, 2) + keyColor[1] + L'/' + valuesColor + wstr.substr(6, 4);
        if (dossier.second != L"")
            wstr += keyColor[0] + dossier.second + valuesColor + L' ';
        if (titre != L"")
            wstr += keyColor[1] + L" : " + valuesColor + keyColor[0] + titre + valuesColor;
        wstr += keyColor[1] + L" : " + valuesColor;
        wstr += saison.second;
        wstr += L' ' + keyColor[1] + L'(' + valuesColor + std::to_wstring(saison.first) + keyColor[1] + L')' + valuesColor;
        std::wcout << wstr << std::endl;
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::Print_Images()                                                                                                                        #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::Print_Images()
{
    if (affichage_Image_actif && image.size() > 0 /*!= 0*/)
    {
        std::wstring image_str = keyColor[1] + L"Image" + ((image.size() > 1) ? L"s" : L"") + L" : [" + valuesColor;
        bool first = true;
        for (auto&& i : image)
        {
            if (!first)
            {
                image_str += keyColor[1] + L"], [" + valuesColor;
            }
            image_str += i;
            first = false;
        }
        image_str += keyColor[1] + L']' + valuesColor + L"\r\n";
        //PrintStringW(m_hOut, image_str, x1);
        //Console_Lire(image_str, x1, 0);
        //Console_Lire(hOut, image_str, 0);// , 0);
        //Console_Lire(hOut, image_str, x1, L' ');
        std::wcout << image_str << std::endl;
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::Print_Netflix()                                                                                                                       #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::Print_Netflix()
{
    if (affichage_Netflix_actif && netflix)
    {
        std::wstring netflix_str = keyColor[1] + L'(' + valuesColor + L"Netflix" + keyColor[1] + L')' + valuesColor;
        std::wcout << netflix_str << std::endl;
    }
}

// ######################################################################################################################################################
// ######################################################################################################################################################

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Serie::Print()                                                                                                                                #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const void Serie::Print()
{
    //std::size_t pos;
    //pos = std::size(saisons);
    Print_Saisons();
}

/*void Serie::PrintAvec(const std::vector<std::pair<std::wstring, std::wstring>> avec)
{
}*/

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const void Serie::PrintEpisode_Titre(std::tuple<unsigned int, std::wstring, std::wstring, std::wstring, std::tm, std::wstring>& e_t)               #
// #                                                                                                                                                    #
// ######################################################################################################################################################

/*const void Serie::PrintEpisode_Titre(std::tuple<unsigned int, std::wstring, std::wstring, std::wstring, std::tm, std::wstring>& e_t)
{
    if (affichage_Episode_Titre_actif)
    {
        std::wstring wstr = get<5>(e_t);
        std::wcout << wstr << std::endl;
    }
}*/


// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const bool Serie::PrintEpisode_Titre_chiffre_et_point_ou_pas(std::wstring& titre)                                                                  #
// #                                                                                                                                                    #
// ######################################################################################################################################################

/*const bool Serie::PrintEpisode_Titre_chiffre_et_point_ou_pas(std::wstring& titre)
{
    if (!std::isdigit(titre[0]))
        return false;
    int i = 0;
    while (titre[i] != std::wstring::npos)
    {
        if (std::isdigit(titre[i]))
            ;
        else if (titre[i] == L'.' && i > 0)
        {
            if (titre[i + 1] != std::wstring::npos && titre[i + 1] == L' ')
                break;
            else return false;
        }
        else
            return false;
        i++;
    }
    titre = titre.substr(i + 2);
    return true;
}*/

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const void Serie::Print_Episodes(Saison saison)                                                                                                    #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const void Serie::Print_Episodes(Saison saison)
{
    for (auto ep : saison.episodes)
    {
        //ep.afficher();
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const void Serie::Print_Saison(Saison saison)                                                                                                      #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const void Serie::Print_Saison(Saison saison)
{
    if (affichage_Saison_actif)
    {
        //PrintSaison_Date_etc(saison);
        //PrintEpisodes(saison);
        
        saison.Print();

        // Avec
        //PrintAvec(saisons[i].avec);
        //PrintEpisodes(saison);
        //PrintAvec(saison.avec);

    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const void Serie::PrintSaison_Date_etc()                                                                                                           #
// #                                                                                                                                                    #
// ######################################################################################################################################################

/*const void Serie::PrintSaison_Date_etc(Saison saison)
{
    if (affichage_Saison_Date_etc_actif)
    {
        wchar_t date_string[15];
        //std::wstring date_string;
        wcsftime(date_string, 15, L"%d/%m/%Y", &saison.dossier.first);
        std::wstring wstr;
        wstr = date_string;
        wstr = wstr.substr(0, 2) + keyColor[1] + L'/' + valuesColor + wstr.substr(3, 2) + keyColor[1] + L'/' + valuesColor + wstr.substr(6, 4);
        if (saison.dossier.second != L"")
            wstr += keyColor[0] + saison.dossier.second + valuesColor + L' ';
        if (saison.titre != L"")
            wstr += keyColor[1] + L" : " + valuesColor + keyColor[0] + saison.titre + valuesColor;
        wstr += keyColor[1] + L" : " + valuesColor;
        wstr += saison.saison.second;
        wstr += L' ' + keyColor[1] + L'(' + valuesColor + std::to_wstring(saison.saison.first) + keyColor[1] + L')' + valuesColor;
        std::wcout << wstr << std::endl;
    }
}*/

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const void Serie::Print_Saisons()                                                                                                                  #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const void Serie::Print_Saisons()
{
    if (affichage_Saisons_actif)
    {
        std::size_t taille;// , taille2;
        //wchar_t date_string[15];
        taille = std::size(saisons);
        //std::wstring saison_str;
        for (int i = 0; i < taille; i++)
        {
            Print_Saison(saisons[i]);
        }
    }
}
