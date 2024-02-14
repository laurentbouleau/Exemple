#pragma once
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#define TXT_                             0
#define JGP_                             1
#define PNG_                             2
#define WEBP_                            3

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
using namespace std;
namespace fs = std::filesystem;

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
    void afficher_Avec(fs::path const& nomFichier);
    void afficher_Dossier(fs::path const& nomDossier);
    void afficher_Fichier(fs::path const& nomFichier);
    void afficher_Date_ou_Dates(fs::path const& nomFichier);
    void afficher_Date_ou_Dates_Titres(fs::path const& nomFichier);
    void afficher_Episodes(fs::path const& nomFichier);
    void afficher_Note(fs::path const& nomFichier);

    unsigned short int episode{ 0 };

private:

    //                               x          t1            t2           t3           temps      p
    std::vector<std::tuple<unsigned int, std::wstring, std::wstring, std::wstring, std::tm, std::wstring>> titres;


    std::vector<std::pair<std::wstring, std::wstring>> avec;
    std::vector<std::tuple<unsigned int, std::vector<DateRecord>, std::wstring>>date_ou_dates;
    std::pair<std::tm, std::wstring>dossier;
    std::pair<unsigned short int, std::wstring>episodes;
    std::vector<std::wstring> image;
    double note = -1.0;
};

class Serie
{
public:
    Serie(fs::path racine);
    ~Serie();
    std::vector<Saison>saisons{};
    fs::path getRacine() { return racine; };
    fs::path getFileName() { return racine.filename(); };
private:
    //const bool PrintDate_ou_Dates();
    void PrintDate_ou_Dates();
    std::filesystem::path racine;
    bool affichage_Date_ou_Dates = true;
};

