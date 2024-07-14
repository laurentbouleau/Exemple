#include "pch.h"

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
Film::Film(std::filesystem::path racine)
{
    this->racine = racine;
    auto nomDossier = racine.filename().wstring();
    assert(nomDossier.length() > 0 && L"Nom de dossier vide");

    std::wregex filename_pattern{ L"(.+?)(?:\\.\\[(\\d{4}\\s|\\d{4}\\-\\d{4}\\s|\\d{4}\\-\\s)?([^\\]]*)\\])?(?:\\.(.+))?" };
    std::wsmatch match;
    if (std::regex_match(nomDossier, match, filename_pattern))
    {
        std::wstring titres = match[1];
        m_titres = Dossier_Titres(titres);
        /*if (match[2].matched)
        {
            std::wstring annees_str = match[2].str();
            std::wsmatch dummy;
            if (std::regex_match(annees_str, dummy, std::wregex(L"\\d{4}\\-\\d{4}\\s")))
            {
                m_f_anneesProduction.first = stoi(annees_str);
                m_f_anneesProduction.second = stoi(annees_str.substr(5));
            }
            else
            {
                m_f_anneesProduction.first = stoi(annees_str);
            }
        }

        m_sur = (match[3].matched) ? match[3].str() : L"";

        std::wstring sous_genre = (match[4].matched) ? match[4].str() : L"";
        m_sous_genre = sous_genre;*/
    }
    else
    {
        assert(false == true && "Le nom du répertoire n'est pas un nom valide.");
    }
}

std::vector<std::wstring> Film::Dossier_Titres(std::wstring titres)
{
    assert(titres.length() > 0 && L"Nom de titres vide"); // ??? pour Mot de... ?
    size_t pos = 0;
    const std::wstring d_p = L" - ";
    pos = titres.find(d_p);
    bool found = false;
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

void Film::initialiser_Fichier(fs::path const& cheminFichier)
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

        // Disney+ SJ
        if (nomFichier == L"Disney+.txt")
        {
            m_disney_sj = recuperer_Disney_SJ(cheminFichier);
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
            m_sj = recuperer_SJ(cheminFichier);
        }
        // Titre
        if (nomFichier == L"Titre.txt")
        {
            initialiser_Titre(cheminFichier, m_titres);
        }
        // Titre original
        if (nomFichier == L"Titre original.txt")
        {
            initialiser_Titre_Original(cheminFichier, m_titres_originaux);
        }
        return;
    }
    else if (nomImage == L".jpg" || nomImage == L".png" || nomImage == L".webp")
        // Image
    {
        initialiser_Image(cheminFichier, m_image);
    }
    else
    {
        std::wcout << L'{' << cheminFichier << L'}' << std::endl;
    }
}

void Film::initialiser_Titre(fs::path const& cheminFichier, std::vector<std::wstring>& m_titre)
{ // Titre
    auto nomFichier = cheminFichier.wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    std::vector<std::wstring> contenu = lire_fichierTxt(cheminFichier.wstring(), { L"\n" });
    assert((contenu.size() != 0));

    std::vector<std::wstring> t;


    //std::wregex titre_pattern{ L"(.+?)(\\s:\\s|:\\s|/|\\s-\\s)(.+)" };
    /*std::wregex filename_pattern{L"(.+?)(?:\\.\\[(\\d{4}\\-\\d{4}\\s?|\\d{4}\\-\\s?|\\d{4}\\s?)?([^\\]]*)\\])?(?:\\.(.+))?"};
    std::wsmatch match;
    if (std::regex_match(contenu[0], match, filename_pattern))
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
        t.push_back(contenu[0]);
    }*/

    std::wregex titre_pattern{ L"(.+?)(\\s:\\s|:\\s|/|\\s-\\s)(.+)" };
    //std::wregex filename_pattern{ L"(.+?)(?:\\.\\[(\\d{4}\\-\\d{4}\\s?|\\d{4}\\-\\s?|\\d{4}\\s?)?([^\\]]*)\\])?(?:\\.(.+))?" };
    std::wsmatch match;
    if (std::regex_match(contenu[0], match, titre_pattern))
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
        t.push_back(contenu[0]);
    }


    contenu.erase(contenu.begin());
    if (contenu.size() > 0)
    {
        //initialiser_Duree(contenu[0]);
        contenu.erase(contenu.begin());

        contenu.erase(contenu.begin());
        if (contenu.size() > 0)
            m_resume = contenu;
    }
}

const void Film::Print()
{
    // Header
    Print_Header();
    // Titre Original
    Print_Titre_Original(m_titres_originaux, affichage_titres_originaux_actif, keyColor, valuesColor);
    // Nationalité(s)
    Print_Nationalites(m_nationalite, affichage_nationalite_actif, keyColor[0], valuesColor);
    // Image(s)
    Print_Images(m_image, affichage_image_actif, keyColor[0], valuesColor);
    // Phrases
    Print_Resume(m_resume, affichage_resume_actif);
    std::wcout << L"\r\n";
}

const void Film::Print_Header()
{
    if (affichage_titres_actif)
    {
        std::wstring titres_str;
        std::wstring annees_str;
        std::wstring sur_str;
        std::wstring sj_str;
        std::wstring note_str;

        titres_str = keyColor[0] + L"Titre : " + valuesColor + m_titres[0];
        if (m_titres.size() > 1)
            titres_str += keyColor[1] + m_titres[1] + valuesColor + m_titres[2];
        // Année(s)
        /*if (affichage_annees_actif)
        {
            annees_str = format_Annees();
        }*/
        // sur
        if (affichage_sur_actif && m_sur != L"")
        {
            sur_str += keyColor[0] + L" (" + keyColor[1] + L"sur " + valuesColor + m_sur + keyColor[1] + L" : " + valuesColor;
            // Disney+ SJ
            if (affichage_disney_sj_actif && m_disney_sj.length() != 0)
                sur_str += m_disney_sj;
            // Netflix SJ
            if (affichage_netflix_sj_actif && m_netflix_sj.length() != 0)
                sur_str += m_netflix_sj;
            sur_str += keyColor[0] + L')' + valuesColor;
        }
        else
        {
            // Disney+ SJ
            if (affichage_disney_sj_actif && m_disney_sj.length() != 0)
                sur_str += keyColor[0] + L" (" + valuesColor + L"Disney+" + keyColor[1] + L" : " + m_disney_sj + keyColor[0] + L')' + valuesColor;
            // Netflix SJ
            if (affichage_netflix_sj_actif && m_netflix_sj.length() != 0)
                sur_str += keyColor[0] + L" (" + valuesColor + L"Netflix" + keyColor[1] + L" : " + m_netflix_sj + keyColor[0] + L')' + valuesColor;
        }
        // La signalétique jeunesse
        if (affichage_sj_actif && m_sj.length() != 0)
            sj_str += keyColor[0] + L" (" + valuesColor + L"SJ" + keyColor[1] + L" : " + valuesColor + m_sj + keyColor[0] + L')' + valuesColor;
        // Note
        /*if (affichage_note_actif)
            note_str += calcul_Note_Affichage();*/

        std::wcout << titres_str << annees_str << sur_str << sj_str << note_str << std::endl;
    }
}