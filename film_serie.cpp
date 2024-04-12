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
// # const void initialiser_Audiodescription(std::wstring& a_filename, std::wstring const& nomFichier, std::wstring& audiodescription)                  #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const void initialiser_Audiodescription(fs::path const& cheminFichier, std::wstring& ad)
{
    auto nomFichier = cheminFichier.wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    std::wstring a = lire_fichierTxt(nomFichier);
    std::size_t pos = a.find(L"Audiodescription");
    if (pos == std::wstring::npos)
        ;
    else
        a = a.substr(0, pos - 3);
    if (std::find(::Audiodescription.begin(), ::Audiodescription.end(), a) != ::Audiodescription.end())
        ad = a;
    assert((ad.size() != 0));
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const void initialiser_Avec(fs::path const& cheminFichier, std::vector<std::pair<std::wstring, std::wstring>>& avec)                               #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const void initialiser_Avec(fs::path const& cheminFichier, std::vector<std::pair<std::wstring, std::wstring>>& avec)
{
    auto nomFichier = cheminFichier.filename().wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    avec = lire_paireCleValeur_depuisFichierTxt(cheminFichier.wstring(), L" : ");
    assert((avec.size() != 0));
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const void initialiser_Genre(fs::path const& cheminFichier,                                                                                        #
// #                           std::vector<std::wstring>& genres_renvoyes,                                                                              #
// #                           const std::vector<std::wstring>& genres_valides)                                                                         #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const void initialiser_Genre(fs::path const& cheminFichier, std::vector<std::wstring>& genres_renvoyes, const std::vector<std::wstring>& genres_valides)
{ // Genre
    auto nomFichier = cheminFichier.wstring();
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
// # const void initialiser_Image(fs::path const& cheminFichier, std::vector<std::wstring>& images)                                                     #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const void initialiser_Image(fs::path const& cheminFichier, std::vector<std::wstring>& images)
{
    auto nomFichier = cheminFichier.filename().wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    images.push_back(nomFichier);
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const void initialiser_Nationalite(std::wstring& nationalite_filename,                                                                             #
// #                                 std::wstring const& nomFichier,                                                                                    #
// #                                 std::vector<std::wstring>& nationalites_renvoyes,                                                                  #
// #                                 const std::vector<std::wstring>& nationalites_valides)                                                             #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const void initialiser_Nationalite(fs::path const& cheminFichier, std::vector<std::wstring>& nationalites_renvoyes, const std::vector<std::wstring>& nationalites_valides)
{ // Nationalite
    auto nomFichier = cheminFichier.wstring();
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
// ######################################################################################################################################################

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const void Print_Audiodescription(const std::wstring& audiodescription, bool affichage_audiodescription_actif,                                     #
// #                                  std::wstring& keyColor, std::wstring& valuesColor)                                                                #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Print_Audiodescription(const std::wstring& audiodescription, bool affichage_audiodescription_actif, std::wstring& keyColor, std::wstring& valuesColor)
{
    if (affichage_audiodescription_actif && audiodescription.size() > 0)
    {
        std::wstring audiodescription_str = keyColor + L"Audiodescription : " + valuesColor + audiodescription + L"\r\n";
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
// # const void Print_Nationalites(const std::vector<std::wstring>& nationalites, bool affichage_nationalite_actif                                      #                                                                                                    #
// #                        std::wstring& keyColor, std::wstring& valuesColor)                                                                          #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Print_Nationalites(const std::vector<std::wstring>& nationalites, bool affichage_nationalite_actif, std::wstring& keyColor, std::wstring& valuesColor)
{
    if (affichage_nationalite_actif && nationalites.size() > 0)
    {
        std::wstring nationalite_str = keyColor + L"Nationalite" + ((nationalites.size() > 1) ? L"s" : L"") + L" : " + valuesColor;

        bool first = true;
        for (auto&& nationalite : nationalites)
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

