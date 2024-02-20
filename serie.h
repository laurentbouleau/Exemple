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
#include <filesystem> // C++17 standard header file name
//using namespace std;

extern const std::vector<std::wstring> Genre;
extern const std::vector<std::wstring> Sur;
extern const std::vector<std::wstring> Sous_Genre;
extern const std::vector<std::wstring> Nationalite;

struct DateRecord;

class Saison
{
public:
    Saison(void);
    ~Saison();
    //void afficher_Avec(std::filesystem::path const& nomFichier);
    //void afficher_Dossier(std::filesystem::path const& nomDossier);
    //void afficher_Fichier(std::filesystem::path const& nomFichier);
    //void afficher_Date_ou_Dates(std::filesystem::path const& nomFichier);
    //void afficher_Date_ou_Dates_Titres(std::filesystem::path const& nomFichier);
    //void afficher_Episodes(std::filesystem::path const& nomFichier);
    //void afficher_Note(std::filesystem::path const& nomFichier);
    void afficher_Avec(std::filesystem::path const& cheminFichier);
    void afficher_Dossier(std::filesystem::path const& cheminDossier);
    void afficher_Fichier(std::filesystem::path const& cheminFichier);
    void afficher_Episode(std::filesystem::path const& cheminFichier);
    void afficher_Episode_Titres(std::filesystem::path const& cheminFichier);
    void afficher_Note(std::filesystem::path const& cheminFichier);
    void afficher_Saison(std::filesystem::path const& cheminFichier);
    void afficher_Titre(std::filesystem::path const& cheminFichier);
    //unsigned short int episode{ 0 };

private:

    //                               x          t1            t2           t3           temps      p
    std::vector<std::tuple<unsigned int, std::wstring, std::wstring, std::wstring, std::tm, std::wstring>> episode_titres;

//protected:
    std::vector<std::pair<std::wstring, std::wstring>> avec;
    std::pair<std::tm, std::wstring>dossier;
    std::wstring dos = L"";
    std::vector<std::tuple<unsigned int, std::vector<DateRecord>, std::wstring>>episode;
    std::pair<unsigned short int, std::wstring>saison;
    std::vector<std::wstring> image;
    double note = -1.0;
};

class Serie
{
public:
    Serie(std::filesystem::path racine);
    ~Serie();
    void Print();
    std::vector<Saison>saisons{};
    std::filesystem::path getRacine() { return racine; };
    std::filesystem::path getFileName() { return racine.filename(); };
private:
    //const bool PrintDate_ou_Dates();
    void PrintEpisode();
    void PrintSaisons();

    std::filesystem::path racine;

    std::vector<std::wstring>keyColor{ L"\x1b[94;1m", L"\x1b[38;2;0;255;0m" }; // keyColor[0] (bleu) et keyColor[1] (vert)
    std::wstring valuesColor = L"\x1b[38;2;255;255;255m"; // Blanc

    //bool affichage;
    bool affichage_Episode_actif = true;
    bool affichage_Saisons_actif = true;
};

