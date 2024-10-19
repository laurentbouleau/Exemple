#pragma once
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#define _CRT_SECURE_NO_WARNINGS

//#include "film_.h"

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


extern const std::vector<std::wstring> Genre;
extern const std::vector<std::wstring> Sur;
extern const std::vector<std::wstring> Sous_Genre;
extern const std::vector<std::wstring> Nationalite;

const class Person;
struct DateRecord;
struct SequenceVisionnage_film;

struct SequenceVisionnage_film
{
    SequenceVisionnage_film(std::filesystem::path const& m_cheminFichier);
    std::filesystem::path m_cheminFichier;

    static const std::wstring c_filenameFormat;

    //void set_Person(const Person& person);
    std::wstring Print_Dates_de_visionnage(std::vector<DateRecord>& m_DatesVisionnage);
    std::wstring Print_Dates_de_visionnage(std::vector<DateRecord>& m_DatesVisionnage, std::vector<std::wstring>&keyColor, std::wstring& valuesColor);
    //std::wstring m_min = L"min";
    std::vector<std::wstring>m_keyColor;// { L"\x1b[94;1m", L"\x1b[38;2;0;255;0m" }; // keyColor[0] (bleu) et keyColor[1] (vert)
    std::wstring m_valuesColor; // { L"\x1b[38;2;255;255;255m" }; // Blanc
 
    std::vector<DateRecord> m_DatesVisionnage{ 0 };
    std::wstring m_streaming{ L"" };
};

class Film
{
public:
    Film(std::filesystem::path racine);

    void initialiser_Fichier(std::filesystem::path const& cheminFichier);
    void initialiser_Date_de_reprise(std::filesystem::path const& cheminFichier);
    void initialiser_Date_de_sortie(std::filesystem::path const& cheminFichier);
    void initialiser_De(std::filesystem::path const& cheminFichier);
    void initialiser_Distributeur(std::filesystem::path const& cheminFichier);
    void initialiser_Par(std::filesystem::path const& cheminFichier);
    void initialiser_Making_of(std::filesystem::path const& cheminFichier);
    void initialiser_Note(std::filesystem::path const& cheminFichier);
    void initialiser_Soundtrack(std::filesystem::path const& cheminFichier);
    void initialiser_Titre(std::filesystem::path const& cheminFichier);

    void set_Person(const Person& person);
    const void Print();

    std::filesystem::path getRacine() { return racine; };
    std::filesystem::path getFileName() { return racine.filename(); };

    bool m_espace1{false};
    std::pair<std::wstring, std::wstring> m_h;
    std::pair<std::wstring, std::wstring> m_min;
    bool m_espace2{false};
    bool m_espace3{false};
    std::vector<std::wstring>m_keyColor;// { L"\x1b[94;1m", L"\x1b[38;2;0;255;0m" }; // keyColor[0] (bleu) et keyColor[1] (vert)
    std::wstring m_valuesColor;// { L"\x1b[38;2;255;255;255m" }; // Blanc
    
    std::vector<std::wstring> m_resume;

private:
    std::wstring calcul_Date_Affichage() const;
    std::wstring calcul_Duree_affichage() const;
    std::wstring calcul_Signaletique_Jeunesse_affichage() const;
    std::pair<std::wstring, std::wstring> calcul_Sur_Affichage() const;
    std::wstring calcul_Titres_Affichage() const;
    std::pair<std::wstring, std::wstring> calcul_X_Signaletique_Jeunesse_affichage(std::pair<std::wstring, std::wstring>& sur) const;
    //
    std::wstring format_DateEtSur(std::wstring date_str, std::pair<std::wstring, std::wstring>& sur_str) const;

    const void Print_Avec();
    void Print_Avec_etc();
    const void Print_Dates();
    const void Print_Date_de_Reprise();
    const void Print_Date_de_Sortie();
    const void Print_De();
    const void Print_Distributeur();
    void Print_Header() const;
    const void Print_Making_of();
    const void Print_Par();
    std::wstring Print_Note() const;
    const void Print_Soundtracks();

    std::vector<SequenceVisionnage_film>dates{};
    std::filesystem::path racine;

    std::wstring m_audiodescription;
    std::vector<std::pair<std::wstring, std::wstring>> m_avec;
    std::vector<std::wstring> m_catalogue;
    std::tm m_date{0}, m_date_de_reprise{0}, m_date_de_sortie{0};
    std::vector<std::wstring> m_de;
    //bool m_disney{ false };
    std::wstring m_disney_sj;
    std::wstring m_distributeur;
    std::vector<std::wstring> m_genre;
    std::vector<std::wstring> m_image;
    std::vector<std::wstring> m_par;
    bool m_making_of{ false };
    long m_making_of_duree{ -1 };
    std::vector<std::wstring> m_making_of_resume;
    std::vector<std::wstring> m_nationalite;
    //bool m_netflix{ false };
    std::wstring m_netflix_sj;
    double m_note = 0.0;
    std::wstring m_sj;
    std::vector<std::pair<std::wstring, std::wstring>> m_soundtrack;
    std::wstring m_sous_genre;
    std::wstring m_sur;

    std::vector<std::wstring> m_titres;
    long m_duree{ -1 };
    std::vector<std::wstring> m_titres_originaux;

    std::vector<SequenceVisionnage_film> m_visionnages;

    bool affichage_audiodescription_actif = true;
    bool affichage_avec_actif = true;
    bool affichage_avec_etc_actif = true;
    bool affichage_catalogue_actif = true;
    bool affichage_date_actif{ true };
    bool affichage_date_de_reprise_actif = true;
    bool affichage_date_de_sortie_actif = true;
    bool affichage_date_en_salle_ou_sur_actif = true;
    bool affichage_de_actif = true;
    bool affichage_disney_sj_actif = true;
    bool affichage_distributeur_actif = true;
    bool affichage_duree_actif = true;
    bool affichage_image_actif = true;
    bool affichage_genres_actif = true;
    bool affichage_making_of_actif = true;
    bool affichage_nationalite_actif = true;
    bool affichage_netflix_sj_actif = true;
    bool affichage_note_actif = true;
    bool affichage_par_actif = true;
    bool affichage_resume_actif = true;
    bool affichage_sj_actif = true;
    bool affichage_sous_genre_actif = true;
    bool affichage_soundtraks_actif = true;
    bool affichage_sur_actif = true;
    bool affichage_titres_actif = true;
    bool affichage_titres_originaux_actif = true;
    bool affichage_visionnages_actif = true;
    bool affichage_x_sj_actif = true;
};
