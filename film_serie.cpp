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
#include <regex>

#include <filesystem> // C++17 standard header file name

using namespace std;
namespace fs = std::filesystem;

//extern const std::vector<std::wstring> lire_fichierTxt(std::wstring const& nomFichier, std::vector<std::wstring> separeteurs);
//extern const std::vector<std::pair<std::wstring, std::wstring>>lire_paireCleValeur_depuisFichierTxt(std::wstring const& nomFichier, std::wstring separeteur);
//extern const std::wstring lire_fichierTxt(std::wstring const& nomFichier);
extern const std::vector<std::wstring> lire_fichierTxt(std::wstring const& nomFichier, std::vector<std::wstring> separeteurs);
extern const std::vector<std::wstring> lire_fichierTxt(std::wstring const& nomFichier, std::vector<std::wstring> separeteurs, bool found);
extern const std::vector<std::pair<std::wstring, std::wstring>>lire_paireCleValeur_depuisFichierTxt(std::wstring const& nomFichier, std::wstring separeteur);
//extern const std::wstring lire_fichierTxt(std::wstring const& nomFichier);
extern const std::wstring lire_et_decouper_fichierTxt(std::wstring const& nomFichier);
extern const std::wstring lire_fichierTxt(std::wstring const& nomFichier);

// ######################################################################################################################################################
// ######################################################################################################################################################
// Allociné : https://www.allocine.fr/

// Exception des éléments
const std::vector<wchar_t> Exception_des_elements
{
    L'\\',
    L'/',
    L':',
    L'*',
    L'?',
    L'"',
    L'<',
    L'>',
    L'|'
};

const std::vector<std::wstring> Audiodescription
{ // AD : Audiodescription
    L"Allemagne",
    L"Anglais",
    L"Espagne",
    L"Français",
    L"Hébreu",
    L"Portugais brésilien"
};

const std::vector<std::wstring>Genre
{
    L"Action",
    L"Arts Martiaux",
    L"Animation",
    L"Aventure",
    L"Biopic",
    L"Comédie",
    L"Comédie musicale",
    L"Comédie dramatique",
    L"Concert",
    L"Divers",

    L"Docu-séries",
    L"Documentaire",
    L"Drama",
    L"Drame",
    L"Espionnage",
    L"Epouvante-horreur",
    L"Erotique",
    L"Expérimental",
    L"Famille",
    L"Fantastique",

    L"Guerre",
    L"Judiciaire",
    L"Historique",
    L"Manga",
    L"Musical",
    L"Péplum",
    L"Policier",
    L"Opéra",
    L"Romance",
    L"Science fiction", L"Science Fiction",

    L"Show",
    L"Survival",
    L"Thriller",
    L"Western"
};

const std::vector<std::wstring> Nationalite
{
    L"Afrique du Sud",
    L"Allemagne",
    L"Argentine",
    L"Australie",
    L"Barbade",
    L"Belgique",
    L"Brésil",
    L"Canada",
    L"Chili",
    L"Chine",
    L"Corée du Sud",
    L"Danemark",
    L"Danoise",
    L"Egypte",
    L"Espagne",
    L"Finlande",
    L"France",
    L"Grande-Bretagne",
    L"Hong-Kong",
    L"Inde",
    L"Indonésie",
    L"Irlande",
    L"Islande",
    L"Israël",
    L"Italie",
    L"Japon",
    L"Jordanie",
    L"Luxembourg",
    L"Mexique",
    L"Nigéria",
    L"Norvège",
    L"Pays-Bas",
    L"Philippines",
    L"Pologne",
    L"République Tchèque",
    L"Russie",
    L"Samoa",
    L"Suède",
    L"Thaïlande",
    L"Turquie",
    L"Ukraine",
    L"U.S.A."
};

const std::vector<std::wstring> Sous_Genre
{
    L"Animation",
    L"Documentaire",
    L"Docu-séries",
    L"Manga",
    L"Mini-série"
};

const std::vector<std::wstring> Soundtrack
{
    L"Compositeur",
    L"Compositeur (chansons du film)",
    L"Montage musical",
    L"Superviseur musical"
};

const std::vector<std::wstring> Sur
{
    L"Apple TV+",
    L"Amazon Prime Video",
    L"Crunchyroll",
    L"Disney+",
    L"DVD",
    L"Netflix",
    L"SⱯLTO",
    L"VOD"
};

// ######################################################################################################################################################
// ######################################################################################################################################################

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void initialiser_Audiodescription(fs::path const& cheminFichier, std::wstring& m_ad)                                                               #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void initialiser_Audiodescription(fs::path const& cheminFichier, std::wstring& m_ad)
{
    auto nomFichier = cheminFichier.wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    std::wstring ad = lire_fichierTxt(nomFichier);
    std::size_t pos = ad.find(L"Audiodescription");
    if (pos == std::wstring::npos)
        ;
    else
        ad = ad.substr(0, pos - 3);
    if (std::find(::Audiodescription.begin(), ::Audiodescription.end(), ad) != ::Audiodescription.end())
        m_ad = ad;
    assert((m_ad.size() != 0));
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void initialiser_Avec(fs::path const& cheminFichier, std::vector<std::pair<std::wstring, std::wstring>>& m_avec)                                   #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void initialiser_Avec(fs::path const& cheminFichier, std::vector<std::pair<std::wstring, std::wstring>>& m_avec)
{
    auto nomFichier = cheminFichier.filename().wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    m_avec = lire_paireCleValeur_depuisFichierTxt(cheminFichier.wstring(), L" : ");
    assert((m_avec.size() != 0));
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void initialiser_Genre(fs::path const& cheminFichier,                                                                                              #
// #                        std::vector<std::wstring>& m_genres_renvoyes,                                                                               #
// #                        const std::vector<std::wstring>& genres_valides)                                                                            #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void initialiser_Genre(fs::path const& cheminFichier, std::vector<std::wstring>& m_genres_renvoyes, const std::vector<std::wstring>& genres_valides)
{ // Genre
    auto nomFichier = cheminFichier.wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");

    std::vector<std::wstring>g;
    g = lire_fichierTxt(nomFichier, { L"\n", L", " });
    for (auto&& genre : g)
    {
        if (std::find(genres_valides.begin(), genres_valides.end(), genre) != genres_valides.end())
            m_genres_renvoyes.push_back(genre);
        else
        {
            assert((false) && "Attention genre non valide !!!");
        }
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void initialiser_Image(fs::path const& cheminFichier, std::vector<std::wstring>& m_images)                                                         #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void initialiser_Image(fs::path const& cheminFichier, std::vector<std::wstring>& m_images)
{
    auto nomFichier = cheminFichier.filename().wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    m_images.push_back(nomFichier);
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void initialiser_Nationalite(fs::path const& cheminFichier,                                                                                        #
// #                              std::vector<std::wstring>& m_nationalites_renvoyes,                                                                   #
// #                              const std::vector<std::wstring>& nationalites_valides)                                                                #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void initialiser_Nationalite(fs::path const& cheminFichier, std::vector<std::wstring>& m_nationalites_renvoyes, const std::vector<std::wstring>& nationalites_valides)
{ // Nationalite
    auto nomFichier = cheminFichier.wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");

    std::vector<std::wstring>n;
    n = lire_fichierTxt(nomFichier, { L"\n", L", " });
    for (auto&& nationalite : n)
    {
        if (std::find(nationalites_valides.begin(), nationalites_valides.end(), nationalite) != nationalites_valides.end())
            m_nationalites_renvoyes.push_back(nationalite);
        else
        {
            assert((false) && "Attention nationalite non valide !!!");
        }
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # bool initialiser_Sous_Genre(std::wstring& m_s_g)                                                                                                   #
// #                                                                                                                                                    #
// ######################################################################################################################################################

//void initialiser_Sous_Genre(std::wstring& m_s_g)
bool initialiser_Sous_Genre(std::wstring& m_s_g)
{ // Sous_Genre
    bool s_g = false;
    if (std::find(::Sous_Genre.begin(), ::Sous_Genre.end(), m_s_g) != ::Sous_Genre.end())
        s_g = true;
    return s_g;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void initialiser_Sur(std::wstring& m_s)                                                                                                            #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void initialiser_Sur(std::wstring& m_s)
{ // Sur
    bool sur = false;
    if (std::find(::Sur.begin(), ::Sur.end(), m_s) != ::Sur.end())
    {
        sur = true;
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void initialiser_Titre_Original(fs::path const& cheminFichier, std::vector<std::wstring>& m_titre_original)                                        #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void initialiser_Titre_Original(fs::path const& cheminFichier, std::vector<std::wstring>& m_titre_original)
{ // Titre Original
    auto nomFichier = cheminFichier.wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    std::wstring titre = lire_fichierTxt(cheminFichier.wstring());
    assert((titre.size() != 0));

    std::wregex titre_pattern{ L"(.+?)(\\s:\\s|:\\s|/|\\s-\\s)(.+)" };
    std::wsmatch match;
    if (std::regex_match(titre, match, titre_pattern))
    {
        m_titre_original.push_back(match[1]);
        if (match.length() > 2)
        {
            m_titre_original.push_back(match[2]);
        }
        if (match.length() > 3)
        {
            m_titre_original.push_back(match[3]);
        }
    }
    else
    {
        m_titre_original.push_back(titre);
    }
}

// ######################################################################################################################################################
// ######################################################################################################################################################

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::wstring filter_values(std::wstring const& content, std::vector<std::wstring> const& values)                                                   #
// #                                                                                                                                                    #
// ######################################################################################################################################################

std::wstring filter_values(std::wstring const& content, std::vector<std::wstring> const& values)
{
    std::wstring pattern;
    for (const auto value : values)
    {
        if (pattern.length() > 0) { pattern += L"|"; }
        pattern += value;
    }
    std::wregex value_pattern{ L"(" + pattern + L")" };
    std::wsmatch match;
    if (std::regex_match(content, match, value_pattern))
    {
        return match[1];
    }
    return L"";
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::wstring recuperer_Disney_SJ(fs::path const& cheminFichier)                                                                                    #
// #                                                                                                                                                    #
// ######################################################################################################################################################

std::wstring recuperer_Disney_SJ(fs::path const& cheminFichier)
{ // Disney+ SJ
    auto nomFichier = cheminFichier.wstring();

    std::wstring content = lire_fichierTxt(nomFichier);

    return filter_values(content, { L"6\\+",L"12\\+",L"14\\+" });
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::wstring recuperer_Netflix_SJ(fs::path const& cheminFichier)                                                                                   #
// #                                                                                                                                                    #
// ######################################################################################################################################################

std::wstring recuperer_Netflix_SJ(fs::path const& cheminFichier)
{ // Netflix SJ
    auto nomFichier = cheminFichier.wstring();

    std::wstring content = lire_fichierTxt(nomFichier);

    return filter_values(content, { L"7\\+",L"10\\+",L"13\\+", L"16\\+", L"18\\+", L"Tous publics" });
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void initialiser_SJ(fs::path const& cheminFichier, std::wstring& m_sj)                                                                             #
// #                                                                                                                                                    #
// ######################################################################################################################################################
// Cinéma et serie
std::wstring recuperer_SJ(fs::path const& cheminFichier)
{ // SJ
    auto nomFichier = cheminFichier.wstring();

    std::wstring content = lire_fichierTxt(nomFichier);

    std::wregex sj_pattern{ L"(\\-7|\\-10|\\-12|\\-16|\\-18)" };
    std::wsmatch match;
    if (std::regex_match(content, match, sj_pattern))
    {
        return match[1];
    }
    return L"";
}

// ######################################################################################################################################################
// ######################################################################################################################################################

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const void Print_Audiodescription(const std::wstring& m_audiodescription, bool affichage_audiodescription_actif,                                   #
// #                                  std::wstring& keyColor, std::wstring& valuesColor)                                                                #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Print_Audiodescription(const std::wstring& m_audiodescription, bool affichage_audiodescription_actif, std::wstring& keyColor, std::wstring& valuesColor)
{
    if (affichage_audiodescription_actif && m_audiodescription.size() > 0)
    {
        std::wstring audiodescription_str = keyColor + L"Audiodescription : " + valuesColor + m_audiodescription + L"\r\n";
        //PrintStringW(m_hOut, creee_par_str, 0);
        //PrintStringW(HANDLE hOut, creee_par_str);
        //Console_Lire(audiodescription_str, 0, 0);
        //Console_Lire(hOut, audiodescription_str, 0);// , 0);
        //Console_Lire(hOut, audiodescription_str, 0, L' ');
        std::wcout << audiodescription_str;// << std::endl;
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Print_Genres(const std::vector<std::wstring>& genres, bool affichage_genres_actif,                                                            #
// #                  const std::wstring& sous_genre, bool affichage_sous_genre_actif,                                                                  #
// #                  std::wstring& keyColor, std::wstring& valuesColor)                                                                                #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Print_Genres(const std::vector<std::wstring>& genres, bool affichage_genres_actif, const std::wstring& sous_genre, bool affichage_sous_genre_actif, std::wstring& keyColor, std::wstring& valuesColor)
{
    if (affichage_genres_actif && genres.size() > 0)
    {
        std::wstring genre_str = keyColor + L"Genre" + ((genres.size() > 1) ? L"s" : L"") + L" : " + valuesColor;

        bool first = true;
        for (auto&& genre : genres)
        {
            if (!first)
            {
                genre_str += keyColor + L", " + valuesColor;
            }
            genre_str += genre;
            first = false;
        }
        genre_str += L"\r\n";
        std::wcout << genre_str;
        if (affichage_sous_genre_actif && sous_genre.size() != 0)
        {
            genre_str = keyColor + L"Sous-genre : " + valuesColor + sous_genre + L"\r\n";
            std::wcout << genre_str;// << std::endl;
        }
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Print_Images(const std::vector<std::wstring>& image, bool affichage_image_actif, std::wstring& keyColor, std::wstring& valuesColor)           #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Print_Images(const std::vector<std::wstring>& image, bool affichage_image_actif, std::wstring& keyColor, std::wstring& valuesColor)
{
    if (affichage_image_actif && image.size() > 0)
    {
        std::wstring image_str = keyColor + L"Image" + ((image.size() > 1) ? L"s" : L"") + L" : [" + valuesColor;
        bool first = true;
        for (auto&& i : image)
        {
            if (!first)
            {
                image_str += keyColor + L"], [" + valuesColor;
            }
            image_str += i;
            first = false;
        }
        image_str += keyColor + L']' + valuesColor + L"\r\n";
        std::wcout << image_str;
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Print_Resume(const std::vector<std::wstring>& m_resume, bool affichage_resume_actif)                                                          #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Print_Resume(const std::vector<std::wstring>& m_resume, bool affichage_resume_actif)
{
    if (affichage_resume_actif && m_resume.size() > 0)
    {
        std::wstring resume_str = L"";
        for (auto&& r : m_resume)
            resume_str += r + L"\r\n";
        std::wcout << resume_str;
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const void Print_Nationalites(const std::vector<std::wstring>& m_nationalites, bool affichage_nationalite_actif                                    #                                                                                                    #
// #                        std::wstring& keyColor, std::wstring& valuesColor)                                                                          #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Print_Nationalites(const std::vector<std::wstring>& m_nationalites, bool affichage_nationalite_actif, std::wstring& keyColor, std::wstring& valuesColor)
{
    if (affichage_nationalite_actif && m_nationalites.size() > 0)
    {
        std::wstring nationalite_str = keyColor + L"Nationalite" + ((m_nationalites.size() > 1) ? L"s" : L"") + L" : " + valuesColor;

        bool first = true;
        for (auto&& nationalite : m_nationalites)
        {
            if (!first)
            {
                nationalite_str += keyColor + L", " + valuesColor;
            }
            nationalite_str += nationalite;
            first = false;
        }
        nationalite_str += L"\r\n";
        std::wcout << nationalite_str;
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Print_Titre_Original(const std::vector<std::wstring>& m_titre_original, bool affichage_titre_original_actif,                                  #
// #                              std::vector<std::wstring>& keyColor, std::wstring& valuesColor)                                                       #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Print_Titre_Original(const std::vector<std::wstring>& m_titre_original, bool affichage_titre_original_actif, std::vector<std::wstring>& keyColor, std::wstring& valuesColor)
{
    if (affichage_titre_original_actif && m_titre_original.size() > 0)
    {
        std::wstring titre_original_str = keyColor[0] + L"Titre original : " + valuesColor + m_titre_original[0];
        if (m_titre_original.size() > 2 && m_titre_original[2] != L"")
        {
            titre_original_str += keyColor[1] + m_titre_original[1] + valuesColor;
            titre_original_str += m_titre_original[2];
        }
        titre_original_str += L"\r\n";
        std::wcout << titre_original_str;
    }
}