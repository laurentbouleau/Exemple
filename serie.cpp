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

using DateVisionnage = DateRecord;

extern const std::vector<std::wstring>Audiodescription;
extern const std::vector<std::wstring>Genre;
extern const std::vector<std::wstring> Nationalite;
extern const std::vector<std::wstring> Sous_Genre;

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

extern const void initialiser_Audiodescription(fs::path const& cheminFichier, std::wstring& ad);
extern const void initialiser_Avec(fs::path const& cheminFichier, std::vector<std::pair<std::wstring, std::wstring>>& avec);
extern const void initialiser_Chaine(fs::path const& cheminFichier, std::wstring& chaine);
extern const void initialiser_Genre(fs::path const& cheminFichier, std::vector<std::wstring>& genres_renvoyes, const std::vector<std::wstring>& genres_valides);
extern const void initialiser_Image(fs::path const& cheminFichier, std::vector<std::wstring>& images);
extern const void initialiser_Nationalite(fs::path const& cheminFichier, std::vector<std::wstring>& nationalites_renvoyes, const std::vector<std::wstring>& nationalites_valides);

extern void Print_Audiodescription(const std::wstring& audiodescription, bool affichage_audiodescription_actif, std::wstring& keyColor, std::wstring& valuesColor);
extern void Print_Images(const std::vector<std::wstring>& image, bool affichage_image_actif, std::wstring& keyColor, std::wstring& valuesColor);
extern void Print_Genres(const std::vector<std::wstring>& genres, bool affichage_genres_actif, const std::wstring& sous_genre, bool affichage_sous_genre_actif, std::wstring& keyColor, std::wstring& valuesColor);
extern void Print_Nationalites(const std::vector<std::wstring>& nationalites, bool affichage_nationalite_actif, std::wstring& keyColor, std::wstring& valuesColor);

// ######################################################################################################################################################
// ######################################################################################################################################################

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # SequenceVisionnage::SequenceVisionnage(fs::path const& cheminFichier)                                                                              #
// #                                                                                                                                                    #
// ######################################################################################################################################################

SequenceVisionnage::SequenceVisionnage(fs::path const& cheminFichier)
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
    //const int dates_fucking_someFlag_index = dates_date_day_day_index + 2;
    const int dates_someFlag_index = dates_date_day_day_index + 2;

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
    //const int filename_fucking_someFlag_index = filename_date_day_day_index + 2;
    const int filename_someFlag_index = filename_date_day_day_index + 2;
    //const int filename_stream_index = filename_fucking_someFlag_index + 2;
    const int filename_stream_index = filename_someFlag_index + 2;


    auto nomFichier = cheminFichier.filename().wstring();

    assert(nomFichier.length() > 0 && L"Nom de fichier Episode vide");

    auto stem = cheminFichier.stem().wstring();
    // prefixe ???
    //assert((stem.length() > (9 + std::to_wstring(prefixe).length() + sep_numero_saison.length())) && L"Nom de fichier Episode trop court pour avoir au moins une date");
    assert((stem.length() > 9) && L"Nom de fichier Episode trop court pour avoir au moins une date");

    assert(std::isdigit(stem[0]) && L"Nom de fichier Episode ne commençant pas par un nombre");
    m_saison = std::stoi(stem);
    assert((m_saison <= 1000) && L"x <= 1000 !!!");
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

            m_dates.emplace_back(dr);
        }
        else if (dates_match[dates_date_month_day_month_index].matched)
        {
            assert(m_dates.size() > 0 && L"Utilisation d'un format mois-jour sans avoir d'année déduite.");

            auto month = std::stoi(dates_match[dates_date_month_day_month_index]);
            auto day = std::stoi(dates_match[dates_date_month_day_day_index]);

            auto lastDateRecord = m_dates.back();
            auto last_year = lastDateRecord.date.tm_year + 1900;

            assert(checkmonth(month));
            assert(checkday(month, day, last_year));

            DateRecord dr{ {0,0,0,day,month - 1,last_year - 1900} };

            m_dates.emplace_back(dr);
        }
        else if (dates_match[dates_date_day_day_index].matched)
        {
            assert(m_dates.size() > 0 && L"Utilisation d'un format jour sans avoir de mois et d'années déduits.");

            auto day = std::stoi(dates_match[dates_date_day_day_index]);

            auto lastDateRecord = m_dates.back();
            auto last_year = lastDateRecord.date.tm_year + 1900;
            auto last_month = lastDateRecord.date.tm_mon + 1;

            assert(checkday(last_month, day, last_year));

            DateRecord dr{ {0,0,0,day,last_month - 1,last_year - 1900} };

            m_dates.emplace_back(dr);
        }
        else
        {
            assert(true && L"format de date d'épisode inconnu.");
        }

        //if (dates_match[dates_fucking_someFlag_index].matched)
        if (dates_match[dates_someFlag_index].matched)
        {
            m_dates.back().someFlag = true;
        }

        dates_str = dates_match.suffix().str();
    }

    if (match[filename_stream_index].matched)
    {
        m_streaming = match[filename_stream_index];
    }

    //episode = std::stoi(match[filename_numero_episode_index]);
    std::vector<std::wstring> t = lire_fichierTxt(cheminFichier.wstring(), { L"\n" }, false);
    if (t[0] == L"")
    {
        m_fichier_pas_zero = false;
        m_numero++;
        return;
    }

    size_t pos;// = 0;
    pos = t[0].find(L". ");
    if (pos == std::wstring::npos || t[0][3] == L'.')
    {
        //saison = 0;
        m_episode = 0;
    }
    else
    {
        m_episode = std::stoi(t[0]);
        t[0] = t[0].substr(pos + 2);
    }
    // ben non !!!
    // episode = std::stoi(t[0]);

    bool found = false;

    /*std::wsmatch soustitre_match;
    const std::wregex soustitre_format_rg{ L"(.+) \\: (.+)" };
    if (std::regex_match(titre, soustitre_match, soustitre_format_rg))
    {
        titre = soustitre_match[1];
        sous_titre = soustitre_match[2];
        //sous_titre = keyColor[1] + L"xxx" + valuesColor + soustitre_match[2];
        found = true;
    }
    const std::wregex soustitre_format_rg2{ L"(.+)\\: (.+)" };
    if (std::regex_match(titre, soustitre_match, soustitre_format_rg2))
    {
        titre = soustitre_match[1];
        sous_titre = soustitre_match[2];
        found = true;
    }
    const std::wregex soustitre_format_rg3{ L"(.+)\\/(.+)" };
    if (std::regex_match(titre, soustitre_match, soustitre_format_rg3))
    {
        titre = soustitre_match[1];
        sous_titre = soustitre_match[2];
        found = true;
    }
    const std::wregex soustitre_format_rg4{ L"(.+) \\- (.+)" };
    if (std::regex_match(titre, soustitre_match, soustitre_format_rg4))
    {
        titre = soustitre_match[1];
        sous_titre = soustitre_match[2];
        found = true;
    }*/

    const std::wstring d_p = L" : ";
    pos = t[0].find(d_p);
    if (!found && pos != std::wstring::npos)
    {
        m_titre = t[0].substr(0, pos);
        m_deux_points = d_p;
        m_sous_titre = t[0].substr(pos + 3);
        found = true;
    }
    const std::wstring d_p2 = L": ";
    pos = t[0].find(d_p2);
    if (!found && pos != std::wstring::npos)
    {
        m_titre = t[0].substr(0, pos);
        m_deux_points = d_p2;
        m_sous_titre = t[0].substr(pos + 2);
        found = true;
    }
    const std::wstring d_p3 = L"/";
    pos = t[0].find(d_p3);
    if (!found && pos != std::wstring::npos)
    {
        m_titre = t[0].substr(0, pos);
        m_deux_points = d_p3;
        m_sous_titre = t[0].substr(pos + 1);
        found = true;
    }
    const std::wstring d_p4 = L" - ";
    pos = t[0].find(d_p4);
    if (!found && pos != std::wstring::npos)
    {
        m_titre = t[0].substr(0, pos);
        m_deux_points = d_p4;
        m_sous_titre = t[0].substr(pos + 3);
        found = true;
    }
    if (!found)
    {
        m_titre = t[0];
        found = true;
    }
    m_fichier_pas_zero = true;
    m_numero = 1;
    initialiser_duree(t[1]);
    for (auto j = 2; j < t.size(); j++)
        m_phrases += t[j];

//    system("PAUSE");
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void SequenceVisionnage::initialiser_duree(std::wstring& m)                                                                                        #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void SequenceVisionnage::initialiser_duree(std::wstring& m)
{
    const std::wregex duree_format_rg{ L"([[:digit:]]+)\\s?(min|MIN|Min)" };

    std::wsmatch match;

    if (std::regex_match(m, match, duree_format_rg))
    {
        auto duree_en_minute = std::stoi(match[1]);
        m_duree_en_seconde = duree_en_minute * 60;
    }
    else
    {
        throw std::invalid_argument("'" + std::string{ m.begin(),m.end() } + "' n'est pas un format de durée valide.");
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void SequenceVisionnage:Print()                                                                                                                    #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void SequenceVisionnage::Print()
{
    std::wstring wstr;
    bool chiffre_et_point_ou_pas = Print_Titre_chiffre_et_point_ou_pas(m_episode);
    if (chiffre_et_point_ou_pas)
    {
        wstr = std::to_wstring(m_saison) + keyColor[1] + L'x' + valuesColor + std::to_wstring(m_episode) + keyColor[1] + L" : " + valuesColor;
    }
    wstr += keyColor[1] + m_titre + valuesColor;
    if (m_deux_points != L"")
        wstr += m_deux_points + keyColor[1] + m_sous_titre + valuesColor;
    if (m_numero == 1)
    {
        wstr += keyColor[1] + L" (" + valuesColor + std::to_wstring(m_duree_en_seconde / 60) + keyColor[1] + m_min + L')' + valuesColor;
    }
    else
    {
        //wstr += keyColor[1] + L" [" + valuesColor + std::to_wstring(numero++) + keyColor[1] + L']' + valuesColor;
        //wstr += keyColor[1] + L" [" + valuesColor + std::to_wstring(saison_episode.numero++) + keyColor[1] + L']' + valuesColor;
        //wstr += keyColor[1] + L" [" + valuesColor + std::to_wstring(1 + saison_episode.numero++) + keyColor[1] + L']' + valuesColor;
        //numero++;
        //saison_episode.numero++;
    }
    wstr += keyColor[1] + L" : " + valuesColor;
    wstr += Print_Dates_de_visionnage(m_dates);

    // phrases
    if (m_numero == 1)//titre != L"")
    {
        wstr += L"\r\n" + m_phrases;
        //saison_episode.numero = 1;
    }
    std::wcout << wstr << std::endl;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::wstring SequenceVisionnage::Print_Dates_de_visionnage(std::vector<DateRecord>& dates_de_visionnage)                                           #
// #                                                                                                                                                    #
// ######################################################################################################################################################

std::wstring SequenceVisionnage::Print_Dates_de_visionnage(std::vector<DateRecord>& dates_de_visionnage)
{
    const std::wstring date_format = L"%d" + keyColor[1] + L"/" + valuesColor + L"%m" + keyColor[1] + L"/" + valuesColor + L"%Y";
    const std::wstring between_parenthesis = keyColor[1] + L"(" + valuesColor + L"%s" + keyColor[1] + L")" + valuesColor;
    const std::wstring same_date_format = between_parenthesis;
    const std::wstring prequel_format = between_parenthesis;
    const std::wstring streaming_format = keyColor[1] + L" : " + valuesColor + L"%s";
    const std::wstring step_by_step_tag = L' ' + keyColor[1] + L'[' + valuesColor + L"pas-à-pas" + keyColor[1] + L']' + valuesColor;

    std::wstring dates_de_visionnage_wstr = L"";

    std::vector<std::wstring> v_wstr;
    std::time_t last_date{ 0 };
    int same_date_counter = 0;
    for (auto dr : dates_de_visionnage)
    {
        std::time_t time = std::mktime(&dr.date);

        if (last_date != time)
        {
            std::tm localtime = *std::localtime(&time);
            std::wstringstream target_stream;
            target_stream << std::put_time(&localtime, date_format.c_str());
            std::wstring date_str = target_stream.str();
            v_wstr.push_back(date_str);
            same_date_counter = 0;
        }
        else
        {
            same_date_counter++;
            if (same_date_counter == 1)
            {
                v_wstr.back() += wstring_format(same_date_format, L"1");
            }
            v_wstr.back() += wstring_format(same_date_format, std::to_wstring(same_date_counter + 1).c_str());
        }
        last_date = time;
    }

    for (auto i = 0; i < v_wstr.size(); i++)
    {
        if (i != 0)
            dates_de_visionnage_wstr += L", ";
        dates_de_visionnage_wstr += v_wstr[i];
    }

    if (dates_de_visionnage.size() == 1)
    {
        if (dates_de_visionnage[0].someFlag)
            dates_de_visionnage_wstr += wstring_format(prequel_format, L"stop ou pas !");
    }
    else
    {
        if (dates_de_visionnage.size() > 0)
        {
            if (dates_de_visionnage.back().someFlag)
            {
                dates_de_visionnage_wstr += wstring_format(prequel_format, L"à suivre");
            }
            dates_de_visionnage_wstr += step_by_step_tag;
        }
    }

    if (m_streaming != L"" && dates_de_visionnage_wstr.length() > 0)
        dates_de_visionnage_wstr += wstring_format(streaming_format, m_streaming.c_str());
//
    return dates_de_visionnage_wstr;
}


// ######################################################################################################################################################
// #                                                                                                                                                    #
// # bool SequenceVisionnage::Print_Titre_chiffre_et_point_ou_pas(unsigned short int epi                                                                           #
// #                                                                                                                                                    #
// ######################################################################################################################################################

bool SequenceVisionnage::Print_Titre_chiffre_et_point_ou_pas(unsigned short int episode)
{
    if (episode == 0)
        return false;
    return true;
}

// ######################################################################################################################################################
// ######################################################################################################################################################

//Episode::Episode(void)
//{}

//Episode::~Episode()
//{}

Episode::Episode(SequenceVisionnage const& seq_vis)
{
    //auto nomFichier = seq_vis.filename().wstring();
    auto nomFichier = seq_vis;
    //system("PAUSE");
    //assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    //Episode episode(cheminFichier);
    //episodes.push_back(episode); // Episode

    //SequenceVisionnage sequencevisionnage(seq_vis);
    //titre = sequencevisionnage.titre;
}



/*Episode::Episode(fs::path const& cheminFichier)
{
    auto nomFichier = cheminFichier.filename().wstring();

    assert(nomFichier.length() > 0 && L"Nom de fichier Episode vide");
    //episode = std::stoi(match[filename_numero_episode_index]);
    //episode = std::stoi(match[filename_numero_episode_index]);
    std::vector<std::wstring> t = lire_fichierTxt(cheminFichier.wstring(), { L"\n" }, false);
    if (t[0] == L"")
    {
        fichier_pas_zero = false;
        //titre = saison_episode.titre;
        //deux_points = saison_episode.deux_points;
        //sous_titre = saison_episode.sous_titre;
        numero++;
        //numero = saison_episode.numero++;
        return;
    }

    size_t pos;// = 0;
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

    bool found = false;


    //std::wsmatch soustitre_match;
    //const std::wregex soustitre_format_rg{ L"(.+) \\: (.+)" };
    //if (std::regex_match(titre, soustitre_match, soustitre_format_rg))
    //{
    //    titre = soustitre_match[1];
    //    sous_titre = soustitre_match[2];
    //    found = true;
    //}
    //const std::wregex soustitre_format_rg2{ L"(.+)\\: (.+)" };
    //if (std::regex_match(titre, soustitre_match, soustitre_format_rg2))
    //{
    //    titre = soustitre_match[1];
    //    sous_titre = soustitre_match[2];
    //    found = true;
    //}
    //const std::wregex soustitre_format_rg3{ L"(.+)\\/(.+)" };
    //if (std::regex_match(titre, soustitre_match, soustitre_format_rg3))
    //{
    //    titre = soustitre_match[1];
    //    sous_titre = soustitre_match[2];
    //    found = true;
    //}
    //const std::wregex soustitre_format_rg4{ L"(.+) \\- (.+)" };
    //if (std::regex_match(titre, soustitre_match, soustitre_format_rg4))
    //{
    //    titre = soustitre_match[1];
    //    sous_titre = soustitre_match[2];
    //    found = true;
    //}



    const std::wstring d_p = L" : ";
    pos = t[0].find(d_p);
    if (!found && pos != std::wstring::npos)
    {
        titre = t[0].substr(0, pos);
        deux_points = d_p;
        sous_titre = t[0].substr(pos + 3);
        found = true;
    }
    const std::wstring d_p2 = L": ";
    pos = t[0].find(d_p2);
    if (!found && pos != std::wstring::npos)
    {
        titre = t[0].substr(0, pos);
        deux_points = d_p2;
        sous_titre = t[0].substr(pos + 2);
        found = true;
    }
    const std::wstring d_p3 = L"/";
    pos = t[0].find(d_p3);
    if (!found && pos != std::wstring::npos)
    {
        titre = t[0].substr(0, pos);
        deux_points = d_p3;
        sous_titre = t[0].substr(pos + 1);
        found = true;
    }
    const std::wstring d_p4 = L" - ";
    pos = t[0].find(d_p4);
    if (!found && pos != std::wstring::npos)
    {
        titre = t[0].substr(0, pos);
        deux_points = d_p4;
        sous_titre = t[0].substr(pos + 3);
        found = true;
    }
    if (!found)
    {
        titre = t[0];
        found = true;
    }
    fichier_pas_zero = true;
    numero = 1;
    initialiser_duree(t[1]);
    for (auto j = 2; j < t.size(); j++)
        phrases += t[j];
    //saison_episode = { titre, deux_points, sous_titre, numero };
    //system("PAUSE");
}*/

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Episode::afficher()                                                                                                                           #
// #                                                                                                                                                    #
// ######################################################################################################################################################

/*void Episode::afficher()
{
}*/

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Episode::initialiser_duree(std::wstring& m)                                                                                                   #
// #                                                                                                                                                    #
// ######################################################################################################################################################

/*void Episode::initialiser_duree(std::wstring& m)
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
}*/

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Episode::Print()                                                                                                                              #
// #                                                                                                                                                    #
// ######################################################################################################################################################

/*void Episode::Print()
{
    std::wstring wstr;
    bool chiffre_et_point_ou_pas = Print_Titre_chiffre_et_point_ou_pas(episode);
    if (chiffre_et_point_ou_pas)
    {
        wstr = std::to_wstring(saison) + keyColor[1] + L'x' + valuesColor + std::to_wstring(episode) + keyColor[1] + L" : " + valuesColor;
    }
    wstr += keyColor[1] + titre + valuesColor;
    if (deux_points != L"")
        wstr += deux_points + keyColor[1] + sous_titre + valuesColor;
    if (numero == 1)
    {
        wstr += keyColor[1] + L" (" + valuesColor + std::to_wstring(duree_en_seconde / 60) + keyColor[1] + min + L')' + valuesColor;
    }
    else
    {
        //wstr += keyColor[1] + L" [" + valuesColor + std::to_wstring(numero++) + keyColor[1] + L']' + valuesColor;
        //wstr += keyColor[1] + L" [" + valuesColor + std::to_wstring(saison_episode.numero++) + keyColor[1] + L']' + valuesColor;
        //wstr += keyColor[1] + L" [" + valuesColor + std::to_wstring(1 + saison_episode.numero++) + keyColor[1] + L']' + valuesColor;
        //numero++;
        //saison_episode.numero++;
    }
    wstr += keyColor[1] + L" : " + valuesColor;
    wstr += Print_Dates_de_visionnage(dates_de_visionnage);

    // phrases
    if (numero == 1)//titre != L"")
    {
        wstr += L"\r\n" + phrases;
        //saison_episode.numero = 1;
    }
    std::wcout << wstr << std::endl;
}*/

void Episode::Print()
{
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::wstring Episode::Print_Dates_de_visionnage(std::vector<DateRecord>& dates_de_visionnage)                                                      #
// #                                                                                                                                                    #
// ######################################################################################################################################################


/*std::wstring Episode::Print_Dates_de_visionnage(std::vector<DateRecord>& dates_de_visionnage)
{
    const std::wstring date_format = L"%d" + keyColor[1] + L"/" + valuesColor + L"%m" + keyColor[1] + L"/" + valuesColor + L"%Y";
    const std::wstring between_parenthesis = keyColor[1] + L"(" + valuesColor + L"%s" + keyColor[1] + L")" + valuesColor;
    const std::wstring same_date_format = between_parenthesis;
    const std::wstring prequel_format = between_parenthesis;
    const std::wstring streaming_format = keyColor[1] + L" : " + valuesColor + L"%s";
    const std::wstring step_by_step_tag = L' ' + keyColor[1] + L'[' + valuesColor + L"pas-à-pas" + keyColor[1] + L']' + valuesColor;

    std::wstring dates_de_visionnage_wstr = L"";

    std::vector<std::wstring> v_wstr;
    std::time_t last_date{ 0 };
    int same_date_counter = 0;
    for (auto dr : dates_de_visionnage)
    {
        std::time_t time = std::mktime(&dr.date);

        if (last_date != time)
        {
            std::tm localtime = *std::localtime(&time);
            std::wstringstream target_stream;
            target_stream << std::put_time(&localtime, date_format.c_str());
            std::wstring date_str = target_stream.str();
            v_wstr.push_back(date_str);
            same_date_counter = 0;
        }
        else
        {
            same_date_counter++;
            if (same_date_counter == 1)
            {
                v_wstr.back() += wstring_format(same_date_format, L"1");
            }
            v_wstr.back() += wstring_format(same_date_format, std::to_wstring(same_date_counter + 1).c_str());
        }
        last_date = time;
    }

    for (auto i = 0; i < v_wstr.size(); i++)
    {
        if (i != 0)
            dates_de_visionnage_wstr += L", ";
        dates_de_visionnage_wstr += v_wstr[i];
    }

    if (dates_de_visionnage.size() == 1)
    {
        if (dates_de_visionnage[0].someFlag)
            dates_de_visionnage_wstr += wstring_format(prequel_format, L"stop ou pas !");
    }
    else
    {
        if (dates_de_visionnage.size() > 0)
        {
            if (dates_de_visionnage.back().someFlag)
            {
                dates_de_visionnage_wstr += wstring_format(prequel_format, L"à suivre");
            }
            dates_de_visionnage_wstr += step_by_step_tag;
        }
    }

    if (streaming != L"" && dates_de_visionnage_wstr.length() > 0)
        dates_de_visionnage_wstr += wstring_format(streaming_format, streaming.c_str());
//
    return dates_de_visionnage_wstr;
}*/

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # bool Episode::Print_Titre_chiffre_et_point_ou_pas(unsigned short int epi                                                                           #
// #                                                                                                                                                    #
// ######################################################################################################################################################

/*bool Episode::Print_Titre_chiffre_et_point_ou_pas(unsigned short int episode)
{
    if (episode == 0)
        return false;
    return true;
}*/

// ######################################################################################################################################################
// ######################################################################################################################################################

Saison::Saison()
{}

Saison::~Saison()
{}

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
// # void Saison::creer_SequenceVisionnage(fs::path const& cheminFichier)                                                                               #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::creer_SequenceVisionnage(fs::path const& m_cheminFichier)
{
    auto nomFichier = m_cheminFichier.filename().wstring();

    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    Episode episode(m_cheminFichier);
    episodes.push_back(episode); // Episode
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::initialiser_Dossier(fs::path const& cheminFichier)                                                                                     #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::initialiser_Dossier(fs::path const& cheminDossier)
{
    auto nomDossier = cheminDossier.filename().wstring();
    assert(nomDossier.length() > 0 && L"Nom de dossier vide");
    assert(nomDossier.length() > 9 && L"Nom de fichier trop court pour avoir au moins une date");
    std::size_t pos = 0;
    std::wstring wstr = nomDossier.substr(pos);

    wchar_t sp = L' ', tiret = L'-';
    //int y;
    auto y = std::stoi(wstr, &pos);
    assert(1582 <= y && L"L'année est inférieur !");
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
    assert((1 <= m && m <= 12) && L"Le mois invalide !");
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
    assert((1 <= d && d <= 31) && L"Le jour invalide !");
    if (!checkday(m, d, y))
    {
        std::wcout << L"Le jour invalide !!!!" << std::endl;
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
// # void Saison::initialiser_Fichier(fs::path const& cheminFichier)                                                                                    #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::initialiser_Fichier(fs::path const& cheminFichier)
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
                initialiser_Avec(cheminFichier, avec);
                return;
            }
            // Chaîne d'origine
            if (nomFichier == L"Chaîne d'origine.txt")
            {
                initialiser_Chaine(cheminFichier);
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
                initialiser_Netflix(cheminFichier);
                return;
            }
            // Note
            if (nomFichier == L"Note.txt")
            {
                initialiser_Note(cheminFichier);
                return;
            }
            // Titre
            if (nomFichier == L"Titre.txt")
            {
                initialiser_Titre(cheminFichier);
                return;
            }
        }
        //
        if (
            (nomFichier[0] == L'1' || nomFichier[0] == L'2' || nomFichier[0] == L'3' || nomFichier[0] == L'4' || nomFichier[0] == L'5' || nomFichier[0] == L'6' || nomFichier[0] == L'7' || nomFichier[0] == L'8' || nomFichier[0] == L'9')
            && nomFichier[1] == L'x'
            )
        {
            //creer_Episode(cheminFichier);
            creer_SequenceVisionnage(cheminFichier);
            return;
        }
        if (
            (nomFichier[0] == L'1' || nomFichier[0] == L'2' || nomFichier[0] == L'3' || nomFichier[0] == L'4' || nomFichier[0] == L'5' || nomFichier[0] == L'6' || nomFichier[0] == L'7' || nomFichier[0] == L'8' || nomFichier[0] == L'9')
            && (std::isdigit(nomFichier[1]))
            && nomFichier[2] == L'x'
            )
        {
            creer_SequenceVisionnage(cheminFichier);
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
        initialiser_Image(cheminFichier, image);
        return;
    }
    else
    {
    }
    return;
 }

// ######################################################################################################################################################

 // ######################################################################################################################################################
 // #                                                                                                                                                    #
 // # void Saison::initialiser_Chaine(fs::path const& cheminFichier)                                                                                     #
 // #                                                                                                                                                    #
 // ######################################################################################################################################################

 void Saison::initialiser_Chaine(fs::path const& cheminFichier)
 { // Chaîne
     auto nomFichier = cheminFichier.filename().wstring();
     assert(nomFichier.length() > 0 && L"Nom de fichier vide");
     m_chaine = lire_fichierTxt(cheminFichier.wstring());
     assert((m_chaine.size() != 0));
 }

 // ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::creer_Episode(fs::path const& cheminFichier)                                                                                          #
// #                                                                                                                                                    #
// ######################################################################################################################################################

/*void Saison::creer_Episode(fs::path const& cheminFichier)
{
    auto nomFichier = cheminFichier.filename().wstring();

    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    //Episode episode(cheminFichier);
    SequenceVisionnage(m_cheminFichier);
    //episodes.push_back(episode); // Episode
    sequencevisionnages.push_back(sequencevisionnage);
}*/


// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::initialiser_Netflix(fs::path const& cheminFichier)                                                                                    #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::initialiser_Netflix(fs::path const& cheminFichier)
{
    auto nomFichier = cheminFichier.filename().wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    netflix = true;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::initialiser_Note(fs::path const& cheminFichier)                                                                                       #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::initialiser_Note(fs::path const& cheminFichier)
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
// # void Saison::initialiser_Titre(fs::path const& cheminFichier)                                                                                      #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::initialiser_Titre(std::filesystem::path const& cheminFichier)
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
    wchar_t date_string[15];
    std::wcsftime(date_string, 15, L"%d/%m/%Y", &dossier.first);
    std::wstring wstr;
    wstr = date_string;

    //std::time_t t = std::mktime(&dossier.first);
    //std::tm local = *std::localtime(&t);
    //std::wstringstream target_stream;
    //target_stream << std::put_time(&local, L"%d/%m/%Y");
    //std::wstring wstr = target_stream.str();

//    https://en.cppreference.com/w/cpp/chrono/c/wcsftime
//    std::locale::global(std::locale("ja_JP.utf8"));
//    std::time_t t = std::time(nullptr);
//    wchar_t wstr[100];
//    if (std::wcsftime(wstr, 100, L"%A %c", std::localtime(&t)))
//        std::wcout << wstr << '\n';


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
    // Chaîne
    Print_Chaine();
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
    if (affichage_avec_actif && avec.size())
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
// # void Saison::Print_Chaine()                                                                                                                        #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::Print_Chaine()
{
    if (affichage_chaine_actif && m_chaine.size() > 0)
    {
        std::wstring chaine_str = keyColor[1] + L"Chaîne d'origine : " + valuesColor + m_chaine + L"\r\n";
        //PrintStringW(m_hOut, creee_par_str, 0);
        //PrintStringW(HANDLE hOut, creee_par_str);
        //Console_Lire(chaine_str, 0, 0);
        //Console_Lire(hOut, chaine_str, 0, L' ');
        std::wcout << chaine_str;
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::Print_Date_etc()                                                                                                                      #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const void Saison::Print_Date_etc()
{
    if (affichage_date_etc_actif)
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
    if (affichage_image_actif && image.size() > 0 /*!= 0*/)
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
    if (affichage_netflix_actif && netflix)
    {
        std::wstring netflix_str = keyColor[1] + L'(' + valuesColor + L"Netflix" + keyColor[1] + L')' + valuesColor;
        std::wcout << netflix_str << std::endl;
    }
}

// ######################################################################################################################################################
// ######################################################################################################################################################

Serie::Serie(std::filesystem::path racine)
{
    this->racine = racine;
}

Serie::~Serie()
{}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Serie::initialiser_Fichier(fs::path const& cheminFichier)                                                                                     #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Serie::initialiser_Fichier(fs::path const& cheminFichier)
{
    auto nomFichier = cheminFichier.filename().wstring();
    auto nomImage = cheminFichier.extension().wstring();
    if (nomImage == L".txt")
    {
        // AD
        if (nomFichier == L"AD.txt")
        {
            initialiser_Audiodescription(cheminFichier, audiodescription);
            return;
        }
        // Chaîne d'origine
        if (nomFichier == L"Chaîne d'origine.txt")
        {
            initialiser_Chaine(cheminFichier);
            return;
        }
        // Genre
        if (nomFichier == L"Genre.txt")
        {
            initialiser_Genre(cheminFichier, genre, ::Genre);
            return;
        }
        // Nationalité
        if (nomFichier == L"Nationalité.txt")
        {
            initialiser_Nationalite(cheminFichier, nationalite, ::Nationalite);
            return;
        }
    }
    else if(nomImage == L".jpg" || nomImage == L".png" || nomImage == L".webp")
        // Image
    {
        initialiser_Image(cheminFichier, image);
        return;
    }
    else
    {
    }
    return;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Serie::initialiser_Chaine(fs::path const& cheminFichier)                                                                                      #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Serie::initialiser_Chaine(fs::path const& cheminFichier)
{ // Chaîne
    auto nomFichier = cheminFichier.wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    chaine = lire_fichierTxt(cheminFichier.wstring());
    assert((chaine.size() != 0));
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Serie::Print()                                                                                                                                #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const void Serie::Print()
{
    // Chaîne d'origine
    Print_Chaine();
    // AD
    Print_Audiodescription(audiodescription, affichage_audiodescription_actif, keyColor[0], valuesColor);
    // Genre(s)
    Print_Genres(genre, affichage_genres_actif, sous_genre, affichage_sous_genre_actif, keyColor[0], valuesColor);
    // Nationalité(s)
    Print_Nationalites(nationalite, affichage_nationalite_actif, keyColor[0], valuesColor);
    // Image(s)
    ::Print_Images(image, affichage_image_actif, keyColor[0], valuesColor);
    // Saison(s)
    Print_Saisons();
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Serie::Print_Chaine()                                                                                                                         #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const void Serie::Print_Chaine()
{
    if (affichage_chaine_actif && chaine.size() > 0)
    {
        std::wstring chaine_str = keyColor[0] + L"Chaîne d'origine : " + valuesColor + chaine + L"\r\n";
        //PrintStringW(m_hOut, creee_par_str, 0);
        //PrintStringW(HANDLE hOut, creee_par_str);
        //Console_Lire(chaine_str, 0, 0);
        //Console_Lire(hOut, chaine_str, 0, L' ');
        std::wcout << chaine_str;
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const void Serie::Print_Saison(Saison saison)                                                                                                      #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const void Serie::Print_Saison(Saison saison)
{
    if (affichage_serie_actif)
    {
        saison.Print();
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const void Serie::Print_Saisons()                                                                                                                  #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const void Serie::Print_Saisons()
{
    if (affichage_saisons_actif)
    {
        std::size_t taille;
        taille = std::size(saisons);
        for (int i = 0; i < taille; i++)
        {
            Print_Saison(saisons[i]);
        }
    }
}

