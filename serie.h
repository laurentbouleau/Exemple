#pragma once
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

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
#include <tuple>
#include <regex>

#include <filesystem> // C++17 standard header file name
//using namespace std;

extern const std::vector<std::wstring> Genre;
extern const std::vector<std::wstring> Sur;
extern const std::vector<std::wstring> Sous_Genre;
extern const std::vector<std::wstring> Nationalite;

/*class exception_Episode_0
{
    std::wstring message;
public:
    exception_Episode_0() : message(L"Saison::afficher_Episode() : pas de x !!!") {}
    std::wstring get_message() const { return message; }
};*/


struct DateRecord;
struct Saison;

struct Episode
{
    //Episode(void);
    //Episode(std::filesystem::path const& cheminFichier);// { m_cheminFichier = cheminFichier; };
    Episode(/*unsigned short int prefixe, */std::filesystem::path const& cheminFichier);
    //~Episode();
    struct Saison;
    void afficher();
    void Print();
    bool Print_Titre_chiffre_et_point_ou_pas(unsigned char episode);
    const bool Print_Date_ou_Dates();

    std::filesystem::path m_cheminFichier;

    std::wstring min = L"min";
    std::vector<std::wstring>keyColor{ L"\x1b[94;1m", L"\x1b[38;2;0;255;0m" }; // keyColor[0] (bleu) et keyColor[1] (vert)
    std::wstring valuesColor = L"\x1b[38;2;255;255;255m"; // Blanc

    unsigned char saison{};
    unsigned char episode{};
    std::vector<DateRecord> dr;
    std::wstring streaming;
    bool b { false };
    std::wstring titre;
    std::wstring deux_points;
    std::wstring sous_titre;
    std::tm tm{ 0 };
    std::wstring phrases;

    bool affichage_Print_actif = true;
    bool affichage_date_ou_dates = true;
};


struct Saison
{
public:
    Saison(void);
    ~Saison();
    void afficher(std::filesystem::path const& cheminFichier);
    void afficher_Dossier(std::filesystem::path const& cheminDossier);
    void afficher_Fichier(std::filesystem::path const& cheminFichier);
    void afficher_Note(std::filesystem::path const& cheminFichier);
    void afficher_Titre(std::filesystem::path const& cheminFichier);

    void Creer_Episode(std::filesystem::path const& cheminFichier);

    void Print();
    void Print_Avec();
    void Print_Images();
    const void Print_Date_etc();

    std::wstring min = L"min";
    std::vector<std::wstring>keyColor{ L"\x1b[94;1m", L"\x1b[38;2;0;255;0m" }; // keyColor[0] (bleu) et keyColor[1] (vert)
    std::wstring valuesColor = L"\x1b[38;2;255;255;255m"; // Blanc

    std::vector<std::pair<std::wstring, std::wstring>> avec;
    std::pair<std::tm, std::wstring>dossier;
    std::vector<Episode> episodes;
    std::vector<std::wstring> image;
    double note = -1.0;
    std::pair<unsigned short int, std::wstring>saison;
    std::wstring titre;

    bool affichage_Avec_actif = true;
    bool affichage_Date_etc_actif = true;
    bool affichage_image_actif = true;
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

