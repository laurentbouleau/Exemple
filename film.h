#pragma once
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#define _CRT_SECURE_NO_WARNINGS

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

struct DateRecord;
struct SequenceVisionnage_film;
class AffichagePersonnalisation;

struct SequenceVisionnage_film
{
    SequenceVisionnage_film(std::filesystem::path const& m_cheminFichier);
    std::filesystem::path m_cheminFichier;

    static const std::wstring c_filenameFormat;

    //    const void AffichagePersonnaliser_film(AffichagePersonnalisation perso);

    std::wstring Print_Dates_de_visionnage(std::vector<DateRecord>& m_DatesVisionnage, std::vector<std::wstring>& keyColor, std::wstring& valuesColor);

    std::vector<std::wstring>m_keyColor;
    std::wstring m_valuesColor;

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

    void AffichagePersonnaliser(AffichagePersonnalisation perso);

    void Print();//

    std::filesystem::path getRacine() { return racine; };
    std::filesystem::path getFileName() { return racine.filename(); };

    std::vector<std::wstring> m_resume;

private:
    std::wstring calcul_Date_Affichage() const;
    std::wstring calcul_Duree_affichage() const;
    std::wstring calcul_Signaletique_Jeunesse_affichage() const;
    std::wstring calcul_Sur_Affichage() const;
    std::wstring calcul_Titres_Affichage() const;
    std::wstring calcul_X_Signaletique_Jeunesse_affichage(std::wstring& sur) const;
    //
    std::wstring format_DateEtSur(std::wstring date_str, std::wstring& sur_str) const;

    void Print_Avec(std::vector<std::pair<std::wstring, std::wstring>> avec, const std::wstring str);
    void Print_Avec_etc();
    void Print_Dates();
    void Print_Date_de_Reprise();
    void Print_Date_de_Sortie();
    void Print_De();
    void Print_Distributeur();
    void Print_Header() const;
    void Print_Making_of();
    void Print_Par();
    std::wstring Print_Note() const;
    void Print_Soundtracks();

    std::vector<SequenceVisionnage_film>dates{};
    std::filesystem::path racine;

    std::wstring m_espace1;
    std::wstring m_labelHeureSingulier;
    std::wstring m_labelHeurePluriel;
    std::wstring m_espace2;
    std::wstring m_espace3;
    std::wstring m_labelMinuteSingulier;
    std::wstring m_labelMinutePluriel;
    std::vector<std::wstring>m_keyColor;
    std::wstring m_valuesColor;

    std::vector<std::pair<std::wstring, std::wstring>> m_acteurs_de_doublage_voix_locales;
    std::vector<std::pair<std::wstring, std::wstring>> m_acteurs_de_doublage_voix_originales;
    std::wstring m_audiodescription;
    std::vector<std::pair<std::wstring, std::wstring>> m_avec;
    std::vector<std::wstring> m_catalogue;
    std::tm m_date{ 0 }, m_date_de_reprise{ 0 }, m_date_de_sortie{ 0 };
    std::vector<std::wstring> m_de;
    std::wstring m_disney_sj;
    std::wstring m_distributeur;
    std::vector<std::wstring> m_genre;
    std::vector<std::wstring> m_image;
    std::vector<std::wstring> m_par;
    bool m_making_of{ false };
    long m_making_of_duree{ -1 };
    std::vector<std::wstring> m_making_of_resume;
    std::vector<std::wstring> m_nationalite;
    std::wstring m_netflix_sj;
    double m_note = 0.0;
    std::wstring m_sj;
    std::wstring m_paramount_sj;
    std::vector<std::pair<std::wstring, std::wstring>> m_soundtrack;
    std::wstring m_sous_genre;
    std::wstring m_sur;

    std::vector<std::wstring> m_titres;
    long m_duree{ -1 };
    std::vector<std::wstring> m_titres_originaux;

    std::vector<SequenceVisionnage_film> m_visionnages;
};
