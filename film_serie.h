#pragma once
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include "utils.h"

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
//#include <sstream>
#include <regex>
#include <optional>

#include <filesystem> // C++17 standard header file name

std::optional<long> extraire_Duree_DepuisUneLigneDUnFichier(const std::wstring& ligne);
std::tuple<std::vector<std::wstring>, std::optional<long>, std::vector<std::wstring>> extraire_Informations_DepuisLeContenuDUnFichier(std::filesystem::path const& cheminFichier);
std::vector<std::pair<std::wstring, std::wstring>> extraire_Liste_De_Paire_Depuis_Contenu_Fichier(std::filesystem::path const& cheminFichier);
std::vector<std::wstring> extraire_Titres_Depuis_NomDeFichierOuDeRepertoire(std::wstring& titres);
std::vector<std::wstring> extraire_Titres_Depuis_UnFichier(std::filesystem::path const& cheminFichier);
std::vector<std::wstring> extraire_Titres_Depuis_UneLigne(std::wstring& file_content);

std::wstring filter_values(std::wstring const& content, std::vector<std::wstring> const& values);
std::vector<std::wstring> fusionner_Titres(const std::vector<std::wstring>& nouveaux_titres, const std::vector<std::wstring>& vieux_titres);

void initialiser_Audiodescription(std::filesystem::path const& cheminFichier, std::wstring& m_ad);
void initialiser_Genre(std::filesystem::path const& cheminFichier, std::vector<std::wstring>& m_genres_renvoyes, const std::vector<std::wstring>& genres_valides);
void initialiser_Image(std::filesystem::path const& cheminFichier, std::vector<std::wstring>& m_images);
void initialiser_Nationalite(std::filesystem::path const& cheminFichier, std::vector<std::wstring>& m_nationalites_renvoyes, const std::vector<std::wstring>& nationalites_valides);

std::wstring stringFormatOneLine(std::wstring str, int lengthMax, std::wstring marqueurTroncature, int marqueurTroncature_len, std::wstring suffixe, int suffixe_len);

std::wstring recuperer_Disney_SJ(std::filesystem::path const& cheminFichier, std::vector<std::wstring>& m_cataloguer);
std::wstring recuperer_Netflix_SJ(std::filesystem::path const& cheminFichier, std::vector<std::wstring>& m_cataloguer);
std::wstring recuperer_SJ(std::filesystem::path const& cheminFichier);

void Print_CleValeur(const std::wstring& cle, const std::wstring& valeur, bool actif, std::wstring& keyColor, std::wstring& valuesColor);
void Print_Genres(const std::vector<std::wstring>& m_genres, bool affichage_genres_actif, const std::wstring& m_sous_genre, bool affichage_sous_genre_actif, std::wstring& keyColor, std::wstring& valuesColor);
void Print_Images(const std::vector<std::wstring>& m_image, bool affichage_image_actif, std::wstring& keyColor, std::wstring& valuesColor);
void Print_Nationalites(const std::vector<std::wstring>& m_nationalites, bool affichage_nationalite_actif, std::wstring& keyColor, std::wstring& valuesColor);
void Print_Catalogue(const std::wstring m_sur, std::vector<std::wstring>& m_catalogue, bool affichage_catalogue_actif, std::wstring& keyColor, std::wstring& m_valuesColor);
void Print_Resume(const std::vector<std::wstring>& m_resume, bool affichage_resume_actif);
void Print_Titre_Original(const std::vector<std::wstring>& m_titre_original, bool affichage_titre_original_actif, std::vector<std::wstring>& keyColor, std::wstring& valuesColor);


AffichagePersonnalisation getCurrentAffichagePersonnalisation();

class AffichagePersonnalisation
{
public:
    AffichagePersonnalisation() {};
    AffichagePersonnalisation(std::tuple<std::wstring, std::wstring, std::wstring> espaces, std::pair<std::wstring, std::wstring> labelsHeure, std::pair<std::wstring, std::wstring> labelsMinute) :
        m_espace1{ std::get<0>(espaces) }, m_espace2{ std::get<1>(espaces) }, m_espace3{ std::get<2>(espaces) }, m_labelsHeure{ labelsHeure }, m_labelsMinute{ labelsMinute } {};
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
