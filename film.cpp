#include "pch.h"

#include "utils.h"
#include "Exemple.h"
#include "film.h"
#include "film_serie.h"
#include "serie.h"

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

using DateVisionnage = DateRecord;

extern const std::vector<std::wstring> Audiodescription;
extern const std::vector<std::wstring> Genre;
extern const std::vector<std::wstring> Nationalite;
extern const std::vector<std::wstring> Sous_Genre;

extern std::wstring replace_all(std::wstring subject, const std::wstring& search, const std::wstring& replace);

extern const std::vector<std::wstring> lire_fichierTxt(std::wstring const& nomFichier, std::vector<std::wstring> separeteurs);
extern const std::vector<std::wstring> lire_fichierTxt(std::wstring const& nomFichier, std::vector<std::wstring> separeteurs, bool found);
extern const std::vector<std::pair<std::wstring, std::wstring>>lire_paireCleValeur_depuisFichierTxt(std::wstring const& nomFichier, std::wstring separeteur);
//extern const std::wstring lire_et_decouper_fichierTxt(std::wstring const& nomFichier);
extern const std::wstring lire_fichierTxt(std::wstring const& nomFichier);

extern void test_date_tire(wchar_t d);
extern void test_date_tiret_sp_etc(wchar_t d);
extern void test_sp_et_npos_ou_pas_isblank(wchar_t sp, bool t);
extern bool checkyear(int y);
extern bool checkmonth(int m);
extern bool checkday(int m, int d, int y);

extern void initialiser_Audiodescription(fs::path const& cheminFichier, std::wstring& m_ad);
extern void initialiser_Avec(fs::path const& cheminFichier, std::vector<std::pair<std::wstring, std::wstring>>& m_avec);
//extern const void initialiser_Chaine(fs::path const& cheminFichier, std::wstring& m_chaine);
extern void initialiser_Genre(fs::path const& cheminFichier, std::vector<std::wstring>& m_genres_renvoyes, const std::vector<std::wstring>& genres_valides);
extern void initialiser_Image(fs::path const& cheminFichier, std::vector<std::wstring>& m_images);
extern void initialiser_Nationalite(fs::path const& cheminFichier, std::vector<std::wstring>& m_nationalites_renvoyes, const std::vector<std::wstring>& nationalites_valides);
//extern void initialiser_Sous_Genre(std::wstring& m_s_g);
extern bool initialiser_Sous_Genre(std::wstring& m_s_g);
extern void initialiser_Sur(std::wstring& m_s);
extern std::vector<std::wstring> xyz_Titre(std::wstring& file_content);
extern void initialiser_Titre_Original(fs::path const& cheminFichier, std::vector<std::wstring>& m_titre_original);

extern std::wstring recuperer_Disney_SJ(fs::path const& cheminFichier);
extern std::wstring recuperer_Netflix_SJ(fs::path const& cheminFichier);
extern std::wstring recuperer_SJ(fs::path const& cheminFichier);

extern void Print_Audiodescription(const std::wstring& m_audiodescription, bool affichage_audiodescription_actif, std::wstring& keyColor, std::wstring& valuesColor);
extern void Print_Images(const std::vector<std::wstring>& m_image, bool affichage_image_actif, std::wstring& keyColor, std::wstring& valuesColor);
extern void Print_Genres(const std::vector<std::wstring>& m_genres, bool affichage_genres_actif, const std::wstring& m_sous_genre, bool affichage_sous_genre_actif, std::wstring& keyColor, std::wstring& valuesColor);
extern void Print_Nationalites(const std::vector<std::wstring>& m_nationalites, bool affichage_nationalite_actif, std::wstring& keyColor, std::wstring& valuesColor);
extern void Print_Resume(const std::vector<wstring>& m_resume, bool affichage_resume_actif);
extern void Print_Titre_Original(const std::vector<std::wstring>& m_titre_original, bool affichage_titre_original_actif, std::vector<std::wstring>& keyColor, std::wstring& valuesColor);


const std::vector<std::wstring> Soundtrack
{
    L"Compositeur",
    L"Compositeur (chansons du film)",
    L"Montage musical",
    L"Superviseur musical"
};


// ######################################################################################################################################################
// ######################################################################################################################################################

/*static bool ends_with(std::wstring_view str, std::wstring_view suffix)
{
    return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}*/

// ######################################################################################################################################################
// ######################################################################################################################################################

Film::Film(std::filesystem::path racine)
{
    this->racine = racine;
    auto nomDossier = racine.filename().wstring();
    assert(nomDossier.length() > 0 && L"Nom de dossier vide");

    std::wregex filename_pattern{ L"(.+?)(?:\\.\\((\\d{4}\\-\\d{2}\\-\\d{2}\\s)?([^\\)]*)\\))?(?:\\.(.+))?" };
    std::wsmatch match;
    if (std::regex_match(nomDossier, match, filename_pattern))
    {
        std::wstring titres = match[1];
        m_titres = Dossier_Titres(titres);
        /*if (match[2].matched)
        {
            std::wstring annees_str = match[2].str();
            std::wsmatch dummy;
            if (std::regex_match(annees_str, dummy, std::wregex(L"\\d{4}\\-\\d{4}\\s")))
            {
                m_f_anneesProduction.first = stoi(annees_str);
                m_f_anneesProduction.second = stoi(annees_str.substr(5));
            }
            else
            {
                m_f_anneesProduction.first = stoi(annees_str);
            }
        }*/

        m_sur = (match[3].matched) ? match[3].str() : L"";

        std::wstring sous_genre = (match[4].matched) ? match[4].str() : L"";
        m_sous_genre = sous_genre;
    }
    else
    {
        assert(false == true && "Le nom du répertoire n'est pas un nom valide.");
    }
}

std::vector<std::wstring> Film::Dossier_Titres(std::wstring titres)
{
    assert(titres.length() > 0 && L"Nom de titres vide"); // ??? pour Mot de... ?
    /*std::size_t pos = 0;
    const std::wstring d_p = L" - ";
    pos = titres.find(d_p);
    bool found = false;
    if (!found && pos != std::wstring::npos)
    {
        m_titres.push_back(titres.substr(0, pos));
        m_titres.push_back(d_p);
        m_titres.push_back(titres.substr(pos + 3));
        found = true;
    }
    const std::wstring d_p2 = L"- ";
    pos = titres.find(d_p2);
    if (!found && pos != std::wstring::npos)
    {
        m_titres.push_back(titres.substr(0, pos));
        m_titres.push_back(d_p2);
        m_titres.push_back(titres.substr(pos + 2));
        found = true;
    }
    const std::wstring d_p3 = L"-";
    pos = titres.find(d_p3);
    if (!found && pos != std::wstring::npos)
    {
        m_titres.push_back(titres.substr(0, pos));
        m_titres.push_back(d_p3);
        m_titres.push_back(titres.substr(pos + 1));
        found = true;
    }
    if (!found)
    {
        m_titres.push_back(titres);
        //found = true;
    }*/
    m_titres = ::xyz_Titre(titres);
    return m_titres;
}

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
            return;
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
            initialiser_Titre(cheminFichier, m_titres);
        }
        // Titre original
        if (nomFichier == L"Titre original.txt")
        {
            initialiser_Titre_Original(cheminFichier, m_titres_originaux);
        }
        // xxxx-yy-zz
        //if (nomFichier != L"")
        if (std::regex_match(nomFichier, std::wregex{ L"([[:digit:]])(.+)" }))
        {
            //E.afficher_X(-1, nomFichier, L'{' + t + L".txt} !!!");
            std::wcout << L'{' << cheminFichier << L'}' << std::endl;
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

void Film::initialiser_De(fs::path const& cheminFichier)
{ // De
    auto nomFichier = cheminFichier.wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    m_de = lire_fichierTxt(nomFichier, { L"\n", L", " });
    assert((m_de.size() != 0));
}

void Film::initialiser_Distributeur(fs::path const& cheminFichier)
{
    auto nomFichier = cheminFichier.wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    m_distributeur = lire_fichierTxt(nomFichier);
    assert((m_distributeur.size() != 0));
}

void Film::initialiser_Duree(std::wstring& m)
{
    const std::wregex duree_format_rg{ L"^(?:(\\d+)(?:h|H))?\\s?(?:(\\d+)(?:min|MIN))?$" };
    std::wsmatch match;

    if (std::regex_match(m, match, duree_format_rg))
    {
        //auto duree_en_heure = std::stoi(match[1]);
        //auto duree_en_minute = std::stoi(match[2]);
        //m_duree = duree_en_heure * 60 * 60 + duree_en_minute * 60;
        m_duree = (match[1].matched ? std::stoi(match[1]) : 0) * 60 * 60 + (match[2].matched ? std::stoi(match[2]) : 0) * 60;
    }
    else
    {
        throw std::invalid_argument("'" + std::string{ m.begin(),m.end() } + "' n'est pas un format de durée valide.");
    }

}

void Film::initialiser_Par(fs::path const& cheminFichier)
{ // Par
    auto nomFichier = cheminFichier.wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    m_par = lire_fichierTxt(nomFichier, { L"\n", L", " });
    assert((m_par.size() != 0));
}

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

void Film::initialiser_Soundtrack(fs::path const& cheminFichier)
{
    auto nomFichier = cheminFichier.wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    m_soundtrack = lire_paireCleValeur_depuisFichierTxt(nomFichier, L" : ");
    assert((m_soundtrack.size() != 0));
}

void Film::initialiser_Titre(fs::path const& cheminFichier, std::vector<std::wstring>& m_titre)
{ // Titre
    auto nomFichier = cheminFichier.wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    std::vector<std::wstring> contenu = lire_fichierTxt(cheminFichier.wstring(), { L"\n" });
    assert((contenu.size() != 0));

/*    std::vector<std::wstring> t;

    std::wregex titre_pattern{ L"(.+?)(\\s:\\s|:\\s|/|\\s-\\s)(.+)" };
    //std::wregex filename_pattern{ L"(.+?)(?:\\.\\[(\\d{4}\\-\\d{4}\\s?|\\d{4}\\-\\s?|\\d{4}\\s?)?([^\\]]*)\\])?(?:\\.(.+))?" };
    std::wsmatch match;
    if (std::regex_match(contenu[0], match, titre_pattern))
    {
        t.push_back(match[1]);
        if (match.length() > 2)
        {
            t.push_back(match[2]);
        }
        if (match.length() > 3)
        {
            t.push_back(match[3]);
        }
    }
    else
    {
        t.push_back(contenu[0]);
    }*/

    m_titres = ::xyz_Titre(contenu[0]);

    contenu.erase(contenu.begin());
    if (contenu.size() > 0)
    {
        initialiser_Duree(contenu[0]);
        //contenu.erase(contenu.begin());

        contenu.erase(contenu.begin());
        if (contenu.size() > 0)
            m_resume = contenu;
    }
}

const void Film::Print()
{
    // Header
    Print_Header();
    // Date de reprise
    Print_Date_de_Reprise();
    // Date de sortie
    Print_Date_de_Sortie();
    // Titre Original
    Print_Titre_Original(m_titres_originaux, affichage_titres_originaux_actif, keyColor, valuesColor);
    // Distributeur
    Print_Distributeur();
    // AD
    Print_Audiodescription(m_audiodescription, affichage_audiodescription_actif, keyColor[0], valuesColor);
    // De
    Print_De();
    // Par
    Print_Par();
    // Nationalité(s)
    Print_Nationalites(m_nationalite, affichage_nationalite_actif, keyColor[0], valuesColor);
    // Resume
    Print_Resume(m_resume, affichage_resume_actif);
    // Avec
    Print_Avec();
    // Soundtracks
    Print_Soundtracks();
    // Image(s)
    Print_Images(m_image, affichage_image_actif, keyColor[0], valuesColor);
    std::wcout << L"\r\n";
}

void Film::Print_Avec()
{
    if (affichage_avec_actif && m_avec.size())
    {
        std::wstring avec_str = keyColor[0] + L"Avec : " + valuesColor;
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
                avec_str += nom + L' ' + keyColor[1] + L'(' + valuesColor + role + keyColor[1] + L')' + valuesColor;
            }
            else if (nom == L"" && role != L"")
            {
                avec_str += keyColor[1] + L'(' + valuesColor + role + keyColor[1] + L')' + valuesColor;
            }
            else
            {
                avec_str += nom;
            }
            if (m_avec.back().first != nom)
                avec_str += keyColor[1] + L", " + valuesColor;
            else
                avec_str += keyColor[1] + L'.' + valuesColor;
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
        std::wcout << keyColor[0] + L"Date de reprise : " + valuesColor + wstr.substr(0, 2) + keyColor[0] + L'/' + valuesColor + wstr.substr(3, 2) + keyColor[0] + L'/' + valuesColor + wstr.substr(6, 4) + L"\r\n";
    }
}

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
        std::wcout << keyColor[0] + L"Date de sortie : " + valuesColor + wstr.substr(0, 2) + keyColor[0] + L'/' + valuesColor + wstr.substr(3, 2) + keyColor[0] + L'/' + valuesColor + wstr.substr(6, 4) + L"\r\n";
    }
}


const void Film::Print_De()
{
    if (affichage_de_actif && m_de.size() > 0)
    {
        std::wstring de_str = keyColor[0] + L"De : " + valuesColor;
        bool first = true;
        for (auto&& d : m_de)
        {
            if (!first)
            {
                de_str += keyColor[0] + L", " + valuesColor;
            }
            de_str += d;
            first = false;
        }
        de_str += L"\r\n";

        std::wcout << de_str;
    }
}

const void Film::Print_Distributeur()
{
    if (affichage_distributeur_actif && m_distributeur.size() > 0)
    {
        std::wstring distributeur_str = keyColor[0] + L"Distributeur : " + valuesColor + m_distributeur + L"\r\n";
        //PrintStringW(m_hOut, creee_par_str, 0);
        //PrintStringW(HANDLE hOut, creee_par_str);
        //Console_Lire(distributeur_str, 0, 0);
        //int i = Console_Lire_txt(distributeur_str, 0, 0);
        std::wcout << distributeur_str;
    }
}

const void Film::Print_Header()
{
    if (affichage_titres_actif)
    {
        std::wstring titres_str;
        std::wstring annees_str;
        std::wstring sur_str;
        std::wstring sj_str;
        std::wstring duree_str;
        std::wstring note_str;

        titres_str = keyColor[0] + L"Titre : " + valuesColor + m_titres[0];
        if (m_titres.size() > 1)
            titres_str += keyColor[1] + m_titres[1] + valuesColor + m_titres[2];
        // Date
        /*if (affichage_date_actif)
        {
            date_str = format_Annees();
        }*/
        // sur
        /*if (affichage_sur_actif && m_sur != L"")
        {
            sur_str += keyColor[0] + L" (" + keyColor[1] + L"sur " + valuesColor + m_sur + keyColor[1] + L" : " + valuesColor;
            // Disney+ SJ
            if (affichage_disney_sj_actif && m_disney_sj.length() != 0)
                sur_str += m_disney_sj;
            // Netflix SJ
            if (affichage_netflix_sj_actif && m_netflix_sj.length() != 0)
                sur_str += m_netflix_sj;
            sur_str += keyColor[0] + L')' + valuesColor;
        }
        else
        {
            // Disney+ SJ
            if (affichage_disney_sj_actif && m_disney_sj.length() != 0)
                sur_str += keyColor[0] + L" (" + valuesColor + L"Disney+" + keyColor[1] + L" : " + m_disney_sj + keyColor[0] + L')' + valuesColor;
            // Netflix SJ
            if (affichage_netflix_sj_actif && m_netflix_sj.length() != 0)
                sur_str += keyColor[0] + L" (" + valuesColor + L"Netflix" + keyColor[1] + L" : " + m_netflix_sj + keyColor[0] + L')' + valuesColor;
        }*/
        if (affichage_sur_actif && m_sur != L"" && m_sur != L"Disney+" && m_sur != L"Netflix")
        {
            sur_str += keyColor[0] + L" (" + keyColor[1] + L"en salle " + valuesColor + m_sur + keyColor[0] + L')' + valuesColor;
        }
        if (affichage_sur_actif && (m_sur == L"Disney+" || m_sur == L"Netflix"))
        {
            sur_str += keyColor[0] + L" (" + keyColor[1] + L"sur " + valuesColor + m_sur + keyColor[1] + L" : " + valuesColor;
            // Disney+ SJ
            if (affichage_disney_sj_actif && m_disney_sj.length() != 0)
                sur_str += m_disney_sj;
            // Netflix SJ
            if (affichage_netflix_sj_actif && m_netflix_sj.length() != 0)
                sur_str += m_netflix_sj;
            sur_str += keyColor[0] + L')' + valuesColor;
        }
        else
        {
            // Disney+ SJ
            if (affichage_disney_sj_actif && m_disney_sj.length() != 0)
                sur_str += keyColor[0] + L" (" + valuesColor + L"Disney+" + keyColor[1] + L" : " + valuesColor + m_disney_sj + keyColor[0] + L')' + valuesColor;
            // Netflix SJ
            if (affichage_netflix_sj_actif && m_netflix_sj.length() != 0)
                sur_str += keyColor[0] + L" (" + valuesColor + L"Netflix" + keyColor[1] + L" : " + valuesColor + m_netflix_sj + keyColor[0] + L')' + valuesColor;
        }
        // La signalétique jeunesse
        if (affichage_sj_actif && m_sj.length() != 0)
            sj_str += keyColor[0] + L" (" + valuesColor + L"SJ" + keyColor[1] + L" : " + valuesColor + m_sj + keyColor[0] + L')' + valuesColor;
        // Durée
        if (affichage_duree_actif)
        {
            //duree_str = L' ' + std::to_wstring(m_duree / (60 * 60)) + keyColor[0] + L"h" + valuesColor + L' ' + std::to_wstring(m_duree / 60) + keyColor[0] + L"min " + valuesColor;
            duree_str = L' ' + std::to_wstring(m_duree / (60 * 60)) + keyColor[0] + L"h" + valuesColor + L' ' + std::to_wstring((m_duree - ((m_duree / (60 * 60)) * 60 * 60)) / 60) + keyColor[0] + L"min " + valuesColor;
        }
        // Note
        if (affichage_note_actif)
            note_str += Print_Note();

        std::wcout << titres_str << annees_str << sur_str << sj_str << duree_str << note_str << std::endl;
    }
}

const std::wstring Film::Print_Note()
{
    if (affichage_note_actif)
    {
        std::wstring note_str;
        if (m_note != 0.0)
        {
            if (m_note == -1.0)
                note_str += L' ' + keyColor[0] + L'(' + valuesColor + L"pas de note !" + keyColor[0] + L')' + valuesColor;
            else
            {
                if (m_note == 0 || m_note == 1 || m_note == 2 || m_note == 3 || m_note == 4 || m_note == 5)
                    //note_str += L' ' + std::to_wstring(note);
                    note_str = L' ' + std::to_wstring(static_cast<int>(std::floor(m_note)));
                else
                {
                    std::wstring wstr = std::to_wstring(m_note);
                    wstr = wstr[0] + keyColor[0] + wstr[1] + valuesColor + wstr[2];
                    note_str += L' ' + wstr;
                }
                note_str += keyColor[0] + L"/5" + valuesColor;
            }
        }
        else
        {
            note_str = L' ' + keyColor[0] + L'(' + valuesColor + L"pas de note !" + keyColor[0] + L')' + valuesColor;
        }
        return note_str;
    }
    return L"";
}


const void Film::Print_Par()
{
    if (affichage_par_actif && m_par.size() > 0)
    {
        std::wstring par_str = keyColor[0] + L"Par : " + valuesColor;
        bool first = true;
        for (auto&& p : m_par)
        {
            if (!first)
            {
                par_str += keyColor[0] + L", " + valuesColor;
            }
            par_str += p;
            first = false;
        }
        par_str += L"\r\n";

        std::wcout << par_str;
    }
}

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
        std::wcout << keyColor[0] + L"Soundtrack :" + valuesColor + L"\r\n";

        for (auto&& [role, nom] : m_soundtrack)
        {
            //PrintTmp(keyColor2 + role + std::wstring(maxKeyLength - role.size(), L' ') + L" : " + valuesColor2 + nom + L"\r\n");
            //Console_Lire(role + keyColor[1] + std::wstring(maxKeyLength - role.size(), L' ') + L" : " + valuesColor + nom + L"\r\n", 4, 0);
            //int i = Console_Lire_txt(role + keyColor[1] + std::wstring(maxKeyLength - role.size(), L' ') + L" : " + valuesColor + nom + L"\r\n", 4, 0);
            std::wcout << role + keyColor[1] + std::wstring(maxKeyLength - role.size(), L' ') + L" : " + valuesColor + nom + L"\r\n";
        }
    }
}