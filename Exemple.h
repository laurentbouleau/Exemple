#pragma once

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#define TXT_                             0
#define JGP_                             1
#define PNG_                             2
#define WEBP_                            3

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

inline std::wstring space(int i, wchar_t espace);
//const void Console_Lire(std::wstring wstr, int x1, int x2);

//
//void Console_Lire(HANDLE hOut, const std::wstring& wstr, int taille_indentation, wchar_t caractere_indentation = L' ');
//


class exception_date_tiret
{
    std::wstring message;
public:
    exception_date_tiret() : message(L"aaaaaaaaaa") {}
    std::wstring get_message() const { return message; }
};


class exception_test_sp_et_npos_ou_pas_isblank
{
    std::wstring message;
public:
    exception_test_sp_et_npos_ou_pas_isblank() : message(L"Espace invalide !!!") {}
    std::wstring get_message() const { return message; }
};

struct DateRecord
{
    std::tm date{ 0 };
    bool someFlag{ false };
};

class Saison
{
public:
    Saison(void);
    //Saison(std::filesystem::path racine);
    ~Saison();
    //void affichier_Dossier(std::wstring const& nomDossier);
    void afficher_Dossier(std::filesystem::path const& nomDossier);
    //void affichier_sous_Fichier(std::wstring const& nomFichier, int const& nomImage = TXT_);
    void afficher_Fichier(std::filesystem::path const& nomFichier);
    void afficher_Date_ou_Dates(std::wstring const& nomFichier);
    void afficher_Titres(std::wstring const& nomFichier);
    void afficher_Episodes(std::filesystem::path const& nomFichier);
    void ok(std::wstring const& nomFichier);
    //                               x          t1            t2           t3           temps      p
    std::vector<std::tuple<unsigned int, std::wstring, std::wstring, std::wstring, std::tm, std::wstring>> titres;
    unsigned short int episode{ 0 };
    //std::filesystem::path getRacine() { return racine; };
    //std::filesystem::path getFileName() { return racine2.filename(); };

private:
    std::pair<std::tm, std::wstring>dossier;
    std::pair<unsigned short int, std::wstring>episodes;
    unsigned short int saison_x = 0;
    std::vector<std::pair<std::wstring, std::wstring>> avec;
    //std::vector<std::pair<std::vector<DateRecord>, std::wstring>>date_ou_dates;
    std::vector<std::tuple<unsigned int, std::vector<DateRecord>, std::wstring>>date_ou_dates;
    //std::vector<std::pair<unsigned int, std::vector<std::wstring>>>titres;
    std::vector<std::wstring> image;
};

class Serie
{
public:
    Serie(std::filesystem::path racine);
    ~Serie();
    std::vector<Saison>saisons{};
    std::filesystem::path getRacine() { return racine; };
    std::filesystem::path getFileName() { return racine.filename(); };
    //protected:
    //std::wstring nomDossier{};
private:
    //const bool PrintDate_ou_Dates();
    void PrintDate_ou_Dates();
    std::filesystem::path racine;
    bool affichage_Date_ou_Dates = true;
 };

