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

#include <filesystem> // C++17 standard header file name

// Hors saison : ???
// Saison 1 : exemple : 2023/02/15
// Saison 2 : exemple : 2024/09/10
// etc...

using namespace std;
namespace fs = std::filesystem;

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

extern void initialiser_Audiodescription(fs::path const& cheminFichier, std::wstring& m_ad);
extern void initialiser_Avec(fs::path const& cheminFichier, std::vector<std::pair<std::wstring, std::wstring>>& m_avec);
//extern const void initialiser_Chaine(fs::path const& cheminFichier, std::wstring& m_chaine);
extern void initialiser_Genre(fs::path const& cheminFichier, std::vector<std::wstring>& m_genres_renvoyes, const std::vector<std::wstring>& genres_valides);
extern void initialiser_Image(fs::path const& cheminFichier, std::vector<std::wstring>& m_images);
extern void initialiser_Nationalite(fs::path const& cheminFichier, std::vector<std::wstring>& m_nationalites_renvoyes, const std::vector<std::wstring>& nationalites_valides);
//extern void initialiser_Sous_Genre(std::wstring& m_s_g);
extern bool initialiser_Sous_Genre(std::wstring& m_s_g);
extern void initialiser_Sur(std::wstring& m_s);
extern void initialiser_Titre_Original(fs::path const& cheminFichier, std::vector<std::wstring>& m_titre_original);

extern std::wstring recuperer_Disney_SJ(fs::path const& cheminFichier);
extern std::wstring recuperer_Netflix_SJ(fs::path const& cheminFichier);
extern std::wstring recuperer_SJ(fs::path const& cheminFichier);

extern void Print_Audiodescription(const std::wstring& m_audiodescription, bool affichage_audiodescription_actif, std::wstring& keyColor, std::wstring& valuesColor);
extern void Print_Images(const std::vector<std::wstring>& m_image, bool affichage_image_actif, std::wstring& keyColor, std::wstring& valuesColor);
extern void Print_Genres(const std::vector<std::wstring>& m_genres, bool affichage_genres_actif, const std::wstring& m_sous_genre, bool affichage_sous_genre_actif, std::wstring& keyColor, std::wstring& valuesColor);
extern void Print_Nationalites(const std::vector<std::wstring>& m_nationalites, bool affichage_nationalite_actif, std::wstring& keyColor, std::wstring& valuesColor);
extern void Print_Resume(const std::vector<wstring>& m_resume, bool affichage_resume_actif);
extern void Print_Titre_Original(const std::vector<std::wstring>& m_titre_original, bool affichage_titre_original_actif, std::vector<std::wstring>& keyColor, std::wstring& valuesColor);

// ######################################################################################################################################################
// ######################################################################################################################################################

static bool ends_with(std::wstring_view str, std::wstring_view suffix)
{
    return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

// ######################################################################################################################################################
// ######################################################################################################################################################

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # SequenceVisionnage::InfosVisionnage(const Saison& saison, fs::path const& m_cheminFichier)                                                       #
// #                                                                                                                                                    #
// ######################################################################################################################################################

InfosVisionnage::InfosVisionnage(const Saison& saison, fs::path const& m_cheminFichier) : m_saison{ saison }
{ // "{" : marche pas !!!
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
    //const std::wregex filename_format_rg{ sep_numero_saison + numero_episode_format + sep_episode_saison + L"(" + dates_format + L"+)" + stream_format };

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


    auto nomFichier = m_cheminFichier.filename().wstring();

    assert(nomFichier.length() > 0 && L"Nom de fichier Episode vide");

    auto stem = m_cheminFichier.stem().wstring();
    // prefixe ???
    //assert((stem.length() > (9 + std::to_wstring(prefixe).length() + sep_numero_saison.length())) && L"Nom de fichier Episode trop court pour avoir au moins une date");
    assert((stem.length() > 9) && L"Nom de fichier Episode trop court pour avoir au moins une date");

    assert(std::isdigit(stem[0]) && L"Nom de fichier Episode ne commençant pas par un nombre");
    m_NumeroSaison = std::stoi(stem);
    assert((m_NumeroSaison <= 1000) && L"x <= 1000 !!!");
    //
    assert((m_NumeroSaison <= 1000) && L"x <= 1000 !!!");// saison == m_NumeroSaison
    //
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

            m_DatesVisionnage.emplace_back(dr);
        }
        else if (dates_match[dates_date_month_day_month_index].matched)
        {
            assert(m_DatesVisionnage.size() > 0 && L"Utilisation d'un format mois-jour sans avoir d'année déduite.");

            auto month = std::stoi(dates_match[dates_date_month_day_month_index]);
            auto day = std::stoi(dates_match[dates_date_month_day_day_index]);

            auto lastDateRecord = m_DatesVisionnage.back();
            auto last_year = lastDateRecord.date.tm_year + 1900;

            assert(checkmonth(month));
            assert(checkday(month, day, last_year));

            DateRecord dr{ {0,0,0,day,month - 1,last_year - 1900} };

            m_DatesVisionnage.emplace_back(dr);
        }
        else if (dates_match[dates_date_day_day_index].matched)
        {
            assert(m_DatesVisionnage.size() > 0 && L"Utilisation d'un format jour sans avoir de mois et d'années déduits.");

            auto day = std::stoi(dates_match[dates_date_day_day_index]);

            auto lastDateRecord = m_DatesVisionnage.back();
            auto last_year = lastDateRecord.date.tm_year + 1900;
            auto last_month = lastDateRecord.date.tm_mon + 1;

            assert(checkday(last_month, day, last_year));

            DateRecord dr{ {0,0,0,day,last_month - 1,last_year - 1900} };

            m_DatesVisionnage.emplace_back(dr);
        }
        else
        {
            assert(true && L"format de date d'épisode inconnu.");
        }

        //if (dates_match[dates_fucking_someFlag_index].matched)
        if (dates_match[dates_someFlag_index].matched)
        {
            m_DatesVisionnage.back().someFlag = true;
        }

        dates_str = dates_match.suffix().str();
    }

    if (match[filename_stream_index].matched)
    {
        m_streaming = match[filename_stream_index];
    }

    //episode = std::stoi(match[filename_numero_episode_index]);
    std::vector<std::wstring> t = lire_fichierTxt(m_cheminFichier.wstring(), { L"\n" }, false);

/*    if (t[0] == L"")
    {
        m_fichier_pas_zero = false;
        //m_numero++;
//        SequenceVisionnage{ info_vis };
        //InfosVisionnage.push_back();
//        SequenceVisionnage.push_back({ InfosVisionnage });
        return;
    }*/
    m_NumeroEpisode = std::stoi(match[filename_numero_episode_index]);

    size_t pos;// = 0;
    pos = t[0].find(L". ");
    if (pos == std::wstring::npos || t[0][3] == L'.')
    {
        //saison = 0;
        m_NumeroEpisode = 0;
    }
    else
    {
        m_NumeroEpisode = std::stoi(t[0]);
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
        m_titres[0] = t[0].substr(0, pos);
        m_titres[1] = d_p;
        m_titres[2] = t[0].substr(pos + 3);
        found = true;
    }
    const std::wstring d_p2 = L": ";
    pos = t[0].find(d_p2);
    if (!found && pos != std::wstring::npos)
    {
        m_titres[0] = t[0].substr(0, pos);
        m_titres[1] = d_p2;
        m_titres[2] = t[0].substr(pos + 2);
        found = true;
    }
    const std::wstring d_p3 = L"/";
    pos = t[0].find(d_p3);
    if (!found && pos != std::wstring::npos)
    {
        m_titres[0] = t[0].substr(0, pos);
        m_titres[1] = d_p3;
        m_titres[2] = t[0].substr(pos + 1);
        found = true;
    }
    const std::wstring d_p4 = L" - ";
    pos = t[0].find(d_p4);
    if (!found && pos != std::wstring::npos)
    {
        m_titres[0] = t[0].substr(0, pos);
        m_titres[1] = d_p4;
        m_titres[2] = t[0].substr(pos + 3);
        found = true;
    }
    if (!found)
    {
        m_titres[0] = t[0];
        found = true;
    }
    t.erase(t.begin());
    //m_fichier_pas_zero = true;
    //m_numero = 1;
    initialiser_Duree(t[0]);
    t.erase(t.begin());
    m_resume = t;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # InfosVisionnage::initialiser_Duree(std::wstring& m)                                                                                                #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void InfosVisionnage::initialiser_Duree(std::wstring& m)
{
    const std::wregex duree_format_rg{ L"([[:digit:]]+)\\s?(min|MIN|Min)" };

    std::wsmatch match;

    if (std::regex_match(m, match, duree_format_rg))
    {
        auto duree_en_minute = std::stoi(match[1]);
        m_duree = duree_en_minute * 60;
    }
    else
    {
        throw std::invalid_argument("'" + std::string{ m.begin(),m.end() } + "' n'est pas un format de durée valide.");
    }
}

/*void InfosVisionnage::Print()
{
    std::wstring wstr;
    bool chiffre_et_point_ou_pas = Print_Titre_chiffre_et_point_ou_pas(m_NumeroEpisode);
    if (chiffre_et_point_ou_pas)
    {
        wstr = std::to_wstring(m_NumeroSaison) + keyColor[1] + L'x' + valuesColor + std::to_wstring(m_NumeroEpisode) + keyColor[1] + L" : " + valuesColor;
    }
    wstr += keyColor[1] + m_titre + valuesColor;
    if (m_deux_points != L"")
        wstr += m_deux_points + keyColor[1] + m_sous_titre + valuesColor;
    if (m_numero == 1)
    {
        wstr += keyColor[1] + L" (" + valuesColor + std::to_wstring(m_duree / 60) + keyColor[1] + m_min + L')' + valuesColor;
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
    wstr += Print_Dates_de_visionnage(m_DatesVisionnage);

    // phrases
    if (m_numero == 1)//titre != L"")
    {
        wstr += L"\r\n" + m_phrases;
        //saison_episode.numero = 1;
    }
    std::wcout << wstr << std::endl;
}*/

/*std::wstring InfosVisionnage::Print_Dates_de_visionnage(std::vector<DateRecord>& dates_de_visionnage)
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
}*/

/*bool InfosVisionnage::Print_Titre_chiffre_et_point_ou_pas(unsigned short int episode)
{
    if (episode == 0)
        return false;
    return true;
}*/

// ######################################################################################################################################################
// ######################################################################################################################################################

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # SequenceVisionnage::SequenceVisionnage(const Episode& episode) :m_episode{ episode } {};                                                           #
// #                                                                                                                                                    #
// ######################################################################################################################################################

// Ok !!!

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # SequenceVisionnage::SequenceVisionnage(const SequenceVisionnage& sep) = default;                                                                   #
// #                                                                                                                                                    #
// ######################################################################################################################################################

// Ok !!!

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void SequenceVisionnage:Print()                                                                                                                    #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void SequenceVisionnage::Print()
{
    ;
}

/*void SequenceVisionnage::Print()
{
    std::wstring wstr;
    bool chiffre_et_point_ou_pas = Print_Titre_chiffre_et_point_ou_pas(m_NumeroEpisod);
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
    std::wcout << wstr;
}*/

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::wstring SequenceVisionnage::Print_Dates_de_visionnage(std::vector<DateRecord>& m_DatesVisionnage)                                             #
// #                                                                                                                                                    #
// ######################################################################################################################################################

std::wstring SequenceVisionnage::Print_Dates_de_visionnage(std::vector<DateRecord>& m_DatesVisionnage)
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
    for (auto dr : m_DatesVisionnage)
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

    if (m_DatesVisionnage.size() == 1)
    {
        if (m_DatesVisionnage[0].someFlag)
            dates_de_visionnage_wstr += wstring_format(prequel_format, L"stop ou pas !");
    }
    else
    {
        if (m_DatesVisionnage.size() > 0)
        {
            if (m_DatesVisionnage.back().someFlag)
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
// # bool SequenceVisionnage::Print_Titre_chiffre_et_point_ou_pas(unsigned short int episode)                                                           #
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

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Episode::Episode(InfosVisionnage const& seq_vis) :m_saison{ info_vis.m_saison } {};                                                           #
// #                                                                                                                                                    #
// ######################################################################################################################################################

// Ok !!!
/*Episode::Episode(InfosVisionnage const& seq_vis)
{
    m_saison{ info_vis.m_saison };
}*/
/*Episode::Episode(SequenceVisionnage const& seq_vis)
{
    //auto nomFichier = seq_vis.filename().wstring();
    //auto nomFichier = seq_vis;
    m_liste_sequence_visionnages.push_back(seq_vis);
    //system("PAUSE");
    //assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    //Episode episode(cheminFichier);
    //episodes.push_back(episode); // Episode

    //SequenceVisionnage sequencevisionnage(seq_vis);

    //titre = sequencevisionnage.titre;
}*/

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Episode::ajouter_SequenceVisionnage(const InfosVisionnage& seq_vis)                                                                           #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Episode::ajouter_SequenceVisionnage(const InfosVisionnage& info_vis)
{
    m_liste_sequence_visionnages.push_back(SequenceVisionnage(*this, info_vis));
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Episode::Print()                                                                                                                              #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Episode::Print()
{
    //Print_Data();
    for (auto vis : m_liste_sequence_visionnages)
    {
        vis.Print();
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # bool Episode::Print_Data()                                                                                                                         #
// #                                                                                                                                                    #
// ######################################################################################################################################################

/*void Episode::Print_Data()
{
}*/

/*bool Episode::Print_Titre_chiffre_et_point_ou_pas(unsigned short int episode)
{
    if (episode == 0)
        return false;
    return true;
}*/

// ######################################################################################################################################################
// ######################################################################################################################################################

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # Saison::Saison(fs::path const& cheminFichier, const Serie& serie) : m_serie{ serie }                                                               #
// #                                                                                                                                                    #
// ######################################################################################################################################################

Saison::Saison(fs::path const& cheminFichier, const Serie& serie) : m_serie{ serie }
{
    auto nomDossier = cheminFichier.filename().wstring();
    assert(nomDossier.length() > 0 && L"Nom de dossier vide");
    assert(nomDossier.length() > 3 && L"Nom de fichier trop court pour avoir au moins une date");
    std::size_t pos = 0;
    std::wstring wstr = nomDossier.substr(pos);

    wchar_t sp = L' ', tiret = L'-';
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
    m_dossier.first = tm;
    m_dossier.second = wstr;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::ajouter_InfosVisionnage(SequenceVisionnage const& seq_vis)                                                                            #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::ajouter_InfosVisionnage(SequenceVisionnage const& seq_vis)
{
    //m_liste_episodes.insert({ 1, seq_vis });
    //m_liste_episodes.insert({ 1, seq_vis });
    //m_numero++;
    //auto [xxx, success] = m_liste_episodes.insert({ m_numero, seq_vis });
    //m_liste_episodes.insert(std::pair<int, Episode>(m_numero, seq_vis));
    //m_liste_episodes.insert({ m_numero, seq_vis });
}

/*void Saison::creer_InfosVisionnage(fs::path const& m_cheminFichier)
{
    auto nomFichier = m_cheminFichier.filename().wstring();

    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    InfosVisionnage infosvisionnage(m_cheminFichier);
    infosvisionnages.push_back(infosvisionnage);
    //Episode episode(m_cheminFichier);
    //episodes.push_back(episode); // Episode
}*/

/*Episode Saison::creer_Episode(InfosVisionnage const& seq_vis)
{
    ;
}*/

//Episode Saison::creer_Episode(SequenceVisionnage const& seq_vis)
/*Episode Saison::creer_Episode(InfosVisionnage const& seq_vis)
{
    //    auto nomFichier = m_cheminFichier.filename().wstring();
    //m_liste_sequence_visionnages.push_back(seq_vis);
//    m_liste_episodes.insert({ 2, seq_vis });
    m_liste_episodes.emplace( 1, seq_vis );
    //    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
        //Episode episode(cheminFichier);
    //    SequenceVisionnage(m_cheminFichier);
        //episodes.push_back(episode); // Episode
    //    sequencevisionnages.push_back(sequencevisionnage);
}*/


// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::creer_SequenceVisionnage(fs::path const& m_cheminFichier)                                                                             #
// #                                                                                                                                                    #
// ######################################################################################################################################################

//void Saison::creer_SequenceVisionnage(fs::path const& m_cheminFichier)
//{
//    auto nomFichier = m_cheminFichier.filename().wstring();
//    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
//    Episode episode(m_cheminFichier);
//    episodes.push_back(episode); // Episode
//}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::initialiser_Dossier(fs::path const& m_cheminFichier)                                                                                  #
// #                                                                                                                                                    #
// ######################################################################################################################################################

/*void Saison::initialiser_Dossier(fs::path const& m_cheminFichier)
{
    auto nomDossier = m_cheminFichier.filename().wstring();
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
    m_dossier.first = tm;
    m_dossier.second = wstr;
}*/

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
                initialiser_Avec(cheminFichier, m_avec);
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
        if (std::regex_match(nomFichier, std::wregex{L"([[:digit:]]{1,2})x(.)+"}))
        {
            return;
            //InfosVisionnage info_vis{*this, m_cheminFichier};
            //bool none();
            /*if (m_liste_episodes.find(info_vis.m_NumeroEpisode) != m_liste_episodes.end())
            {
                m_liste_episodes[info_vis.m_NumeroEpisode]->ajouter_SequenceVisionnage(info_vis);
            }
            else
            {
                m_liste_episodes.emplace(std::pair<const int, shared_ptr<Episode>>{ info_vis.m_NumeroEpisode, make_shared<Episode>(info_vis) });
            }*/
        }
        //
        if (int j = std::stoi(nomFichier))
        {
            m_numero = j;
            initialiser_Resume(cheminFichier);
            return;
        }
        // Erreur !
        if (nomFichier != L"")
        {
            //E.afficher_X(-1, nomFichier, L'{' + t + L".txt} !!!");
            std::wcout << L'{' << cheminFichier << L'}' << std::endl;
            return;
        }
    }
    else if(nomImage == L".jpg" || nomImage == L".png" || nomImage == L".webp")
        // Image
    {
        initialiser_Image(cheminFichier, m_image);
    }
    else
    {
        std::wcout << L'{' << cheminFichier << L'}' << std::endl;
    }
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
// # void Saison::initialiser_Netflix(fs::path const& cheminFichier)                                                                                    #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::initialiser_Netflix(fs::path const& cheminFichier)
{
    auto nomFichier = cheminFichier.filename().wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    m_netflix = true;
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

    std::wstring str = lire_fichierTxt(cheminFichier.wstring());

    std::wregex note_pattern{ L"([0-5])((\\.|,)(\\d+))?" };
    std::wsmatch match;
    if (std::regex_match(str, match, note_pattern))
    {
        if (match[2].matched)
        {
            str[1] = L',';
        }
        m_note = std::stod(str);
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::initialiser_Resume(fs::path const& cheminFichier)                                                                                     #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::initialiser_Resume(fs::path const& cheminFichier)
{
    m_resume = lire_fichierTxt(cheminFichier.wstring(), { L"\n" });
    assert((m_resume.size() != 0));
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
    std::vector<std::wstring> titre = lire_fichierTxt(cheminFichier.wstring(), { L"\n" });
    assert((titre.size() != 0));

    std::wregex titre_pattern{ L"(.+?)(\\s:\\s|:\\s|/|\\s-\\s)(.+)" };
    std::wsmatch match;
    if (std::regex_match(titre[0], match, titre_pattern))
    {
        m_titres.push_back(match[1]);
        if (match.length() > 2)
        {
            m_titres.push_back(match[2]);
        }
        if (match.length() > 3)
        {
            m_titres.push_back(match[3]);
        }
    }
    else
    {
        m_titres.push_back(titre[0]);
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::wstring stringFormatOneLine(std::wstring str,                                                                                                 #
// #                                  int lengthMax,                                                                                                    #
// #                                  std::wstring marqueurTroncature,                                                                                  #
// #                                  int marqueurTroncature_len,                                                                                       #
// #                                  std::wstring suffixe,                                                                                             #
// #                                  int suffixe_len)                                                                                                  #
// #                                                                                                                                                    #
// ######################################################################################################################################################

std::wstring stringFormatOneLine(std::wstring str, int lengthMax, std::wstring marqueurTroncature, int marqueurTroncature_len, std::wstring suffixe, int suffixe_len)
{
    std::wstring res;

    if (str.size() < lengthMax - suffixe_len)
    {
        res = str;
    }
    else
    {
        res = str.substr(0, lengthMax - marqueurTroncature_len - suffixe_len) + marqueurTroncature;
    }
    res += suffixe;
    return res;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::Print_Header()                                                                                                                        #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::Print_Header()
{
    wchar_t date_tab[15];
    std::wcsftime(date_tab, 15, L"%d/%m/%Y", &m_dossier.first);
    std::wstring date_tab_str = date_tab;

    std::wstring date_str = date_tab_str.substr(0, 2) + keyColor[1] + L'/' + valuesColor + date_tab_str.substr(3, 2) + keyColor[1] + L'/' + valuesColor + date_tab_str.substr(6, 4);

    std::wstring dossier_str;
    if (m_dossier.second != L"")
        dossier_str = keyColor[0] + m_dossier.second + valuesColor + L' ';

    std::wstring titre_str;
    if (m_titres.size() != 0)
    {
        titre_str = keyColor[0] + m_titres[0] + valuesColor;
        if (m_titres.size() > 1)
        {
            titre_str += keyColor[1] + m_titres[1] + valuesColor + keyColor[0] + m_titres[2] + valuesColor;
        }
        titre_str += keyColor[1] + L" : " + valuesColor;
    }

    std::wstring resume_str;
    if (m_serie.m_resume != m_resume)
    {
        for (auto r : m_resume)
            resume_str += r;
    }
    else
    {
        resume_str = stringFormatOneLine(m_resume.size() > 0 ? m_resume[0] : L"", 40 + 3 + 5, L"...", 3, keyColor[1] + L'(' + valuesColor + L"Bis" + keyColor[1] + L')' + valuesColor, 5);
    }

    std::wstring numero_str = L' ' + keyColor[1] + L'(' + valuesColor + std::to_wstring(m_numero) + keyColor[1] + L')' + valuesColor;

    std::wcout << date_str << dossier_str << keyColor[1] + L" : " + valuesColor << titre_str << resume_str << numero_str << std::endl;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::Print()                                                                                                                               #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::Print()
{
    Print_Header();

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

    /*std::size_t taille;
    taille = std::size(m_liste_episodes);
    for (auto i = 0; i < taille; i++)
    {
        m_liste_episodes[i]->Print();
    }*/
    // Note
    Print_Note();    
    // Chaîne
    Print_Chaine();
    // Netflix
    Print_Netflix();
    // Images(s)
    Print_Images();
    // Avec
    Print_Avec();
    // Saison ok !
    std::wcout << L"\r\n";
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::Print_Avec()                                                                                                                          #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::Print_Avec()
{
    if (affichage_avec_actif && m_avec.size())
    {
        std::wstring avec_str = keyColor[1] + L"Avec : " + valuesColor;
        bool found = false;
        for (auto&& [nom, role] : m_avec)
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
            if (m_avec.back().first != nom)
                avec_str += keyColor[1] + L", " + valuesColor;
            else
                avec_str += keyColor[1] + L'.' + valuesColor;
        }
        if (found)
            avec_str += L"...";
        avec_str += L"\r\n";
        std::wcout << avec_str;
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
// ???
const void Saison::Print_Date_etc()
{
    if (affichage_date_etc_actif)
    {
        wchar_t date_string[15];
        std::wcsftime(date_string, 15, L"%d/%m/%Y", &m_dossier.first);
        std::wstring date_etc_str;
        date_etc_str = date_string;
        date_etc_str = date_etc_str.substr(0, 2) + keyColor[1] + L'/' + valuesColor + date_etc_str.substr(3, 2) + keyColor[1] + L'/' + valuesColor + date_etc_str.substr(6, 4);
        if (m_dossier.second != L"")
            date_etc_str += keyColor[0] + m_dossier.second + valuesColor + L' ';
        date_etc_str += keyColor[0] + m_titres[0] + valuesColor;
        if (m_titres[2] != L"")
            date_etc_str += keyColor[1] + m_titres[1] + valuesColor + keyColor[0] + m_titres[2] + valuesColor;

        date_etc_str += keyColor[1] + L" : " + valuesColor;
        for (auto r : m_resume)
            date_etc_str += r;
        date_etc_str += L' ' + keyColor[1] + L'(' + valuesColor + std::to_wstring(m_numero) + keyColor[1] + L')' + valuesColor;
        date_etc_str += L"\r\n";
        std::wcout << date_etc_str;
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::Print_Images()                                                                                                                        #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::Print_Images()
{
    if (affichage_image_actif && m_image.size() > 0 )
    {
        std::wstring image_str = keyColor[1] + L"Image" + ((m_image.size() > 1) ? L"s" : L"") + L" : [" + valuesColor;
        bool first = true;
        for (auto&& i : m_image)
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
        std::wcout << image_str;
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::Print_Netflix()                                                                                                                       #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::Print_Netflix()
{
    if (affichage_netflix_actif && m_netflix)
    {
        std::wstring netflix_str = keyColor[1] + L'(' + valuesColor + L"Netflix" + keyColor[1] + L')' + valuesColor;
        netflix_str += L"\r\n";
        std::wcout << netflix_str;
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::Print_Netflix()                                                                                                                       #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::Print_Note()
{ // 0...5 ou -1
    if (affichage_note_actif && m_note)
    {
        std::wstring note_str;
        if (m_note == -1.0)
        {
            note_str = keyColor[1] + L'(' + valuesColor + L"Pas de note !" + keyColor[1] + L')' + valuesColor;
        }
        else if (m_note == 0 || m_note == 1 || m_note == 2 || m_note == 3 || m_note == 4 || m_note == 5)
        {
            note_str += keyColor[1] + L"Note : " + valuesColor;
            note_str += std::to_wstring(static_cast<int>(std::floor(m_note)));
            note_str += keyColor[1] + L"/5" + valuesColor;
        }
        else
        {
            note_str += keyColor[1] + L"Note : " + valuesColor;
            std::wstring wstr = std::to_wstring(m_note);
            wstr = wstr[0] + keyColor[1] + wstr[1] + valuesColor + wstr[2];
            note_str += wstr;
            note_str += keyColor[1] + L"/5" + valuesColor;
        }
        note_str += L"\r\n";
        std::wcout << note_str;
    }

}

// ######################################################################################################################################################
// ######################################################################################################################################################

Serie::Serie(std::filesystem::path racine)
{
    this->racine = racine;
    auto nomDossier = racine.filename().wstring();
    assert(nomDossier.length() > 0 && L"Nom de dossier vide");

    std::size_t pos, pos2;
    pos = nomDossier.find_first_of(L".[");
    pos2 = nomDossier.find_first_of(L']');
    assert((pos != std::wstring::npos && pos2 != std::wstring::npos && pos < pos2) && L"Attention : pas \".[\" et/ou \"]\" !!!");

    const std::wstring titres = nomDossier.substr(0, pos);
    m_titres = Dossier_Titres(titres);

    nomDossier = nomDossier.substr(pos + 2);
    std::wstring textes = nomDossier;
    pos = textes.find_last_of(L"]");
    textes = textes.substr(0, pos);
    bool found = false;
    for (unsigned i = 0; i < textes.length(); ++i)
    {
        if (textes.at(i) == L' ' || textes.at(i) == std::wstring::npos)
        {
            m_annees = textes.substr(0, i);
            found = true;
            if (textes.at(i) == L' ')
            {
                textes = textes.substr(i);
            }
        }
    }
    if (!found)
    {
        m_sur = L"";
    }
    if (textes[0] != L']')
    {
        //textes = textes.substr(1);
        //m_sur = textes;
    }
    std::wstring sous_genre = nomDossier;
    pos = sous_genre.find_last_of(L"]");
    if (sous_genre.at(pos) != std::wstring::npos)
    {
        sous_genre = sous_genre.substr(pos + 2);
        found = initialiser_Sous_Genre(sous_genre);
        m_sous_genre = sous_genre;
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::vector<std::wstring> Serie::Dossier_Titres(std::wstring titres)                                                                               #
// #                                                                                                                                                    #
// ######################################################################################################################################################
/// ???
std::vector<std::wstring> Serie::Dossier_Titres(std::wstring titres)
{
    assert(titres.length() > 0 && L"Nom de dossier vide"); // ??? pour Mot de... ?
    bool found = false;
    size_t pos = 0;
    const std::wstring d_p = L" - ";
    pos = titres.find(d_p);
    if (!found && pos != std::wstring::npos)
    {
        m_titres.push_back(titres.substr(0, pos));
        m_titres.push_back(d_p);
        m_titres.push_back(titres.substr(pos + 3));
        found = true;
    }
    const std::wstring d_p2 = L"- ";
    pos = titres.find(d_p2);
    if (!found && pos != std::wstring::npos)
    {
        m_titres.push_back(titres.substr(0, pos));
        m_titres.push_back(d_p2);
        m_titres.push_back(titres.substr(pos + 2));
        found = true;
    }
    const std::wstring d_p3 = L"-";
    pos = titres.find(d_p3);
    if (!found && pos != std::wstring::npos)
    {
        m_titres.push_back(titres.substr(0, pos));
        m_titres.push_back(d_p3);
        m_titres.push_back(titres.substr(pos + 1));
        found = true;
    }
    if (!found)
    {
        m_titres.push_back(titres);
        //found = true;
    }
    return m_titres;
}

    /*int tm_year = 0;// , tm_mon = 0, tm_mday = 0;
    std::wstring t2;
    std::size_t idx, idx2 = 0;
    idx = nomFichier.find(L".[", 0);
    t2 = nomFichier.substr(0, idx);
    //i = ::afficher_Titre(t2, titre, affichage_titre_actif);
    m_titres2 = t2;
    idx = nomFichier.find_last_of(L"[");
    idx2 = nomFichier.find_last_of(L"]");
    if (idx2 - idx - 1 == 0)
        return;// -1;
    idx2--;
    if (!(nomFichier.at(idx2) == L' ' || nomFichier.at(idx2) == L'-' ||
        nomFichier.at(idx2) == L'0' || nomFichier.at(idx2) == L'1' || nomFichier.at(idx2) == L'2' || nomFichier.at(idx2) == L'3' || nomFichier.at(idx2) == L'4' ||
        nomFichier.at(idx2) == L'5' || nomFichier.at(idx2) == L'6' || nomFichier.at(idx2) == L'7' || nomFichier.at(idx2) == L'8' || nomFichier.at(idx2) == L'9'))
    { // Sur
        std::size_t idx3;
        idx3 = nomFichier.find_first_of(L" ", idx);
        idx3++;
        t2 = nomFichier.substr(idx3, idx2 - idx3 + 1);
        initialiser_Sur(t2);
        if (m_sur == t2)
        {
            //
        }
        else
        {
            m_sur = t2;
        }
        //if (m_sur == L"Netflix")
        //    netflix_ok_ou_non = true;
        idx2 = idx3 - 2;
    }
    idx++;
    if (nomFichier.at(idx2) == L' ' || nomFichier.at(idx2) == L'-' ||
        nomFichier.at(idx2) == L'0' || nomFichier.at(idx2) == L'1' || nomFichier.at(idx2) == L'2' || nomFichier.at(idx2) == L'3' || nomFichier.at(idx2) == L'4' ||
        nomFichier.at(idx2) == L'5' || nomFichier.at(idx2) == L'6' || nomFichier.at(idx2) == L'7' || nomFichier.at(idx2) == L'8' || nomFichier.at(idx2) == L'9')
    { // Date
        t2 = nomFichier.substr(idx, idx2 - idx + 1);
        //i = afficher_Date(t2);
    }
    idx = nomFichier.find(L"].");
    if (idx != std::wstring::npos)
    { // Sous_Genre
        idx += 2;
        m_sous_genre = nomFichier.substr(idx);
        //wstring sous_genre = L"";
        initialiser_Sous_Genre(m_sous_genre);
        //affichage_sous_genre_actif = true;
    }*/

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
            initialiser_Audiodescription(cheminFichier, m_audiodescription);
        }
        // Chaîne d'origine
        if (nomFichier == L"Chaîne d'origine.txt")
        {
            initialiser_Chaine(cheminFichier);
        }
        // Créée par
        if (nomFichier == L"Créée par.txt")
        {
            initialiser_Creee_par(cheminFichier);
        }
        // Disney+ SJ
        if (nomFichier == L"Disney+.txt")
        {
            m_disney_sj = recuperer_Disney_SJ(cheminFichier);
        }
        // En relation avec
        if (nomFichier == L"En relation avec.txt")
        {
            initialiser_En_relation_avec(cheminFichier);
        }
        // Genre
        if (nomFichier == L"Genre.txt")
        {
            initialiser_Genre(cheminFichier, m_genre, ::Genre);
        }
        // Nationalité
        if (nomFichier == L"Nationalité.txt")
        {
            initialiser_Nationalite(cheminFichier, m_nationalite, ::Nationalite);
        }
        // Netflix
        if (nomFichier == L"Netflix.txt")
        {
            m_netflix_sj = recuperer_Netflix_SJ(cheminFichier);
        }
        // SJ
        if (nomFichier == L"SJ.txt")
        {
            m_sj =  recuperer_SJ(cheminFichier);
        }
        // Titre
        if (nomFichier == L"Titre.txt")
        {
            initialiser_Titre(cheminFichier, m_titres);
            return;
        }
        // Titre original
        if (nomFichier == L"Titre original.txt")
        {
            initialiser_Titre_Original(cheminFichier, m_titres_originaux);
        }
    }
    else if(nomImage == L".jpg" || nomImage == L".png" || nomImage == L".webp")
        // Image
    {
        initialiser_Image(cheminFichier, m_image);
    }
    else
    {
        std::wcout << L'{' << cheminFichier << L'}' << std::endl;
    }
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
    m_chaine = lire_fichierTxt(cheminFichier.wstring());
    assert((m_chaine.size() != 0));
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Serie::initialiser_Creee_par(fs::path const& cheminFichier)                                                                                   #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Serie::initialiser_Creee_par(fs::path const& cheminFichier)
{ // Creee par
    auto nomFichier = cheminFichier.wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    m_creee_par = lire_fichierTxt(nomFichier, { L"\n", L", " });
    assert((m_creee_par.size() != 0));
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # Serie::initialiser_Duree(std::wstring& m)                                                                                                          #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Serie::initialiser_Duree(std::wstring& m)
{
    const std::wregex duree_format_rg{ L"([[:digit:]]+)\\s?(min|MIN|Min)" };

    std::wsmatch match;

    if (std::regex_match(m, match, duree_format_rg))
    {
        auto duree_en_minute = std::stoi(match[1]);
        m_duree = duree_en_minute * 60;
    }
    else
    {
        throw std::invalid_argument("'" + std::string{ m.begin(),m.end() } + "' n'est pas un format de durée valide.");
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Serie::initialiser_En_relation_avec(fs::path const& cheminFichier)                                                                            #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Serie::initialiser_En_relation_avec(fs::path const& cheminFichier)
{ 
    auto nomFichier = cheminFichier.wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    m_en_relation_avec = lire_fichierTxt(cheminFichier.wstring());
    assert((m_en_relation_avec.size() != 0));
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # Serie::void Serie::initialiser_Titre(fs::path const& cheminFichier, std::vector<std::wstring>& m_titre)                                            #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Serie::initialiser_Titre(fs::path const& cheminFichier, std::vector<std::wstring>& m_titre)
{ // Titre
    auto nomFichier = cheminFichier.wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    std::vector<std::wstring> titres = lire_fichierTxt(cheminFichier.wstring(), { L"\n" });
    assert((titres.size() != 0));

    std::vector<std::wstring> t;


    std::wregex titre_pattern{ L"(.+?)(\\s:\\s|:\\s|/|\\s-\\s)(.+)" };
    std::wsmatch match;
    if (std::regex_match(titres[0], match, titre_pattern))
    {
        t.push_back(match[1]);
        if (match.length() > 2)
        {
            t.push_back(match[2]);
        }
        if (match.length() > 3)
        {
            t.push_back(match[3]);
        }
    }
    else
    {
        t.push_back(titres[0]);
    }

 
    /*if (m_titres.size() == 1 && t.size() == 1 && m_titres == t)
    {
        found = true;
    }*/
/*    if (m_titres[0] == t[0])
        assert(m_titres[0] == t[0] && L"Erreur : m_titre[0] != t[0] !!!");
    bool found = false;
    if (!found && m_titres[0].size() == t[0].size())
    {
        int j = 0;
        for (int i = 0; i < m_titres.size(); i++)
        {
            if (m_titres[i] != t[i])
            {
                j++;
                m_titres[i] = t[i];
            }

        }
        try
        {
            if (j > 2)
                ;
            else
            {
                throw 505;
            }
        }
        catch (...)
        {
            std::wcout << L"Erreur : j > 4" << std::endl;
        }
        if (m_titres.size() == 1 && t.size() == 1)
        {
            //m_titres[0] = t[0];
            //throw wstring(L"ERREUR : m_titres[0] par de... !");
            found = true;
        }
    }
    if (!found && m_titres.size() == 3 && m_titres[0].length() == t[0].length() && m_titres[2].length() == t[2].length())
    {
        m_titres[1] = t[1];
        found = true;
    }
*/
    //???

    titres.erase(titres.begin());
    if (titres.size() > 0)
    {
        initialiser_Duree(titres[0]);
        titres.erase(titres.begin());
        if (titres.size() > 0)
            m_resume = titres;
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const std::wstring Serie::Calcul_Note_Affichage()                                                                                                  #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const std::wstring Serie::Calcul_Note_Affichage()
{
    std::wstring res;

    std::vector<double>notes;
    for (const auto& saison : saisons)
    {
        if (saison.m_note >= 0.0)
        {
            notes.push_back(saison.m_note);
        }
    }
    if (notes.size() < 1)
        res = keyColor[0] + L'(' + valuesColor + L"pas de note !" + keyColor[0] + L')';
    else
    {
        double note = std::accumulate(notes.begin(), notes.end(), 0.0) / notes.size();
        double whole, fractional;
        fractional = std::modf(note, &whole);

        std::wstring whole_str = wstring_format(L"%.0f", whole);
        std::wstring fractional_tmp = wstring_format(L"%.2f", fractional);
        std::wstring sepDecimal = fractional_tmp.substr(1, 1);
        std::wstring fractional_str;
        if (ends_with(fractional_tmp, L"00"))
        {
            fractional_str = L"";
        }
        else if (ends_with(fractional_tmp, L"0"))
        {
            fractional_str = fractional_tmp.substr(2, 1);
        }
        else
        {
            fractional_str = fractional_tmp.substr(2, 2);
        }

        res = whole_str + keyColor[0] + sepDecimal + valuesColor + fractional_str + keyColor[0] + L"/5";
    }
    return (res.length() > 0) ? L" " + res + valuesColor : L"";
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Serie::Print()                                                                                                                                #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const void Serie::Print()
{
    // Titre
    Print_Titre();
    // Titre Original
    Print_Titre_Original(m_titres_originaux, affichage_titres_originaux_actif, keyColor, valuesColor);
    // Chaîne d'origine
    Print_Chaine();
    // AD
    Print_Audiodescription(m_audiodescription, affichage_audiodescription_actif, keyColor[0], valuesColor);
    // Creee par
    Print_Creee_par();
    // Genre(s)
    Print_Genres(m_genre, affichage_genres_actif, m_sous_genre, affichage_sous_genre_actif, keyColor[0], valuesColor);
    // En relation avec
    Print_En_relation_avec();
    // Nationalité(s)
    Print_Nationalites(m_nationalite, affichage_nationalite_actif, keyColor[0], valuesColor);
    // Image(s)
    Print_Images(m_image, affichage_image_actif, keyColor[0], valuesColor);
    // Phrases
    Print_Resume(m_resume, affichage_resume_actif);
    std::wcout << L"\r\n";
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
    if (affichage_chaine_actif && m_chaine.size() > 0)
    {
        std::wstring chaine_str = keyColor[0] + L"Chaîne d'origine : " + valuesColor;
        chaine_str += m_chaine + L"\r\n";
        //PrintStringW(m_hOut, creee_par_str, 0);
        //PrintStringW(HANDLE hOut, creee_par_str);
        //Console_Lire(chaine_str, 0, 0);
        //Console_Lire(hOut, chaine_str, 0, L' ');
        std::wcout << chaine_str;
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Serie::Print_Creee_par()                                                                                                                      #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const void Serie::Print_Creee_par()
{
    if (affichage_creee_par_actif && m_creee_par.size() > 0)
    {
        std::wstring creee_par_str = keyColor[0] + L"Créée" + ((m_creee_par.size() > 1) ? L"s" : L"") + L" par : " + valuesColor;
        bool first = true;
        for (auto&& c : m_creee_par)
        {
            if (!first)
            {
                creee_par_str += keyColor[0] + L", " + valuesColor;
            }
            creee_par_str += c;
            first = false;
        }
        creee_par_str += L"\r\n";

        //PrintStringW(m_hOut, creee_par_str, 0);
        //PrintStringW(HANDLE hOut, creee_par_str);
        //int i = Console_Lire_txt(creee_par_str, 0, 0);
        //Console::PrintStringW(creee_par_str, 0);
        std::wcout << creee_par_str;
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const void Serie::Print_En_relation_avec()                                                                                                         #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const void Serie::Print_En_relation_avec()
{
    if (affichage_en_relation_avec_actif && m_en_relation_avec.size() > 0)
    {
        std::wstring en_relation_avec_str = keyColor[0] + L"En relation avec : " + valuesColor;
        en_relation_avec_str += m_en_relation_avec + L"\r\n";
        //PrintStringW(m_hOut, creee_par_str, 0);
        //PrintStringW(HANDLE hOut, creee_par_str);
        //Console_Lire(chaine_str, 0, 0);
        //Console_Lire(hOut, chaine_str, 0, L' ');
        std::wcout << en_relation_avec_str;
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

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const void Serie::Print_Titre()                                                                                                                    #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const void Serie::Print_Titre()
{
    if (affichage_titres_actif)
    {
        std::wstring titres_str;
        std::wstring wstr;
        titres_str = keyColor[0] + L"Titre : " + valuesColor + m_titres[0];
        if (m_titres.size() > 1)
            titres_str += keyColor[1] + m_titres[1] + valuesColor + m_titres[2];
        /*if (m_date_Diffusee_a_partir_de_[0] && Date_Diffusee_a_partir_de[0].tm_year != 0)
        {
            wchar_t date_string[22];
            wcsftime(date_string, 15, L"%Y", &Date_Diffusee_a_partir_de[0]);
            wstr = date_string;
            titres_str += keyColor[0] + L" (" + valuesColor + wstr + keyColor[0] + L')' + valuesColor;
        }*/
        // sur
        if (affichage_sur_actif && m_sur != L"")
        {
            titres_str += keyColor[0] + L" (sur " + valuesColor + m_sur + keyColor[0] + L" : " + valuesColor;
            // Disney+ SJ
            if (affichage_disney_sj_actif && m_disney_sj.length() != 0)
                titres_str += m_disney_sj;
            // Netflix SJ
            if (affichage_netflix_sj_actif && m_netflix_sj.length() != 0)
                titres_str += m_netflix_sj;
            titres_str += keyColor[0] + L')' + valuesColor;
        }
        else
        {
            // Disney+ SJ
            if (affichage_disney_sj_actif && m_disney_sj.length() != 0)
                titres_str += keyColor[0] + L" (" + valuesColor + L"Disney+ : " + m_disney_sj + keyColor[0] + L')' + valuesColor;
            // Netflix SJ
            if (affichage_netflix_sj_actif && m_netflix_sj.length() != 0)
                titres_str += keyColor[0] + L" (" + valuesColor + L"Netflix : " + m_netflix_sj + keyColor[0] + L')' + valuesColor;
        }
        // La signalétique jeunesse
        if (affichage_sj_actif && m_sj.length() != 0)
            titres_str += keyColor[0] + L" (" + valuesColor + L"SJ" + keyColor[0] + L" : " + valuesColor + m_sj + keyColor[0] + L')' + valuesColor;
        // Note
        if (affichage_note_actif)
            titres_str += Calcul_Note_Affichage();
        //int i = Console_Lire_txt(titre_str + wstr, 0, 0);
        //Console_Lire(titre_str, 0, 0);
        //Console_Lire(hOut, titre_str + L"\r\n", 0, L' ');
        titres_str += L"\r\n";
        std::wcout << titres_str;
    }
}
