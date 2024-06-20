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
#include <numeric>
#include <string_view>

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
struct InfosVisionnage;
struct SequenceVisionnage;
struct Episode;
struct Saison;
class Serie;

struct InfosVisionnage
{
    const Saison& m_saison;

    InfosVisionnage(const Saison& saison, std::filesystem::path const& m_cheminFichier);
    //void Print();
    //std::wstring Print_Dates_de_visionnage(std::vector<DateRecord>& dr);
    //bool Print_Titre_chiffre_et_point_ou_pas(unsigned short int episode);

    std::filesystem::path m_cheminFichier;

    std::wstring m_min = L"min";
    std::vector<std::wstring>keyColor{ L"\x1b[94;1m", L"\x1b[38;2;0;255;0m" }; // keyColor[0] (bleu) et keyColor[1] (vert)
    std::wstring valuesColor = L"\x1b[38;2;255;255;255m"; // Blanc

    unsigned short int m_NumeroSaison{};
    unsigned short int m_NumeroEpisode{}; 
    std::vector<DateRecord> m_DatesVisionnage{ 0 };
    std::wstring m_streaming{ L"" };
 //   bool m_fichier_pas_zero{ false };
    //std::wstring m_titre;
    //std::wstring m_deux_points;
    //std::wstring m_sous_titre;
    std::vector<std::wstring> m_titres;
//    unsigned short int m_numero{ 1 };
    long m_duree{ -1 };
    //std::wstring m_phrases{ L"" };
    std::vector<std::wstring> m_resume;

private:
    void initialiser_Duree(std::wstring& m);

};

struct SequenceVisionnage
{
    //SequenceVisionnage(const Episode& episode) :m_episode{ episode } {};
    //SequenceVisionnage(const SequenceVisionnage& sep) = default;
    SequenceVisionnage(const Episode& episode, const InfosVisionnage& info_vis) :
        m_episode{ episode }, m_titres{ info_vis.m_titres }, m_streaming{ info_vis.m_streaming },
        m_duree_en_seconde{ info_vis.m_duree * 60 }, m_resume{ info_vis.m_resume },
        m_DatesVisionnage{ info_vis.m_DatesVisionnage }
    {};

    void Print();
    std::wstring Print_Dates_de_visionnage(std::vector<DateRecord>& dr);
    bool Print_Titre_chiffre_et_point_ou_pas(unsigned short int episode);

    std::wstring m_min = L"min";
    std::vector<std::wstring>keyColor{ L"\x1b[94;1m", L"\x1b[38;2;0;255;0m" }; // keyColor[0] (bleu) et keyColor[1] (vert)
    std::wstring valuesColor = L"\x1b[38;2;255;255;255m"; // Blanc

    std::wstring m_streaming{ L"" };
    //bool m_fichier_pas_zero{ false };
    //std::wstring m_titre;
    //std::wstring m_deux_points;
    //std::wstring m_sous_titre;
    std::vector<std::wstring> m_titres;
    long m_duree_en_seconde{ -1 };
    //std::wstring m_phrases{ L"" };
    std::vector<std::wstring> m_resume;
    std::vector<DateRecord> m_DatesVisionnage{};

private:
    const Episode& m_episode;
};

struct Episode
{
    const Saison& m_saison;
    Episode(InfosVisionnage const& info_vis) :m_saison{ info_vis.m_saison } {};

    void ajouter_SequenceVisionnage(const InfosVisionnage& info_vis);

    void Print();
    std::vector<SequenceVisionnage> m_liste_sequence_visionnages{};

    int m_numero{ -1 };
    long m_duree{ -1 };
    //std::wstring m_phrases{ L"" };
    std::vector<std::wstring> m_resume;
};

struct Saison
{
public:
    const Serie& m_serie;
    std::vector<std::wstring> m_resume;
    Saison(std::filesystem::path const& cheminFichier, const Serie& serie);
    void ajouter_InfosVisionnage(SequenceVisionnage const& seq_vis);

    void initialiser_Fichier(std::filesystem::path const& cheminFichier);

    //void afficher(std::filesystem::path const& m_cheminFichier);
    //void ajouter_InfosVisionnage(SequenceVisionnage const& seq_vis);
    //Episode creer_Episode(InfosVisionnage const& seq_vis);
    //void creer_SequenceVisionnage(std::filesystem::path const& m_cheminFichier);
    //void initialiser_Dossier(std::filesystem::path const& m_cheminFichier);

    void initialiser_Chaine(std::filesystem::path const& cheminFichier);
    void initialiser_Netflix(std::filesystem::path const& cheminFichier);
    void initialiser_Note(std::filesystem::path const& cheminFichier);
    void initialiser_Resume(std::filesystem::path const& cheminFichier);
    void initialiser_Titre(std::filesystem::path const& cheminFichier);
 
    void Print_Header();
    void Print();
    void Print_Avec();
    void Print_Chaine();
    const void Print_Date_etc();
    void Print_Images();
    void Print_Netflix();
    void Print_Note();

    std::wstring min = L"min";
    std::vector<std::wstring>keyColor{ L"\x1b[94;1m", L"\x1b[38;2;0;255;0m" }; // keyColor[0] (bleu) et keyColor[1] (vert)
    std::wstring valuesColor = L"\x1b[38;2;255;255;255m"; // Blanc

    std::vector<std::pair<std::wstring, std::wstring>> m_avec;
    std::pair<std::tm, std::wstring>m_dossier;
    //std::vector<Episode> episodes;
    //std::vector<InfosVisionnage>infosvisionnages;
    std::wstring m_chaine;
    std::vector<std::wstring> m_image;
    bool m_netflix{ false };
    double m_note = -1.0;
    //std::pair<unsigned short int, std::vector<std::wstring>>saison;
    std::vector<std::wstring> m_titres;

    bool affichage_avec_actif = true;
    bool affichage_chaine_actif = true;
    bool affichage_date_etc_actif = true;
    bool affichage_image_actif = true;
    bool affichage_netflix_actif = true;
    bool affichage_note_actif = true;

    std::map<int, std::shared_ptr<Episode>> m_liste_episodes;

    int m_numero{ -1 };
};

class Serie
{
public:
    Serie(std::filesystem::path racine);
 
    std::vector<std::wstring> Dossier_Titres(std::wstring titres);
    void initialiser_Fichier(std::filesystem::path const& cheminFichier);
    void initialiser_Chaine(std::filesystem::path const& cheminFichier);
    void initialiser_Creee_par(std::filesystem::path const& cheminFichier);
    void initialiser_Duree(std::wstring& m);
    void initialiser_En_relation_avec(std::filesystem::path const& cheminFichier);
    void initialiser_Titre(std::filesystem::path const& cheminFichier, std::vector<std::wstring>& m_titre);

    const void Print();

    std::vector<Saison>saisons{};
    std::filesystem::path getRacine() { return racine; };
    std::filesystem::path getFileName() { return racine.filename(); };

    std::vector<std::wstring> m_resume;

private:
    const std::wstring Calcul_Note_Affichage();

    const void Print_Saison(Saison saison);
    const void Print_Saisons();
    const void Print_Chaine();
    const void Print_Creee_par();
    const void Print_En_relation_avec();
    const void Print_Titre();

    std::filesystem::path racine;

    std::wstring min = L"min";
    std::vector<std::wstring>keyColor{ L"\x1b[94;1m", L"\x1b[38;2;0;255;0m" }; // keyColor[0] (bleu) et keyColor[1] (vert)
    std::wstring valuesColor = L"\x1b[38;2;255;255;255m"; // Blanc

    std::wstring m_audiodescription;
    std::wstring m_chaine;
    std::vector<std::wstring> m_creee_par;
    std::wstring m_disney_sj;
    std::wstring m_en_relation_avec;
    std::vector<std::wstring> m_genre;
    std::vector<std::wstring> m_image;
    std::vector<std::wstring> m_nationalite;
    std::wstring m_netflix_sj;
    double m_note = 0.0;
    std::wstring m_sj;
    std::wstring m_sous_genre;// {};
    std::wstring m_sur;

    std::vector<std::wstring> m_titres;
    long m_duree{ -1 };
    std::vector<std::wstring> m_titres_originaux;

    std::wstring m_dates2;
    //std::vector<std::tm> dates;// { 0 };

    bool affichage_avec_actif = true;
    bool affichage_audiodescription_actif = true;
    bool affichage_chaine_actif = true;
    bool affichage_creee_par_actif = true;
    bool affichage_disney_sj_actif = true;
    bool affichage_episode_titre_actif = true;
    bool affichage_en_relation_avec_actif = true;
    bool affichage_image_actif = true;
    bool affichage_genres_actif = true;
    bool affichage_nationalite_actif = true;
    bool affichage_netflix_sj_actif = true;
    bool affichage_note_actif = true;
    bool affichage_resume_actif = true;
    //bool affichage_saison_actif = true;
    bool affichage_saison_date_etc_actif = true;
    bool affichage_saisons_actif = true;
    bool affichage_serie_actif = true;
    bool affichage_sj_actif = true;
    bool affichage_sous_genre_actif = true;
    bool affichage_sur_actif = true;
    bool affichage_titres_actif = true;
    bool affichage_titres_originaux_actif = true;
};

