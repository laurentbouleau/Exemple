#include "pch.h"
#include "film_serie.h"
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
#include <list>
#include <optional>
#include <cctype>

#include <filesystem> // C++17 standard header file name

// Hors saison : 0 !!!
// Saison 1 : exemple : 2023/02/15
// Saison 2 : exemple : 2024/09/10
// etc...

using namespace std;
namespace fs = std::filesystem;

using DateVisionnage = DateRecord;

//const std::vector<std::wstring> Genre;
//const std::vector<std::wstring> Sur;
//const std::vector<std::wstring> Sous_Genre;
//const std::vector<std::wstring> Nationalite;

std::wostream& PrintSerieOnOutstream(/*const*/ Serie& serie, std::wostream& out)
{
    serie.Print();
    return out;
}

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
// # SequenceVisionnage::InfosVisionnage(const Saison& saison, fs::path const& m_cheminFichier)                                                         #
// #                                                                                                                                                    #
// ######################################################################################################################################################

/*InfosVisionnage::InfosVisionnage(const Saison& saison, fs::path const& m_cheminFichier) : m_saison{saison}
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
    const int filename_someFlag_index = filename_date_day_day_index + 2;
    const int filename_stream_index = filename_someFlag_index + 2;

    auto nomFichier = m_cheminFichier.filename().wstring();

    assert(nomFichier.length() > 0 && L"Nom de fichier �pisode vide");

    auto stem = m_cheminFichier.stem().wstring();
    assert((stem.length() > 9) && L"Nom de fichier �pisode trop court pour avoir au moins une date");

    assert(std::isdigit(stem[0]) && L"Nom de fichier �pisode ne commen�ant pas par un nombre");
    
    assert((m_NumeroSaison <= 1000) && L"x <= 1000 !!!");// saison == m_NumeroSaison

    assert((stem.find(L"x", 0) != std::wstring::npos) && L"Saison::afficher_Episode() :  x !!!");

    std::wsmatch match;
    auto str = stem;
    //Exemple assez complexe de nom de fichier
    //str = L"1x01.2024-02-01_2024-02-02_02-03_0405 Netflix";
    std::regex_match(str, match, filename_format_rg);

    if (match[filename_numero_saison_index].matched)
        m_NumeroSaison = std::stoi(match[filename_numero_saison_index]);

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
            assert(m_DatesVisionnage.size() > 0 && L"Utilisation d'un format mois-jour sans avoir d'ann�e d�duite.");

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
            assert(m_DatesVisionnage.size() > 0 && L"Utilisation d'un format jour sans avoir de mois et d'ann�es d�duits.");

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
            assert(true && L"format de date d'�pisode inconnu.");
        }

        if (dates_match[dates_someFlag_index].matched)
        {
            m_DatesVisionnage.back().someFlag = true;
        }

        dates_str = dates_match.suffix().str();
    }

    m_NumeroEpisodeDansSaison = std::stoi(match[filename_numero_episode_index]);
    
    //InfosVisionnage_dates(filename_format_rg, filename_numero_saison_index, filename_dates_index, dates_format, dates_date_year_month_day_year_index, stem);

    std::vector<std::wstring> file_content = lire_fichierTxt(m_cheminFichier.wstring(), { L"\n" }, false);

    if (file_content.size() > 0)
    {
        InfosVisionnage_file_content(file_content);
    }
}*/

InfosVisionnage::InfosVisionnage(const Saison& saison, fs::path const& m_cheminFichier) : m_saison{ saison }
{


    auto nomFichier = m_cheminFichier.filename().wstring();

    assert(nomFichier.length() > 0 && L"Nom de fichier �pisode vide");

    auto stem = m_cheminFichier.stem().wstring();
    assert((stem.length() > 9) && L"Nom de fichier �pisode trop court pour avoir au moins une date");

    assert(std::isdigit(stem[0]) && L"Nom de fichier �pisode ne commen�ant pas par un nombre");

    assert((m_NumeroSaison <= 1000) && L"x <= 1000 !!!");// saison == m_NumeroSaison

    assert((stem.find(L"x", 0) != std::wstring::npos) && L"Saison::afficher_Episode() :  x !!!");

    AnalyserStem(stem);

    std::vector<std::wstring> file_content = lire_fichierTxt(m_cheminFichier.wstring(), { L"\n" }, false);

    if (file_content.size() > 0)
    {
        AnalyserContenu(file_content);
    }
}

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
const int filename_someFlag_index = filename_date_day_day_index + 2;
const int filename_stream_index = filename_someFlag_index + 2;

void InfosVisionnage::AnalyserStem(std::wstring stem)
{
    std::wsmatch match;
    auto str = stem;
    //Exemple assez complexe de nom de fichier
    //str = L"1x01.2024-02-01_2024-02-02_02-03_0405 Netflix";
    std::regex_match(str, match, filename_format_rg);

    if (match[filename_numero_saison_index].matched)
        m_NumeroSaison = std::stoi(match[filename_numero_saison_index]);

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
            assert(m_DatesVisionnage.size() > 0 && L"Utilisation d'un format mois-jour sans avoir d'ann�e d�duite.");

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
            assert(m_DatesVisionnage.size() > 0 && L"Utilisation d'un format jour sans avoir de mois et d'ann�es d�duits.");

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
            assert(true && L"format de date d'�pisode inconnu.");
        }

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

    m_NumeroEpisodeDansSaison = std::stoi(match[filename_numero_episode_index]);
}

void InfosVisionnage::AnalyserContenu(std::vector<std::wstring>file_content)
{
    const std::wregex numeroPlusTitres_rg{ L"(?:(\\d)+\\.)?(.*)" };
    std::wsmatch titles_match;
    std::regex_match(file_content[0], titles_match, numeroPlusTitres_rg);

    if (titles_match[1].matched)
    {
        m_NumeroEpisodeDansSerie = std::stoi(titles_match[1]);
    }

    std::wstring titres = titles_match[2];

    trim(titres);

    m_titres = extraire_Titres_Depuis_UneLigne(titres);

    if (file_content.size() > 1)
        initialiser_Duree(file_content[1]);

    if (file_content.size() > 2)
    {
        file_content.erase(file_content.begin(), file_content.begin() + 2);
        m_resume = file_content;
    }

}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void InfosVisionnage::initialiser_Duree(std::wstring& m)                                                                                           #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void InfosVisionnage::initialiser_Duree(std::wstring& m)
{
    //const std::wregex duree_format_rg{ L"([[:digit:]]+)\\s?(min|MIN|Min)" };
    const std::wregex duree_format_rg{ L"([[:digit:]]+)\\s?(min|MIN|Min)\\s*" };
    std::wsmatch match;

    if (std::regex_match(m, match, duree_format_rg))
    {
        auto duree_en_minute = std::stoi(match[1]);
        m_duree_en_seconde = duree_en_minute * 60;
    }
    else
    {
        throw std::invalid_argument("'" + std::string{ m.begin(),m.end() } + "' n'est pas un format de dur�e valide.");
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void InfosVisionnage::initialiser_file_content(std::vector<std::wstring>file_content)                                                              #
// #                                                                                                                                                    #
// ######################################################################################################################################################
// ???
void InfosVisionnage::InfosVisionnage_file_content(std::vector<std::wstring>file_content)
{
    const std::wregex numeroPlusTitres_rg{ L"(?:(\\d)+\\.)?(.*)" };
    std::wsmatch titles_match;
    std::regex_match(file_content[0], titles_match, numeroPlusTitres_rg);

    if (titles_match[1].matched)
    {
        m_NumeroEpisodeDansSerie = std::stoi(titles_match[1]);
    }

    std::wstring titres = titles_match[2];

    trim(titres);

    m_titres = extraire_Titres_Depuis_UneLigne(titres);

    if (file_content.size() > 1)
        initialiser_Duree(file_content[1]);

    if (file_content.size() > 2)
    {
        file_content.erase(file_content.begin(), file_content.begin() + 2);
        m_resume = file_content;
    }
}

// ######################################################################################################################################################
// ######################################################################################################################################################

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::wstring SequenceVisionnage::calcul_Duree_affichage() const                                                                                    #
// #                                                                                                                                                    #
// ######################################################################################################################################################

std::wstring SequenceVisionnage::calcul_Duree_affichage(int numero_sequence) const
{
    std::wstring duree_str;
    if (numero_sequence == 1)
    {
        duree_str +=  miseEnFormeDuree(m_duree_en_seconde, m_labelMinuteSingulier, m_labelMinutePluriel, m_keyColor[1], m_valuesColor, L" (", L")", m_espace3);
    }
    else
    {
        duree_str += L' ' + m_keyColor[1] + L'[' + m_valuesColor + L"bis" + m_keyColor[1] + L']' + m_valuesColor;
    }
    return duree_str;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void SequenceVisionnage::AffichagePersonnaliser(AffichagePersonnalisation perso)                                                                   #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void SequenceVisionnage::AffichagePersonnaliser(AffichagePersonnalisation perso)
{
    m_espace1 = perso.m_espace1;
    m_espace2 = perso.m_espace2;
    m_espace3 = perso.m_espace3;
    m_labelHeureSingulier = perso.m_labelsHeure.first;
    m_labelHeurePluriel = perso.m_labelsHeure.second;
    m_labelMinuteSingulier = perso.m_labelsMinute.first;
    m_labelMinutePluriel = perso.m_labelsMinute.second;

    m_keyColor = perso.m_keyColor;
    m_valuesColor = perso.m_valuesColor;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void SequenceVisionnage::Print(int numero_sequence, bool hors_saison) const                                                                        #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void SequenceVisionnage::Print(int numero_sequence, bool hors_saison) const
{
    std::wstring numero_str;
    std::wstring titre_str;
    std::wstring duree_str;

    if (hors_saison)
    {
        //numero_str = std::to_wstring(m_episode.m_numero) + m_keyColor[1] + L" : " + m_valuesColor;
        numero_str = std::to_wstring(m_episode.m_numero) + m_keyColor[1] + L" : " + m_valuesColor;
    }
    else
    {
        numero_str = std::to_wstring(m_episode.m_saison.m_numero) + m_keyColor[1] + L'x' + m_valuesColor + ((m_episode.m_numero < 10 && m_episode.m_saison.m_nombre_episodes>10) ? L"0" : L"") + std::to_wstring(m_episode.m_numero) + m_keyColor[1] + L" : " + m_valuesColor;
    }

    assert((m_titres.size() > 0 && m_titres[0].size() > 0) || numero_sequence > 1);
    std::vector<std::wstring> titres = m_titres;
    if (m_titres.size() == 0 || (m_titres.size() == 1 && m_titres[0].size() == 0))
    {
        titres = m_episode.m_liste_sequence_visionnages_ordonnee_chronologiquement[0].m_titres;
    }

    std::vector<std::wstring> titre_prefixe_list{ m_keyColor[1] , m_valuesColor };
    std::wstring titre_prefixe = titre_prefixe_list[0];
    unsigned int titre_prefixe_index = 0;
    for (auto& titre : titres)
    {
        titre_str += titre_prefixe_list[titre_prefixe_index] + titre;
        titre_prefixe_index = ((titre_prefixe_index + 1) % titre_prefixe_list.size());
    }
    if (titre_str.size() > 0)
        titre_str += m_valuesColor;

    duree_str = calcul_Duree_affichage(numero_sequence);

    std::wstring dates_str = m_keyColor[1] + L" : " + m_valuesColor + Print_Dates_de_visionnage();

    std::wstring resume_str;

    if (numero_sequence == 1)
    {
        resume_str += L"\r\n";
        for (auto r : m_resume)
            resume_str += r;
    }

    std::wcout << numero_str << titre_str << duree_str << dates_str << resume_str << L"\r\n";
}
/*void SequenceVisionnage::Print(int numero_sequence, bool hors_saison, long numero) const
{
    std::wstring numero_str;
    std::wstring titre_str;
    std::wstring duree_str;

    if (hors_saison)
    {
        if (numero != -1)
            numero_str = std::to_wstring(m_episode.m_numero) + m_keyColor[1] + L" : " + m_valuesColor;
    }
    else
    {
        if (numero != -1)
            numero_str = std::to_wstring(m_episode.m_saison.m_numero) + m_keyColor[1] + L'x' + m_valuesColor + ((m_episode.m_numero < 10 && m_episode.m_saison.m_nombre_episodes>10) ? L"0" : L"") + std::to_wstring(m_episode.m_numero) + m_keyColor[1] + L" : " + m_valuesColor;
    }

    assert((m_titres.size() > 0 && m_titres[0].size() > 0) || numero_sequence > 1);
    std::vector<std::wstring> titres = m_titres;
    if (m_titres.size() == 0 || (m_titres.size() == 1 && m_titres[0].size() == 0))
    {
        titres = m_episode.m_liste_sequence_visionnages_ordonnee_chronologiquement[0].m_titres;
    }

    std::vector<std::wstring> titre_prefixe_list{ m_keyColor[1] , m_valuesColor };
    std::wstring titre_prefixe = titre_prefixe_list[0];
    unsigned int titre_prefixe_index = 0;
    for (auto& titre : titres)
    {
        titre_str += titre_prefixe_list[titre_prefixe_index] + titre;
        titre_prefixe_index = ((titre_prefixe_index + 1) % titre_prefixe_list.size());
    }
    if (titre_str.size() > 0)
        titre_str += m_valuesColor;

    duree_str = calcul_Duree_affichage(numero_sequence);

    std::wstring dates_str = m_keyColor[1] + L" : " + m_valuesColor + Print_Dates_de_visionnage();

    std::wstring resume_str;

    if (numero_sequence == 1)
    {
        resume_str += L"\r\n";
        for (auto r : m_resume)
            resume_str += r;
    }

    std::wcout << numero_str << titre_str << duree_str << dates_str << resume_str << L"\r\n";
}*/

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::wstring SequenceVisionnage::Print_Dates_de_visionnage() const                                                                                 #
// #                                                                                                                                                    #
// ######################################################################################################################################################

std::wstring SequenceVisionnage::Print_Dates_de_visionnage() const
{
    const std::wstring date_format = L"%d" + m_keyColor[1] + L"/" + m_valuesColor + L"%m" + m_keyColor[1] + L"/" + m_valuesColor + L"%Y";
    const std::wstring between_parenthesis = m_keyColor[1] + L"(" + m_valuesColor + L"%s" + m_keyColor[1] + L")" + m_valuesColor;
    const std::wstring same_date_format = between_parenthesis;
    const std::wstring prequel_format = between_parenthesis;
    const std::wstring streaming_format = m_keyColor[1] + L" : " + m_valuesColor + L"%s";
    const std::wstring step_by_step_tag = L' ' + m_keyColor[1] + L'[' + m_valuesColor + L"pas-�-pas" + m_keyColor[1] + L']' + m_valuesColor;

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
                dates_de_visionnage_wstr += wstring_format(prequel_format, L"� suivre");
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
// # long SequenceVisionnage::Print_Titre_chiffre(long episode) const                                                                                   #
// #                                                                                                                                                    #
// ######################################################################################################################################################

long SequenceVisionnage::Print_Titre_chiffre(long episode) const
{
    if (episode == 1)
        return 1;
    return -1;
}

// ######################################################################################################################################################
// ######################################################################################################################################################

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # Episode::Episode(const InfosVisionnage& info_vis) :m_saison{ info_vis.m_saison }, m_numero{ info_vis.m_NumeroEpisodeDansSaison }                   #
// #                                                                                                                                                    #
// ######################################################################################################################################################

Episode::Episode(const InfosVisionnage& info_vis) :m_saison{ info_vis.m_saison }, m_numero{ info_vis.m_NumeroEpisodeDansSaison }
{
    m_saison.m_numero = info_vis.m_NumeroSaison;
    ajouter_SequenceVisionnage(info_vis);
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Episode::ajouter_SequenceVisionnage(const InfosVisionnage& seq_vis)                                                                           #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Episode::ajouter_SequenceVisionnage(const InfosVisionnage& info_vis)
{
    m_liste_sequence_visionnages_ordonnee_chronologiquement.push_back(SequenceVisionnage{ *this, info_vis });
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Episode::AffichagePersonnaliser(AffichagePersonnalisation perso)                                                                              #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Episode::AffichagePersonnaliser(AffichagePersonnalisation perso)
{
    m_espace1 = perso.m_espace1;
    m_espace2 = perso.m_espace2;
    m_espace3 = perso.m_espace3;
    m_labelHeureSingulier = perso.m_labelsHeure.first;
    m_labelHeurePluriel = perso.m_labelsHeure.second;
    m_labelMinuteSingulier = perso.m_labelsMinute.first;
    m_labelMinutePluriel = perso.m_labelsMinute.second;

    m_keyColor = perso.m_keyColor;
    m_valuesColor = perso.m_valuesColor;

    for (auto& sequencevisionnage : m_liste_sequence_visionnages_ordonnee_chronologiquement)
    {
        sequencevisionnage.AffichagePersonnaliser(perso);
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # long long Episode::GetNumeroSequenceVisionnage(const SequenceVisionnage& sev_vis) const                                                            #
// #                                                                                                                                                    #
// ######################################################################################################################################################

/*long long Episode::GetNumeroSequenceVisionnage(const SequenceVisionnage& sev_vis) const
{
    auto it = std::find(m_liste_sequence_visionnages_ordonnee_chronologiquement.begin(), m_liste_sequence_visionnages_ordonnee_chronologiquement.end(), sev_vis);
    return (it - m_liste_sequence_visionnages_ordonnee_chronologiquement.begin()) + 1; // +1 parce que les num�ro de s�quence commencent � 1
}*/

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Episode::Print(bool hors_saison)                                                                                                              #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Episode::Print(bool hors_saison)
{
    int numero_sequence = 1;

    for (const auto& sequence : m_liste_sequence_visionnages_ordonnee_chronologiquement)
    {
        sequence.Print(numero_sequence, hors_saison);
        //sequence.Print(numero_sequence, hors_saison, m_liste_sequence_visionnages_ordonnee_chronologiquement[0].m_numero);
        numero_sequence++;
    }
}

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

    //std::wregex filename_pattern{ L"(\\d{4})(?:-(\\d{2})(?:-(\\d{2}))?)?(?:\\s(.+))?" };
    std::wregex filename_pattern{ L"(\\d{4})(?:\\-(\\d{2})(?:-(\\d{2}))?)?(?:\\s(.+))?" };
    std::wsmatch match;
    std::tm tm{ -1, -1, -1, -1, -1, -1, -1, -1, -1 };
    if (std::regex_match(nomDossier, match, filename_pattern))
    {
        std::wstring year = match[1];
        auto y = std::stoi(year);
        assert(1582 <= y && L"L'ann�e est inf�rieur !");
        auto m = 0;
        tm.tm_year = y - 1900;
        if (match[2].matched)
        {
            std::wstring mon = match[2];
            auto m = std::stoi(mon);
            assert((1 <= m && m <= 12) && L"Le mois invalide !");
            tm.tm_mon = m - 1;

            if (match[3].matched)
            {
                std::wstring mday = match[3];
                auto d = std::stoi(mday);
                assert((1 <= d && d <= 31) && L"Le jour invalide !");
                if (!checkday(m, d, y))
                {
                    std::wcout << L"Le jour invalide !!!" << std::endl;
                    exit(1);
                }
                tm.tm_mday = d;
            }
        }
    }
    else
    {
        assert(false == true && "Le nom du r�pertoire n'est pas un nom valide.");
    }
    if (match[4].matched)
    {
        m_date_diffusee_a_partir_de.second = match[4];
    }
    m_date_diffusee_a_partir_de.first = tm;
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
            // AD
            if (nomFichier == L"AD.txt")
            {
                initialiser_Audiodescription(cheminFichier, m_audiodescription);
            }
            // Avec
            if (nomFichier == L"Avec.txt")
            {
                m_avec = extraire_Liste_De_Paire_Depuis_Contenu_Fichier(cheminFichier);
            }
            // Cha�ne d'origine
            if (nomFichier == L"Cha�ne d'origine.txt")
            {
                initialiser_Chaine(cheminFichier);
            }
            // Disney+
            if (nomFichier == L"Disney+.txt")
            {
                std::wcout << L"Disney+.txt" << L"\r\n";
            }
            // DVD
            if (nomFichier == L"DVD.txt")
            {
                //D_DVD[I] = true;
            }
            // Hors saison
            if (nomFichier == L"Hors saison.txt")
            {
                initialiser_Hors_Saison(cheminFichier);
            }
            // Netflix
            if (nomFichier == L"Netflix.txt")
            {
                initialiser_Netflix(cheminFichier);
            }
            // Note
            if (nomFichier == L"Note.txt")
            {
                initialiser_Note(cheminFichier);
            }
            // Saison
            if (nomFichier == L"Saison.txt")
            {
                initialiser_Saison(cheminFichier);
            }
            // Titre
            if (nomFichier == L"Titre.txt")
            {
                initialiser_Titre(cheminFichier);
            }
            return;
        }
        //
        if (std::regex_match(nomFichier, std::wregex{ L"([[:digit:]]{1,2})x(.)+" }))
        {
            InfosVisionnage info_vis{ *this, cheminFichier };
            auto it = std::find_if(m_liste_episodes.begin(), m_liste_episodes.end(), [&info_vis](const auto& x) { return x.m_numero == info_vis.m_NumeroEpisodeDansSaison; });
            if (it != m_liste_episodes.end())
            {
                (*it).ajouter_SequenceVisionnage(info_vis);
            }
            else
            {
                m_liste_episodes.emplace_back(Episode{ info_vis });
            }
            return;
        }
        //
        if (int j = std::stoi(nomFichier))
        {
            initialiser_Resume(cheminFichier);
            return;
        }
        // Erreur !
        if (nomFichier != L"")
        {
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
        std::wcout << m_keyColor[2] << L'{' << cheminFichier << L'}' << m_valuesColor << std::endl;
    }
}

// ######################################################################################################################################################

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::initialiser_Chaine(fs::path const& cheminFichier)                                                                                     #
// #                                                                                                                                                    #
// ######################################################################################################################################################

 void Saison::initialiser_Chaine(fs::path const& cheminFichier)
 { // Cha�ne
     auto nomFichier = cheminFichier.filename().wstring();
     assert(nomFichier.length() > 0 && L"Nom de fichier vide");
     m_chaine = lire_fichierTxt(cheminFichier.wstring());
     assert((m_chaine.size() != 0));
 }

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::initialiser_Hors_Saison(std::filesystem::path const& cheminFichier)                                                                   #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::initialiser_Hors_Saison(std::filesystem::path const& cheminFichier)
{ // Hors Saison
//    auto nomFichier = cheminFichier.filename().wstring();
//    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
//    m_hors_saison = true;

 //   m_non_hors_saison = false;

    /*std::vector<std::wstring> saison = lire_fichierTxt(cheminFichier.wstring(), {L"\n"});
    if (saison.size() < 2)
        throw FileFormatException("Le fichier " + cheminFichier.generic_u8string() + " n'a pas le nombre de ligne n�cessaire pour une description de saison : " + std::to_string(saison.size()));

    try
    {
        m_nombre_episodes = std::stoi(saison[0]);
    }
    catch (exception& ex)
    {
        throw FileFormatException("Le fichier " + cheminFichier.generic_u8string() + " n'a pas le nombre d'�pisode de la saison en premi�re ligne : " + ex.what());
    }

    if (m_nombre_episodes < 0 || m_nombre_episodes>99)
        throw FileFormatException("Le fichier " + cheminFichier.generic_u8string() + " n'a pas un nombre d'�pisode de saison compris entre 0 et 99 : " + std::to_string(m_nombre_episodes));

    m_resume = std::vector<std::wstring>(std::next(saison.begin()), saison.end());
    */

    //std::vector<std::wstring> hors_saison = lire_fichierTxt(cheminFichier.wstring(), { L"\n" });
    std::vector<std::wstring> saison = lire_fichierTxt(cheminFichier.wstring(), { L"\n" });
    if (saison.size() < 2)
        throw FileFormatException("Le fichier " + cheminFichier.generic_u8string() + " n'a pas le nombre de ligne n�cessaire pour une description de saison : " + std::to_string(saison.size()));

    try
    {
        m_nombre_episodes = std::stoi(saison[0]);
    }
    catch (exception& ex)
    {
        throw FileFormatException("Le fichier " + cheminFichier.generic_u8string() + " n'a pas le nombre d'�pisode de la saison en premi�re ligne : " + ex.what());
    }

    if (m_nombre_episodes < 0 || m_nombre_episodes>99)
        throw FileFormatException("Le fichier " + cheminFichier.generic_u8string() + " n'a pas un nombre d'�pisode de saison compris entre 0 et 99 : " + std::to_string(m_nombre_episodes));

    m_resume = std::vector<std::wstring>(std::next(saison.begin()), saison.end());
    m_hors_saison = true;
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
    m_nombre_episodes = std::stoi(cheminFichier.filename().stem().wstring());
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::initialiser_Saison(std::filesystem::path const& cheminFichier)                                                                        #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::initialiser_Saison(std::filesystem::path const& cheminFichier)
{
    std::vector<std::wstring> saison = lire_fichierTxt(cheminFichier.wstring(), { L"\n" });
    if (saison.size() < 2)
        throw FileFormatException("Le fichier " + cheminFichier.generic_u8string() + " n'a pas le nombre de ligne n�cessaire pour une description de saison : " + std::to_string(saison.size()));

    try
    {
        m_nombre_episodes = std::stoi(saison[0]);
    }
    catch (exception& ex)
    {
        throw FileFormatException("Le fichier " + cheminFichier.generic_u8string() + " n'a pas le nombre d'�pisode de la saison en premi�re ligne : " + ex.what());
    }

    if (m_nombre_episodes < 0 || m_nombre_episodes>99)
        throw FileFormatException("Le fichier " + cheminFichier.generic_u8string() + " n'a pas un nombre d'�pisode de saison compris entre 0 et 99 : " + std::to_string(m_nombre_episodes));

    m_resume = std::vector<std::wstring>(std::next(saison.begin()), saison.end());
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

    m_titres = extraire_Titres_Depuis_UneLigne(titre[0]);
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::AffichagePersonnaliser(AffichagePersonnalisation perso)                                                                               #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::AffichagePersonnaliser(AffichagePersonnalisation perso)
{
    m_espace1 = perso.m_espace1;
    m_espace2 = perso.m_espace2;
    m_espace3 = perso.m_espace3;
    m_labelHeureSingulier = perso.m_labelsHeure.first;
    m_labelHeurePluriel = perso.m_labelsHeure.second;
    m_labelMinuteSingulier = perso.m_labelsMinute.first;
    m_labelMinutePluriel = perso.m_labelsMinute.second;

    m_keyColor = perso.m_keyColor;
    m_valuesColor = perso.m_valuesColor;

    for (auto& episode : m_liste_episodes)
    {
        episode.AffichagePersonnaliser(perso);
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::Print()                                                                                                                               #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::Print()
{
    Print_Header();

    for (auto& episode : m_liste_episodes)
    {
        episode.Print(m_hors_saison);
    }

    // Note
    Print_Note();    
    // Cha�ne
    Print_Chaine();
    // Netflix
    Print_Netflix();
    // AD
    Print_CleValeur(L"Audiodescription", m_audiodescription, m_keyColor[1], m_valuesColor);
    // Avec
    Print_Avec();
    // Images(s)
    Print_Images();
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
    if (m_avec.size())
    {
        std::wstring avec_str = m_keyColor[1] + L"Avec : " + m_valuesColor;
        bool found = false;
        for (auto&& [nom, role] : m_avec)
        {
            if (nom == L"�" || nom == L"..." || nom == L".")
            {
                found = true;
                break;
            }
            if (nom != L"" && role != L"")
            {
                avec_str += nom + L' ' + m_keyColor[1] + L'(' + m_valuesColor + role + m_keyColor[1] + L')' + m_valuesColor;
            }
            else if (nom == L"" && role != L"")
            {
                avec_str += m_keyColor[1] + L'(' + m_valuesColor + role + m_keyColor[1] + L')' + m_valuesColor;
            }
            else
            {
                avec_str += nom;
            }
            if (m_avec.back().first != nom)
                avec_str += m_keyColor[1] + L", " + m_valuesColor;
            else
                avec_str += m_keyColor[1] + L'.' + m_valuesColor;
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
    if (m_chaine.size() > 0)
    {
        std::wstring chaine_str = m_keyColor[1] + L"Cha�ne d'origine : " + m_valuesColor + m_chaine + L"\r\n";
        //PrintStringW(m_hOut, creee_par_str, 0);
        //PrintStringW(HANDLE hOut, creee_par_str);
        //Console_Lire(chaine_str, 0, 0);
        //Console_Lire(hOut, chaine_str, 0, L' ');
        std::wcout << chaine_str;
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::Print_Header()                                                                                                                        #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::Print_Header()
{
    if (!m_hors_saison)
        Print_Header_Saison();
    else
        Print_Header_Hors_Saison();
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::Print_Header_Hors_Saison()                                                                                                            #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::Print_Header_Hors_Saison()
{
    std::wstring saison_str = m_keyColor[0] + L"Hors saison :" + m_valuesColor + L"\r\n";

    wchar_t date_tab[15];
    std::wstring date_str{};
    if (m_date_diffusee_a_partir_de.first.tm_mday == -1 && m_date_diffusee_a_partir_de.first.tm_mon == -1)
    {
        std::wcsftime(date_tab, 15, L"%Y", &m_date_diffusee_a_partir_de.first);
        std::wstring date_tab_str = date_tab;
        date_str = date_tab_str;
    }
    else if (m_date_diffusee_a_partir_de.first.tm_mday == -1)
    {
        std::wcsftime(date_tab, 15, L"%m/%Y", &m_date_diffusee_a_partir_de.first);
        std::wstring date_tab_str = date_tab;
        date_str = date_tab_str.substr(0, 2) + m_keyColor[1] + L'/' + m_valuesColor + date_tab_str.substr(3, 4);
    }
    else
    {
        std::wcsftime(date_tab, 15, L"%d/%m/%Y", &m_date_diffusee_a_partir_de.first);
        std::wstring date_tab_str = date_tab;
        date_str = date_tab_str.substr(0, 2) + m_keyColor[1] + L'/' + m_valuesColor + date_tab_str.substr(3, 2) + m_keyColor[1] + L'/' + m_valuesColor + date_tab_str.substr(6, 4);
    }

    std::wstring media_str;
    if (m_date_diffusee_a_partir_de.second != L"")
        media_str = m_keyColor[0] + m_date_diffusee_a_partir_de.second + m_valuesColor + L' ';

    std::wstring titre_str;
    if (m_titres.size() != 0)
    {
        titre_str = m_keyColor[0] + m_titres[0] + m_valuesColor;
        if (m_titres.size() > 1)
        {
            titre_str += m_keyColor[1] + m_titres[1] + m_valuesColor + m_keyColor[0] + m_titres[2] + m_valuesColor;
        }
        titre_str += m_keyColor[1] + L" : " + m_valuesColor;
    }

    std::wstring resume_str;
    for (auto r : m_resume)
        resume_str += ((resume_str.size() == 0) ? L"" : L"\r\n") + r;

    std::wstring nombre_episodes_str;
    if (m_nombre_episodes >= 0)
        nombre_episodes_str = m_keyColor[1] + L" (" + m_valuesColor + std::to_wstring(m_nombre_episodes) + m_keyColor[1] + L')' + m_valuesColor;

    std::wcout << saison_str << date_str << media_str << m_keyColor[1] + L" : " + m_valuesColor << titre_str << resume_str << nombre_episodes_str << L"\r\n";
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::Print_Header_Saison()                                                                                                                 #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::Print_Header_Saison()
{
    std::wstring saison_str = m_keyColor[0] + L"Saison " + m_keyColor[1] + std::to_wstring(m_numero) + m_keyColor[0] + L" :" + m_valuesColor + L"\r\n";

    std::wstring titre_str;
    if (m_titres.size() != 0)
    {
        titre_str = m_keyColor[0] + m_titres[0] + m_valuesColor;
        if (m_titres.size() > 1)
        {
            titre_str += m_keyColor[1] + m_titres[1] + m_valuesColor + m_keyColor[0] + m_titres[2] + m_valuesColor;
        }
        titre_str += m_keyColor[1] + L" : " + m_valuesColor;
    }

    std::wstring resume_str;
    for (auto r : m_resume)
        resume_str += ((resume_str.size() == 0) ? L"" : L"\r\n") + r;

    std::wstring nombre_episodes_str;
    if (m_nombre_episodes >= 0)
        nombre_episodes_str = m_keyColor[1] + L" (" + m_valuesColor + std::to_wstring(m_nombre_episodes) + m_keyColor[1] + L')' + m_valuesColor;

    std::wcout << saison_str << m_valuesColor << titre_str << resume_str << nombre_episodes_str << L"\r\n";
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::Print_Images()                                                                                                                        #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::Print_Images()
{
    if (m_image.size() > 0 )
    {
        std::wstring image_str = m_keyColor[1] + L"Image" + ((m_image.size() > 1) ? L"s" : L"") + L" : [" + m_valuesColor;
        bool first = true;
        for (auto&& i : m_image)
        {
            if (!first)
            {
                image_str += m_keyColor[1] + L"], [" + m_valuesColor;
            }
            image_str += i;
            first = false;
        }
        image_str += m_keyColor[1] + L']' + m_valuesColor + L"\r\n";
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
    if (m_netflix)
    {
        std::wstring netflix_str = m_keyColor[1] + L'(' + m_valuesColor + L"Netflix" + m_keyColor[1] + L')' + m_valuesColor;
        netflix_str += L"\r\n";
        std::wcout << netflix_str;
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # Saison::Print_Note()                                                                                                                               #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::Print_Note()
{ // 0...5 ou -1
    if (m_note)
    {
        std::wstring note_str;
        if (m_note == -1.0)
        {
            note_str = m_keyColor[1] + L'(' + m_valuesColor + L"Pas de note !" + m_keyColor[1] + L')' + m_valuesColor;
        }
        else if (m_note == 0 || m_note == 1 || m_note == 2 || m_note == 3 || m_note == 4 || m_note == 5)
        {
            note_str += m_keyColor[1] + L"Note : " + m_valuesColor;
            note_str += std::to_wstring(static_cast<int>(std::floor(m_note)));
            note_str += m_keyColor[1] + L"/5" + m_valuesColor;
        }
        else
        {
            note_str += m_keyColor[1] + L"Note : " + m_valuesColor;
            std::wstring wstr = std::to_wstring(m_note);
            wstr = wstr[0] + m_keyColor[1] + wstr[1] + m_valuesColor + wstr[2];
            note_str += wstr;
            note_str += m_keyColor[1] + L"/5" + m_valuesColor;
        }
        note_str += L"\r\n";
        std::wcout << note_str;
    }
}

// ######################################################################################################################################################
// ######################################################################################################################################################

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # Serie::Serie(std::filesystem::path racine)                                                                                                         #
// #                                                                                                                                                    #
// ######################################################################################################################################################

Serie::Serie(std::filesystem::path racine)
{
    this->racine = racine;
    auto nomDossier = racine.filename().wstring();
    assert(nomDossier.length() > 0 && L"Nom de dossier vide");

    std::wregex filename_pattern{ L"^(.+?)(?:\\.\\[(\\d{4}\\-\\d{4}\\s|\\d{4}\\-\\d{4}|\\d{4}\\-\\s|\\d{4}\\s|\\d{4})([^\\]]*)\\])(?:\\.(.+))?$|^(.+)(?:\\.(.+))$" };
    std::wsmatch match;
    if (std::regex_match(nomDossier, match, filename_pattern))
    {
        std::wstring titres = match[1];
        m_titres = extraire_Titres_Depuis_NomDeFichierOuDeRepertoire(titres);
        if (match[2].matched)
        {
            std::wstring annees_str = match[2].str();
            std::wsmatch dummy;
            if (std::regex_match(annees_str, dummy, std::wregex(L"\\d{4}\\-\\d{4}\\s?")))
            {
                m_f_anneesProduction.first = stoi(annees_str);
                m_f_anneesProduction.second = stoi(annees_str.substr(5));
            }
            else if (std::regex_match(annees_str, dummy, std::wregex(L"\\d{4}\\s?")))
            {
                m_f_anneesProduction.first = stoi(annees_str);
                m_f_anneesProduction.second = stoi(annees_str);
            }
            else
            {
                m_f_anneesProduction.first = stoi(annees_str);
            }
        }

        m_sur = (match[3].matched) ? match[3].str() : L"";
        m_sous_genre = (match[4].matched) ? match[4].str() : L"";
    }
    else
    {
        assert(false == true && "Le nom du r�pertoire n'est pas un nom valide.");
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::pair<int, int> Serie::calculer_Annees_Diffusion() const                                                                                       #
// #                                                                                                                                                    #
// ######################################################################################################################################################

std::pair<int, int> Serie::calculer_Annees_Diffusion() const
{
    return make_pair<int, int>(m_f_anneesProduction.first.value_or(0), m_f_anneesProduction.second.value_or(0));
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::wstring Serie::calcul_Duree_affichage() const                                                                                                 #
// #                                                                                                                                                    #
// ######################################################################################################################################################

std::wstring Serie::calcul_Duree_affichage() const
{
    return miseEnFormeDuree(m_duree_en_seconde, m_labelMinuteSingulier, m_labelMinutePluriel, m_keyColor[0], m_valuesColor, L" (", L")", m_espace3);
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::wstring Serie::calcul_Note_Affichage() const                                                                                                  #
// #                                                                                                                                                    #
// ######################################################################################################################################################

std::wstring Serie::calcul_Note_Affichage() const
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
        res = m_keyColor[0] + L'(' + m_valuesColor + L"pas de note !" + m_keyColor[0] + L')';
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

        res = whole_str + m_keyColor[1] + sepDecimal + m_valuesColor + fractional_str + m_keyColor[0] + L"/5";
    }
    return (res.length() > 0) ? L" " + res + m_valuesColor : L"";
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::wstring Serie::calcul_Signaletique_Jeunesse_affichage() const                                                                                 #
// #                                                                                                                                                    #
// ######################################################################################################################################################

std::wstring Serie::calcul_Signaletique_Jeunesse_affichage() const
{
    // SJ
    std::wstring signaletique_jeunesse_str;
    if (m_sj.length() != 0)
        signaletique_jeunesse_str = m_keyColor[0] + L" (" + m_valuesColor + L"SJ" + m_keyColor[1] + L" : " + m_valuesColor + m_sj + m_keyColor[0] + L')' + m_valuesColor;
    return signaletique_jeunesse_str;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::wstring Serie::calcul_Sur_Affichage() const                                                                                                   #
// #                                                                                                                                                    #
// ######################################################################################################################################################

std::wstring Serie::calcul_Sur_Affichage() const
{
    std::wstring sur_str;
    // sur
    sur_str = m_sur;
    return sur_str;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::wstring Serie::calcul_Titres_Affichage() const                                                                                                #
// #                                                                                                                                                    #
// ######################################################################################################################################################

std::wstring Serie::calcul_Titres_Affichage() const
{
    std::wstring titres_str;
    titres_str = m_keyColor[0] + L"Titre : " + m_valuesColor + m_titres[0];
    if (m_titres.size() == 3)
        titres_str += m_keyColor[1] + m_titres[1] + m_valuesColor + m_titres[2];
    return titres_str;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::wstring Serie::calcul_X_Signaletique_Jeunesse_affichage(std::wstring& sur) const                                                              #
// #                                                                                                                                                    #
// ######################################################################################################################################################

std::wstring Serie::calcul_X_Signaletique_Jeunesse_affichage(std::wstring& sur) const
{
    std::wstring x_signaletique_jeunesse_str;
    if (m_disney_sj != L"" && sur != L"Disney+")
        x_signaletique_jeunesse_str += m_keyColor[0] + L" (" + m_valuesColor + L"Disney+" + m_keyColor[1] + L" : " + m_valuesColor + m_disney_sj + m_keyColor[0] + L')' + m_valuesColor;
    if (m_netflix_sj != L"" && sur != L"Netflix")
        x_signaletique_jeunesse_str += m_keyColor[0] + L" (" + m_valuesColor + L"Netflix" + m_keyColor[1] + L" : " + m_valuesColor + m_netflix_sj + m_keyColor[0] + L')' + m_valuesColor;
    return x_signaletique_jeunesse_str;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Serie::corriger_Annee_Debut()                                                                                                                 #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Serie::corriger_Annee_Debut()
{
    assert((m_f_anneesProduction.first || (saisons.size() > 0 && saisons[0].m_f_anneesDiffusion)) && L"Il faut au moins une date de d�but.");

    if (!m_f_anneesProduction.first || (saisons.size() > 0 && saisons[0].m_f_anneesDiffusion && m_f_anneesProduction.first > saisons[0].m_f_anneesDiffusion))
        m_f_anneesProduction.first = saisons[0].m_f_anneesDiffusion;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Serie::corriger_Annee_Fin()                                                                                                                   #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Serie::corriger_Annee_Fin()
{
    assert((m_f_anneesProduction.first || (saisons.size() > 0 && saisons.back().m_f_anneesDiffusion)) && L"Il faut au moins une date de fin.");

    if (!m_f_anneesProduction.first || (saisons.size() > 0 && saisons.back().m_f_anneesDiffusion && m_f_anneesProduction.first > saisons.back().m_f_anneesDiffusion))
        m_f_anneesProduction.first = saisons[0].m_f_anneesDiffusion;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::wstring Serie::format_Annees() const                                                                                                          #
// #                                                                                                                                                    #
// ######################################################################################################################################################

std::wstring Serie::format_Annees() const
{
    if (m_f_anneesProduction.first && m_f_anneesProduction.second)
    {
        if (m_f_anneesProduction.first == m_f_anneesProduction.second)
        {
            return m_valuesColor + std::to_wstring(m_f_anneesProduction.first.value());
        }
        else
        {
            return m_valuesColor + std::to_wstring(m_f_anneesProduction.first.value()) + m_keyColor[1] + L'-' + m_valuesColor + std::to_wstring(m_f_anneesProduction.second.value());
        }
    }
    else if (m_f_anneesProduction.first && !(m_f_anneesProduction.second))
    {
        return m_valuesColor + std::to_wstring(m_f_anneesProduction.first.value()) + m_keyColor[1] + L'-';
    }
    else
    {
        std::pair<int, int> anneesDiffusion = calculer_Annees_Diffusion();
        return std::to_wstring(anneesDiffusion.first) + m_keyColor[1] + L'-' + m_valuesColor + std::to_wstring(anneesDiffusion.second);
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::wstring Serie::format_AnneesEtSur(std::wstring& annees_str, std::wstring& sur_str) const                                                      #
// #                                                                                                                                                    #
// ######################################################################################################################################################

std::wstring Serie::format_AnneesEtSur(std::wstring& annees_str, std::wstring& sur_str) const
{
    const std::wstring crochet_ouvrant_str = m_keyColor[0] + L" [" + m_valuesColor;
    const std::wstring crochet_fermant_str = m_keyColor[0] + L"]" + m_valuesColor;
    const wchar_t espace_str = L' ';
    std::wstring annees_et_sur = crochet_ouvrant_str;
    annees_et_sur += annees_str;
    if (sur_str == L"-" || sur_str == L"")
        annees_et_sur += L"";
    else if (sur_str == L"Disney+" && m_disney_sj.size() != 0)
        annees_et_sur += espace_str + m_keyColor[0] + m_valuesColor + m_keyColor[1] + L"sur " + m_valuesColor + sur_str + m_keyColor[1] + L" : " + m_valuesColor + m_disney_sj;
    else if (sur_str == L"Netflix" && m_netflix_sj.size() != 0)
        annees_et_sur += espace_str + m_keyColor[0] + m_valuesColor + m_keyColor[1] + L"sur " + m_valuesColor + sur_str + m_keyColor[1] + L" : " + m_valuesColor + m_netflix_sj;
    else if (sur_str == L"Paramount+" && m_paramount_sj.size() != 0)
        annees_et_sur += espace_str + m_keyColor[0] + m_valuesColor + m_keyColor[1] + L"sur " + m_valuesColor + sur_str + m_keyColor[1] + L" : " + m_valuesColor + m_paramount_sj;
    else if (sur_str.size() != 0)
        annees_et_sur += espace_str + m_keyColor[0] + m_valuesColor + m_keyColor[1] + L"sur " + m_valuesColor + sur_str;
    else
        ; // ???
    return annees_et_sur + crochet_fermant_str;
}

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
        // Cha�ne d'origine
        if (nomFichier == L"Cha�ne d'origine.txt")
        {
            initialiser_Chaine(cheminFichier);
        }
        // Cr��e par
        if (nomFichier == L"Cr��e par.txt")
        {
            initialiser_Creee_par(cheminFichier);
        }
        // Disney+ SJ
        if (nomFichier == L"Disney+.txt")
        {
            m_disney_sj = recuperer_Disney_SJ(cheminFichier, m_catalogue);
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
        // Nationalit�
        if (nomFichier == L"Nationalit�.txt")
        {
            initialiser_Nationalite(cheminFichier, m_nationalite, ::Nationalite);
        }
        // Netflix
        if (nomFichier == L"Netflix.txt")
        {
            m_netflix_sj = recuperer_Netflix_SJ(cheminFichier, m_catalogue);
        }
        // SJ
        if (nomFichier == L"SJ.txt")
        {
            m_sj = recuperer_SJ(cheminFichier);
        }
        // Titre
        if (nomFichier == L"Titre.txt")
        {
            initialiser_Titre(cheminFichier);
        }
        // Titre original
        if (nomFichier == L"Titre original.txt")
        {
            m_titres_originaux = extraire_Titres_Depuis_UnFichier(cheminFichier);
        }
        return;
    }
    else if(nomImage == L".jpg" || nomImage == L".png" || nomImage == L".webp")
        // Image
    {
        initialiser_Image(cheminFichier, m_image);
    }
    else
    {
        std::wcout << m_keyColor[2] << L'{' << cheminFichier << L'}' << m_valuesColor << std::endl;
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Serie::initialiser_Chaine(fs::path const& cheminFichier)                                                                                      #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Serie::initialiser_Chaine(fs::path const& cheminFichier)
{ // Cha�ne
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
// # Serie::void Serie::initialiser_Titre(fs::path const& cheminFichier                                                                                 #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Serie::initialiser_Titre(fs::path const& cheminFichier)
{
    auto res = extraire_Informations_DepuisLeContenuDUnFichier(cheminFichier);
    m_titres = fusionner_Titres(m_titres, std::get<0>(res));
    m_duree_en_seconde = std::get<1>(res) ? std::get<1>(res).value() : -1;
    m_resume = std::get<2>(res);
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Serie::AffichagePersonnaliser(AffichagePersonnalisation perso)                                                                                #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Serie::AffichagePersonnaliser(AffichagePersonnalisation perso)
{
    m_espace1 = perso.m_espace1;
    m_espace2 = perso.m_espace2;
    m_espace3 = perso.m_espace3;
    m_labelHeureSingulier = perso.m_labelsHeure.first;
    m_labelHeurePluriel = perso.m_labelsHeure.second;
    m_labelMinuteSingulier = perso.m_labelsMinute.first;
    m_labelMinutePluriel = perso.m_labelsMinute.second;

    m_keyColor = perso.m_keyColor;
    m_valuesColor = perso.m_valuesColor;

    for (auto& saison : saisons)
    {
        saison.AffichagePersonnaliser(perso);
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Serie::PostTraitement()                                                                                                                       #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Serie::PostTraitement()
{
    //    std::list<Saison> m_hors_saisons;
    //    std::list<Saison> m_non_hors_saisons;
    for (auto& saison : saisons)
    {
        if (saison.m_hors_saison)
        {
            
            m_hors_saisons.emplace_back(move(saison));
        }
        else
        {
            m_non_hors_saisons.emplace_back(move(saison));
        }
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Serie::Print()                                                                                                                                #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Serie::Print()
{
    PostTraitement();
    // Header
    Print_Header();
    // Titre Original
    Print_Titre_Original(m_titres_originaux, m_keyColor, m_valuesColor);
    // Cha�ne d'origine
    Print_Chaine();
    // AD
    Print_CleValeur(L"Audiodescription", m_audiodescription, m_keyColor[0], m_valuesColor);
    // Creee par
    Print_Creee_par();
    // Genre(s)
    Print_Genres(m_genre, m_sous_genre, m_keyColor[0], m_valuesColor);
    // En relation avec
    Print_En_relation_avec();
    // Avec etc
    Print_Avec_etc();
    // Nationalit�(s)
    Print_Nationalites(m_nationalite, m_keyColor[0], m_valuesColor);
    // Image(s)
    Print_Images(m_image, m_keyColor[0], m_valuesColor);
    // Catalogue
    Print_Catalogue(m_sur, m_catalogue, m_keyColor[0], m_valuesColor);
    // Resume
    Print_Resume(m_resume);
    std::wcout << L"\r\n";
    // Saison(s)
    Print_Saisons();
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::wostream& Serie::PrintSerieOnOutstream(const Serie& serie, std::wostream& out)                                                                #
// #                                                                                                                                                    #
// ######################################################################################################################################################

/*std::wostream& Serie::PrintSerieOnOutstream(const Serie& serie, std::wostream& out)
{
    serie.Print();
    return out;
}*/

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Serie::Print_Avec_etc()                                                                                                                       #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Serie::Print_Avec_etc()
{
    if (saisons.size() > 0 && saisons.back().m_avec.size() > 0)
    {
        std::wstring avec_str = m_keyColor[0] + L"Avec : " + m_valuesColor;

        int item_count = 0;
        for (auto&& [nom, role] : saisons.back().m_avec)
        {
            if (nom == L"�")
                break;
            if (nom != L"")
            {
                if (item_count > 0)
                {
                    avec_str += m_keyColor[0] + L", " + m_valuesColor;
                }
                avec_str += nom;
                item_count++;
                if (item_count > 2)
                    break;
            }
        }
        avec_str += L"\r\n";
        std::wcout << avec_str;
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Serie::Print_Chaine()                                                                                                                         #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Serie::Print_Chaine()
{
    if (m_chaine.size() > 0)
    {
        std::wstring chaine_str = m_keyColor[0] + L"Cha�ne d'origine : " + m_valuesColor;
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

void Serie::Print_Creee_par()
{
    if (m_creee_par.size() > 0)
    {
        std::wstring creee_par_str = m_keyColor[0] + L"Cr��e" + ((m_creee_par.size() > 1) ? L"s" : L"") + L" par : " + m_valuesColor;
        bool first = true;
        for (auto&& c : m_creee_par)
        {
            if (!first)
            {
                creee_par_str += m_keyColor[0] + L", " + m_valuesColor;
            }
            creee_par_str += c;
            first = false;
        }
        creee_par_str += L"\r\n";
        std::wcout << creee_par_str;
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Serie::Print_En_relation_avec()                                                                                                               #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Serie::Print_En_relation_avec()
{
    if (m_en_relation_avec.size() > 0)
    {
        std::wstring en_relation_avec_str = m_keyColor[0] + L"En relation avec : " + m_valuesColor;
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
// # void Serie::Print_Header() const                                                                                                                   #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Serie::Print_Header() const
{
    std::wstring titres_str = calcul_Titres_Affichage();
    std::wstring annees_str = format_Annees();
    std::wstring sur_str = calcul_Sur_Affichage();
    std::wstring anneesEtSur_str = format_AnneesEtSur(annees_str, sur_str);
    std::wstring x_signaletique_jeunesse_str = calcul_X_Signaletique_Jeunesse_affichage(sur_str);
    std::wstring signaletique_jeunesse_str = calcul_Signaletique_Jeunesse_affichage();
    std::wstring duree_str = calcul_Duree_affichage();
    std::wstring note_str = calcul_Note_Affichage();

    std::wcout << titres_str << anneesEtSur_str << x_signaletique_jeunesse_str << signaletique_jeunesse_str << duree_str << note_str << std::endl;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Serie::Print_Saison(Saison saison)                                                                                                            #
// #                                                                                                                                                    #
// ######################################################################################################################################################

/*void Serie::Print_Saison(Saison saison)
{
    saison.Print();
}*/

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Serie::Print_Saisons()                                                                                                                        #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Serie::Print_Saisons()
{
    for (auto& saison : m_hors_saisons)
    {
        saison.Print();
    }

    for (auto& saison : m_non_hors_saisons)
    {
        saison.Print();
    }
}
