#include "pch.h"

#include "Exemple.h"
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
//#include <stdio.h>
#include <locale>
#include <algorithm>
#include <codecvt>
#include <tuple>

#include <filesystem> // C++17 standard header file name
//#include <experimental/filesystem> // Header file for pre-standard implementation
//using namespace std::experimental::filesystem::v1;
using namespace std;
namespace fs = std::filesystem;

extern const std::vector<std::wstring> lire_fichierTxt(std::wstring const& nomFichier, std::vector<std::wstring> separeteurs);
extern const std::vector<std::wstring> lire_fichierTxt(std::wstring const& nomFichier, std::vector<std::wstring> separeteurs, bool found);
extern const std::vector<std::pair<std::wstring, std::wstring>>lire_paireCleValeur_depuisFichierTxt(std::wstring const& nomFichier, std::wstring separeteur);
extern const std::wstring lire_fichierTxt(std::wstring const& nomFichier);

extern void test_date_tire(wchar_t d);
extern void test_sp_et_npos_ou_pas_isblank(wchar_t sp, bool t);
extern bool checkyear(int y);
extern bool checkmonth(int m);
extern bool checkday(int m, int d, int y);

extern const void afficher_Avec(fs::path const& cheminFichier, std::vector<std::pair<std::wstring, std::wstring>>& avec);

Episode::Episode(void)
{}

Episode::~Episode()
{}

Saison::Saison()
{}

Saison::~Saison()
{}

Serie::Serie(std::filesystem::path racine)
{
    this->racine = racine;
}

Serie::~Serie()
{}

/*const void PrintDate_ou_Dates(const std::vector<std::pair<std::vector<DateRecord>, std::wstring>>& dates)
{
    bool affichage_Date_ou_Dates = true;
    if (affichage_Date_ou_Dates && dates.size() > 0)
    {
        std::vector<std::wstring>keyColor{ L"\x1b[94;1m", L"\x1b[38;2;0;255;0m" }; // keyColor[0] (bleu) et keyColor[1] (vert)
        std::wstring valuesColor = L"\x1b[38;2;255;255;255m"; // Blanc
        std::size_t taille, taille2;
        wchar_t date_string[15];
        taille = std::size(dates);
        std::wstring wstr;
        for (int i = 0; i < taille; i++)
        {
            taille2 = std::size(dates[i].first);
            if (taille2 == 1)
            {
                wcsftime(date_string, 15, L"%d/%m/%Y", &dates[i].first[0].date);
                wstr = date_string;
                wstr = wstr.substr(0, 2) + keyColor[1] + L'/' + valuesColor + wstr.substr(3, 2) + keyColor[1] + L'/' + valuesColor + wstr.substr(6, 4);
                if(dates[i].second != L"")
                    wstr += keyColor[1] + L" : " + valuesColor + dates[i].second;
                if (dates[i].first[0].someFlag)
                    wstr += keyColor[1] + L" (" + valuesColor + L"préquel ou pas !" + keyColor[1] + L')' + valuesColor;
                std::wcout << wstr << std::endl;
            }
            else
            {
                int j;
                wstr = L"";
                std::wstring wstr2;
                std::size_t pos = 0;
                std::vector<wstring>k(taille2);
                std::tm temp{ 0 };
                int temp2 = 1;
                for (j = 0; j < taille2; j++)
                {
                    if (dates[i].first[j].date.tm_year == temp.tm_year && dates[i].first[j].date.tm_mon == temp.tm_mon && dates[i].first[j].date.tm_mday == temp.tm_mday)
                        // dates[i].first[j].date == temp : Marche pas !!!
                    {
                        k[j] = keyColor[1] + L'(' + valuesColor + std::to_wstring(temp2 + 1) + keyColor[1] + L')' + valuesColor;
                        if (temp2 == 1)
                            k[j - 1] += L' ' + keyColor[1] + L'(' + valuesColor + std::to_wstring(temp2) + keyColor[1] + L')' + valuesColor;
                        temp2++;
                    }
                    else
                    {
                        wcsftime(date_string, 15, L"%d/%m/%Y", &dates[i].first[j].date);
                        wstr2 = date_string;
                        k[j] = wstr2.substr(0, 2) + keyColor[1] + L'/' + valuesColor + wstr2.substr(3, 2) + keyColor[1] + L'/' + valuesColor + wstr2.substr(6, 4);
                        temp.tm_year = dates[i].first[j].date.tm_year;
                        temp.tm_mon = dates[i].first[j].date.tm_mon;
                        temp.tm_mday = dates[i].first[j].date.tm_mday;
                        temp2 = 1;
                    }

                }
                wstr2 = L"";
                for (j = 1; j < taille2 - 1; j++)
                {
                    pos = k[j].find(L"/");
                    if (pos != string::npos)
                        k[j] = keyColor[1] + L", " + valuesColor + k[j];
                }
                k.back() = L" et " + k.back();
                for (j = 0; j < taille2; j++)
                    wstr2 += k[j];
                wstr += wstr2;
                wstr += L' ' + keyColor[1] + L'[' + valuesColor + L"pas-à-pas" + keyColor[1] + L']' + valuesColor;
                if (dates[i].first.back().someFlag == true)
                    wstr += keyColor[1] + L" (" + valuesColor + L"préquel" + keyColor[1] + L')' + valuesColor;
                std::wcout << wstr << std::endl;
            }
        }
    }
}*/
// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Episode::afficher()                                                                                                                           #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Episode::afficher()
{
    auto nomFichier = m_cheminFichier.filename().wstring();
    std::size_t pos = 0;
    auto strRestant = nomFichier.substr(pos);
    pos = strRestant.length();
    strRestant = strRestant.substr(0, pos - 4);
    assert((strRestant[0] != L'0') && L"Pas de zero !!!");
    try
    {
        if (!(
            (std::isdigit(strRestant[0]) && strRestant[1] == L'x')
            ||
            (std::isdigit(strRestant[0]) && std::isdigit(strRestant[1]) && strRestant[2] == L'x')
            ||
            (std::isdigit(strRestant[0]) && std::isdigit(strRestant[1]) && std::isdigit(strRestant[2]) && strRestant[3] == L'x')
            ))
        {
        }
    }
    catch (std::runtime_error const& exception)
    {
        std::cout << "Erreur : " << exception.what() << std::endl;
    }
    x = std::stoi(strRestant, &pos);
    pos++;
    strRestant = strRestant.substr(pos);
    e = stoi(strRestant.substr(0, pos + 1));

    /*try
    {
        if (e <= saison.first)
        {
        }
    }
    catch (std::runtime_error const& exception)
    {
        std::cout << "Erreur : " << exception.what() << std::endl;
    }*/

    pos = strRestant.find(L'.');
    strRestant = strRestant.substr(pos);
    assert((strRestant[0] == L'.') && L"Pas de . (poing) !!!");
    strRestant = strRestant.substr(1);
    assert((strRestant.length() > 9) && L"Nom de fichier trop court pour avoir au moins une date");
    std::vector<DateRecord> dr;
    std::wstring streaming = L"";

    wchar_t sp = L' ', tiret = L'-', tiret_bas = L'_';
    int y, m, d;

    int firstYear = 0, firstMon = 0, firstDay = 0;
    int i = 0;
    do
    {
        if (strRestant[0] == sp)
        {
            if (strRestant[1] == std::wstring::npos || strRestant[1] == sp)
            {
                // try cath !!!
                // Explique-moi ??? 
                // ou : 
                exit(1);
            }
            strRestant = strRestant.substr(1);
            // try cath !!!
            // Explique-moi ??? 
            // ou : 
            try // Erreur !!!
            {
                test_sp_et_npos_ou_pas_isblank(strRestant[0], isblank(strRestant[1]));
            }
            catch (exception_test_sp_et_npos_ou_pas_isblank e)
            {
                exit(1);
            }
            if (strRestant.length() > 0)
            {
                streaming = strRestant;
            }
            strRestant = L"";
            break;
        }
        if (!isdigit(strRestant[0]))
        {
            // try cath !!!
            // Explique-moi ??? 
            // ou : 
            exit(1);
        }
        // year + mon + mday
        if ((y = stoi(strRestant.substr(0, 4))) && checkyear(y)
            &&
            strRestant[4] == tiret
            &&
            (m = std::stoi(strRestant.substr(5, 2))) && checkmonth(m)
            &&
            strRestant[7] == tiret
            &&
            (d = std::stoi(strRestant.substr(8, 2))) && checkday(m, d, y)
            &&
            firstYear < y)
        {
            assert(firstYear < y && L"L'année aaaaa");
            firstYear = y;
            assert(firstMon < m && L"Le mois aaaaa");
            firstMon = m;
            assert(firstDay <= d && L"Le jours aaaaa");
            firstDay = d;
            dr.push_back(DateRecord{ 0 });
            dr[i].date.tm_year = y - 1900;
            dr[i].date.tm_mon = m - 1;
            dr[i].date.tm_mday = d;
            strRestant = strRestant.substr(10);
            if (strRestant[0] == tiret_bas)
            {
                dr[i].someFlag = true;
                strRestant = strRestant.substr(1);
            }
            i++;
            continue;
        }
        // mon + mday
        if ((m = std::stoi(strRestant.substr(0, 2))) && checkmonth(m)
            &&
            strRestant[2] == tiret
            &&
            (d = std::stoi(strRestant.substr(3, 2))) && checkday(m, d, firstYear)
            &&
            firstMon < m)
        {
            assert(firstMon < m && L"Le mois aaaaa");
            firstMon = m;
            dr.push_back(DateRecord{ 0 });
            dr[i].date.tm_year = firstYear - 1900;
            dr[i].date.tm_mon = m - 1;
            dr[i].date.tm_mday = d;
            assert(firstDay <= d && L"Le jours aaaaa");
            firstDay = d;
            strRestant = strRestant.substr(5);
            if (strRestant[0] == tiret_bas)
            {
                dr[i].someFlag = true;
                strRestant = strRestant.substr(1);
            }
            i++;
            continue;
        }
        // mday
        if ((d = std::stoi(strRestant.substr(0, 2))) && checkday(firstMon, d, firstYear)
            &&
            firstDay <= d)
        {
            firstDay = d;
            assert(firstDay <= d && L"Le jours aaaaa");
            dr.push_back(DateRecord{ 0 });
            dr[i].date.tm_year = firstYear - 1900;
            dr[i].date.tm_mon = firstMon - 1;
            dr[i].date.tm_mday = d;
            strRestant = strRestant.substr(2);
            if (strRestant[0] == tiret_bas)
            {
                dr[i].someFlag = true;
                strRestant = strRestant.substr(1);
            }
            i++;
            continue;
        }
        // try cath !!!
        // Explique-moi ???
        // ou : 
        exit(1);
    } while (strRestant.length() > 0);

    bool found;
    std::vector<std::wstring> t = lire_fichierTxt(m_cheminFichier.wstring(), { L"\n" }, false);
    unsigned int ee;
    if (t[0] != L"")
        found = false;
    else
    {
        pos = t[0].find(L". ");
        if (std::isdigit(t[0][0]) && pos == std::wstring::npos)
        {
            ee = 0;
        }
        else
        {
            ee = std::stoi(t[0], &pos);
            t[0] = t[0].substr(pos + 2);
        }
        // t
        bool found2 = false;
        pos = t[0].find(L" - ");
        if (pos != std::wstring::npos && !found2)
        {
            t1 = t[0].substr(0, pos);
            t2 = L" : ";
            t3 = t[0].substr(pos + 3);
            found2 = true;
        }
        pos = t[0].find(L" : ");
        if (pos != std::wstring::npos && !found2)
        {
            t1 = t[0].substr(0, pos);
            t2 = L" : ";
            t3 = t[0].substr(pos + 3);
            found2 = true;
        }
        pos = t[0].find(L": ");
        if (pos != std::wstring::npos && !found2)
        {
            t1 = t[0].substr(0, pos);
            t2 = L": ";
            t3 = t[0].substr(pos + 2);
            found2 = true;
        }
        pos = t[0].find(L'/');
        if (pos != std::wstring::npos && !found2)
        {
            t1 = t[0].substr(0, pos);
            t2 = L"/";
            t3 = t[0].substr(pos + 1);
            found2 = true;
        }
        if (pos == std::wstring::npos && !found2)
        {
            t1 = t[0];
            t2 = L"";
            t3 = L"";
            found2 = true;
        }
        bool temps_ = afficher_Temps(t[1]);
        pos = 0;
        tm_temps.tm_min = std::stoi(t[1], &pos);
        found = true;
    }

}
// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::afficher_Dossier(fs::path const& cheminFichier)                                                                                       #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::afficher_Dossier(fs::path const& cheminDossier)
{
    auto nomDossier = cheminDossier.filename().wstring();
    assert(nomDossier.length() > 0 && L"Nom de dossier vide");
    assert(nomDossier.length() > 9 && L"Nom de fichier trop court pour avoir au moins une date");
    std::size_t pos = 0;
    std::wstring wstr = nomDossier.substr(pos);

    wchar_t sp = L' ', tiret = L'-';
    //int y;
    auto y = std::stoi(wstr, &pos);
    assert(1582 <= y && L"L'année aaaaa");
    wstr = wstr.substr(4);
    try
    {
        test_date_tire(wstr[0]);
    }
    catch (exception_date_tiret e2)
    {
        std::wcout << L"Exception a été capturée : " << e2.get_message() << std::endl;
        exit(1);
    }
    wstr = wstr.substr(1);
    auto m = std::stoi(wstr, &pos);
    assert((1 <= m && m <= 12) && L"Le mois aaaaa");
    wstr = wstr.substr(2);
    try
    {
        test_date_tire(wstr[0]);
    }
    catch (exception_date_tiret e2)
    {
        std::wcout << L"Exception a été capturée : " << e2.get_message() << std::endl;
        exit(1);
    }
    wstr = wstr.substr(1);
    auto d = std::stoi(wstr, &pos);
    assert((1 <= d && d <= 31) && L"Le jour aaaaa");
    if (!checkday(m, d, y))
    {
        std::wcout << L"Le jour aaaa !!!" << std::endl;
        exit(1);
    }
    std::tm tm;
    tm.tm_year = y - 1900;
    tm.tm_mon = m - 1;
    tm.tm_mday = d;
    wstr = wstr.substr(2);
    dossier.first = tm;
    dossier.second = wstr;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::afficher_Fichier(fs::path const& cheminFichier)                                                                                       #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::afficher_Fichier(fs::path const& cheminFichier)
{
    auto nomFichier = cheminFichier.filename().wstring();
 
    auto nomImage = cheminFichier.extension().wstring();
    if (nomImage == L".txt")
    {
        if (nomFichier == L"_you_.txt")
        {
            return;
        }
        if (!(std::isdigit(nomFichier[0])))
        {
            // Avec
            if (nomFichier == L"Avec.txt")
            {
                afficher_Avec(cheminFichier);
                return;
            }
            // Chaîne d'origine -
            if (nomFichier == L"Chaîne d'origine.txt")
            {
                //afficher_Chaine(filename, nomFichier, d_chaine[I]);
                return;
            }
            // DVD
            if (nomFichier == L"DVD.txt")
            {
                //D_DVD[I] = true;
                return;
            }
            // Note
            if (nomFichier == L"Note.txt")
            {
                afficher_Note(cheminFichier);
                return;
            }
            // Titre
            if (nomFichier == L"Titre.txt")
            {
                afficher_Titre(cheminFichier);
                return;
            }
        }
        //
        if (
            (nomFichier[0] == L'1' || nomFichier[0] == L'2' || nomFichier[0] == L'3' || nomFichier[0] == L'4' || nomFichier[0] == L'5' || nomFichier[0] == L'6' || nomFichier[0] == L'7' || nomFichier[0] == L'8' || nomFichier[0] == L'9')
            && nomFichier[1] == L'x'
            )
        {
            Creer_Episode(cheminFichier);
            return;
        }
        if (
            (nomFichier[0] == L'1' || nomFichier[0] == L'2' || nomFichier[0] == L'3' || nomFichier[0] == L'4' || nomFichier[0] == L'5' || nomFichier[0] == L'6' || nomFichier[0] == L'7' || nomFichier[0] == L'8' || nomFichier[0] == L'9')
            && (std::isdigit(nomFichier[1]))
            && nomFichier[2] == L'x'
            )
        {
            Creer_Episode(cheminFichier);
            return;
        }
        if (int j = std::stoi(nomFichier))
        {
            afficher(cheminFichier);
            return;
        }
        // Erreur !
        if (nomFichier != L"")
        {
            //E.afficher_X(-1, nomFichier, L'{' + t + L".txt} !!!");
            return;// EXIT_FAILURE;
        }
    }
    else if(nomImage == L".jgp" || nomImage == L".png" || nomImage == L".webp")
        // Image
    {
        afficher_Image(cheminFichier, image);
        return;
    }
    else
    {
    }
    return;
 }

 // ######################################################################################################################################################
 // ######################################################################################################################################################

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::afficher(fs::path const& cheminFichier)                                                                                               #
// #                                                                                                                                                    #
// ######################################################################################################################################################

 void Saison::afficher(fs::path const& cheminFichier)
 {
     auto nomFichier = cheminFichier.filename().wstring();
     assert(nomFichier.length() > 0 && L"Nom de fichier vide");
     std::size_t pos;
     saison.first = std::stoi(nomFichier, &pos);
     try
     {
         saison.second = lire_fichierTxt(cheminFichier.wstring());
     }
     catch (runtime_error const& exception)
     {
         std::wcout << L"Erreur : " << exception.what() << std::endl;
     }
     assert((saison.second.size() != 0));
 }

 // ######################################################################################################################################################
// #                                                                                                                                                    #
// # void afficher_Avec(fs::path const& cheminFichier)                                                                                                  #
// #                                                                                                                                                    #
// ######################################################################################################################################################

 void Saison::afficher_Avec(fs::path const& cheminFichier)
 {
     auto nomFichier = cheminFichier.filename().wstring();
     assert(nomFichier.length() > 0 && L"Nom de fichier vide");
     auto avec = lire_paireCleValeur_depuisFichierTxt(cheminFichier.wstring(), L" : ");
     assert((avec.size() != 0));
 }


// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::Creer_Episode(fs::path const& cheminFichier)                                                                                          #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::Creer_Episode(fs::path const& cheminFichier)
{
    auto nomFichier = cheminFichier.filename().wstring();

    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    //afficher(cheminFichier);// , saison* / ); // saison ???
    Episode episode(cheminFichier);
    //m_episode.push_back(make_tuple(x, e, dr, streaming, found, t1, t2, t3, tm_temps, t[2]));
    //episodes.push_back(episoode);
    episodes.push_back(episode); // Episode
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::afficher_Note(fs::path const& cheminFichier)                                                                                          #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::afficher_Note(fs::path const& cheminFichier)
{ // 0...5 ou -1
    auto nomFichier = cheminFichier.filename().wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");

    //assert((n_filename == createdBy_filename) && L"Erreur !!! Note... !");
    std::wstring n = lire_fichierTxt(cheminFichier.wstring());
    std::size_t pos = n.length();
    if (n == L"")
    {
        note = -1.0;
        return;
    }
    if (pos != 1 && pos != 3)
    {
        //E.afficher_X(-1, n, L"Pas de [a-z] et/ou [0-9] et/ou etc…");
        return;
    }
    if (!iswdigit(n[0]))
    {
        //E.afficher_X(-1, n, L"Pas de [a-z] et/ou etc…");
        return;
    }
    if (!(n[0] == L'0' || n[0] == L'1' || n[0] == L'2' || n[0] == L'3' || n[0] == L'4' || n[0] == L'5'))
    {
        //E.afficher_X(-1, n, L"Pas de [0-5]");
        return;
    }
    if (pos == 1)
    {
        note = std::stod(n);
    }
    else
    {
        if ((n[1] == L',' || n[1] == L'.') && iswdigit(n[2]) && pos == 3)
        {
            //
            n[1] = L',';
            //
            // Ok !!!
            note = std::stod(n);
        }
        else
        {
            //E.afficher_X(-1, n, L"Pas de…");
        }
    }
    return;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::afficher_Titre(fs::path const& cheminFichier)                                                                                         #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::afficher_Titre(std::filesystem::path const& cheminFichier)
{
    auto nomFichier = cheminFichier.filename().wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    titre = lire_fichierTxt(cheminFichier.wstring());
    assert((titre.size() != 0));
}

// ######################################################################################################################################################
// ######################################################################################################################################################

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Serie::Print()                                                                                                                                #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const void Serie::Print()
{
    //std::size_t pos;
    //pos = std::size(saisons);
    PrintSaisons();
}

/*void Serie::Print_Avec(const std::vecor<Avec)
{}*/
// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const void Serie::PrintEpisode_Titre(std::tuple<unsigned int, std::wstring, std::wstring, std::wstring, std::tm, std::wstring>& e_t)               #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const void Serie::PrintEpisode_Titre(std::tuple<unsigned int, std::wstring, std::wstring, std::wstring, std::tm, std::wstring>& e_t)
{
    if (affichage_Episode_Titre_actif)
    {
        std::wstring wstr = get<5>(e_t);
        std::wcout << wstr << std::endl;
    }
}


// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const bool Serie::PrintEpisode_Titre_chiffre_et_point_ou_pas(std::wstring& titre)                                                                  #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const bool Serie::PrintEpisode_Titre_chiffre_et_point_ou_pas(std::wstring& titre)
{
    if (!std::isdigit(titre[0]))
        return false;
    int i = 0;
    while (titre[i] != std::wstring::npos)
    {
        if (std::isdigit(titre[i]))
            ;
        else if (titre[i] == L'.' && i > 0)
        {
            if (titre[i + 1] != std::wstring::npos && titre[i + 1] == L' ')
                break;
            else return false;
        }
        else
            return false;
        i++;
    }
    titre = titre.substr(i + 2);
    return true;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const void Serie::PrintEpisodes(Saison saison)                                                                                                     #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const void Serie::PrintEpisodes(Saison saison)
{
    for (auto ep : saison.episodes)
    {
        ep.afficher();
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const void Serie::PrintSaison(Saison saison)                                                                                                       #
// #                                                                                                                                                    #
// ######################################################################################################################################################

/*const void Serie::PrintSaison(Saison saison)
{
    if (affichage_Saison_actif)
    {
        PrintSaison_Date_etc(saison);
        PrintEpisodes(saison);
        wchar_t date_string[15];
        wcsftime(date_string, 15, L"%d/%m/%Y", &saison.dossier.first);
        std::wstring wstr;
        wstr = date_string;
        wstr = wstr.substr(0, 2) + keyColor[1] + L'/' + valuesColor + wstr.substr(3, 2) + keyColor[1] + L'/' + valuesColor + wstr.substr(6, 4);
        if (saison.dossier.second != L"")
            wstr += keyColor[0] + saison.dossier.second + valuesColor + L' ';
        if (saison.titre != L"")
            wstr += keyColor[1] + L" : " + valuesColor + keyColor[0] + saison.titre + valuesColor;
        wstr += keyColor[1] + L" : " + valuesColor;
        wstr += saison.saison.second;
        wstr += L' ' + keyColor[1] + L'(' + valuesColor + std::to_wstring(saison.saison.first) + keyColor[1] + L')' + valuesColor;
        std::wcout << wstr << std::endl;
        

        //std::size_t taille2;
        //taille2 = std::size(saisons[i].episode);
        //for (int j = 0; j < taille2; j++)
        //{
        //    PrintEpisode();
        //}

        // Avec
        //PrintAvec(saisons[i].avec);

    }
}*/

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const void Serie::PrintSaison_Date_etc()                                                                                                           #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const void Serie::PrintSaison_Date_etc(Saison saison)
{
    if (affichage_Saison_Date_etc_actif)
    {
        wchar_t date_string[15];
        //std::wstring date_string;
        wcsftime(date_string, 15, L"%d/%m/%Y", &saison.dossier.first);
        std::wstring wstr;
        wstr = date_string;
        wstr = wstr.substr(0, 2) + keyColor[1] + L'/' + valuesColor + wstr.substr(3, 2) + keyColor[1] + L'/' + valuesColor + wstr.substr(6, 4);
        if (saison.dossier.second != L"")
            wstr += keyColor[0] + saison.dossier.second + valuesColor + L' ';
        if (saison.titre != L"")
            wstr += keyColor[1] + L" : " + valuesColor + keyColor[0] + saison.titre + valuesColor;
        wstr += keyColor[1] + L" : " + valuesColor;
        wstr += saison.saison.second;
        wstr += L' ' + keyColor[1] + L'(' + valuesColor + std::to_wstring(saison.saison.first) + keyColor[1] + L')' + valuesColor;
        std::wcout << wstr << std::endl;
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const void Serie::PrintSaisons()                                                                                                                   #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const void Serie::PrintSaisons()
{
    if (affichage_Saisons_actif /* && dates.size() > 0*/)
    {
        std::size_t taille;// , taille2;
        //wchar_t date_string[15];
        taille = std::size(saisons);
        //std::wstring saison_str;
        for (int i = 0; i < taille; i++)
        {
            //PrintSaison(saisons[i]);
        }
    }
}
