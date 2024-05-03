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
#include <tuple>

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
// Allociné : https://www.allocine.fr/

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
    L"Irlande",
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

// ######################################################################################################################################################
// ######################################################################################################################################################

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const void initialiser_Audiodescription(fs::path const& m_cheminFichier, std::wstring& m_ad)                                                       #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const void initialiser_Audiodescription(fs::path const& m_cheminFichier, std::wstring& m_ad)
{
    auto nomFichier = m_cheminFichier.wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    std::wstring a = lire_fichierTxt(nomFichier);
    std::size_t pos = a.find(L"Audiodescription");
    if (pos == std::wstring::npos)
        ;
    else
        a = a.substr(0, pos - 3);
    if (std::find(::Audiodescription.begin(), ::Audiodescription.end(), a) != ::Audiodescription.end())
        m_ad = a;
    assert((m_ad.size() != 0));
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const void initialiser_Avec(fs::path const& m_cheminFichier, std::vector<std::pair<std::wstring, std::wstring>>& avec)                             #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const void initialiser_Avec(fs::path const& m_cheminFichier, std::vector<std::pair<std::wstring, std::wstring>>& avec)
{
    auto nomFichier = m_cheminFichier.filename().wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    avec = lire_paireCleValeur_depuisFichierTxt(m_cheminFichier.wstring(), L" : ");
    assert((avec.size() != 0));
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const void initialiser_Genre(fs::path const& m_cheminFichier,                                                                                      #
// #                           std::vector<std::wstring>& genres_renvoyes,                                                                              #
// #                           const std::vector<std::wstring>& genres_valides)                                                                         #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const void initialiser_Genre(fs::path const& m_cheminFichier, std::vector<std::wstring>& genres_renvoyes, const std::vector<std::wstring>& genres_valides)
{ // Genre
    auto nomFichier = m_cheminFichier.wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");

    std::vector<std::wstring>g;
    g = lire_fichierTxt(nomFichier, { L"\n", L", " });
    for (auto&& genre : g)
    {
        if (std::find(genres_valides.begin(), genres_valides.end(), genre) != genres_valides.end())
            genres_renvoyes.push_back(genre);
        else
        {
            assert((false) && "Attention genre non valide !!!");
        }
    }
    //system("PAUSSE");
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const void initialiser_Image(fs::path const& m_cheminFichier, std::vector<std::wstring>& images)                                                   #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const void initialiser_Image(fs::path const& m_cheminFichier, std::vector<std::wstring>& images)
{
    auto nomFichier = m_cheminFichier.filename().wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    images.push_back(nomFichier);
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const void initialiser_Nationalite(fs::path const& m_cheminFichier,                                                                                #
// #                                 std::vector<std::wstring>& nationalites_renvoyes,                                                                  #
// #                                 const std::vector<std::wstring>& nationalites_valides)                                                             #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const void initialiser_Nationalite(fs::path const& m_cheminFichier, std::vector<std::wstring>& nationalites_renvoyes, const std::vector<std::wstring>& nationalites_valides)
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
            nationalites_renvoyes.push_back(nationalite);
        else
        {
            assert((false) && "Attention nationalite non valide !!!");
        }
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const int initialiser_Sous_Genre(std::wstring& s_g)                                                                                                #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const void initialiser_Sous_Genre(std::wstring& s_g)
{ // Sous_Genre
    bool s_g_ = false;
    if (std::find(::Sous_Genre.begin(), ::Sous_Genre.end(), s_g) != ::Sous_Genre.end())
        s_g_ = true;
    return;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Serie::initialiser_Titre_Original(fs::path const& m_cheminFichier, std::vector<std::wstring>& m_titre_original)                               #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const void initialiser_Titre_Original(fs::path const& m_cheminFichier, std::vector<std::wstring>& m_titre_original)
{ // Chaîne
    auto nomFichier = m_cheminFichier.wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    std::wstring titre = lire_fichierTxt(m_cheminFichier.wstring());
    assert((titre.size() != 0));
    bool found = false;
    const std::wstring d_p = L" : ";
    size_t pos;
    pos = titre.find(d_p);
    if (!found && pos != std::wstring::npos)
    {
        m_titre_original[0] = titre.substr(0, pos);
        m_titre_original[1] = d_p;
        m_titre_original[2] = titre.substr(pos + 3);
        found = true;
    }
    const std::wstring d_p2 = L": ";
    pos = titre.find(d_p2);
    if (!found && pos != std::wstring::npos)
    {
        m_titre_original[0] = titre.substr(0, pos);
        m_titre_original[1] = d_p2;
        m_titre_original[2] = titre.substr(pos + 2);
        found = true;
    }
    const std::wstring d_p3 = L"/";
    pos = titre.find(d_p3);
    if (!found && pos != std::wstring::npos)
    {
        m_titre_original[0] = titre.substr(0, pos);
        m_titre_original[1] = d_p3;
        m_titre_original[2] = titre.substr(pos + 1);
        found = true;
    }
    const std::wstring d_p4 = L" - ";
    pos = titre.find(d_p4);
    if (!found && pos != std::wstring::npos)
    {
        m_titre_original[0] = titre.substr(0, pos);
        m_titre_original[1] = d_p4;
        m_titre_original[2] = titre.substr(pos + 3);
        found = true;
    }
    if (!found)
    {
        m_titre_original[0] = titre;
        found = true;
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
        //int i = Console_Lire_txt(genre_str, 0, 0);
        //Console_Lire(genre_str, 0, 0);
        //Console_Lire(hOut, genre_str, 0);// , 0);
        //Console_Lire(hOut, genre_str, 0, L' ');
        std::wcout << genre_str;// << std::endl;
        if (affichage_sous_genre_actif && sous_genre.size() != 0)
        {
            genre_str = keyColor + L"Sous-genre : " + valuesColor + sous_genre + L"\r\n";
            //Console_Lire(genre_str, 0, 0);
            //Console_Lire(hOut, genre_str, 0);// , 0);
            //Console_Lire(hOut, genre_str, 0, L' ');
            std::wcout << genre_str;// << std::endl;
        }
        //PrintStringW(m_hOut, genre_str, 0);
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
        //PrintStringW(m_hOut, image_str, x1);
        //Console_Lire(image_str, x1, 0);
        //Console_Lire(hOut, image_str, 0);// , 0);
        //Console_Lire(hOut, image_str, x1, L' ');
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

        //PrintStringW(m_hOut, genre_str, 0);
        //Console_Lire(nationalite_str, 0, 0);
        //Console_Lire(hOut, nationalite_str, 0);// , 0);
        //Console_Lire(hOut, nationalite_str, 0, L' ');
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
        //Titre_Original.begin();
        std::wstring titre_original_str = keyColor[0] + L"Titre original : " + valuesColor + m_titre_original[0];
        if (m_titre_original[2] != L"")
        {
            titre_original_str += keyColor[1] + m_titre_original[1] + valuesColor;
            titre_original_str += m_titre_original[2];
        }
        //Console_Lire(titre_original_str, 0, 17);
        //Console_Lire(hOut, titre_original_str + L"\r\n", 0);// , 0);
        //Console_Lire(hOut, titre_original_str + L"\r\n", 0, L' ');
        std::wcout << titre_original_str;
    }
}