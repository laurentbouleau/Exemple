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
#include <list>
#include <optional>

#include <filesystem> // C++17 standard header file name

extern const std::vector<std::wstring> Genre;
extern const std::vector<std::wstring> Sur;
extern const std::vector<std::wstring> Sous_Genre;
extern const std::vector<std::wstring> Nationalite;

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
        m_DatesVisionnage{ src.m_DatesVisionnage }, m_numero{ src.m_numero }
    {
    };
    SequenceVisionnage(const Episode& episode, const InfosVisionnage& info_vis) :
        m_episode{ episode }, m_titres{ info_vis.m_titres }, m_streaming{ info_vis.m_streaming },
        m_duree_en_seconde{ info_vis.m_duree_en_seconde }, m_resume{ info_vis.m_resume },
        m_DatesVisionnage{ info_vis.m_DatesVisionnage }, m_numero{ info_vis.m_numero }
    {
    };
    boolean operator==(const SequenceVisionnage& rhs) const { return this == &rhs; };

    std::wstring calcul_Duree_affichage(int numero_sequence) const;

    const void AffichagePersonnaliser(AffichagePersonnalisation perso);

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

    //long m_NumeroEpisode{};


    int m_numero{ -1 };
    long m_duree { -1 };

    //bool affichage_duree_actif = true;// false;

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

    const void AffichagePersonnaliser(AffichagePersonnalisation perso);

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

    const void AffichagePersonnaliser(AffichagePersonnalisation perso);

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

    const void AffichagePersonnaliser(AffichagePersonnalisation perso);

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

    void Print_Saison(Saison saison);
    //void Print_Saisons();
 
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
    std::list<Saison> m_non_hors_saisons;

    std::vector<std::wstring> m_titres;
    long m_duree{ -1 };
    std::vector<std::wstring> m_titres_originaux;
};

