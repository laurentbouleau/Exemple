#include "pch.h"

//#include "film_.h"
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

using namespace std;
namespace fs = std::filesystem;

using DateVisionnage = DateRecord;

// ######################################################################################################################################################
// ######################################################################################################################################################

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const std::wstring SequenceVisionnage_film::c_filenameFormat = L"^(\\d{4}\\-\\d{2}\\-\\d{2}.*)$";                                                  #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const std::wstring SequenceVisionnage_film::c_filenameFormat = L"^(\\d{4}\\-\\d{2}\\-\\d{2}.*)$";

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # SequenceVisionnage_film::SequenceVisionnage_film(fs::path const& m_cheminFichier)                                                                  #
// #                                                                                                                                                    #
// ######################################################################################################################################################

SequenceVisionnage_film::SequenceVisionnage_film(fs::path const& m_cheminFichier)
{
    // (((([[:digit:]]{4})-([[:digit:]]{2})-([[:digit:]]{2})|([[:digit:]]{2})-([[:digit:]]{2})|([[:digit:]]{2}))(_?))+)(\s(.+))?
    //const std::wstring date_year_month_day_format = L"([[:digit:]]{4})-([[:digit:]]{2})-([[:digit:]]{2})";
    //const std::wstring date_month_day_format = L"([[:digit:]]{2})-([[:digit:]]{2})";
    //const std::wstring date_day_format = L"([[:digit:]]{2})";
    //const std::wstring stream_format = L"(\\s(.+))?";
    //const std::wstring dates_format = L"((" + date_year_month_day_format + L"|" + date_month_day_format + L"|" + date_day_format + L")(_?))";

    // (((([[:digit:]]{4})-([[:digit:]]{2})-([[:digit:]]{2})|([[:digit:]]{2})-([[:digit:]]{2})|([[:digit:]]{2}))(_?))+)(\s(.+))?
    const std::wstring date_year_month_day_format = L"([[:digit:]]{4})-([[:digit:]]{2})-([[:digit:]]{2})";

    const std::wstring date_month_day_format = L"([[:digit:]]{2})-([[:digit:]]{2})";

    const std::wstring date_day_format = L"([[:digit:]]{2})";

    const std::wstring stream_format = L"(\\s(.+))?";

    const std::wstring dates_format = L"((" + date_year_month_day_format + L"|" + date_month_day_format + L"|" + date_day_format + L")(_?))";

    //const std::wstring dates_format = L"((" + date_year_month_day_format + L"|" + date_month_day_format + L"|" + date_day_format + L")" + stream_format;

    const int dates_full_match_index = 0;

    const int dates_date_year_month_day_year_index = dates_full_match_index + 3;

    const int dates_date_year_month_day_month_index = dates_date_year_month_day_year_index + 1;

    const int dates_date_year_month_day_day_index = dates_date_year_month_day_month_index + 1;

    const int dates_date_month_day_month_index = dates_date_year_month_day_day_index + 1;

    const int dates_date_month_day_day_index = dates_date_month_day_month_index + 1;

    const int dates_date_day_day_index = dates_date_month_day_day_index + 1;

    const int dates_someFlag_index = dates_date_day_day_index + 2;


    const int dates_full_match_index_f = 0;
    const int dates_date_year_month_day_year_index_f = dates_full_match_index + 3;
    const int dates_date_year_month_day_month_index_f = dates_date_year_month_day_year_index + 1;
    const int dates_date_year_month_day_day_index_f = dates_date_year_month_day_month_index + 1;
    const int dates_date_month_day_month_index_f = dates_date_year_month_day_day_index + 1;
    const int dates_date_month_day_day_index_f = dates_date_month_day_month_index + 1;
    const int dates_date_day_day_index_f = dates_date_month_day_day_index + 1;
    const int dates_someFlag_index_f = dates_date_day_day_index + 2;

    ///const std::wregex filename_format_rg{ L"(" + dates_format + L"+)" + stream_format };
 
    const int filename_dates_index = 0;
    const int filename_date_year_month_day_year_index = filename_dates_index + 2;
    const int filename_date_year_month_day_month_index = filename_date_year_month_day_year_index + 1;
    const int filename_date_year_month_day_day_index = filename_date_year_month_day_month_index + 1;
    const int filename_date_month_day_month_index = filename_date_year_month_day_day_index + 1;
    const int filename_date_month_day_day_index = filename_date_month_day_month_index + 1;
    const int filename_date_day_day_index = filename_date_month_day_day_index + 1;
    //const int filename_someFlag_index = filename_date_day_day_index + 2;
    const int filename_someFlag_index = filename_date_day_day_index + 2;
    //const int filename_stream_index = filename_someFlag_index + 2;
    //const int filename_stream_index = filename_someFlag_index + 3;
    const int filename_stream_index = filename_someFlag_index + 3;

    const std::wregex filename_format_rg{ L"(" + dates_format + L"+)" + stream_format };
    auto nomFichier = m_cheminFichier.wstring();

    assert(nomFichier.length() > 0 && L"Nom de fichier Film vide");

    auto stem = m_cheminFichier.stem().wstring();
    // prefixe ???
    assert(std::regex_match(stem, filename_format_rg) && L"Le nom du fichier n'est pas valide");
    std::wsmatch match;
    auto str = stem;
    //Exemple assez complexe de nom de fichier
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
            assert(true && L"format de date film inconnu.");
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
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::wstring SequenceVisionnage_film::Print_Dates_de_visionnage(std::vector<DateRecord>& m_DatesVisionnage,                                        #
// #                                                                 std::vector<std::wstring>&keyColor, std::wstring& valuesColor)                     #
// #                                                                                                                                                    #
// ######################################################################################################################################################

std::wstring SequenceVisionnage_film::Print_Dates_de_visionnage(std::vector<DateRecord>& m_DatesVisionnage, std::vector<std::wstring>& keyColor, std::wstring& valuesColor)
{
    m_keyColor = keyColor;
    m_valuesColor = valuesColor;
    const std::wstring date_format = L"%d" + m_keyColor[1] + L"/" + m_valuesColor + L"%m" + m_keyColor[1] + L"/" + m_valuesColor + L"%Y";
    const std::wstring between_parenthesis = m_keyColor[1] + L"(" + m_valuesColor + L"%s" + m_keyColor[1] + L")" + m_valuesColor;
    const std::wstring same_date_format = between_parenthesis;
    const std::wstring prequel_format = between_parenthesis;
    //const std::wstring streaming_format = m_keyColor[1] + L" : " + m_valuesColor + L"%s";
    const std::wstring streaming_format = L"%s : %s%s";
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
            dates_de_visionnage_wstr += m_keyColor[1] + L", " + m_valuesColor;
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
         //dates_de_visionnage_wstr += wstring_format(streaming_format, m_streaming.c_str());
         dates_de_visionnage_wstr += wstring_format(streaming_format, m_keyColor[1].c_str(), m_valuesColor.c_str(), m_streaming.c_str());
    //
    return dates_de_visionnage_wstr;
}

// ######################################################################################################################################################
// ######################################################################################################################################################

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # Film::Film(std::filesystem::path racine)                                                                                                           #
// #                                                                                                                                                    #
// ######################################################################################################################################################

Film::Film(std::filesystem::path racine)
{
    this->racine = racine;
    auto nomDossier = racine.filename().wstring();
    assert(nomDossier.length() > 0 && L"Nom de dossier vide");
    std::wregex filename_pattern{ L"(.+?)(?:\\.\\((?:(\\d{4})\\-(\\d{2})\\-(\\d{2})\\s*([^\\)]*))\\))?(?:\\.(.+))?$" };
    std::wsmatch match;
    if (std::regex_match(nomDossier, match, filename_pattern))
    {
        std::wstring part_1 = match[1];
        m_titres = extraire_Titres_Depuis_NomDeFichierOuDeRepertoire(part_1);
        if (match[2].matched)
        {
            std::wstring annees_str = match[2].str();
            std::wstring mois_str = match[3].str();
            std::wstring jours_str = match[4].str();
            m_date.tm_year = stoi(annees_str) - 1900;  //beurk!!
            m_date.tm_mon = stoi(mois_str) - 1;           //beurk!!
            m_date.tm_mday = stoi(jours_str);
        }
        m_sur = (match[5].matched) ? match[5].str() : L"";
        m_sous_genre = (match[6].matched) ? match[6].str() : L"";
    }
    else
    {
        assert(false == true && "Le nom du r�pertoire n'est pas un nom valide.");
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::wstring Film::calcul_Date_Affichage() const                                                                                                   #
// #                                                                                                                                                    #
// ######################################################################################################################################################

std::wstring Film::calcul_Date_Affichage() const
{
    std::wstring date_str;
    wchar_t date_string[15];
    std::wcsftime(date_string, 15, L"%d/%m/%Y", &m_date);
    date_str = date_string;
    date_str = date_str.substr(0, 2) + m_keyColor[0] + L'/' + m_valuesColor +
        date_str.substr(3, 2) + m_keyColor[0] + L'/' + m_valuesColor +
        date_str.substr(6, 4);
    return date_str;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::wstring Film::calcul_Duree_affichage() const                                                                                                  #
// #                                                                                                                                                    #
// ######################################################################################################################################################

std::wstring Film::calcul_Duree_affichage() const
{
    std::wstring duree_str;
    long heures = m_duree / (60 * 60);
    long minutes = (m_duree % (60 * 60)) / 60;
    long secondes = m_duree % 60;
    duree_str = L' ' + std::to_wstring(heures) + m_keyColor[0] + m_espace1 + (heures <= 1 ? m_labelHeureSingulier : m_labelHeurePluriel) + m_valuesColor + m_espace2 +
        std::to_wstring(minutes) + m_keyColor[0] + m_espace3 + (minutes <= 1 ? m_labelMinuteSingulier : m_labelMinutePluriel) + m_valuesColor;
    return duree_str;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::wstring Film::calcul_Signaletique_Jeunesse_affichage() const                                                                                  #
// #                                                                                                                                                    #
// ######################################################################################################################################################

std::wstring Film::calcul_Signaletique_Jeunesse_affichage() const
{
    // SJ
    std::wstring signaletique_jeunesse_str;
    if (m_sj.length() != 0)
        signaletique_jeunesse_str = m_keyColor[0] + L" (" + m_valuesColor + L"SJ" + m_keyColor[1] + L" : " + m_valuesColor + m_sj + m_keyColor[0] + L')' + m_valuesColor;
    return signaletique_jeunesse_str;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::wstring Film::calcul_Sur_Affichage() const                                                                                                    #
// #                                                                                                                                                    #
// ######################################################################################################################################################

std::wstring Film::calcul_Sur_Affichage() const
{
    std::wstring sur_str;
    // sur
    sur_str = m_sur;
    return sur_str;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::wstring Film::calcul_X_Signaletique_Jeunesse_affichage(std::wstring& sur) const                                                               #
// #                                                                                                                                                    #
// ######################################################################################################################################################

std::wstring Film::calcul_X_Signaletique_Jeunesse_affichage(std::wstring& sur) const
{
    std::wstring x_signaletique_jeunesse_str;
    if (m_disney_sj != L"" && sur != L"Disney+")
        x_signaletique_jeunesse_str += m_keyColor[0] + L" (" + m_valuesColor + L"Disney+" + m_keyColor[1] + L" : " + m_valuesColor + m_disney_sj + m_keyColor[0] + L')' + m_valuesColor;
    if (m_netflix_sj != L"" && sur != L"Netflix")
        x_signaletique_jeunesse_str += m_keyColor[0] + L" (" + m_valuesColor + L"Netflix" + m_keyColor[1] + L" : " + m_valuesColor + m_netflix_sj + m_keyColor[0] + L')' + m_valuesColor;
    if (m_paramount_sj != L"" && sur != L"Paramount+")
        x_signaletique_jeunesse_str += m_keyColor[0] + L" (" + m_valuesColor + L"Paramount+" + m_keyColor[1] + L" : " + m_valuesColor + m_paramount_sj + m_keyColor[0] + L')' + m_valuesColor;
    return x_signaletique_jeunesse_str;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::wstring Film::calcul_Titres_Affichage() const                                                                                                 #
// #                                                                                                                                                    #
// ######################################################################################################################################################

std::wstring Film::calcul_Titres_Affichage() const
{
    std::wstring titres_str;
    titres_str = m_keyColor[0] + L"Titre : " + m_valuesColor + m_titres[0];
    if (m_titres.size() == 3)
        titres_str += m_keyColor[1] + m_titres[1] + m_valuesColor + m_titres[2];
    return titres_str;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::wstring Film::format_DateEtSur(std::wstring date_str, std::wstring& sur_str) const                                                            #
// #                                                                                                                                                    #
// ######################################################################################################################################################

std::wstring Film::format_DateEtSur(std::wstring date_str, std::wstring& sur_str) const
{
    const std::wstring parenthese_ouvrant_str = m_keyColor[0] + L" (" + m_valuesColor;
    const std::wstring parenthese_fermant_str = m_keyColor[0] + L")" + m_valuesColor;
    const wchar_t espace_str = L' ';
    std::wstring date_et_sur = parenthese_ouvrant_str;
    date_et_sur += date_str;
    if (sur_str == L"Disney+" && m_disney_sj.size() != 0)
        date_et_sur += espace_str + m_keyColor[0] + m_valuesColor + m_keyColor[1] + L"sur " + m_valuesColor + sur_str + m_keyColor[1] + L" : " + m_valuesColor + m_disney_sj;
    else if (sur_str == L"Netflix" && m_netflix_sj.size() != 0)
        date_et_sur += espace_str + m_keyColor[0] + m_valuesColor + m_keyColor[1] + L"sur " + m_valuesColor + sur_str + m_keyColor[1] + L" : " + m_valuesColor + m_netflix_sj;
    else if (sur_str.size() != 0)
        date_et_sur += espace_str + m_keyColor[0] + m_valuesColor + m_keyColor[1] + L"sur " + m_valuesColor + sur_str;
    else
        ;
    return date_et_sur + parenthese_fermant_str;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Film::initialiser_Fichier(fs::path const& cheminFichier)                                                                                      #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Film::initialiser_Fichier(fs::path const& cheminFichier)
{
    auto nomFichier = cheminFichier.filename().wstring();
    auto extensionFichier = cheminFichier.extension().wstring();

    if (nomFichier == L"Acteurs de doublage (Voix locales).txt")
    {
        m_acteurs_de_doublage_voix_locales = extraire_Liste_De_Paire_Depuis_Contenu_Fichier(cheminFichier);
    }
    else if (nomFichier == L"Acteurs de doublage (Voix originales).txt")
    {
        m_acteurs_de_doublage_voix_originales = extraire_Liste_De_Paire_Depuis_Contenu_Fichier(cheminFichier);
    }
    else if (nomFichier == L"AD.txt")
    {
        initialiser_Audiodescription(cheminFichier, m_audiodescription);
    }
    else if (nomFichier == L"Avec.txt")
    {
        m_avec = extraire_Liste_De_Paire_Depuis_Contenu_Fichier(cheminFichier);
    }
    else if (nomFichier == L"Date de reprise.txt")
    {
        initialiser_Date_de_reprise(cheminFichier); // ???
    }
    else if (nomFichier == L"Date de sortie.txt")
    {
        initialiser_Date_de_sortie(cheminFichier);// ???
    }
    else if (nomFichier == L"De.txt")
    {
        initialiser_De(cheminFichier);
    }
    else if (nomFichier == L"Disney+.txt")
    {
        m_disney_sj = recuperer_Disney_SJ(cheminFichier, m_catalogue);
    }
    else if (nomFichier == L"Distributeur.txt")
    {
        initialiser_Distributeur(cheminFichier);
    }
    else if (nomFichier == L"Genre.txt")
    {
        initialiser_Genre(cheminFichier, m_genre, ::Genre);
    }
    else if (nomFichier == L"Making-of.txt")
    {
        initialiser_Making_of(cheminFichier);
    }
    else if (nomFichier == L"Nationalit�.txt")
    {
        initialiser_Nationalite(cheminFichier, m_nationalite, ::Nationalite);
    }
    else if (nomFichier == L"Netflix.txt")
    {
        m_netflix_sj = recuperer_Netflix_SJ(cheminFichier, m_catalogue);
    }
    else if (nomFichier == L"Note.txt")
    {
        initialiser_Note(cheminFichier);
    }
    else if (nomFichier == L"Par.txt")
    {
        initialiser_Par(cheminFichier);
    }
    else if (nomFichier == L"SJ.txt")
    {
        m_sj = recuperer_SJ(cheminFichier);
    }
    else if (nomFichier == L"Soundtrack.txt")
    {
        initialiser_Soundtrack(cheminFichier);
    }
    else if (nomFichier == L"Titre.txt")
    {
        initialiser_Titre(cheminFichier);
    }
    else if (nomFichier == L"Titre original.txt")
    {
        m_titres_originaux = extraire_Titres_Depuis_UnFichier(cheminFichier);
    }
    else if (nomFichier == L"_you_.txt")
    {
    }
    else if (std::regex_match(nomFichier, std::wregex{ SequenceVisionnage_film::c_filenameFormat}))
    {
        SequenceVisionnage_film vis{cheminFichier};
        m_visionnages.push_back(vis);
    }
    else if (extensionFichier == L".jpg" || extensionFichier == L".png" || extensionFichier == L".webp")
    {
        initialiser_Image(cheminFichier, m_image);
    }
    else
    {
        assert(false && L"fichier inconnue");
    }
}


// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Film::initialiser_Date_de_reprise(fs::path const& cheminFichier)                                                                              #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Film::initialiser_Date_de_reprise(fs::path const& cheminFichier)
{
    auto nomFichier = cheminFichier.wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    std::wstring wstr = lire_fichierTxt(nomFichier);
    std::size_t pos = 0;

    wchar_t sp = L' ', tiret = L'-';
    auto y = std::stoi(wstr, &pos);
    assert(1582 <= y && L"L'ann�e est inf�rieur !");
    wstr = wstr.substr(4);
    try
    {
        test_date_tiret_sp_etc(wstr[0]);
    }
    catch (exception_date_tiret e2)
    {
        std::wcout << L"Exception a �t� captur�e : " << e2.get_message() << std::endl;
        exit(1);
    }
    wstr = wstr.substr(1);
    auto m = std::stoi(wstr, &pos);
    assert((1 <= m && m <= 12) && L"Le mois invalide !");
    wstr = wstr.substr(2);
    try
    {
        test_date_tiret_sp_etc(wstr[0]);
    }
    catch (exception_date_tiret e2)
    {
        std::wcout << L"Exception a �t� captur�e : " << e2.get_message() << std::endl;
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
    m_date_de_reprise = tm;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Film::initialiser_Date_de_sortie(std::filesystem::path const& cheminFichier)                                                                  #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Film::initialiser_Date_de_sortie(std::filesystem::path const& cheminFichier)
{
    auto nomFichier = cheminFichier.wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    std::wstring wstr = lire_fichierTxt(nomFichier);
    std::size_t pos = 0;

    wchar_t sp = L' ', tiret = L'-';
    auto y = std::stoi(wstr, &pos);
    assert(1582 <= y && L"L'ann�e est inf�rieur !");
    wstr = wstr.substr(4);
    try
    {
        test_date_tiret_sp_etc(wstr[0]);
    }
    catch (exception_date_tiret e2)
    {
        std::wcout << L"Exception a �t� captur�e : " << e2.get_message() << std::endl;
        exit(1);
    }
    wstr = wstr.substr(1);
    auto m = std::stoi(wstr, &pos);
    assert((1 <= m && m <= 12) && L"Le mois invalide !");
    wstr = wstr.substr(2);
    try
    {
        test_date_tiret_sp_etc(wstr[0]);
    }
    catch (exception_date_tiret e2)
    {
        std::wcout << L"Exception a �t� captur�e : " << e2.get_message() << std::endl;
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
    m_date_de_sortie = tm;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Film::initialiser_De(fs::path const& cheminFichier)                                                                                           #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Film::initialiser_De(fs::path const& cheminFichier)
{ // De
    auto nomFichier = cheminFichier.wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    m_de = lire_fichierTxt(nomFichier, { L"\n", L", " });
    assert((m_de.size() != 0));
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Film::initialiser_Distributeur(fs::path const& cheminFichier)                                                                                 #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Film::initialiser_Distributeur(fs::path const& cheminFichier)
{
    auto nomFichier = cheminFichier.wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    m_distributeur = lire_fichierTxt(nomFichier);
    assert((m_distributeur.size() != 0));
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Film::initialiser_Par(fs::path const& cheminFichier)                                                                                          #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Film::initialiser_Par(fs::path const& cheminFichier)
{ // Par
    auto nomFichier = cheminFichier.wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    m_par = lire_fichierTxt(nomFichier, { L"\n", L", " });
    assert((m_par.size() != 0));
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Film::initialiser_Making_of(std::filesystem::path const& cheminFichier)                                                                       #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Film::initialiser_Making_of(std::filesystem::path const& cheminFichier)
{
    auto nomFichier = cheminFichier.wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    std::vector<std::wstring> contenu = lire_fichierTxt(cheminFichier.wstring(), { L"\n" });
    assert((contenu.size() != 0));
    if (contenu.size() > 0)
    {
        m_making_of_duree = extraire_Duree_DepuisUneLigneDUnFichier(contenu[0]).value();
        contenu.erase(contenu.begin());
        if (contenu.size() > 0)
        {
            m_making_of_resume = contenu;
            m_making_of = true;
        }
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Film::initialiser_Note(fs::path const& cheminFichier)                                                                                         #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Film::initialiser_Note(fs::path const& cheminFichier)
{ // 0...5 ou -1
    auto nomFichier = cheminFichier.wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    std::wstring n = lire_fichierTxt(nomFichier);
    std::size_t pos = n.length();
    if (n == L"")
    {
        m_note = -1.0;
        return;
    }
    if (pos != 1 && pos != 3)
    {
        return;
    }
    if (!iswdigit(n[0]))
    {
        return;
    }
    if (!(n[0] == L'0' || n[0] == L'1' || n[0] == L'2' || n[0] == L'3' || n[0] == L'4' || n[0] == L'5'))
    {
        return;
    }
    if (pos == 1)
        m_note = std::stod(n);
    else
    {
        if ((n[1] == L',' || n[1] == L'.') && iswdigit(n[2]) && pos == 3)
        {
            n[1] = L',';
            m_note = std::stod(n);
        }
        else
        {
            return;
        }
    }
    return;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Film::initialiser_Soundtrack(fs::path const& cheminFichier)                                                                                   #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Film::initialiser_Soundtrack(fs::path const& cheminFichier)
{
    auto nomFichier = cheminFichier.wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    m_soundtrack = lire_paireCleValeur_depuisFichierTxt(nomFichier, L"[[:blank:]]\\:[[:blank:]]");
    assert((m_soundtrack.size() != 0));
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Film::initialiser_Titre(fs::path const& cheminFichier, std::vector<std::wstring>& ligne)                                                      #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Film::initialiser_Titre(fs::path const& cheminFichier)
{
    auto res = extraire_Informations_DepuisLeContenuDUnFichier(cheminFichier);
    m_titres = fusionner_Titres(m_titres, std::get<0>(res));
    m_duree = std::get<1>(res) ? std::get<1>(res).value() : -1;
    m_resume = std::get<2>(res);
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Film::AffichagePersonnaliser(AffichagePersonnalisation perso)                                                                                 #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Film::AffichagePersonnaliser(AffichagePersonnalisation perso)
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
// # void Film::Print()                                                                                                                                 #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Film::Print()
{
    // Header
    Print_Header();
    // Date de reprise
    Print_Date_de_Reprise();
    // Date de sortie
    Print_Date_de_Sortie();
    // Titre Original
    Print_Titre_Original(m_titres_originaux, m_keyColor, m_valuesColor);
    // Distributeur
    Print_Distributeur();
    // AD
    Print_CleValeur(L"Audiodescription", m_audiodescription, m_keyColor[0], m_valuesColor);
    // De
    Print_De();
    // Par
    Print_Par();
    // Avec etc
    Print_Avec_etc();
    // Nationalit�(s)
    Print_Nationalites(m_nationalite, m_keyColor[0], m_valuesColor);
    // Catalogue
    Print_Catalogue(m_sur, m_catalogue, m_keyColor[0], m_valuesColor);
    // Resume
    Print_Resume(m_resume);
    // Dates
    Print_Dates();
    // Avec
    Print_Avec(m_avec, L"Avec :");
    // Acteurs de doublage (Voix locales)
    Print_Avec(m_acteurs_de_doublage_voix_locales, L"Acteurs de doublage (Voix locales) :");
    // Acteurs de doublage (Voix originales)
    Print_Avec(m_acteurs_de_doublage_voix_originales, L"Acteurs de doublage (Voix originales) :");
    // Soundtracks
    Print_Soundtracks();
    // Image(s)
    Print_Images(m_image, m_keyColor[0], m_valuesColor);
    // Making-of
    Print_Making_of();
    std::wcout << L"\r\n";
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Film::Print_Avec(std::vector<std::pair<std::wstring, std::wstring>> avec, const std::wstring str)                                             #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Film::Print_Avec(std::vector<std::pair<std::wstring, std::wstring>> avec, const std::wstring str)
{
    if (avec.size())
    {
        std::wstring avec_str = m_keyColor[0] + str + m_valuesColor + L"\r\n";
        bool found = false;
        for (auto&& [nom, role] : avec)
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
            if (avec.back().first != nom)
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
// # void Film::Print_Avec_etc()                                                                                                                  #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Film::Print_Avec_etc()
{
    if (m_avec.size() > 0)
    {
        std::wstring avec_str = m_keyColor[0] + L"Avec : " + m_valuesColor;
        int item_count = 0;
        for (auto&& [nom, role] : m_avec)
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
// # void Film::Print_Dates()                                                                                                                           #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Film::Print_Dates()
{
    std::wstring dates_str = L"\r\n";
    for (auto& visionnage : m_visionnages)
    {
        //dates_str += visionnage.Print_Dates_de_visionnage(visionnage.m_DatesVisionnage) + L"\r\n";
        dates_str += visionnage.Print_Dates_de_visionnage(visionnage.m_DatesVisionnage, m_keyColor, m_valuesColor) + L"\r\n";
    }
    dates_str += L"\r\n";
    std::wcout << dates_str;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Film::Print_Date_de_Reprise()                                                                                                                 #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Film::Print_Date_de_Reprise()
{
    if (m_date_de_reprise.tm_mday != 0)
    {
        wchar_t date_string[22];
        std::wstring wstr;
        wcsftime(date_string, 18, L"%d/%m/%Y", &m_date_de_reprise);
        wstr = date_string;
        //Console_Lire(keyColor[0] + L"Date de reprise : " + valuesColor + wstr.substr(0, 2) + keyColor[0] + L'/' + valuesColor + wstr.substr(3, 2) + keyColor[0] + L'/' + valuesColor + wstr.substr(6, 4), 0, 19);
        //int i = Console_Lire_txt(keyColor[0] + L"Date de reprise : " + valuesColor + wstr.substr(0, 2) + keyColor[0] + L'/' + valuesColor + wstr.substr(3, 2) + keyColor[0] + L'/' + valuesColor + wstr.substr(6, 4), 0, 19);
        std::wcout << m_keyColor[0] + L"Date de reprise : " + m_valuesColor + wstr.substr(0, 2) + m_keyColor[0] + L'/' + m_valuesColor + wstr.substr(3, 2) + m_keyColor[0] + L'/' + m_valuesColor + wstr.substr(6, 4) + L"\r\n";
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Film::Print_Date_de_Sortie()                                                                                                            #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Film::Print_Date_de_Sortie()
{
    if (m_date_de_sortie.tm_mday != 0)
    {
        wchar_t date_string[22];
        std::wstring wstr;
        wcsftime(date_string, 14, L"%d/%m/%Y", &m_date_de_sortie);
        wstr = date_string;
        //Console_Lire(keyColor[0] + L"Date de sortie : " + valuesColor + wstr.substr(0, 2) + keyColor[0] + L'/' + valuesColor + wstr.substr(3, 2) + keyColor[0] + L'/' + valuesColor + wstr.substr(6, 4), 0, 18);
        //int i = Console_Lire_txt(keyColor[0] + L"Date de sortie : " + valuesColor + wstr.substr(0, 2) + keyColor[0] + L'/' + valuesColor + wstr.substr(3, 2) + keyColor[0] + L'/' + valuesColor + wstr.substr(6, 4), 0, 18);
        std::wcout << m_keyColor[0] + L"Date de sortie : " + m_valuesColor + wstr.substr(0, 2) + m_keyColor[0] + L'/' + m_valuesColor + wstr.substr(3, 2) + m_keyColor[0] + L'/' + m_valuesColor + wstr.substr(6, 4) + L"\r\n";
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Film::Print_De()                                                                                                                              #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Film::Print_De()
{
    if (m_de.size() > 0)
    {
        std::wstring de_str = m_keyColor[0] + L"De : " + m_valuesColor;
        bool first = true;
        for (auto&& d : m_de)
        {
            if (!first)
            {
                de_str += m_keyColor[0] + L", " + m_valuesColor;
            }
            de_str += d;
            first = false;
        }
        de_str += L"\r\n";
        std::wcout << de_str;
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Film::Print_Distributeur()                                                                                                                    #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Film::Print_Distributeur()
{
    if (m_distributeur.size() > 0)
    {
        std::wstring distributeur_str = m_keyColor[0] + L"Distributeur : " + m_valuesColor + m_distributeur + L"\r\n";
        //PrintStringW(m_hOut, creee_par_str, 0);
        //PrintStringW(HANDLE hOut, creee_par_str);
        //Console_Lire(distributeur_str, 0, 0);
        //int i = Console_Lire_txt(distributeur_str, 0, 0);
        std::wcout << distributeur_str;
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Film::Print_Header() const                                                                                                                    #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Film::Print_Header() const
{
    std::wstring titres_str = calcul_Titres_Affichage();
    std::wstring date_str = calcul_Date_Affichage();
    std::wstring sur_str = calcul_Sur_Affichage();
    std::wstring dateEtSur_str = format_DateEtSur(date_str, sur_str);
    std::wstring x_signaletique_jeunesse_str = calcul_X_Signaletique_Jeunesse_affichage(sur_str);
    std::wstring signaletique_jeunesse_str = calcul_Signaletique_Jeunesse_affichage();
    std::wstring duree_str = calcul_Duree_affichage();
    std::wstring note_str = Print_Note();

    std::wcout << titres_str << dateEtSur_str << x_signaletique_jeunesse_str << signaletique_jeunesse_str << duree_str << note_str << std::endl;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Film::Print_Making_of()                                                                                                                       #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Film::Print_Making_of()
{
    if (m_making_of)
    {
        std::wstring making_of_str = m_keyColor[0] + L"Making of" + m_valuesColor + L' ';
        std::wstring duree_str;
        std::wstring resume_str;
        duree_str = m_keyColor[0] + L"(" + m_valuesColor +
            std::to_wstring(m_making_of_duree / (60 * 60)) + m_keyColor[1] + L"h" + m_valuesColor + L' ' + std::to_wstring((m_making_of_duree - ((m_making_of_duree / (60 * 60)) * 60 * 60)) / 60) + m_keyColor[1] + L"min" + m_valuesColor +
            m_keyColor[0] + L") " + m_valuesColor;
        if(m_making_of)
        {
            duree_str += m_keyColor[0] + L':' + m_valuesColor + L"\r\n";
            for (auto r : m_making_of_resume)
            {
                resume_str += r + L"\r\n";
            }
        }
        std::wcout << making_of_str << duree_str << resume_str;
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::wstring Film::Print_Note() const                                                                                                              #
// #                                                                                                                                                    #
// ######################################################################################################################################################

std::wstring Film::Print_Note() const
{
    return L"";
    std::wstring note_str;
    if (m_note != 0.0)
    {
        if (m_note == -1.0)
            note_str += L' ' + m_keyColor[0] + L'(' + m_valuesColor + L"pas de note !" + m_keyColor[0] + L')' + m_valuesColor;
        else
        {
            if (m_note == 0 || m_note == 1 || m_note == 2 || m_note == 3 || m_note == 4 || m_note == 5)
                note_str = L' ' + std::to_wstring(static_cast<int>(std::floor(m_note)));
            else
            {
                std::wstring wstr = std::to_wstring(m_note);
                wstr = wstr[0] + m_keyColor[0] + wstr[1] + m_valuesColor + wstr[2];
                note_str += L' ' + wstr;
            }
            note_str += m_keyColor[0] + L"/5" + m_valuesColor;
        }
    }
    else
    {
        note_str = L' ' + m_keyColor[0] + L'(' + m_valuesColor + L"pas de note !" + m_keyColor[0] + L')' + m_valuesColor;
    }
    return note_str;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Film::Print_Par()                                                                                                                             #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Film::Print_Par()
{
    if (m_par.size() > 0)
    {
        std::wstring par_str = m_keyColor[0] + L"Par : " + m_valuesColor;
        bool first = true;
        for (auto&& p : m_par)
        {
            if (!first)
            {
                par_str += m_keyColor[0] + L", " + m_valuesColor;
            }
            par_str += p;
            first = false;
        }
        par_str += L"\r\n";

        std::wcout << par_str;
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Film::Print_Soundtracks()                                                                                                                     #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Film::Print_Soundtracks()
{
    if (m_soundtrack.size() > 0)
    {
        std::size_t maxKeyLength = 0;
        for (int i = 0; i < m_soundtrack.size(); i++)
        {
            maxKeyLength = max(maxKeyLength, m_soundtrack[i].first.length());
            //maxKeyLength = std::max(maxKeyLength, m_soundtrack[i].first.length()); // Erreur !!!
        }

        //PrintTmp(keyColor + L"Soundtrack :" + valuesColor + L"\r\n");
        //Console_Lire(keyColor[0] + L"Soundtrack :" + valuesColor + L"\r\n", 0, 0);
        std::wcout << m_keyColor[0] + L"Soundtrack :" + m_valuesColor + L"\r\n";

        for (auto&& [role, nom] : m_soundtrack)
        {
            //PrintTmp(keyColor2 + role + std::wstring(maxKeyLength - role.size(), L' ') + L" : " + valuesColor2 + nom + L"\r\n");
            //Console_Lire(role + keyColor[1] + std::wstring(maxKeyLength - role.size(), L' ') + L" : " + valuesColor + nom + L"\r\n", 4, 0);
            //int i = Console_Lire_txt(role + keyColor[1] + std::wstring(maxKeyLength - role.size(), L' ') + L" : " + valuesColor + nom + L"\r\n", 4, 0);
            std::wcout << role + m_keyColor[1] + std::wstring(maxKeyLength - role.size(), L' ') + L" : " + m_valuesColor + nom + L"\r\n";
        }
    }
}