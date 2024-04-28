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
#include <map>
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

template<typename ... Args>
std::wstring wstring_format(const std::wstring& format, Args ... args)
{
    int size_s = _snwprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
    if (size_s <= 0) { throw std::runtime_error("Error during formatting."); }
    auto size = static_cast<size_t>(size_s);
    std::unique_ptr<wchar_t[]> buf(new wchar_t[size]);
    _snwprintf(buf.get(), size, format.c_str(), args ...);
    return std::wstring(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

struct DateRecord;
struct SequenceVisionnage;
struct Episode;
struct Saison;
 
struct InfosVisionnage
{
    struct Saison;
    struct Episode;
    //InfosVisionnage(void);
    InfosVisionnage(std::filesystem::path const& m_cheminFichier);
    void creer_InfosVisionnage(std::filesystem::path const& m_cheminFichier);
    void initialiser_duree(std::wstring& m);
    void Print();
    std::wstring Print_Dates_de_visionnage(std::vector<DateRecord>& dr);
    bool Print_Titre_chiffre_et_point_ou_pas(unsigned short int episode);

    std::filesystem::path m_cheminFichier;

    std::wstring m_min = L"min";
    std::vector<std::wstring>keyColor{ L"\x1b[94;1m", L"\x1b[38;2;0;255;0m" }; // keyColor[0] (bleu) et keyColor[1] (vert)
    std::wstring valuesColor = L"\x1b[38;2;255;255;255m"; // Blanc

    unsigned short int m_NumeroSaison{};
    unsigned short int m_NumeroEpisode{}; 
    //std::vector<DateRecord> m_dates{ 0 };
    std::vector<DateRecord> m_DatesVisionnage{ 0 };
    std::wstring m_streaming{ L"" };
    bool m_fichier_pas_zero{ false };
    std::wstring m_titre;
    std::wstring m_deux_points;
    std::wstring m_sous_titre;
    unsigned short int m_numero{ 1 };
    long m_Duree{ -1 };
    std::wstring m_phrases{ L"" };
    // ???
};

struct SequenceVisionnage
{
    struct Saison;
    //SequenceVisionnage(std::filesystem::path const& m_cheminFichier);
    SequenceVisionnage(InfosVisionnage const& vis);
    //void initialiser_duree(std::wstring& m);
    void Print();
    //std::wstring Print_Dates_de_visionnage(std::vector<DateRecord>& dr);
    //bool Print_Titre_chiffre_et_point_ou_pas(unsigned short int episode);

    std::filesystem::path m_cheminFichier;

    std::wstring m_min = L"min";
    std::vector<std::wstring>keyColor{ L"\x1b[94;1m", L"\x1b[38;2;0;255;0m" }; // keyColor[0] (bleu) et keyColor[1] (vert)
    std::wstring valuesColor = L"\x1b[38;2;255;255;255m"; // Blanc

    //unsigned short int m_saison{};
    //unsigned short int m_episode{};
    //std::vector<DateRecord> m_dates{ 0 };
    //std::wstring m_streaming{ L"" };
    //bool m_fichier_pas_zero{ false };
    //std::wstring m_titre;
    //std::wstring m_deux_points;
    //std::wstring m_sous_titre;
    //unsigned short int m_numero{ 1 };
    //long m_duree_en_seconde{ -1 };
    //std::wstring m_phrases{ L"" };


    std::vector<InfosVisionnage>m_liste_visionnages;
};

struct Episode
{
    struct Saison;
    Episode(std::filesystem::path const& cheminFichier);
    Episode(SequenceVisionnage const& seq_vis);

    void ajouter_InfosVisionnage(InfosVisionnage const& seq_vis);
    void creer_Episode(InfosVisionnage const& seq_vis);

    void Print();
    void Print_Data();
    //::vector<InfosVisionnage> info_vis;
    //std::vector<SequenceVisionnage> sequencevisionnages;
    std::vector<SequenceVisionnage> m_liste_visionnages{ 0 };
};

struct Saison
{
public:
    Saison(void);
    ~Saison();
    //struct Episode;
    void afficher(std::filesystem::path const& m_cheminFichier);
    void ajouter_InfosVisionnage(SequenceVisionnage const& seq_vis);

    //
//    void creer_InfosVisionnage(std::filesystem::path const& m_cheminFichier);
    //
    
    //void creer_Episode(SequenceVisionnage const& seq_vis);
    Episode creer_Episode(SequenceVisionnage const& seq_vis);
    void creer_SequenceVisionnage(std::filesystem::path const& m_cheminFichier);

    void initialiser_Dossier(std::filesystem::path const& m_cheminFichier);
    void initialiser_Fichier(std::filesystem::path const& m_cheminFichier);

    void initialiser_Chaine(std::filesystem::path const& m_cheminFichier);
    void initialiser_Netflix(std::filesystem::path const& m_cheminFichier);
    void initialiser_Note(std::filesystem::path const& m_cheminFichier);
    void initialiser_Titre(std::filesystem::path const& m_cheminFichier);

 
    void Print();
    void Print_Avec();
    void Print_Chaine();
    const void Print_Date_etc();
    void Print_Images();
    void Print_Netflix();

    std::wstring min = L"min";
    std::vector<std::wstring>keyColor{ L"\x1b[94;1m", L"\x1b[38;2;0;255;0m" }; // keyColor[0] (bleu) et keyColor[1] (vert)
    std::wstring valuesColor = L"\x1b[38;2;255;255;255m"; // Blanc

    std::vector<std::pair<std::wstring, std::wstring>> m_avec;
    std::pair<std::tm, std::wstring>m_dossier;
    std::vector<Episode> episodes;
    std::vector<InfosVisionnage>infosvisionnages;
    std::wstring m_chaine;
    std::vector<std::wstring> m_image;
    bool netflix{ false };
    double m_note = -1.0;
    std::pair<unsigned short int, std::wstring>saison;
    std::wstring m_titre;

    bool affichage_avec_actif = true;
    bool affichage_chaine_actif = true;
    bool affichage_date_etc_actif = true;
    bool affichage_image_actif = true;
    bool affichage_netflix_actif = true;

    std::map<int, Episode> m_liste_episodes;


};

class Serie
{
public:
    Serie(std::filesystem::path racine);
    ~Serie();
    void initialiser_Fichier(std::filesystem::path const& m_cheminFichier);
    void initialiser_Chaine(std::filesystem::path const& m_cheminFichier);

    const void Print();

    std::vector<Saison>saisons{};
    std::filesystem::path getRacine() { return racine; };
    std::filesystem::path getFileName() { return racine.filename(); };
private:
    const void Print_Saison(Saison saison);
    const void Print_Saisons();
    const void Print_Chaine();

    std::filesystem::path racine;

    std::wstring min = L"min";
    std::vector<std::wstring>keyColor{ L"\x1b[94;1m", L"\x1b[38;2;0;255;0m" }; // keyColor[0] (bleu) et keyColor[1] (vert)
    std::wstring valuesColor = L"\x1b[38;2;255;255;255m"; // Blanc

    std::wstring m_audiodescription;
    std::wstring m_chaine;
    std::vector<std::wstring> m_genre;
    std::vector<std::wstring> m_image;
    std::vector<std::wstring> m_nationalite;
    std::wstring m_sous_genre{};


    bool affichage_avec_actif = true;
    bool affichage_audiodescription_actif = true;
    bool affichage_chaine_actif = true;
    bool affichage_episode_titre_actif = true;
    bool affichage_image_actif = true;
    bool affichage_genres_actif = true;
    bool affichage_nationalite_actif = true;
    //bool affichage_saison_actif = true;
    bool affichage_saison_date_etc_actif = true;
    bool affichage_saisons_actif = true;
    bool affichage_serie_actif = true;
    bool affichage_sous_genre_actif = true;
};

