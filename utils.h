#pragma once
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#define _CRT_SECURE_NO_WARNINGS

#include "Exemple.h"

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

std::wstring replace_all(std::wstring subject, const std::wstring& search, const std::wstring& replace);
static inline void rtrim(std::wstring& s);
std::string wstr_to_u8(std::wstring uneWString);
const std::vector<std::pair<std::wstring, std::wstring>>lire_paireCleValeur_depuisFichierTxt(std::wstring const& nomFichier);
const std::vector<std::pair<std::wstring, std::wstring>>lire_paireCleValeur_depuisFichierTxt(std::wstring const& nomFichier, std::wstring separeteur_pattern);
const std::vector<std::wstring> lire_fichierTxt(std::wstring const& nomFichier, std::vector<std::wstring> separeteurs);
const std::vector<std::wstring> lire_fichierTxt(std::wstring const& nomFichier, std::vector<std::wstring> separeteurs, bool f);
const std::wstring lire_fichierTxt(std::wstring const& nomFichier);
inline std::wstring space(int i, wchar_t espace);
//void Console_Lire(HANDLE hOut, const std::wstring& wstr, int taille_indentation, wchar_t caractere_indentation = L' ');

void test_date_tiret_sp_etc(wchar_t d);
void test_sp_et_npos_ou_pas_isblank(wchar_t sp, bool t);
void test_sp_et_npos_ou_pas_isblank(wchar_t sp, bool t);
bool checkyear(int y);
bool checkmonth(int m);
bool checkday(int m, int d, int y);

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # Classes                                                                                                                                            #
// #                                                                                                                                                    #
// ######################################################################################################################################################

class exception_date_tiret
{
    std::wstring message;
public:
    exception_date_tiret() : message(L"CCCC-BB-AA pas de...") {}
    std::wstring get_message() const { return message; }
};

class exception_date_tiret_sp_etc
{
    std::wstring message;
public:
    exception_date_tiret_sp_etc() : message(L"bbbbbbbbbbb") {}
    std::wstring get_message() const { return message; }
};

class exception_test_sp_et_npos_ou_pas_isblank
{
    std::wstring message;
public:
    exception_test_sp_et_npos_ou_pas_isblank() : message(L"Espace invalide !!!") {}
    std::wstring get_message() const { return message; }
};

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # Structs                                                                                                                                            #
// #                                                                                                                                                    #
// ######################################################################################################################################################

struct DateRecord
{
    std::tm date{ 0 };
    bool someFlag{ false };
};

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # Template<typename ... Args>                                                                                                                        #
// #                                                                                                                                                    #
// ######################################################################################################################################################

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

//
//

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
    std::vector<std::pair<std::wstring, std::wstring>> m_soundtrack;
    std::wstring m_sous_genre;
    std::wstring m_sur;

    std::vector<std::wstring> m_titres;
    long m_duree{ -1 };
    std::vector<std::wstring> m_titres_originaux;

    std::vector<SequenceVisionnage_film> m_visionnages;
};

//
//

struct DateRecord;
struct InfosVisionnage;
struct SequenceVisionnage;
struct Episode;
struct Saison;
class Serie;

class FileFormatException : public std::runtime_error
{
public:
    explicit FileFormatException(const std::string& message)
        : std::runtime_error(message) {}
};

struct InfosVisionnage
{
    const Saison& m_saison;

    InfosVisionnage(const Saison& saison, std::filesystem::path const& m_cheminFichier);

    std::filesystem::path m_cheminFichier;

    std::vector<std::wstring>m_keyColor{ L"\x1b[94;1m", L"\x1b[38;2;0;255;0m" }; // keyColor[0] (bleu) et keyColor[1] (vert)
    std::wstring m_valuesColor = L"\x1b[38;2;255;255;255m"; // Blanc

    long m_NumeroSaison{};
    long m_NumeroEpisode{};
    std::vector<DateRecord> m_DatesVisionnage{ 0 };
    std::wstring m_streaming{ L"" };
    std::vector<std::wstring> m_titres;

    int m_numero{ -1 };

    long m_duree_en_seconde{ 0 };
    std::vector<std::wstring> m_resume;

private:
    void initialiser_Duree(std::wstring& m);
};

struct SequenceVisionnage
{
    SequenceVisionnage(const Episode& episode, const SequenceVisionnage& src) :
        m_episode{ episode }, m_titres{ src.m_titres }, m_streaming{ src.m_streaming },
        m_duree_en_seconde{ src.m_duree_en_seconde }, m_resume{ src.m_resume },
        m_DatesVisionnage{ src.m_DatesVisionnage }/*, m_numero{src.m_numero}*/, m_NumeroEpisode{ src.m_NumeroEpisode },
        m_espace3{ src.m_espace3 }, m_labelMinuteSingulier{ src.m_labelMinuteSingulier }, m_labelMinutePluriel{ src.m_labelMinutePluriel },
        m_keyColor{ src.m_keyColor }, m_valuesColor{ src.m_valuesColor }
    {
    };
    SequenceVisionnage(const Episode& episode, const InfosVisionnage& info_vis) :
        m_episode{ episode }, m_titres{ info_vis.m_titres }, m_streaming{ info_vis.m_streaming },
        m_duree_en_seconde{ info_vis.m_duree_en_seconde }, m_resume{ info_vis.m_resume },
        m_DatesVisionnage{ info_vis.m_DatesVisionnage }/*, m_numero{info_vis.m_numero}*/, m_NumeroEpisode{ info_vis.m_NumeroEpisode },
        //m_espace3{ info_vis.m_espace3 }, m_labelMinuteSingulier{ info_vis.m_labelMinuteSingulier }, m_labelMinutePluriel{ info_vis.m_labelMinutePluriel },
        m_keyColor{ info_vis.m_keyColor }, m_valuesColor{ info_vis.m_valuesColor }
    {
    };
    boolean operator==(const SequenceVisionnage& rhs) const { return this == &rhs; };
    //static std::wstring m_tr;
    std::wstring calcul_Duree_affichage(int numero_sequence) const;

    void AffichagePersonnaliser(AffichagePersonnalisation perso);

    void Print(int numero_sequence) const;

    std::wstring Print_Dates_de_visionnage() const;
    long Print_Titre_chiffre(long episode) const;

    std::wstring m_espace1;
    std::wstring m_labelHeureSingulier;
    std::wstring m_labelHeurePluriel;
    std::wstring m_espace2;
    std::wstring m_espace3;
    std::wstring m_labelMinuteSingulier;
    std::wstring m_labelMinutePluriel;
    std::vector<std::wstring>m_keyColor;
    std::wstring m_valuesColor;

    std::wstring m_streaming{ L"" };
    std::vector<std::wstring> m_titres;
    long m_duree_en_seconde{ -1 };
    std::vector<std::wstring> m_resume{};
    std::vector<DateRecord> m_DatesVisionnage{};

    long m_NumeroEpisode{ -1 };
    long m_duree{ -1 };

private:
    const Episode& m_episode;
};

struct Episode
{
    const Saison& m_saison;
    Episode(const InfosVisionnage& info_vis);
    Episode(const Episode& src) :m_saison{ src.m_saison }
    {
        operator=(src);
    }
    Episode(const Saison& saison, Episode&& src) :m_saison{ saison }
    {
        operator=(std::move(src));
    };

    void ajouter_SequenceVisionnage(const InfosVisionnage& info_vis);

    void AffichagePersonnaliser(AffichagePersonnalisation perso);

    //long long GetNumeroSequenceVisionnage(const SequenceVisionnage& sev_vis) const;

    void Print();

    std::wstring m_espace1;
    std::wstring m_labelHeureSingulier;
    std::wstring m_labelHeurePluriel;
    std::wstring m_espace2;
    std::wstring m_espace3;
    std::wstring m_labelMinuteSingulier;
    std::wstring m_labelMinutePluriel;
    std::vector<std::wstring>m_keyColor;
    std::wstring m_valuesColor;

    long m_episode{};
    std::vector<SequenceVisionnage> m_liste_sequence_visionnages_ordonnee_chronologiquement;

    int m_numero{ -1 };
    long m_duree{ -1 };
    std::vector<std::wstring> m_resume;

    //int lInfoQuiMInteresse;

    //long m_NumeroEpisode{1};
private:
    Episode& operator=(const Episode& src)
    {
        if (&src != this)
        {
            for (auto const& lsvoc : src.m_liste_sequence_visionnages_ordonnee_chronologiquement)
            {
                m_liste_sequence_visionnages_ordonnee_chronologiquement.emplace_back(SequenceVisionnage{ *this, lsvoc });
            }
            m_resume = src.m_resume;
            m_duree = src.m_duree;
            m_numero = src.m_numero;

            m_keyColor = src.m_keyColor;
            m_valuesColor = src.m_valuesColor;
        }
        return *this;
    }
    Episode& operator=(Episode&& src) noexcept
    {
        operator=(src);
        return *this;
    }
};

struct Saison
{
public:
    const Serie& m_serie;
    Saison(std::filesystem::path const& cheminFichier, const Serie& serie);
    Saison(Saison&& src) noexcept : m_serie{ src.m_serie }// the expression "arg.member" is lvalue
    {
        operator=(std::move(src));
    }

    void initialiser_Fichier(std::filesystem::path const& cheminFichier);

    void initialiser_Chaine(std::filesystem::path const& cheminFichier);
    void initialiser_Hors_Saison(std::filesystem::path const& cheminFichier);
    void initialiser_Netflix(std::filesystem::path const& cheminFichier);
    void initialiser_Note(std::filesystem::path const& cheminFichier);
    void initialiser_Resume(std::filesystem::path const& cheminFichier);
    void initialiser_Saison(std::filesystem::path const& cheminFichier);
    void initialiser_Titre(std::filesystem::path const& cheminFichier);

    void AffichagePersonnaliser(AffichagePersonnalisation perso);

    void Print();
    void Print_Avec();
    void Print_Chaine();
    void Print_Header();
    void Print_Images();
    void Print_Netflix();
    void Print_Note();

    std::wstring m_min = L"min";
    std::wstring m_espace1;
    std::wstring m_labelHeureSingulier;
    std::wstring m_labelHeurePluriel;
    std::wstring m_espace2;
    std::wstring m_espace3;
    std::wstring m_labelMinuteSingulier;
    std::wstring m_labelMinutePluriel;
    std::vector<std::wstring>m_keyColor;
    std::wstring m_valuesColor;

    std::wstring m_audiodescription;
    std::vector<std::pair<std::wstring, std::wstring>> m_avec;
    std::pair<std::tm, std::wstring>m_date_diffusee_a_partir_de;
    bool m_disney{ false };
    int m_f_anneesDiffusion{ 0 };
    std::wstring m_chaine;
    bool m_hors_saison{ false };

    std::vector<std::wstring> m_image;
    bool m_netflix{ false };
    double m_note{ -1.0 };

    std::vector<std::wstring> m_titres;
    std::vector<std::wstring> m_resume;

    std::vector<Episode>m_liste_episodes;
    int m_nombre_episodes{ -1 };
    mutable int m_numero{ -1 };

private:
    Saison& operator=(Saison&& src) noexcept
    {
        if (&src != this)
        {
            for (auto& ep : src.m_liste_episodes)
            {
                m_liste_episodes.emplace_back(Episode{ *this, std::move(ep) });
            }
            m_audiodescription = std::move(src.m_audiodescription);
            m_avec = std::move(src.m_avec);
            m_date_diffusee_a_partir_de = std::move(src.m_date_diffusee_a_partir_de);
            m_disney = std::move(src.m_disney);
            m_f_anneesDiffusion = std::move(src.m_f_anneesDiffusion);
            m_chaine = std::move(src.m_chaine);
            m_hors_saison = std::move(src.m_hors_saison);

            m_image = std::move(src.m_image);
            m_netflix = std::move(src.m_netflix);
            m_note = std::move(src.m_note);

            m_titres = std::move(src.m_titres);
            m_resume = std::move(src.m_resume);

            m_nombre_episodes = std::move(src.m_nombre_episodes);
            m_numero = std::move(src.m_numero);

            m_keyColor = std::move(src.m_keyColor);
            m_valuesColor = std::move(src.m_valuesColor);
        }
        return *this;
    }
};

class Serie
{
public:
    Serie(std::filesystem::path racine);

    void initialiser_Fichier(std::filesystem::path const& cheminFichier);
    void initialiser_Chaine(std::filesystem::path const& cheminFichier);
    void initialiser_Creee_par(std::filesystem::path const& cheminFichier);
    void initialiser_En_relation_avec(std::filesystem::path const& cheminFichier);
    void initialiser_Titre(std::filesystem::path const& cheminFichier);

    void AffichagePersonnaliser(AffichagePersonnalisation perso);

    void Print();

    std::vector<Saison>saisons{};
    std::filesystem::path getRacine() { return racine; };
    std::filesystem::path getFileName() { return racine.filename(); };

    std::vector<std::wstring> m_resume;

    int lInfoQuiMInteresse{};

private:
    std::pair<int, int> calculer_Annees_Diffusion() const;
    std::wstring calcul_Duree_affichage() const;
    std::wstring calcul_Note_Affichage() const;
    std::wstring calcul_Signaletique_Jeunesse_affichage() const;
    std::wstring calcul_Sur_Affichage() const;
    std::wstring calcul_Titres_Affichage() const;
    std::wstring calcul_X_Signaletique_Jeunesse_affichage(std::wstring& sur_str) const;
    void corriger_Annee_Debut();// const;
    void corriger_Annee_Fin();

    std::wstring format_Annees() const;
    std::wstring format_AnneesEtSur(std::wstring& annees_str, std::wstring& sur_str) const;
    std::pair<std::optional<int>, std::optional<int>> m_f_anneesProduction;

    void PostTraitement();

    //void Print_Saison(Saison saison);
    void Print_Saisons();

    void Print_Avec_etc();
    void Print_Chaine();
    void Print_Creee_par();
    void Print_En_relation_avec();
    void Print_Header() const;

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

    int m_annees{};
    std::wstring m_audiodescription;
    std::vector<std::wstring> m_catalogue;
    std::wstring m_chaine;
    std::vector<std::wstring> m_creee_par;
    std::wstring m_disney_sj;
    std::wstring m_en_relation_avec;
    std::vector<std::wstring> m_genre;
    std::vector<std::wstring> m_image;
    std::vector<std::wstring> m_nationalite;
    std::wstring m_netflix_sj;
    double m_note{ 0.0 };
    std::wstring m_sj;
    std::wstring m_sous_genre;
    std::wstring m_sur;

    std::list<Saison> m_hors_saisons{};
    std::list<Saison> m_non_hors_saisons{};

    std::vector<std::wstring> m_titres;
    long m_duree{ -1 };
    std::vector<std::wstring> m_titres_originaux;
};

//
//

AffichagePersonnalisation getCurrentAffichagePersonnalisation();

class AffichagePersonnalisation
{
public:
    AffichagePersonnalisation() {};
    AffichagePersonnalisation(std::tuple<std::wstring, std::wstring, std::wstring> espaces, std::pair<std::wstring, std::wstring> labelsHeure, std::pair<std::wstring, std::wstring> labelsMinute) :
        m_espace1{ std::get<0>(espaces) }, m_espace2{ std::get<1>(espaces) }, m_espace3{ std::get<2>(espaces) }, m_labelsHeure{ labelsHeure }, m_labelsMinute{ labelsMinute } {
    };
    // _ h _ _ min
    std::wstring m_espace1{ L" " };
    std::wstring m_espace2{ L" " };
    std::wstring m_espace3{ L" " };
    // h
    std::pair<std::wstring, std::wstring> m_labelsHeure{ L"h", L"h" };
    // min
    std::pair<std::wstring, std::wstring> m_labelsMinute{ L"min", L"min" };
    //
    std::vector<std::wstring> m_keyColor{ L"\x1b[94;1m", L"\x1b[38;2;0;255;0m", L"\x1b[38;2;255;0;0m", L"\x1b[38;2;255;255;0m" }; // Bleu, Vert, Rouge, Jaune
    std::wstring m_valuesColor{ L"\x1b[38;2;255;255;255m" }; // Blanc
};
