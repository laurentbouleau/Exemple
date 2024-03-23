#pragma once
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#define _CRT_SECURE_NO_WARNINGS

//#define TXT_                             0
//#define JGP_                             1
//#define PNG_                             2
//#define WEBP_                            3

#include "Exemple.h"
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
//#include <stdio.h>
#include <locale>
#include <algorithm>
#include <codecvt>
#include <sstream>
//#include <tuple>
#include <regex>

#include <filesystem> // C++17 standard header file name

extern const std::vector<std::wstring> Genre;
extern const std::vector<std::wstring> Sur;
extern const std::vector<std::wstring> Sous_Genre;
extern const std::vector<std::wstring> Nationalite;

/*const std::vector<std::wstring> min =
{
    L"min",
    L"Min",
    L"MIN"
};*/

struct DateRecord;
struct Saison;
 
struct Saison_Episode
{
    friend struct Episode;
    std::wstring titre;
    std::wstring deux_points;
    std::wstring sous_titre;
};

struct Episode
{
    //Episode(std::filesystem::path const& cheminFichier);// { m_cheminFichier = cheminFichier; };
    Episode(std::filesystem::path const& cheminFichier);
    struct Saison;
    friend Saison_Episode;// saison_epise{};
    //struct Saison_Episode;
    void afficher();
    void initialiser_duree(std::wstring& m);
    void Print();
    bool Print_Titre_chiffre_et_point_ou_pas(unsigned short int episode);
    std::wstring Print_Date_ou_Dates(std::vector<DateRecord>& dr);

    std::filesystem::path m_cheminFichier;

    std::wstring min = L"min";
    std::vector<std::wstring>keyColor{ L"\x1b[94;1m", L"\x1b[38;2;0;255;0m" }; // keyColor[0] (bleu) et keyColor[1] (vert)
    std::wstring valuesColor{ L"\x1b[38;2;255;255;255m" }; // Blanc

    unsigned short int saison{};
    unsigned short int episode{};
    std::vector<DateRecord> dates_de_diffusion{ 0 };
    std::wstring streaming{ L"" };
    bool fichier_pas_zero { false };
    std::wstring titre;
    std::wstring deux_points;
    std::wstring sous_titre;
    long duree_en_seconde{ -1 };
    std::wstring phrases{ L"" };

    bool affichage_Print_actif = true;
    bool affichage_Date_ou_dates = true;
};


struct Saison
{
public:
    Saison(void);
    ~Saison();
    void afficher(std::filesystem::path const& cheminFichier);
    void afficher_Dossier(std::filesystem::path const& cheminDossier);
    void afficher_Fichier(std::filesystem::path const& cheminFichier);
    void afficher_Netflix(std::filesystem::path const& cheminFichier);
    void afficher_Note(std::filesystem::path const& cheminFichier);
    void afficher_Titre(std::filesystem::path const& cheminFichier);

    void Creer_Episode(std::filesystem::path const& cheminFichier);

    void Print();
    void Print_Avec();
    void Print_Images();
    void Print_Netflix();
    const void Print_Date_etc();

    std::wstring min = L"min";
    std::vector<std::wstring>keyColor{ L"\x1b[94;1m", L"\x1b[38;2;0;255;0m" }; // keyColor[0] (bleu) et keyColor[1] (vert)
    std::wstring valuesColor = L"\x1b[38;2;255;255;255m"; // Blanc

    std::vector<std::pair<std::wstring, std::wstring>> avec;
    std::pair<std::tm, std::wstring>dossier;
    std::vector<Episode> episodes;
    std::vector<std::wstring> image;
    bool netflix{ false };
    double note = -1.0;
    std::pair<unsigned short int, std::wstring>saison;
    std::wstring titre;

    //struct Saison_Episode saison_epise {};

    bool affichage_Avec_actif = true;
    bool affichage_Date_etc_actif = true;
    bool affichage_Image_actif = true;
    bool affichage_Netflix_actif = true;
};

class Serie
{
public:
    Serie(std::filesystem::path racine);
    ~Serie();
    const void Print();
    std::vector<Saison>saisons{};
    std::filesystem::path getRacine() { return racine; };
    std::filesystem::path getFileName() { return racine.filename(); };
private:
    //const bool PrintDate_ou_Dates();
 //   const void PrintAvec(const std::vector<std::pair<std::wstring, std::wstring>> avec);
    //const void afficher();
    //const bool PrintEpisode_Titre_chiffre_et_point_ou_pas(std::wstring& titre);
    //const void PrintEpisode_Titre(std::tuple<unsigned int, std::wstring, std::wstring, std::wstring, std::tm, std::wstring>& e_t);
    const void Print_Episodes(Saison saison);
    const void Print_Saison(Saison saison);
    //const void PrintSaison_Date_etc(Saison saison);
    const void Print_Saisons();

    std::filesystem::path racine;

    std::wstring min = L"min";
    std::vector<std::wstring>keyColor{ L"\x1b[94;1m", L"\x1b[38;2;0;255;0m" }; // keyColor[0] (bleu) et keyColor[1] (vert)
    std::wstring valuesColor = L"\x1b[38;2;255;255;255m"; // Blanc

    //bool affichage;
    bool afficage_Avec_actif = true;
    bool affichage_Episode_Titre_actif = true;
    //bool affichage_Episodes_actif = true;
    bool affichage_Saison_actif = true;
    bool affichage_Saison_Date_etc_actif = true;
    bool affichage_Saisons_actif = true;
};

