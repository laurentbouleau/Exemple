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
extern const std::vector<std::pair<std::wstring, std::wstring>>lire_paireCleValeur_depuisFichierTxt(std::wstring const& nomFichier, std::wstring separeteur);
extern const std::wstring lire_fichierTxt(std::wstring const& nomFichier);

extern void test_date_tire(wchar_t d);
extern void test_sp_et_npos_ou_pas_isblank(wchar_t sp, bool t);
extern bool checkyear(int y);
extern bool checkmonth(int m);
extern bool checkday(int m, int d, int y);

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
// # void Saison::afficher_Dossier(fs::path const& cheminFichier)                                                                                       #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::afficher_Dossier(fs::path const& cheminDossier)
{
    auto nomDossier = cheminDossier.filename().wstring();
    std::wcout << L"nomDossier=[" << nomDossier << L']' << std::endl;
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
    dos = tm.tm_year;
    wstr = wstr.substr(2);
    dossier.first = tm;
    dossier.second = wstr;
    std::wcout << L"dossier.first ok !!!" << std::endl;
    std::wcout << L"dossier.second=[" << dossier.second << L']' << L" ok !!!" << std::endl;
    std::wcout << L"Dossier(1)=[" << wstr << L']' << std::endl;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::afficher_Fichier(fs::path const& cheminFichier)                                                                                       #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::afficher_Fichier(fs::path const& cheminFichier)
{
    std::wcout << L"afficher_Fichier cheminFichier=[" << cheminFichier << L']' << std::endl;
    auto nomFichier = cheminFichier.filename().wstring();
    std::wcout << L"nomFichier=[" << nomFichier << L']' << std::endl;

    std::wcout << std::endl;
    std::wcout << std::endl;
    std::wcout << L"===== [" << nomFichier << L"] =====" << std::endl;
    std::wcout << std::endl;
    std::wcout << std::endl;
 
    //auto nomImage = wstring(fs::path(nomFichier).extension());
    auto nomImage = cheminFichier.extension().wstring();
    std::wcout << L"nomImage[" << nomImage << L']' << std::endl;

    if (nomImage == L".txt")
    {
        if (nomFichier == L"_you_.txt")
        {
            std::wcout << L"_you_.txt ok !!!" << std::endl;
            return;
        }
        if (!(std::isdigit(nomFichier[0])))
        {
            // Avec
            if (nomFichier == L"Avec.txt")
            {
                std::wcout << L"----> Avec.txt" << std::endl;
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
            afficher_Episode(cheminFichier);
            return;
        }
        if (
            (nomFichier[0] == L'1' || nomFichier[0] == L'2' || nomFichier[0] == L'3' || nomFichier[0] == L'4' || nomFichier[0] == L'5' || nomFichier[0] == L'6' || nomFichier[0] == L'7' || nomFichier[0] == L'8' || nomFichier[0] == L'9')
            && (std::isdigit(nomFichier[1]))
            && nomFichier[2] == L'x'
            )
        {
            std::wcout << L"Episode !!! :" << std::endl;
            afficher_Episode(cheminFichier);
            return;
        }
        if (int j = std::stoi(nomFichier/*, 0*/))
        {
            auto t2 = std::to_wstring(j);
            std::wcout << L"Saison :" << std::endl;
            afficher_Saison(cheminFichier);
            std::wcout << L"Saison !!! ok !!!" << std::endl;
            return;
        }
        // Erreur !
        if (nomFichier != L"")
        {
            //E.afficher_X(-1, nomFichier, L'{' + t + L".txt} !!!");
            //return EXIT_FAILURE;
        }
    }
    else if(nomImage == L".jgp" || nomImage == L".png" || nomImage == L".webp")
        // Image
    {
        std::wcout << L"Image :" << std::endl;
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
// # void afficher_Avec(fs::path const& cheminFichier)                                                                                                  #
// #                                                                                                                                                    #
// ######################################################################################################################################################

 void Saison::afficher_Avec(fs::path const& cheminFichier)
 {
     auto nomFichier = cheminFichier.filename().wstring();
     std::wcout << L"---> Avec nomFichier=" << nomFichier << std::endl;
     assert(nomFichier.length() > 0 && L"Nom de fichier vide");
     auto avec = lire_paireCleValeur_depuisFichierTxt(cheminFichier.wstring(), L" : ");
     assert((avec.size() != 0));
 }


// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::afficher_Episode(fs::path const& cheminFichier)                                                                                       #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::afficher_Episode(fs::path const& cheminFichier)
{
    std::wcout << L"cheminFichier=[" << cheminFichier << L']' << std::endl;
    auto nomFichier = cheminFichier.filename().wstring();
    std::wcout << L"nomFichier=[" << nomFichier << L']' << std::endl;

    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    std::size_t pos = 0;
//    pos = nomFichier.find_last_of(L"\\");
//    pos++;
    std::wstring strRestant = nomFichier.substr(pos);
    pos = strRestant.length();
    std::wcout << L"strRestant=[" << strRestant << L']' << std::endl;
    strRestant = strRestant.substr(0, pos - 4);
    pos = strRestant.length();
    assert((strRestant.length() < (9 + saison.first + 1)) && L"Nom de fichier trop court pour avoir au moins une date");

    //std::wcout << L"aaa" << std::endl;
    pos = 0;
    int x = std::stoi(strRestant, &pos);
    //if (saison_x == x && x >= 1000)
    if (saison.first == x && x >= 1000)
    {
        std::wcout << L"x <= 1000 !!!" << std::endl;
        exit(1);
    }
    std::wcout << L"x=" << x << std::endl;
    pos = strRestant.find(L"x", 0);
    if (pos == std::wstring::npos)
    {
        std::wcout << L"aaa" << std::endl;
        exit(1);
    }
    strRestant = strRestant.substr(pos + 1);
    if (x >= saison.first)
    {
        std::wcout << L"saison.first != x" << std::endl;
        exit(1);
    }
    pos = strRestant.find(L'.');
    pos++;
    strRestant = strRestant.substr(pos);
    std::wcout << L"strRestant=[" << strRestant << L']' << std::endl;
    std::vector<DateRecord> dr;
    std::wstring streaming = L"";

    wchar_t sp = L' ', tiret = L'-', tiret_bas = L'_';
    int y, m, d;

    int firstYear = 0, firstMon = 0, firstDay = 0;
    int i = 0;
    do
    {
        std::wcout << L"strRestant=[" << strRestant << L']' << std::endl;
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

            std::wcout << L"dr[] ok !!! ----------------" << std::endl;
            std::wcout << L"dr[] ok !!! ----------------" << std::endl;
            std::wcout << L"dr[] ok !!! ----------------" << std::endl;

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
    std::wcout << L"strRestant=[" << strRestant << L']' << std::endl;
    episode.push_back(make_tuple(x, dr, streaming));
    afficher_Episode_Titres(cheminFichier);
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::afficher_Episode_Titres(fs::path const& cheminFichier)                                                                                #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::afficher_Episode_Titres(fs::path const& cheminFichier)
{
    std::wcout << L"cheminFichier=[" << cheminFichier << L']' << std::endl;
    auto nomFichier = cheminFichier.filename();
    std::wcout << L"nomFichier=[" << nomFichier << L']' << std::endl;
    
    std::wcout << L"===> afficher_Date_ou_Dates_Titre() = nomFichier=[" << nomFichier << L']' << std::endl;
    //system("PAUSE");
    auto nf = fs::path(nomFichier).wstring();
    if (nf.length() == 0)
        return;

    std::vector<std::wstring> t = lire_fichierTxt(cheminFichier.wstring(), { L"\n" });
    std::vector<std::wstring>::iterator iter;
    int i;
    for (iter = t.begin(), i = 0; iter != t.end(); iter++, i++)
    {
        std::wcout << L"t[" << i << L"]=[" << *iter << L']' << std::endl;
    }
    auto pos = t[0].find(L". ");
    unsigned int x;
    if (std::isdigit(t[0][0]) && pos == std::wstring::npos)
    {
        x = 0;
        std::wcout << L"t[" << 0 << L"]=[" << t[0] << L']' << std::endl;
    }
    else
    {
        x = std::stoi(t[0], &pos);
        t[0] = t[0].substr(pos + 2);
        std::wcout << L"t[" << 0 << L"]=[" << t[0] << L']' << std::endl;
    }
    // t
    bool found = false;
    std::wstring t1, t2, t3, temps;
    pos = t[0].find(L" - ");
    if (pos != std::wstring::npos && !found)
    {
        t1 = t[0].substr(0, pos);
        t2 = L" : ";
        t3 = t[0].substr(pos + 3);
        found = true;
    }
    pos = t[0].find(L" : ");
    if (pos != std::wstring::npos && !found)
    {
        t1 = t[0].substr(0, pos);
        t2 = L" : ";
        t3 = t[0].substr(pos + 3);
        found = true;
    }
    pos = t[0].find(L": ");
    if (pos != std::wstring::npos && !found)
    {
        t1 = t[0].substr(0, pos);
        t2 = L": ";
        t3 = t[0].substr(pos + 2);
        found = true;
    }
    pos = t[0].find(L'/');
    if (pos != std::wstring::npos && !found)
    {
        t1 = t[0].substr(0, pos);
        t2 = L"/";
        t3 = t[0].substr(pos + 1);
        found = true;
    }
    if (pos == std::wstring::npos && !found)
    {
        t1 = t[0];
        t2 = L"";
        t3 = L"";
        found = true;
    }
    std::wcout << L"x=[" << x << L"]" << std::endl;
    std::wcout << L"t1=[" << t1 << L"]" << std::endl;
    std::wcout << L"t2=[" << t2 << L"]" << std::endl;
    std::wcout << L"t3=[" << t3 << L"]" << std::endl;
    std::wcout << L"found=[" << found << L"]" << std::endl;

    bool temps_ = afficher_Temps(t[1]);
    std::tm tm_temps{ 0 };
    pos = 0;
    tm_temps.tm_min = std::stoi(t[1], &pos);
    episode_titres.push_back(make_tuple(x, t1, t2, t3, tm_temps, t[2]));
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::afficher_Note(fs::path const& cheminFichier)                                                                                          #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::afficher_Note(fs::path const& cheminFichier)
{ // 0...5 ou -1
    std::wcout << L"---> Note" << std::endl;
    std::wcout << L"cheminFichier=[" << cheminFichier << L']' << std::endl;
    auto nomFichier = cheminFichier.filename().wstring();
    std::wcout << L"nomFichier=[" << nomFichier << L']' << std::endl;

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
    std::wcout << L"---> Note=" << note << std::endl;
    return;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::afficher_Saison(fs::path const& cheminFichier)                                                                                        #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::afficher_Saison(fs::path const& cheminFichier)
{
    std::wcout << L"cheminFichier=[" << cheminFichier << L']' << std::endl;
    auto nomFichier = cheminFichier.filename().wstring();
    std::wcout << L"nomFichier=[" << nomFichier << L']' << std::endl;

    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    std::size_t pos = 0;// = nomFichier.find_last_of(L"\\");
    //    auto wstr = nf.substr(pos);
    std::wstring wstr;
    //episodes.first = std::stoi(wstr, &pos);
    saison.first = std::stoi(nomFichier, &pos);
    try
    {
        //wstr = lire_fichierTxt(nomFichier);
        wstr = lire_fichierTxt(cheminFichier.wstring());
    }
    catch (runtime_error const& exception)
    {
        std::wcout << L"Erreur : " << exception.what() << std::endl;
    }

    saison.second = wstr;
    assert((saison.second.size() != 0));
    std::wcout << L"saison.first=[" << saison.first << L"], saison.second=[" << saison.second << L']' << std::endl;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::afficher_Titre(fs::path const& cheminFichier)                                                                                         #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::afficher_Titre(std::filesystem::path const& cheminFichier)
{
    auto nomFichier = cheminFichier.filename().wstring();
    std::wcout << L"---> Titre nomFichier=" << nomFichier << std::endl;
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    auto titre = lire_paireCleValeur_depuisFichierTxt(cheminFichier.wstring(), L" : ");
    assert((titre.size() != 0));
}

// ######################################################################################################################################################
// ######################################################################################################################################################

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Serie::Print()                                                                                                                                #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Serie::Print()
{
    //std::size_t pos;
    //pos = std::size(saisons);
    PrintSaisons();
    std::wcout << L"serie !!!" << std::endl;

}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Serie::PrintEpisode()                                                                                                                         #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Serie::PrintEpisode()
{
    if (affichage_Episode_actif /* && dates.size() > 0*/)
    {
        std::size_t taille;// , taille2;
        wchar_t date_string[15];
        //taille = std::size(date_ou_dates);
        taille = std::size(saisons);
        std::wstring wstr;
        for (int i = 0; i < taille; i++)
        {
        }
    }
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Serie::PrintSaison()                                                                                                                          #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Serie::PrintSaisons()
{
    if (affichage_Saisons_actif /* && dates.size() > 0*/)
    {
        std::size_t taille;// , taille2;
        wchar_t date_string[15];
        //taille = std::size(date_ou_dates);
        taille = std::size(saisons);
        std::wstring saison_str;
        std::wstring wstr;
        for (int i = 0; i < taille; i++)
        {
            //std::pair<std::tm, std::wstring>dossier;
            //wcsftime(date_string, 15, L"%d/%m/%Y", &saisons[i].dossier.first);
            //wcsftime(date_string, 15, L"%d/%m/%Y", &saisons[i].dossier.first);
            //std::wcout << L"saisons[i].dossier.first=" << saisons[i].dossier.first.tm_year << std::endl;
            std::wcout << saisons[i].saison.dos << std::end;
            //wstr = date_string;
            std::wcout << L"saison x" << std::endl;

        }
        //wstr = L"saison x";
    }
}
