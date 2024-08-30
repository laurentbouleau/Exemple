#include "pch.h"

#include "utils.h"
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
#include <optional>

#include <filesystem> // C++17 standard header file name

// Hors saison : ???
// Saison 1 : exemple : 2023/02/15
// Saison 2 : exemple : 2024/09/10
// etc...

using namespace std;
namespace fs = std::filesystem;

/*const std::vector<std::wstring> Soundtrack
{
    L"Compositeur",
    L"Compositeur (chansons du film)",
    L"Montage musical",
    L"Superviseur musical"
};*/

// ######################################################################################################################################################
// ######################################################################################################################################################

/*static bool ends_with(std::wstring_view str, std::wstring_view suffix)
{
    return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}*/

// ######################################################################################################################################################
// ######################################################################################################################################################

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # Film::Film(std::filesystem::path racine)                                                                                                           #
// #                                                                                                                                                    #
// ######################################################################################################################################################

/*Film::Film(std::filesystem::path racine)
{
    this->racine = racine;
    auto nomDossier = racine.filename().wstring();
    assert(nomDossier.length() > 0 && L"Nom de dossier vide");

    std::wregex filename_pattern{ L"(.+?)(?:\\.\\((\\d{4}\\-\\d{2}\\-\\d{2}\\s?)?([^\\)]*)\\))?(?:\\.(.+))?$" };
    std::wsmatch match;
    if (std::regex_match(nomDossier, match, filename_pattern))
    {
        std::wstring titres = match[1];
        m_titres = extraire_Titres_Depuis_NomDeFichierOuDeRepertoire(titres);

        if (match[2].matched)
        {
            std::wstring wstr = match[2].str();
            m_date_en_salle_ou_sur.tm_year = stoi(wstr) - 1900;
            wstr = wstr.substr(5);
            m_date_en_salle_ou_sur.tm_mon = stoi(wstr) - 1;
            wstr = wstr.substr(3);
            m_date_en_salle_ou_sur.tm_mday = stoi(wstr);
        }
        m_sur = (match[3].matched) ? match[3].str() : L"";
        m_sous_genre = (match[4].matched) ? match[4].str() : L"";
    }
    else
    {
        assert(false == true && "Le nom du répertoire n'est pas un nom valide.");
    }
}*/

Film::Film(std::filesystem::path racine)
{
    this->racine = racine;
    auto nomDossier = racine.filename().wstring();
    assert(nomDossier.length() > 0 && L"Nom de dossier vide");

    std::wregex filename_pattern{ L"(.+?)(?:\\.\\((?:(\\d{4})\\-(\\d{2})\\-(\\d{2})\\)))?(?:\\.(.+))?$" };
    std::wsmatch match;
    if (std::regex_match(nomDossier, match, filename_pattern))
    {
        std::wstring part_1 = match[1];
        m_titres = extraire_Titres_Depuis_NomDeFichierOuDeRepertoire(part_1);
        if (match[2].matched)
        {
            std::wstring annees_str = match[2].str();
            std::wstring mois_str = match[3].str();
            std::wstring jours_str = match[4].str();
            m_date.tm_year = stoi(annees_str) - 1900;  //beurk!!
            m_date.tm_mon = stoi(mois_str) - 1;           //beurk!!
            m_date.tm_mday = stoi(jours_str);
        }

        m_sur = (match[5].matched) ? match[5].str() : L"";

        m_sous_genre = (match[6].matched) ? match[6].str() : L"";
    }
    else
    {
        assert(false == true && "Le nom du répertoire n'est pas un nom valide.");
    }
}
// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Film::initialiser_Fichier(fs::path const& cheminFichier)                                                                                      #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Film::initialiser_Fichier(fs::path const& cheminFichier)
{
    auto nomFichier = cheminFichier.filename().wstring();
    auto nomImage = cheminFichier.extension().wstring();
    if (nomImage == L".txt")
    {
        // AD
        if (nomFichier == L"AD.txt")
        {
            initialiser_Audiodescription(cheminFichier, m_audiodescription);
        }
        // Avec
        if (nomFichier == L"Avec.txt")
        {
            initialiser_Avec(cheminFichier, m_avec);
        }
        // Date de reprisé
        if (nomFichier == L"Date de reprise.txt")
        {
            initialiser_Date_de_reprise(cheminFichier); // ???
        }
        // Date de sortie
        if (nomFichier == L"Date de sortie.txt")
        {
            initialiser_Date_de_sortie(cheminFichier);// ???
        }
        // De
        if (nomFichier == L"De.txt")
        {
            initialiser_De(cheminFichier);
        }
        // Disney+ SJ
        if (nomFichier == L"Disney+.txt")
        {
            m_disney_sj = recuperer_Disney_SJ(cheminFichier);
        }
        // Distributeur
        if (nomFichier == L"Distributeur.txt")
        {
            initialiser_Distributeur(cheminFichier);
        }
        // Genre
        if (nomFichier == L"Genre.txt")
        {
            initialiser_Genre(cheminFichier, m_genre, ::Genre);
        }
        // Making-of
        if (nomFichier == L"Making-of.txt")
        {
            initialiser_Making_of(cheminFichier);
        }
        // Nationalité
        if (nomFichier == L"Nationalité.txt")
        {
            initialiser_Nationalite(cheminFichier, m_nationalite, ::Nationalite);
        }
        // Netflix
        if (nomFichier == L"Netflix.txt")
        {
            m_netflix_sj = recuperer_Netflix_SJ(cheminFichier);
        }
        // Note
        if (nomFichier == L"Note.txt")
        {
            initialiser_Note(cheminFichier);
        }
        // Par
        if (nomFichier == L"Par.txt")
        {
            initialiser_Par(cheminFichier);
        }
        // SJ
        if (nomFichier == L"SJ.txt")
        {
            m_sj = recuperer_SJ(cheminFichier);
        }
        // Soundtrack
        if (nomFichier == L"Soundtrack.txt")
        {
            initialiser_Soundtrack(cheminFichier);
        }
        // Titre
        if (nomFichier == L"Titre.txt")
        {
            initialiser_Titre(cheminFichier);
        }
        // Titre original
        if (nomFichier == L"Titre original.txt")
        {
            m_titres_originaux = initialiser_Titre_Original(cheminFichier, m_titres_originaux);
        }
        // xxxx-yy-zz
        //if (nomFichier != L"")
        if (std::regex_match(nomFichier, std::wregex{ L"([[:digit:]])(.+)" }))
        {
            std::wcout << L"{[" << cheminFichier << L"]}" << std::endl;
        }
    }
    else if (nomImage == L".jpg" || nomImage == L".png" || nomImage == L".webp")
    // Image
    {
        initialiser_Image(cheminFichier, m_image);
    }
    else
    {
        std::wcout << L'{' << cheminFichier << L'}' << std::endl;
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Film::initialiser_Date_de_reprise(fs::path const& cheminFichier)                                                                              #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Film::initialiser_Date_de_reprise(fs::path const& cheminFichier)
{
    auto nomFichier = cheminFichier.wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    std::wstring wstr = lire_fichierTxt(nomFichier);
    std::size_t pos = 0;

    wchar_t sp = L' ', tiret = L'-';
    auto y = std::stoi(wstr, &pos);
    assert(1582 <= y && L"L'année est inférieur !");
    wstr = wstr.substr(4);
    try
    {
        test_date_tiret_sp_etc(wstr[0]);
    }
    catch (exception_date_tiret e2)
    {
        std::wcout << L"Exception a été capturée : " << e2.get_message() << std::endl;
        exit(1);
    }
    wstr = wstr.substr(1);
    auto m = std::stoi(wstr, &pos);
    assert((1 <= m && m <= 12) && L"Le mois invalide !");
    wstr = wstr.substr(2);
    try
    {
        test_date_tiret_sp_etc(wstr[0]);
    }
    catch (exception_date_tiret e2)
    {
        std::wcout << L"Exception a été capturée : " << e2.get_message() << std::endl;
        exit(1);
    }
    wstr = wstr.substr(1);
    auto d = std::stoi(wstr, &pos);
    assert((1 <= d && d <= 31) && L"Le jour invalide !");
    if (!checkday(m, d, y))
    {
        std::wcout << L"Le jour invalide !!!!" << std::endl;
        exit(1);
    }
    std::tm tm;
    tm.tm_year = y - 1900;
    tm.tm_mon = m - 1;
    tm.tm_mday = d;
    wstr = wstr.substr(2);
    m_date_de_reprise = tm;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Film::initialiser_Date_de_sortie(std::filesystem::path const& cheminFichier)                                                                  #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Film::initialiser_Date_de_sortie(std::filesystem::path const& cheminFichier)
{
    auto nomFichier = cheminFichier.wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    std::wstring wstr = lire_fichierTxt(nomFichier);
    std::size_t pos = 0;

    wchar_t sp = L' ', tiret = L'-';
    auto y = std::stoi(wstr, &pos);
    assert(1582 <= y && L"L'année est inférieur !");
    wstr = wstr.substr(4);
    try
    {
        test_date_tiret_sp_etc(wstr[0]);
    }
    catch (exception_date_tiret e2)
    {
        std::wcout << L"Exception a été capturée : " << e2.get_message() << std::endl;
        exit(1);
    }
    wstr = wstr.substr(1);
    auto m = std::stoi(wstr, &pos);
    assert((1 <= m && m <= 12) && L"Le mois invalide !");
    wstr = wstr.substr(2);
    try
    {
        test_date_tiret_sp_etc(wstr[0]);
    }
    catch (exception_date_tiret e2)
    {
        std::wcout << L"Exception a été capturée : " << e2.get_message() << std::endl;
        exit(1);
    }
    wstr = wstr.substr(1);
    auto d = std::stoi(wstr, &pos);
    assert((1 <= d && d <= 31) && L"Le jour invalide !");
    if (!checkday(m, d, y))
    {
        std::wcout << L"Le jour invalide !!!!" << std::endl;
        exit(1);
    }
    std::tm tm;
    tm.tm_year = y - 1900;
    tm.tm_mon = m - 1;
    tm.tm_mday = d;
    wstr = wstr.substr(2);
    m_date_de_sortie = tm;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Film::initialiser_De(fs::path const& cheminFichier)                                                                                           #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Film::initialiser_De(fs::path const& cheminFichier)
{ // De
    auto nomFichier = cheminFichier.wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    m_de = lire_fichierTxt(nomFichier, { L"\n", L", " });
    assert((m_de.size() != 0));
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Film::initialiser_Distributeur(fs::path const& cheminFichier)                                                                                 #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Film::initialiser_Distributeur(fs::path const& cheminFichier)
{
    auto nomFichier = cheminFichier.wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    m_distributeur = lire_fichierTxt(nomFichier);
    assert((m_distributeur.size() != 0));
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Film::initialiser_Par(fs::path const& cheminFichier)                                                                                          #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Film::initialiser_Par(fs::path const& cheminFichier)
{ // Par
    auto nomFichier = cheminFichier.wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    m_par = lire_fichierTxt(nomFichier, { L"\n", L", " });
    assert((m_par.size() != 0));
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Film::initialiser_Making_of(std::filesystem::path const& cheminFichier)                                                                       #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Film::initialiser_Making_of(std::filesystem::path const& cheminFichier)
{
    auto nomFichier = cheminFichier.wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    std::vector<std::wstring> contenu = lire_fichierTxt(cheminFichier.wstring(), { L"\n" });
    assert((contenu.size() != 0));
    if (contenu.size() > 0)
    {
        m_making_of_duree = extraire_Duree_DepuisUneLigneDUnFichier(contenu[0]).value();
        contenu.erase(contenu.begin());
        if (contenu.size() > 0)
        {
            m_making_of_resume = contenu;
            m_making_of = true;
        }
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Film::initialiser_Note(fs::path const& cheminFichier)                                                                                         #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Film::initialiser_Note(fs::path const& cheminFichier)
{ // 0...5 ou -1
    auto nomFichier = cheminFichier.wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    std::wstring n = lire_fichierTxt(nomFichier);
    std::size_t pos = n.length();
    if (n == L"")
    {
        m_note = -1.0;
        return;
    }
    if (pos != 1 && pos != 3)
    {
        return;
    }
    if (!iswdigit(n[0]))
    {
        return;
    }
    if (!(n[0] == L'0' || n[0] == L'1' || n[0] == L'2' || n[0] == L'3' || n[0] == L'4' || n[0] == L'5'))
    {
        return;
    }
    if (pos == 1)
        m_note = std::stod(n);
    else
    {
        if ((n[1] == L',' || n[1] == L'.') && iswdigit(n[2]) && pos == 3)
        {
            n[1] = L',';
            m_note = std::stod(n);
        }
        else
        {
            return;
        }
    }
    return;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Film::initialiser_Soundtrack(fs::path const& cheminFichier)                                                                                   #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Film::initialiser_Soundtrack(fs::path const& cheminFichier)
{
    auto nomFichier = cheminFichier.wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    m_soundtrack = lire_paireCleValeur_depuisFichierTxt(nomFichier, L" : ");
    assert((m_soundtrack.size() != 0));
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Film::initialiser_Titre(fs::path const& cheminFichier, std::vector<std::wstring>& ligne)                                                      #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Film::initialiser_Titre(fs::path const& cheminFichier)
{

    auto res = extraire_Informations_DepuisLeContenuDUnFichier(cheminFichier);

    m_titres = fusionner_Titres(m_titres, std::get<0>(res));
    m_duree = std::get<1>(res) ? std::get<1>(res).value() : -1;
    m_resume = std::get<2>(res);
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const void Film::Print()                                                                                                                           #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const void Film::Print()
{
    // Header
    Print_Header();
    // Date de reprise
    Print_Date_de_Reprise();
    // Date de sortie
    Print_Date_de_Sortie();
    // Titre Original
    Print_Titre_Original(m_titres_originaux, affichage_titres_originaux_actif, m_keyColor, m_valuesColor);
    // Distributeur
    Print_Distributeur();
    // AD
    Print_CleValeur(L"Audiodescription", m_audiodescription, affichage_audiodescription_actif, m_keyColor[0], m_valuesColor);
    // De
    Print_De();
    // Par
    Print_Par();
    // Nationalité(s)
    Print_Nationalites(m_nationalite, affichage_nationalite_actif, m_keyColor[0], m_valuesColor);
    // Resume
    Print_Resume(m_resume, affichage_resume_actif);
    // Avec
    Print_Avec();
    // Soundtracks
    Print_Soundtracks();
    // Image(s)
    Print_Images(m_image, affichage_image_actif, m_keyColor[0], m_valuesColor);
    // Making-of
    Print_Making_of();
    std::wcout << L"\r\n";
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Film::Print_Avec()                                                                                                                            #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Film::Print_Avec()
{
    if (affichage_avec_actif && m_avec.size())
    {
        std::wstring avec_str = m_keyColor[0] + L"Avec : " + m_valuesColor;
        bool found = false;
        for (auto&& [nom, role] : m_avec)
        {
            if (nom == L"…" || nom == L"..." || nom == L".")
            {
                found = true;
                break;
            }
            if (nom != L"" && role != L"")
            {
                avec_str += nom + L' ' + m_keyColor[1] + L'(' + m_valuesColor + role + m_keyColor[1] + L')' + m_valuesColor;
            }
            else if (nom == L"" && role != L"")
            {
                avec_str += m_keyColor[1] + L'(' + m_valuesColor + role + m_keyColor[1] + L')' + m_valuesColor;
            }
            else
            {
                avec_str += nom;
            }
            if (m_avec.back().first != nom)
                avec_str += m_keyColor[1] + L", " + m_valuesColor;
            else
                avec_str += m_keyColor[1] + L'.' + m_valuesColor;
        }
        if (found)
            avec_str += L"...";
        avec_str += L"\r\n";
        std::wcout << avec_str;
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const void Film::Print_Date_de_Reprise()                                                                                                           #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const void Film::Print_Date_de_Reprise()
{
    if (affichage_date_de_reprise_actif && m_date_de_reprise.tm_year != 0)
    {
        wchar_t date_string[22];
        std::wstring wstr;
        wcsftime(date_string, 18, L"%d/%m/%Y", &m_date_de_reprise);
        wstr = date_string;
        //Console_Lire(keyColor[0] + L"Date de reprise : " + valuesColor + wstr.substr(0, 2) + keyColor[0] + L'/' + valuesColor + wstr.substr(3, 2) + keyColor[0] + L'/' + valuesColor + wstr.substr(6, 4), 0, 19);
        //int i = Console_Lire_txt(keyColor[0] + L"Date de reprise : " + valuesColor + wstr.substr(0, 2) + keyColor[0] + L'/' + valuesColor + wstr.substr(3, 2) + keyColor[0] + L'/' + valuesColor + wstr.substr(6, 4), 0, 19);
        std::wcout << m_keyColor[0] + L"Date de reprise : " + m_valuesColor + wstr.substr(0, 2) + m_keyColor[0] + L'/' + m_valuesColor + wstr.substr(3, 2) + m_keyColor[0] + L'/' + m_valuesColor + wstr.substr(6, 4) + L"\r\n";
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const void Film::Print_Date_de_Sortie()                                                                                                            #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const void Film::Print_Date_de_Sortie()
{
    if (affichage_date_de_sortie_actif && m_date_de_sortie.tm_year != 0)
    {
        wchar_t date_string[22];
        std::wstring wstr;
        wcsftime(date_string, 14, L"%d/%m/%Y", &m_date_de_sortie);
        wstr = date_string;
        //Console_Lire(keyColor[0] + L"Date de sortie : " + valuesColor + wstr.substr(0, 2) + keyColor[0] + L'/' + valuesColor + wstr.substr(3, 2) + keyColor[0] + L'/' + valuesColor + wstr.substr(6, 4), 0, 18);
        //int i = Console_Lire_txt(keyColor[0] + L"Date de sortie : " + valuesColor + wstr.substr(0, 2) + keyColor[0] + L'/' + valuesColor + wstr.substr(3, 2) + keyColor[0] + L'/' + valuesColor + wstr.substr(6, 4), 0, 18);
        std::wcout << m_keyColor[0] + L"Date de sortie : " + m_valuesColor + wstr.substr(0, 2) + m_keyColor[0] + L'/' + m_valuesColor + wstr.substr(3, 2) + m_keyColor[0] + L'/' + m_valuesColor + wstr.substr(6, 4) + L"\r\n";
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const void Film::Print_De()                                                                                                                        #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const void Film::Print_De()
{
    if (affichage_de_actif && m_de.size() > 0)
    {
        std::wstring de_str = m_keyColor[0] + L"De : " + m_valuesColor;
        bool first = true;
        for (auto&& d : m_de)
        {
            if (!first)
            {
                de_str += m_keyColor[0] + L", " + m_valuesColor;
            }
            de_str += d;
            first = false;
        }
        de_str += L"\r\n";
        std::wcout << de_str;
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const void Film::Print_Distributeur()                                                                                                              #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const void Film::Print_Distributeur()
{
    if (affichage_distributeur_actif && m_distributeur.size() > 0)
    {
        std::wstring distributeur_str = m_keyColor[0] + L"Distributeur : " + m_valuesColor + m_distributeur + L"\r\n";
        //PrintStringW(m_hOut, creee_par_str, 0);
        //PrintStringW(HANDLE hOut, creee_par_str);
        //Console_Lire(distributeur_str, 0, 0);
        //int i = Console_Lire_txt(distributeur_str, 0, 0);
        std::wcout << distributeur_str;
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Film::Print_Header() const                                                                                                                    #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Film::Print_Header() const
{
    if (affichage_titres_actif)
    {
        std::wstring titres_str;
        std::wstring date_en_salle_ou_sur_str;
        std::wstring x_sj_str;
        std::wstring sj_str;
        std::wstring duree_str;
        std::wstring note_str;

        titres_str = m_keyColor[0] + L"Titre : " + m_valuesColor + m_titres[0];
        if (m_titres.size() > 1)
            titres_str += m_keyColor[1] + m_titres[1] + m_valuesColor + m_titres[2];
        // Date en salle ou sur
        if (affichage_date_en_salle_ou_sur_actif)
        {
            wchar_t date_string[15];
            wcsftime(date_string, 15, L"%d/%m/%Y", &m_date);
            date_en_salle_ou_sur_str = date_string;
            date_en_salle_ou_sur_str = m_keyColor[0] + L" (" + m_valuesColor +
                date_en_salle_ou_sur_str.substr(0, 2) + m_keyColor[0] + L'/' + m_valuesColor +
                date_en_salle_ou_sur_str.substr(3, 2) + m_keyColor[0] + L'/' + m_valuesColor +
                date_en_salle_ou_sur_str.substr(6, 4);
            if (affichage_sur_actif && m_sur == L"")
            {
                date_en_salle_ou_sur_str += m_keyColor[1] + L" en salle" + m_valuesColor;
            }
            else if (affichage_sur_actif && (m_sur == L"Disney+" || m_sur == L"Netflix"))
            {
                date_en_salle_ou_sur_str += m_keyColor[1] + L" sur " + m_valuesColor;
                if (m_sur == L"Disney+")
                {
                    date_en_salle_ou_sur_str += L"Disney+ " + m_keyColor[1] + L": " + m_valuesColor + m_disney_sj;
                }
                else
                {
                    date_en_salle_ou_sur_str += L"Netflix " + m_keyColor[1] + L": " + m_valuesColor + m_netflix_sj;
                }
            }
            else 
            {
                date_en_salle_ou_sur_str += m_sur;
            }
            date_en_salle_ou_sur_str += m_keyColor[0] + L')' + m_valuesColor;
        }
        // x signalétique jeunesse
        if (affichage_x_sj_actif && m_sur == L"")
        {
            // Disney+ SJ
            if (affichage_disney_sj_actif && m_disney_sj.length() != 0)
                x_sj_str += m_keyColor[0] + L" (" + m_valuesColor + L"Disney+" + m_keyColor[1] + L" : " + m_valuesColor + m_disney_sj + m_keyColor[0] + L')' + m_valuesColor;
            // Netflix SJ
            if (affichage_netflix_sj_actif && m_netflix_sj.length() != 0)
                x_sj_str += m_keyColor[0] + L" (" + m_valuesColor + L"Netflix" + m_keyColor[1] + L" : " + m_valuesColor + m_netflix_sj + m_keyColor[0] + L')' + m_valuesColor;
        }
        // La signalétique jeunesse
        if (affichage_sj_actif && m_sj.length() != 0)
            sj_str += m_keyColor[0] + L" (" + m_valuesColor + L"SJ" + m_keyColor[1] + L" : " + m_valuesColor + m_sj + m_keyColor[0] + L')' + m_valuesColor;
        // Durée
        if (affichage_duree_actif)
        {
            duree_str = L' ' + std::to_wstring(m_duree / (60 * 60)) + m_keyColor[0] + L"h" + m_valuesColor + L' ' + std::to_wstring((m_duree - ((m_duree / (60 * 60)) * 60 * 60)) / 60) + m_keyColor[0] + L"min " + m_valuesColor;
        }
        // Note
        if (affichage_note_actif)
            note_str += Print_Note();

        std::wcout << titres_str << date_en_salle_ou_sur_str << x_sj_str << sj_str << duree_str << note_str << std::endl;
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const void Film::Print_Making_of()                                                                                                                 #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const void Film::Print_Making_of()
{
    if (affichage_making_of_actif && m_making_of)
    {
        std::wstring making_of_str = m_keyColor[0] + L"Making of" + m_valuesColor + L' ';
        std::wstring duree_str;
        std::wstring resume_str;
        if (affichage_duree_actif)
        {
            duree_str = m_keyColor[0] + L"(" + m_valuesColor +
                std::to_wstring(m_making_of_duree / (60 * 60)) + m_keyColor[1] + L"h" + m_valuesColor + L' ' + std::to_wstring((m_making_of_duree - ((m_making_of_duree / (60 * 60)) * 60 * 60)) / 60) + m_keyColor[1] + L"min" + m_valuesColor +
                m_keyColor[0] + L") " + m_valuesColor;
        }
        if(m_making_of)
        {
            duree_str += m_keyColor[0] + L':' + m_valuesColor + L"\r\n";
            for (auto r : m_making_of_resume)
            {
                resume_str += r + L"\r\n";
            }
        }
        std::wcout << making_of_str << duree_str << resume_str;
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # std::wstring Film::Print_Note() const                                                                                                              #
// #                                                                                                                                                    #
// ######################################################################################################################################################

std::wstring Film::Print_Note() const
{
    if (affichage_note_actif)
    {
        std::wstring note_str;
        if (m_note != 0.0)
        {
            if (m_note == -1.0)
                note_str += L' ' + m_keyColor[0] + L'(' + m_valuesColor + L"pas de note !" + m_keyColor[0] + L')' + m_valuesColor;
            else
            {
                if (m_note == 0 || m_note == 1 || m_note == 2 || m_note == 3 || m_note == 4 || m_note == 5)
                    //note_str += L' ' + std::to_wstring(note);
                    note_str = L' ' + std::to_wstring(static_cast<int>(std::floor(m_note)));
                else
                {
                    std::wstring wstr = std::to_wstring(m_note);
                    wstr = wstr[0] + m_keyColor[0] + wstr[1] + m_valuesColor + wstr[2];
                    note_str += L' ' + wstr;
                }
                note_str += m_keyColor[0] + L"/5" + m_valuesColor;
            }
        }
        else
        {
            note_str = L' ' + m_keyColor[0] + L'(' + m_valuesColor + L"pas de note !" + m_keyColor[0] + L')' + m_valuesColor;
        }
        return note_str;
    }
    return L"";
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const void Film::Print_Par()                                                                                                                       #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const void Film::Print_Par()
{
    if (affichage_par_actif && m_par.size() > 0)
    {
        std::wstring par_str = m_keyColor[0] + L"Par : " + m_valuesColor;
        bool first = true;
        for (auto&& p : m_par)
        {
            if (!first)
            {
                par_str += m_keyColor[0] + L", " + m_valuesColor;
            }
            par_str += p;
            first = false;
        }
        par_str += L"\r\n";

        std::wcout << par_str;
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const void Film::Print_Soundtracks()                                                                                                               #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const void Film::Print_Soundtracks()
{
    if (affichage_soundtraks_actif && m_soundtrack.size() > 0)
    {
        std::size_t maxKeyLength = 0;
        for (int i = 0; i < m_soundtrack.size(); i++)
        {
            maxKeyLength = max(maxKeyLength, m_soundtrack[i].first.length());
            //maxKeyLength = std::max(maxKeyLength, m_soundtrack[i].first.length()); // Erreur !!!
        }

        //PrintTmp(keyColor + L"Soundtrack :" + valuesColor + L"\r\n");
        //Console_Lire(keyColor[0] + L"Soundtrack :" + valuesColor + L"\r\n", 0, 0);
        std::wcout << m_keyColor[0] + L"Soundtrack :" + m_valuesColor + L"\r\n";

        for (auto&& [role, nom] : m_soundtrack)
        {
            //PrintTmp(keyColor2 + role + std::wstring(maxKeyLength - role.size(), L' ') + L" : " + valuesColor2 + nom + L"\r\n");
            //Console_Lire(role + keyColor[1] + std::wstring(maxKeyLength - role.size(), L' ') + L" : " + valuesColor + nom + L"\r\n", 4, 0);
            //int i = Console_Lire_txt(role + keyColor[1] + std::wstring(maxKeyLength - role.size(), L' ') + L" : " + valuesColor + nom + L"\r\n", 4, 0);
            std::wcout << role + m_keyColor[1] + std::wstring(maxKeyLength - role.size(), L' ') + L" : " + m_valuesColor + nom + L"\r\n";
        }
    }
}