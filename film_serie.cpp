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
//#include <experimental/filesystem> // Header file for pre-standard implementation
//using namespace std::experimental::filesystem::v1;
using namespace std;
namespace fs = std::filesystem;

//extern const std::vector<std::wstring> lire_fichierTxt(std::wstring const& nomFichier, std::vector<std::wstring> separeteurs);
//extern const std::vector<std::pair<std::wstring, std::wstring>>lire_paireCleValeur_depuisFichierTxt(std::wstring const& nomFichier, std::wstring separeteur);
//extern const std::wstring lire_fichierTxt(std::wstring const& nomFichier);
extern const std::vector<std::wstring> lire_fichierTxt(std::wstring const& nomFichier, std::vector<std::wstring> separeteurs);
extern const std::vector<std::wstring> lire_fichierTxt(std::wstring const& nomFichier, std::vector<std::wstring> separeteurs, bool found);
extern const std::vector<std::pair<std::wstring, std::wstring>>lire_paireCleValeur_depuisFichierTxt(std::wstring const& nomFichier, std::wstring separeteur);
//extern const std::wstring lire_fichierTxt(std::wstring const& nomFichier);
extern const std::wstring lire_fichierTxt(std::wstring const& nomFichier);

// ######################################################################################################################################################
// ######################################################################################################################################################
// Allocin� : https://www.allocine.fr/

const std::vector<std::wstring> Audiodescription
{ // AD : Audiodescription
    L"Allemagne",
    L"Anglais",
    L"Espagne",
    L"Fran�ais",
    L"H�breu",
    L"Portugais br�silien"
};

const std::vector<std::wstring>Genre
{
    L"Action",
    L"Arts Martiaux",
    L"Animation",
    L"Aventure",
    L"Biopic",
    L"Com�die",
    L"Com�die musicale",
    L"Com�die dramatique",
    L"Concert",
    L"Divers",

    L"Docu-s�ries",
    L"Documentaire",
    L"Drama",
    L"Drame",
    L"Espionnage",
    L"Epouvante-horreur",
    L"Erotique",
    L"Exp�rimental",
    L"Famille",
    L"Fantastique",

    L"Guerre",
    L"Judiciaire",
    L"Historique",
    L"Manga",
    L"Musical",
    L"P�plum",
    L"Policier",
    L"Op�ra",
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
    L"Br�sil",
    L"Canada",
    L"Chili",
    L"Chine",
    L"Cor�e du Sud",
    L"Danemark",
    L"Danoise",
    L"Egypte",
    L"Espagne",
    L"Finlande",
    L"France",
    L"Grande-Bretagne",
    L"Hong-Kong",
    L"Inde",
    L"Irlande",
    L"Isra�l",
    L"Italie",
    L"Japon",
    L"Jordanie",
    L"Luxembourg",
    L"Mexique",
    L"Nig�ria",
    L"Norv�ge",
    L"Pays-Bas",
    L"Philippines",
    L"Pologne",
    L"R�publique Tch�que",
    L"Russie",
    L"Samoa",
    L"Su�de",
    L"Tha�lande",
    L"Turquie",
    L"Ukraine",
    L"U.S.A."
};

const std::vector<std::wstring> Sous_Genre
{
    L"Animation",
    L"Documentaire",
    L"Docu-s�ries",
    L"Manga",
    L"Mini-s�rie"
};

// ######################################################################################################################################################
// ######################################################################################################################################################

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void initialiser_Audiodescription(fs::path const& m_cheminFichier, std::wstring& m_ad)                                                             #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void initialiser_Audiodescription(fs::path const& m_cheminFichier, std::wstring& m_ad)
{
    auto nomFichier = m_cheminFichier.wstring();
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
// # void initialiser_Avec(fs::path const& m_cheminFichier, std::vector<std::pair<std::wstring, std::wstring>>& m_avec)                                 #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void initialiser_Avec(fs::path const& m_cheminFichier, std::vector<std::pair<std::wstring, std::wstring>>& m_avec)
{
    auto nomFichier = m_cheminFichier.filename().wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    m_avec = lire_paireCleValeur_depuisFichierTxt(m_cheminFichier.wstring(), L" : ");
    assert((m_avec.size() != 0));
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void initialiser_Genre(fs::path const& m_cheminFichier,                                                                                            #
// #                        std::vector<std::wstring>& m_genres_renvoyes,                                                                               #
// #                        const std::vector<std::wstring>& genres_valides)                                                                            #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void initialiser_Genre(fs::path const& m_cheminFichier, std::vector<std::wstring>& m_genres_renvoyes, const std::vector<std::wstring>& genres_valides)
{ // Genre
    auto nomFichier = m_cheminFichier.wstring();
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
// # void initialiser_Image(fs::path const& m_cheminFichier, std::vector<std::wstring>& m_images)                                                       #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void initialiser_Image(fs::path const& m_cheminFichier, std::vector<std::wstring>& m_images)
{
    auto nomFichier = m_cheminFichier.filename().wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    m_images.push_back(nomFichier);
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void initialiser_Nationalite(fs::path const& m_cheminFichier,                                                                                      #
// #                              std::vector<std::wstring>& m_nationalites_renvoyes,                                                                   #
// #                              const std::vector<std::wstring>& nationalites_valides)                                                                #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void initialiser_Nationalite(fs::path const& m_cheminFichier, std::vector<std::wstring>& m_nationalites_renvoyes, const std::vector<std::wstring>& nationalites_valides)
{ // Nationalite
    auto nomFichier = m_cheminFichier.wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");

//    std::size_t pos_txt = nationalite_filename.find(L".txt");
//    assert((pos_txt != std::wstring::npos) && L"Erreur Nationalite(s) !!!");
    //std::wstring radical = nationalite_filename.substr(0, pos_txt);
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
// # const void initialiser_Sous_Genre(std::wstring& m_s_g)                                                                                             #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void initialiser_Sous_Genre(std::wstring& m_s_g)
{ // Sous_Genre
    bool s_g = false;
    if (std::find(::Sous_Genre.begin(), ::Sous_Genre.end(), m_s_g) != ::Sous_Genre.end())
        s_g = true;
    return;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void initialiser_Titre_Original(fs::path const& cheminFichier, std::vector<std::wstring>& m_titre_original)                                        #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void initialiser_Titre_Original(fs::path const& m_cheminFichier, std::vector<std::wstring>& m_titre_original)
{ // Titre Original
    auto nomFichier = m_cheminFichier.wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    std::wstring titre = lire_fichierTxt(m_cheminFichier.wstring());
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
        if (m_titre_original[2] != L"")
        {
            titre_original_str += keyColor[1] + m_titre_original[1] + valuesColor;
            titre_original_str += m_titre_original[2];
        }
        titre_original_str += L"\r\n";
        std::wcout << titre_original_str;
    }
}