#include "pch.h"

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
#include <regex>
#include <optional>

#include <filesystem> // C++17 standard header file name

using namespace std;
namespace fs = std::filesystem;

// ######################################################################################################################################################
// ######################################################################################################################################################
// Allociné : https://www.allocine.fr/

// Exception des éléments
const std::vector<wchar_t> Exception_des_elements
{
    L'\\',
    L'/',
    L':',
    L'*',
    L'?',
    L'"',
    L'<',
    L'>',
    L'|'
};

const std::vector<std::wstring> Audiodescription
{ // AD : Audiodescription
    L"Allemagne",
    L"Anglais",
    L"Espagne",
    L"Français",
    L"Hébreu",
    L"Portugais brésilien"
};

const std::vector<std::wstring>Genre
{
    L"Action",
    L"Arts Martiaux",
    L"Animation",
    L"Aventure",
    L"Biopic",
    L"Comédie",
    L"Comédie musicale",
    L"Comédie dramatique",
    L"Concert",
    L"Divers",

    L"Docu-séries",
    L"Documentaire",
    L"Drama",
    L"Drame",
    L"Espionnage",
    L"Epouvante-horreur",
    L"Erotique",
    L"Expérimental",
    L"Famille",
    L"Fantastique",

    L"Guerre",
    L"Judiciaire",
    L"Historique",
    L"Manga",
    L"Musical",
    L"Péplum",
    L"Policier",
    L"Opéra",
    L"Romance",
    L"Science fiction", L"Science Fiction",

    L"Show",
    L"Survival",
    L"Thriller",
    L"Western"
};

const std::vector<std::wstring> Nationalite
{
    L"Afrique du Sud",
    L"Allemagne",
    L"Argentine",
    L"Australie",
    L"Barbade",
    L"Belgique",
    L"Brésil",
    L"Canada",
    L"Chili",
    L"Chine",
    L"Corée du Sud",
    L"Danemark",
    L"Danoise",
    L"Egypte",
    L"Espagne",
    L"Finlande",
    L"France",
    L"Grande-Bretagne",
    L"Hong-Kong",
    L"Inde",
    L"Indonésie",
    L"Irlande",
    L"Islande",
    L"Israël",
    L"Italie",
    L"Japon",
    L"Jordanie",
    L"Luxembourg",
    L"Mexique",
    L"Nigéria",
    L"Norvège",
    L"Pays-Bas",
    L"Philippines",
    L"Pologne",
    L"République Tchèque",
    L"Russie",
    L"Samoa",
    L"Suède",
    L"Thaïlande",
    L"Turquie",
    L"Ukraine",
    L"U.S.A."
};

const std::vector<std::wstring> Sous_Genre
{
    L"Animation",
    L"Documentaire",
    L"Docu-séries",
    L"Manga",
    L"Mini-série"
};

const std::vector<std::wstring> Sur
{
    L"Amazon Prime Video",
    L"Arte",
    L"Apple TV+",
    L"Canal+",
    L"Crunchyroll",
    L"Disney+",
    L"DVD",
    L"Max",
    L"Netflix",
    L"Paramount+",
    L"SⱯLTO",
    L"TF1+",
    L"VOD"
};

// ######################################################################################################################################################
// ######################################################################################################################################################

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::optional<long> extraire_Duree_DepuisUneLigneDUnFichier(const std::wstring& ligne)                                                             #
// #                                                                                                                                                    #
// ######################################################################################################################################################

std::optional<long> extraire_Duree_DepuisUneLigneDUnFichier(const std::wstring& ligne)
{
    const std::wregex duree_format_rg{ L"^(?:(\\d+)\\s?(?:h|H)\\s?)?(?:(\\d+)\\s?(?:min|MIN)\\s?)?$" };
    std::wsmatch match;
    std::optional<long>  duree;
    if (std::regex_match(ligne, match, duree_format_rg))
    {
        duree = (match[1].matched ? std::stoi(match[1]) : 0) * 60 * 60 + (match[2].matched ? std::stoi(match[2]) : 0) * 60;
    }
    else
    {
        throw std::invalid_argument("'" + std::string{ ligne.begin(), ligne.end() } + "' n'est pas un format de durée valide.");
    }
    return duree;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::tuple<std::vector<std::wstring>, std::optional<long>, std::vector<std::wstring>>                                                              #
// #                       extraire_Informations_DepuisLeContenuDUnFichier(fs::path const& cheminFichier)                                               #
// #                                                                                                                                                    #
// ######################################################################################################################################################

std::tuple<std::vector<std::wstring>, std::optional<long>, std::vector<std::wstring>> extraire_Informations_DepuisLeContenuDUnFichier(fs::path const& cheminFichier)
{
    auto nomFichier = cheminFichier.wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    std::vector<std::wstring> contenu = lire_fichierTxt(cheminFichier.wstring(), { L"\n" });
    assert((contenu.size() != 0));

    std::vector<std::wstring>nouveaux_titres = extraire_Titres_Depuis_UneLigne(contenu[0]);

    std::optional<long> duree;

    contenu.erase(contenu.begin());
    if (contenu.size() > 0)
    {
        duree = extraire_Duree_DepuisUneLigneDUnFichier(contenu[0]);
        contenu.erase(contenu.begin());
    }

    return { nouveaux_titres , duree , contenu };
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::vector<std::wstring> extraire_Titres_Depuis_UneLigne(std::wstring>& ligne)                                                                    #
// #                                                                                                                                                    #
// ######################################################################################################################################################

std::vector<std::wstring> extraire_Titres_Depuis_UneLigne(std::wstring& ligne)
{ // Titres
    assert(ligne.length() > 0 && L"Nom de titres vide"); // ??? pour Mot de... ?
    std::wstring& t = ligne;

    std::vector<std::wstring> titres;

    std::size_t pos = 0;
    bool found = false;
    const std::wstring d_p = L" : ";
    pos = t.find(d_p);
    if (!found && pos != std::wstring::npos)
    {
        titres.push_back(t.substr(0, pos));
        titres.push_back(d_p);
        titres.push_back(t.substr(pos + 3));
        found = true;
    }
    const std::wstring d_p2 = L": ";
    pos = t.find(d_p2);
    if (!found && pos != std::wstring::npos)
    {
        titres.push_back(t.substr(0, pos));
        titres.push_back(d_p2);
        titres.push_back(t.substr(pos + 2));
        found = true;
    }
    const std::wstring d_p3 = L"/";
    pos = t.find(d_p3);
    if (!found && pos != std::wstring::npos)
    {
        titres.push_back(t.substr(0, pos));
        titres.push_back(d_p3);
        titres.push_back(t.substr(pos + 1));
        found = true;
    }
    const std::wstring d_p4 = L" - ";
    pos = t.find(d_p4);
    if (!found && pos != std::wstring::npos)
    {
        titres.push_back(t.substr(0, pos));
        titres.push_back(d_p4);
        titres.push_back(t.substr(pos + 3));
        found = true;
    }
    const std::wstring d_p5 = L"- ";
    pos = t.find(d_p5);
    if (!found && pos != std::wstring::npos)
    {
        titres.push_back(t.substr(0, pos));
        titres.push_back(d_p5);
        titres.push_back(t.substr(pos + 2));
        found = true;
    }
    if (!found)
    {
        titres.push_back(t);
        found = true;
    }
    return titres;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::vector<std::wstring> extraire_Titres_Depuis_UnFichier(fs::path const& cheminFichier)                                                          #
// #                                                                                                                                                    #
// ######################################################################################################################################################

std::vector<std::wstring> extraire_Titres_Depuis_UnFichier(fs::path const& cheminFichier)
{
    auto nomFichier = cheminFichier.wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    std::wstring titres = lire_fichierTxt(cheminFichier.wstring());
    assert((titres.size() != 0));

    return extraire_Titres_Depuis_UneLigne(titres);
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::vector<std::wstring> extraire_Titres_Depuis_NomDeFichierOuDeRepertoire(std::wstring& titres)                                                  #
// #                                                                                                                                                    #
// ######################################################################################################################################################

std::vector<std::wstring> extraire_Titres_Depuis_NomDeFichierOuDeRepertoire(std::wstring& titres)
{
    assert(titres.length() > 0 && L"Nom de titres vide"); // ??? pour Mot de... ?
    const std::vector<std::wstring> t = extraire_Titres_Depuis_UneLigne(titres);
    return t;
}

// ######################################################################################################################################################
// ######################################################################################################################################################

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::wstring filter_values(std::wstring const& content, std::vector<std::wstring> const& values)                                                   #
// #                                                                                                                                                    #
// ######################################################################################################################################################

std::wstring filter_values(std::wstring const& content, std::vector<std::wstring> const& values)
{
    std::wstring pattern;
    for (const auto value : values)
    {
        if (pattern.length() > 0) { pattern += L"|"; }
        pattern += value;
    }
    std::wregex value_pattern{ L"(" + pattern + L")" };
    std::wsmatch match;
    if (std::regex_match(content, match, value_pattern))
    {
        return match[1];
    }
    return L"";
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::vector<std::wstring> fusionner_Titres(const std::vector<std::wstring>& nouveaux_titres, std::vector<std::wstring>& vieux_titres)              #
// #                                                                                                                                                    #
// ######################################################################################################################################################

std::vector<std::wstring> fusionner_Titres(const std::vector<std::wstring>& nouveaux_titres, const std::vector<std::wstring>& vieux_titres)
{
    assert(nouveaux_titres.size() < 4 && L"???");
    assert(vieux_titres.size() < 4 && L"???");

    std::vector<std::wstring> resultat = nouveaux_titres;

    if (vieux_titres.size() == nouveaux_titres.size())
    {
        // double titres_ration{ 80.0 }; ???
        if (vieux_titres.size() == 1 && nouveaux_titres.size() == 1)
        {
            resultat = vieux_titres;
        }
        if (vieux_titres.size() == 3 && nouveaux_titres.size() == 3 && vieux_titres[0] == nouveaux_titres[0] && vieux_titres[1] != nouveaux_titres[1] && vieux_titres[2] == nouveaux_titres[2])
        {
            resultat[1] = vieux_titres[1];
        }
        if (vieux_titres.size() == 3 && nouveaux_titres.size() == 3 && (vieux_titres[0] != nouveaux_titres[0] || vieux_titres[1] != nouveaux_titres[1] || vieux_titres[2] != nouveaux_titres[2]))
        {
            resultat = vieux_titres;
        }
    }

    return resultat;
}

// ######################################################################################################################################################
// ######################################################################################################################################################

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void initialiser_Audiodescription(fs::path const& cheminFichier, std::wstring& m_ad)                                                               #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void initialiser_Audiodescription(fs::path const& cheminFichier, std::wstring& m_ad)
{
    auto nomFichier = cheminFichier.wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    std::wstring ad = lire_fichierTxt(nomFichier);
    std::size_t pos = ad.find(L"Audiodescription");
    if (pos == std::wstring::npos)
        ;
    else
        ad = ad.substr(0, pos - 3);
    if (std::find(::Audiodescription.begin(), ::Audiodescription.end(), ad) != ::Audiodescription.end())
        m_ad = ad;
    assert((m_ad.size() != 0));
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void initialiser_Avec(fs::path const& cheminFichier, std::vector<std::pair<std::wstring, std::wstring>>& m_avec)                                   #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void initialiser_Avec(fs::path const& cheminFichier, std::vector<std::pair<std::wstring, std::wstring>>& m_avec)
{
    auto nomFichier = cheminFichier.filename().wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    m_avec = lire_paireCleValeur_depuisFichierTxt(cheminFichier.wstring());
    assert((m_avec.size() != 0));
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void initialiser_Genre(fs::path const& cheminFichier,                                                                                              #
// #                        std::vector<std::wstring>& m_genres_renvoyes,                                                                               #
// #                        const std::vector<std::wstring>& genres_valides)                                                                            #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void initialiser_Genre(fs::path const& cheminFichier, std::vector<std::wstring>& m_genres_renvoyes, const std::vector<std::wstring>& genres_valides)
{ // Genre
    auto nomFichier = cheminFichier.wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");

    std::vector<std::wstring>g;
    g = lire_fichierTxt(nomFichier, { L"\n", L", " });
    for (auto&& genre : g)
    {
        if (std::find(genres_valides.begin(), genres_valides.end(), genre) != genres_valides.end())
            m_genres_renvoyes.push_back(genre);
        else
        {
            assert((false) && "Attention genre non valide !!!");
        }
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void initialiser_Image(fs::path const& cheminFichier, std::vector<std::wstring>& m_images)                                                         #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void initialiser_Image(fs::path const& cheminFichier, std::vector<std::wstring>& m_images)
{
    auto nomFichier = cheminFichier.filename().wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    m_images.push_back(nomFichier);
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void initialiser_Nationalite(fs::path const& cheminFichier,                                                                                        #
// #                              std::vector<std::wstring>& m_nationalites_renvoyes,                                                                   #
// #                              const std::vector<std::wstring>& nationalites_valides)                                                                #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void initialiser_Nationalite(fs::path const& cheminFichier, std::vector<std::wstring>& m_nationalites_renvoyes, const std::vector<std::wstring>& nationalites_valides)
{ // Nationalite
    auto nomFichier = cheminFichier.wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");

    std::vector<std::wstring>n;
    n = lire_fichierTxt(nomFichier, { L"\n", L", " });
    for (auto&& nationalite : n)
    {
        if (std::find(nationalites_valides.begin(), nationalites_valides.end(), nationalite) != nationalites_valides.end())
            m_nationalites_renvoyes.push_back(nationalite);
        else
        {
            assert((false) && "Attention nationalite non valide !!!");
        }
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # bool initialiser_Sous_Genre(std::wstring& m_s_g)                                                                                                   #
// #                                                                                                                                                    #
// ######################################################################################################################################################

bool initialiser_Sous_Genre(std::wstring& m_s_g)
{ // Sous_Genre
    bool s_g = false;
    if (std::find(::Sous_Genre.begin(), ::Sous_Genre.end(), m_s_g) != ::Sous_Genre.end())
        s_g = true;
    return s_g;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void initialiser_Sur(std::wstring& m_sur)                                                                                                          #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void initialiser_Sur(std::wstring& m_sur)
{ // Sur
    bool sur = false;
    if (std::find(::Sur.begin(), ::Sur.end(), m_sur) != ::Sur.end())
    {
        sur = true;
    }
}

// ######################################################################################################################################################
// ######################################################################################################################################################

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::wstring recuperer_Disney_SJ(fs::path const& cheminFichier, std::vector<std::wstring>& m_catalogue)                                            #
// #                                                                                                                                                    #
// ######################################################################################################################################################

std::wstring recuperer_Disney_SJ(fs::path const& cheminFichier, std::vector<std::wstring>& m_catalogue)
{ // Disney+ SJ
    auto nomFichier = cheminFichier.wstring();
    std::wstring content = lire_fichierTxt(nomFichier);

    auto stem = cheminFichier.stem().wstring();
    m_catalogue.push_back(stem);

    return filter_values(content, { L"6\\+",L"12\\+",L"14\\+",L"16\\+",L"18\\+" });
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::wstring recuperer_Max_SJ(fs::path const& cheminFichier, std::vector<std::wstring>& m_catalogue)                                               #
// #                                                                                                                                                    #
// ######################################################################################################################################################

std::wstring recuperer_Max_SJ(fs::path const& cheminFichier, std::vector<std::wstring>& m_catalogue)
{ // Max SJ
    auto nomFichier = cheminFichier.wstring();
    std::wstring content = lire_fichierTxt(nomFichier);

    auto stem = cheminFichier.stem().wstring();
    m_catalogue.push_back(stem);

    return filter_values(content, { L"\\-12" });
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::wstring recuperer_Netflix_SJ(fs::path const& cheminFichier, std::vector<std::wstring>& m_catalogue)                                           #
// #                                                                                                                                                    #
// ######################################################################################################################################################

std::wstring recuperer_Netflix_SJ(fs::path const& cheminFichier, std::vector<std::wstring>& m_catalogue)
{ // Netflix SJ
    auto nomFichier = cheminFichier.wstring();
    std::wstring content = lire_fichierTxt(nomFichier);

    auto stem = cheminFichier.stem().wstring();
    m_catalogue.push_back(stem);

    return filter_values(content, { L"7\\+",L"10\\+",L"13\\+", L"16\\+", L"18\\+", L"Tous publics" });
}

// ######################################################################################################################################################
// ######################################################################################################################################################

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void initialiser_SJ(fs::path const& cheminFichier, std::wstring& m_sj)                                                                             #
// #                                                                                                                                                    #
// ######################################################################################################################################################
// Cinéma et serie
std::wstring recuperer_SJ(fs::path const& cheminFichier)
{ // SJ
    auto nomFichier = cheminFichier.wstring();

    std::wstring content = lire_fichierTxt(nomFichier);

    std::wregex sj_pattern{ L"(\\-7|\\-10|\\-12|\\-16|\\-18)" };
    std::wsmatch match;
    if (std::regex_match(content, match, sj_pattern))
    {
        return match[1];
    }
    return L"";
}

// ######################################################################################################################################################
// ######################################################################################################################################################

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Print_Catalogue(const std::wstring sur, std::vector<std::wstring>& catalogue, bool affichage_catalogue_actif,                                 #
// #                      std::wstring& keyColor, std::wstring& valuesColor)                                                                            #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Print_Catalogue(const std::wstring sur, std::vector<std::wstring>& catalogue, bool affichage_catalogue_actif, std::wstring& keyColor, std::wstring& valuesColor)
{
    // Sur == m_sur ???
    std::wstring sur_str = L"";
    if (affichage_catalogue_actif)
    {
        for (auto c : catalogue)
        {
            if (sur == c)
            {
                sur_str = sur;
                break;
            }
        }
        //std::wstring sur_str;
        for (auto c : catalogue)
        {
            if (c == sur)
            {
                continue;
            }
            else
            {
                sur_str += keyColor + c + L" : " + valuesColor + L"Oui" + keyColor + L" !" + valuesColor + L"\r\n";
            }
        }
        std::wcout << sur_str;
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Print_CleValeur(const std::wstring& cle, const std::wstring& valeur, bool actif, std::wstring& keyColor, std::wstring& m_valuesColor)         #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Print_CleValeur(const std::wstring& cle, const std::wstring& valeur, bool actif, std::wstring& keyColor, std::wstring& m_valuesColor)
{
    if (actif && valeur.size() > 0)
    {
        std::wstring valeur_str = keyColor + cle + L" : " + m_valuesColor + valeur + L"\r\n";
        std::wcout << valeur_str;// << std::endl;
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Print_Genres(const std::vector<std::wstring>& genres, bool affichage_genres_actif,                                                            #
// #                  const std::wstring& sous_genre, bool affichage_sous_genre_actif,                                                                  #
// #                  std::wstring& keyColor, std::wstring& m_valuesColor)                                                                              #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Print_Genres(const std::vector<std::wstring>& genres, bool affichage_genres_actif, const std::wstring& sous_genre, bool affichage_sous_genre_actif, std::wstring& keyColor, std::wstring& m_valuesColor)
{
    if (affichage_genres_actif && genres.size() > 0)
    {
        std::wstring genre_str = keyColor + L"Genre" + ((genres.size() > 1) ? L"s" : L"") + L" : " + m_valuesColor;

        bool first = true;
        for (auto&& genre : genres)
        {
            if (!first)
            {
                genre_str += keyColor + L", " + m_valuesColor;
            }
            genre_str += genre;
            first = false;
        }
        genre_str += L"\r\n";
        std::wcout << genre_str;
        if (affichage_sous_genre_actif && sous_genre.size() != 0)
        {
            genre_str = keyColor + L"Sous-genre : " + m_valuesColor + sous_genre + L"\r\n";
            std::wcout << genre_str;// << std::endl;
        }
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Print_Images(const std::vector<std::wstring>& image, bool affichage_image_actif, std::wstring& keyColor, std::wstring& m_valuesColor)         #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Print_Images(const std::vector<std::wstring>& image, bool affichage_image_actif, std::wstring& keyColor, std::wstring& m_valuesColor)
{
    if (affichage_image_actif && image.size() > 0)
    {
        std::wstring image_str = keyColor + L"Image" + ((image.size() > 1) ? L"s" : L"") + L" : [" + m_valuesColor;
        bool first = true;
        for (auto&& i : image)
        {
            if (!first)
            {
                image_str += keyColor + L"], [" + m_valuesColor;
            }
            image_str += i;
            first = false;
        }
        image_str += keyColor + L']' + m_valuesColor + L"\r\n";
        std::wcout << image_str;
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const void Print_Nationalites(const std::vector<std::wstring>& m_nationalites, bool affichage_nationalite_actif                                    #                                                                                                    #
// #                        std::wstring& keyColor, std::wstring& m_valuesColor)                                                                        #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Print_Nationalites(const std::vector<std::wstring>& m_nationalites, bool affichage_nationalite_actif, std::wstring& keyColor, std::wstring& m_valuesColor)
{
    if (affichage_nationalite_actif && m_nationalites.size() > 0)
    {
        std::wstring nationalite_str = keyColor + L"Nationalite" + ((m_nationalites.size() > 1) ? L"s" : L"") + L" : " + m_valuesColor;

        bool first = true;
        for (auto&& nationalite : m_nationalites)
        {
            if (!first)
            {
                nationalite_str += keyColor + L", " + m_valuesColor;
            }
            nationalite_str += nationalite;
            first = false;
        }
        nationalite_str += L"\r\n";
        std::wcout << nationalite_str;
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Print_Resume(const std::vector<std::wstring>& m_resume, bool affichage_resume_actif)                                                          #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Print_Resume(const std::vector<std::wstring>& m_resume, bool affichage_resume_actif)
{
    if (affichage_resume_actif && m_resume.size() > 0)
    {
        std::wstring resume_str = L"";
        for (auto&& r : m_resume)
            resume_str += r + L"\r\n";
        std::wcout << L"\r\n" + resume_str;
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Print_Titre_Original(const std::vector<std::wstring>& m_titre_original, bool affichage_titre_original_actif,                                  #
// #                              std::vector<std::wstring>& keyColor, std::wstring& m_valuesColor)                                                     #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Print_Titre_Original(const std::vector<std::wstring>& m_titre_original, bool affichage_titre_original_actif, std::vector<std::wstring>& keyColor, std::wstring& m_valuesColor)
{
    if (affichage_titre_original_actif && m_titre_original.size() > 0)
    {
        std::wstring titre_original_str = keyColor[0] + L"Titre original : " + m_valuesColor + m_titre_original[0];
        if (m_titre_original.size() > 2 && m_titre_original[2] != L"")
        {
            titre_original_str += keyColor[1] + m_titre_original[1] + m_valuesColor;
            titre_original_str += m_titre_original[2];
        }
        titre_original_str += L"\r\n";
        std::wcout << titre_original_str;
    }
}