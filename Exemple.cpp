#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include "pch.h"
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
//#include <stdio.h>
//#include <locale>
#include <algorithm>
#include <codecvt>
#include <tuple>

#include <filesystem> // C++17 standard header file name
//#include <experimental/filesystem> // Header file for pre-standard implementation
//namespace fs = std::filesystem;
using namespace std;


//using namespace std::experimental::filesystem::v1;

HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
DWORD dwMode = 0;
CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
int X;


std::wstring replace_all(std::wstring subject, const std::wstring& search, const std::wstring& replace)
{
    std::size_t pos = 0;
    // function find return string::npos if not found.
    while ((pos = subject.find(search, pos)) != wstring::npos)
    {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
    return subject;
}

void init()
{
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    GetConsoleScreenBufferInfo(hOut, &csbiInfo);
    _wsetlocale(LC_ALL, L"fr");
    //std::locale("fr_FR.utf8");
    X = csbiInfo.srWindow.Right + 1;
    //
}

static inline void rtrim(std::wstring& s)
{
    s.erase(find_if(s.rbegin(), s.rend(), [](int ch)
        {
            return !isspace(ch);
        }).base(), s.end());
}

std::string wstr_to_u8(std::wstring uneWString)
{
    return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(uneWString);
}

const std::vector<std::wstring> lire_fichierTxt(std::wstring const& nomFichier, std::vector<std::wstring> separeteurs)
{
    std::string contenuFichier{ u8"" };
    std::string ligneCourante{ u8"" };
    std::vector<std::wstring> retVal{};

    ifstream fichier{ nomFichier };
    if (!fichier)
    {
        throw std::runtime_error("Fichier impossible à ouvrir.");
    }

    while (getline(fichier, ligneCourante, u8'\n'))
    {
        contenuFichier += ligneCourante + u8'\n';
    }

    if (contenuFichier == u8"")
    {
        throw std::runtime_error("Le fichier '" + wstr_to_u8(nomFichier) + "' est vide.");
    }
    //wstring_convert<codecvt_utf8_utf16<wchar_t>, wchar_t> convertiseur;

    wstring_convert<codecvt_utf8<wchar_t>, wchar_t> convertiseur;
    std::wstring converti = convertiseur.from_bytes(contenuFichier);
    rtrim(converti);

    bool found = false;

    do
    {
        found = false;
        size_t pos_found = std::string::npos;
        std::wstring sep_found = L"";

        for (auto&& sep : separeteurs)
        {
            std::size_t pos = converti.find(sep);
            if (pos != std::wstring::npos && (!found || pos_found > pos))
            {
                pos_found = pos;
                found = true;
                sep_found = sep;
            }
        }

        if (found)
        {
            retVal.push_back(converti.substr(0, pos_found));
            converti = converti.substr(pos_found + sep_found.length());
        }
    } while (found);

    if (converti.length() > 0)
    {
        retVal.push_back(converti);
    }

    //std::vector<std::wstring>::iterator iter;
    //for (iter = retVal.begin(); iter != retVal.end(); iter++)
    //    std::wcout << L"iter={" << *iter << L'}' << std::endl;
    return retVal;
}

const std::wstring lire_fichierTxt(std::wstring const& nomFichier)
{
    std::string contenuFichier{ u8"" };
    std::string ligneCourante{ u8"" };
    ifstream fichier{ nomFichier };
    if (!fichier)
    {
        throw std::runtime_error("Fichier impossible à ouvrir.");
    }

    while (getline(fichier, ligneCourante, u8'\n'))
    {
        contenuFichier += ligneCourante + u8'\n';
    }

    if (contenuFichier == u8"")
    {
        return L"";
        //throw std::runtime_error("Le fichier '" + wstr_to_u8(nomFichier) + "' est vide.");
    }
    //wstring_convert<codecvt_utf8_utf16<wchar_t>, wchar_t> convertiseur;
    wstring_convert<codecvt_utf8<wchar_t>, wchar_t> convertiseur;
    std::wstring converti = convertiseur.from_bytes(contenuFichier);
    rtrim(converti);
    return converti;
}

void test_sp_et_npos_ou_pas_isblank(wchar_t sp, bool t)
{
    if (sp != L' ' && t)
    {
        throw exception_test_sp_et_npos_ou_pas_isblank();
    }
    return;
}

bool checkyear(int y)
{
    return 1582 <= y;
}

bool checkmonth(int m)
{
    return (1 <= m && m <= 12);
}

bool checkday(int m, int d, int y)
{
    bool retVal = false;

    if (checkyear(y))
    {
        if (checkmonth(m))
        {
            if (d > 0)
            {
                if (d <= 28)
                {
                    retVal = true;
                }
                else if (d == 29)
                {
                    if (m != 2)
                    {
                        retVal = true;
                    }
                    else
                    {
                        retVal = (y % 4 == 0) && ((y % 100 != 0) || (y % 400 == 0));
                    }
                }
                else if (d == 30)
                {
                    retVal = m != 2;
                }
                else if (d == 31)
                {
                    retVal = (m != 2 && m != 4 && m != 6 && m != 9 && m != 11);
                }
            }
        }
    }

    return retVal;
}

const bool afficher_Min(std::wstring& t)
{
    if (
        (t[0] == L'M' && t[1] == L'I' && t[2] == L'N' /* && t[3] == wstring::npos*/)
        ||
        (t[0] == L'M' && t[1] == L'i' && t[2] == L'n' /* && t[3] == wstring::npos*/)
        ||
        (t[0] == L'm' && t[1] == L'i' && t[2] == L'n' /* && t[3] == wstring::npos*/)
        )
    {
    }
    else
    {
        return false;
    }
    return true;
}

const bool afficher_Temps(std::wstring ttt)
// "_h__min" ou "_h __min" ou "_ h __ min" ou "__min" ou "___min" ou "__ min" ou "___ min"
{
    static const basic_string <char>::size_type npos = -1;
    std::wstring t = ttt;
    std::size_t length = t.length();
    bool h = false;
    bool ok = false;
    bool w1 = false;
    // 1
    if (t[0] == npos)
        return false;
    if (std::isdigit(t[0]))
        w1 = true; // oui w1
    else
        return false;
    // 2
    t = t.substr(1);
    if (t[0] == std::wstring::npos)
        return false;
    bool w2 = false;
    bool espace1 = false;
    if (!ok)
    {
        if (w1 && afficher_Min(t))
            return true; // "_min"
        if (w1 && !espace1 && t[0] == L' ')
            espace1 = true;
        if (w1 && !w2 && std::isdigit(t[0]))
            w2 = true; // Oui w2
        if (w1 && !h && (t[0] == L'h' || t[0] == L'H'))
            h = true;
        if (w1 && (!espace1 && t[0] == L' '))
            espace1 = true;
    }
    // 3
    t = t.substr(1);
    if (t[0] == std::wstring::npos)
        return false;
    bool w3 = false;
    if (!ok)
    {
        if (w1 && espace1 && !w2 && afficher_Min(t))
            return true;
        if (w1 == true && w2 == true && afficher_Min(t))
            return true;
        if (w1 && w2 && (!espace1 && t[0] == L' '))
            espace1 = true;
        if (w1 && w2 && !w3 && std::isdigit(t[0]))
            w3 = true;
        if (w1 && h && std::isdigit(t[0]))
            w3 = true;
        if (w1 && h && (!espace1 && t[0] == L' '))
            espace1 = true;
        if (w1 && espace1 && !h && (t[0] == L'h' || t[0] == L'H'))
            h = true;
    }
    // 4
    t = t.substr(1);
    if (t[0] == std::wstring::npos)
        return false;
    bool w4 = false;
    bool espace2 = false;
    if (!ok)
    {
        if (w1 && w2 && espace1 && !w3 && afficher_Min(t))
            return true;
        if (w1 && w2 && w3 && afficher_Min(t))
        {
            if (ttt[0] == L'1')
                return true;
            else
                return false;
        }
        if (w1 && w2 && w3 && (!espace1 && t[0] == L' '))
            espace1 = true;
        if (w1 && h && w3 && std::isdigit(t[0]))
            w4 = true;
        if (w1 && h && espace1 && !w4 && std::isdigit(t[0]))
            w4 = true;
        if (w1 && espace1 && h && (!espace2 && t[0] == L' '))
            espace2 = true;
    }
    // 5
    t = t.substr(1);
    if (t[0] == std::wstring::npos)
        return false;
    bool w5 = false;
    if (!ok)
    {
        if (w1 && w2 && w3 && espace1 && afficher_Min(t))
        {
            if (ttt[0] == L'1')
                return true;
            else
                return false;
        }
        if (w1 == true && h == true && w3 == true && w4 == true && afficher_Min(t))
        {
            if (ttt[2] == L'6' || ttt[2] == L'7' || ttt[2] == L'8' || ttt[2] == L'9')
                return false;
            else
                return true;
        }
        if (w1 == true && h == true && espace1 == true && w4 == true && w5 == false && std::isdigit(t[0]))
            w5 = true;
        if (w1 == true && espace1 == true && h == true && espace2 == true && w5 == false && std::isdigit(t[0]))
            w5 = true;
    }
    // 6
    t = t.substr(1);
    if (t[0] == std::wstring::npos)
        return false;
    bool w6 = false;
    if (!ok)
    {
        if (w1 && h && espace1 && w4 && w5 && afficher_Min(t))
        {
            if (ttt[3] == L'6' || ttt[3] == L'7' || ttt[3] == L'8' || ttt[3] == L'9')
                return false;
            else
                return true;
        }
        if (w1 && espace1 && h && espace2 && w5 && !w6 && std::isdigit(t[0]))
            w6 = true;
    }
    // 7
    t = t.substr(1);
    if (t[0] == std::wstring::npos)
        return false;
    bool espace3 = false;
    if (!ok)
    {
        if (w1 && espace1 && h && espace2 && w5 && w6 && (!espace3 && t[0] == L' '))
            espace3 = true;
    }
    // 8
    t = t.substr(1);
    if (!ok)
    {
        if (w1 && espace1 && h && espace2 && w5 && w6 && espace3 && afficher_Min(t))
        {
            if (ttt[4] == L'6' || ttt[4] == L'7' || ttt[4] == L'8' || ttt[4] == L'9')
                return false;
            else
                return true;
        }
    }
    return false;
}
//using namespace std::literals;




Saison::Saison() 
{
    saison = 0;
}
Saison::~Saison()
{}

Serie::Serie()
{}
Serie::~Serie()
{}

/*void afficher_Date_ou_Dates(std::wstring const& nomFichier, std::vector<std::pair<std::vector<DateRecord>, std::wstring>>& dates)
{
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");

    std::size_t pos;
    pos = nomFichier.length();
    std::wstring strRestant = nomFichier.substr(0, pos - 4);
    assert(strRestant.length() > 9 && L"Nom de fichier trop court pour avoir au moins une date");

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
    dates.push_back(make_pair(dr, streaming));
}*/


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
void Saison::afficher_Episodes(std::wstring const& nomFichier)
{
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");

    std::wcout << L"zzz" << std::endl;

    std::size_t pos;
    pos = nomFichier.find_last_of(L"\\");
    pos++;
    std::wstring wstr = nomFichier.substr(pos);

    episodes.first = std::stoi(wstr, &pos);
    std::wcout << L"episodes.first=[" << episodes.first << L']' << std::endl;
    wstr = L"";
    try
    {
        wstr = lire_fichierTxt(nomFichier);
    }
    catch (runtime_error const& exception)
    {
        std::wcout << L"Erreur : " << exception.what() << std::endl;
    }

    //assert((e_filename == createdBy_filename) && L"Erreur !!! Episodes... !");
    episodes.second = wstr;
    assert((episodes.second.size() != 0));
    std::wcout << L"episodes.first=[" << episodes.first << L"], episodes.second=[" << episodes.second << L']' << std::endl;
}

void Saison::afficher_Titres(std::wstring const& nomFichier)
{
    if (nomFichier.length() == 0)
        return;

    std::wcout << L"zzz" << std::endl;

    std::vector<std::wstring> t = lire_fichierTxt(nomFichier, { L"\n" });
    std::vector<std::wstring>::iterator iter;
    int i;
    for (iter = t.begin(), i = 0; iter != t.end(); iter++, i++)
    {
        std::wcout << L"t[" << i << L"]=[" << *iter << L']' << std::endl;
    }
    std::size_t pos;
    //pos = nomFichier.find_last_of(L"\\");
    //pos++;
    pos = t[0].find(L". ");
    //std::isdigit(d[2])
    unsigned int x;
    if (std::isdigit(t[0][0]) && pos == std::wstring::npos)
    {
        x = 0;
        std::wcout << L"t[" << 0 << L"]=[" << t[0] << L']' << std::endl;
        //titres.push_back(make_pair(0, t));
    }
    else
    {
        x = std::stoi(t[0], &pos);
        t[0] = t[0].substr(pos + 2);
        std::wcout << L"t[" << 0 << L"]=[" << t[0] << L']' << std::endl;
        //titres.push_back(make_pair(x, t));
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
        //wcout << L"t={" << t << L'}' << endl;
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
    //                               x           t1             t2           t3       temps      p             s
    //std::vector<std::tuple<unsigned int, std::wstring, std::wstring, std::wstring, std::tm, std::wstring, std::wstring>> titres;
    std::wcout << L"xxx" << std::endl;
    //titres.push_back(make_tuple(

        //x, 
        //t1, 
        //t2, 
        //t3,
        //std::tm{0},
        //t[2]));// , L"2"));

    //std::tuple<unsigned int, std::wstring, std::wstring, std::wstring, std::wstring> tup;
    //tup = make_tuple(x, t1, t2, t3, t[2]);

    //std::wcout << L"x=[" << get<0>(tup) << L"]" << std::endl;
    //std::wcout << L"t1=[" << get<1>(tup) << L"]" << std::endl;
    //std::wcout << L"t2=[" << get<2>(tup) << L"]" << std::endl;
    //std::wcout << L"t3=[" << get<3>(tup) << L"]" << std::endl;
    //std::wcout << L"t[2]=[" << get<4>(tup) << L"]" << std::endl;
    //std::wcout << L"yyy" << std::endl;
    bool temps_ = afficher_Temps(t[1]);
    std::tm tm_temps{ 0 };
    pos = 0;
    tm_temps.tm_min = std::stoi(t[1], &pos);
    titres.push_back(make_tuple(x, t1, t2, t3, tm_temps, t[2]));
    //titres.push_back({ tup });
    //titre[0]
    std::wcout << L"xxx" << std::endl;
}


void Saison::afficher_Date_ou_Dates(std::wstring const& nomFichier)//, std::vector<std::tuple<unsigned int, std::vector<DateRecord>, std::wstring>>& date_ou_dates)
{
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");

    std::size_t pos;
    pos = nomFichier.find_last_of(L"\\");
    pos++;
    std::wstring strRestant = nomFichier.substr(pos);
    pos = strRestant.length();
    strRestant = strRestant.substr(0, pos - 4);
    pos = strRestant.length();
    assert((strRestant.length() > (9 + episodes.first + 1)) && L"Nom de fichier trop court pour avoir au moins une date");

    std::wcout << L"aaa" << std::endl;
    pos = 0;
    int x = std::stoi(strRestant, &pos);
    if (saison == x && x >= 1000)
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
    if (x >= episodes.first)
    {
        std::wcout << L"episodes.first != x" << std::endl;
        exit(1);
    }
    pos = strRestant.find(L'.');
    pos++;
    strRestant = strRestant.substr(pos);
    std::wcout << L"strRestant=[" << strRestant << L']' << std::endl;
    std::wcout << L"rrr" << std::endl;


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
    date_ou_dates.push_back(make_tuple(x, dr, streaming));
    afficher_Titres(nomFichier); 
}

void Saison::ok(std::wstring const& nomFichier)
{
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");

    std::size_t pos;
    pos = nomFichier.length();
    std::wstring strRestant = nomFichier.substr(0, pos - 4);
    assert(strRestant.length() > 9 && L"Nom de fichier trop court pour avoir au moins une date");

    pos = 0;
    saison = std::stoi(strRestant, &pos);
    if (saison >= 1000)
    {
        std::wcout << L"saison <= 1000 !!!" << std::endl;
        exit(1);
    }
    std::wcout << L"saison=" << saison << std::endl;
}

void afficher()
{

}

int wmain(int argc, wchar_t* argv[])
{

    int i = 0;
    //
    init();
    //hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
    {
        return GetLastError();
    }
    //    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode))
    {
        return GetLastError();
    }
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode))
    {
        return GetLastError();
    }

    //    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    GetConsoleScreenBufferInfo(hOut, &csbiInfo);
    const std::wstring nomFichier = L"e:\\Séries.[]\\Gate.[2015 Netflix].Manga\\2015-07-03\\24.txt";
    const std::wstring nomFichier3 = L"e:\\Séries.[]\\Gate.[2015 Netflix].Manga\\2015-07-03\\1x01.2023-12-05.txt";
    //const std::wstring nomFichier3 = L"e:\\Séries.[]\\La Créature de Kyongsong.[2023 Netflix]\\2023-12-22\\1x01.2023-12-28.txt";
    const std::wstring nomFichier6 = L"1x1.2022-08-31_ Netflix.txt"; /// 
    const std::wstring nomFichier7 = L"1x2.2023-11-28_29_30_12-30_ abc.txt";
    const std::wstring nomFichier8 = L"1x3.2022-08-30_31.txt";
    const std::wstring nomFichier9 = L"1x4.2022-08-30_31 Netflix.txt";
    //std::vector<std::tuple<unsigned int, std::vector<DateRecord>, std::wstring>>date_ou_dates;
    //std::size_t pos;

    const wchar_t* p{ L"../" };
    path filePath(p);

    filePath /= L"Exemple.txt";
    std::wstring t;
    try
    {
        // Dans le try, on est assuré que toute exception levée
        // pourra être traitée dans le bloc catch situé après.
        t = lire_fichierTxt(L"you.txt");
    }
    // Notez qu'une exception s'attrape par référence constante.
    catch (runtime_error const& exception)
    {
        // On affiche la cause de l'exception.
        std::wcout << L"Erreur : " << exception.what() << std::endl;
    }

    Serie serie;
    Saison saison;
    std::wcout << L"ttt" << std::endl;
    serie.afficher();
    serie.saison[0].afficher_Episodes(nomFichier);
    std::wcout << L"ttt" << std::endl;
    //pos = nomFichier3.find_last_of(L"\\");
    //pos++;
    //wstr = nomFichier3.substr(pos);
    //saison.ok(nomFichier3);
    saison.afficher_Date_ou_Dates(nomFichier3);

    std::size_t s = saison.titres.size(); //to get the size of the vector
    std::tm j;
    for (int i = 0; i < s; i++)
    { //to print the elements stored in vector of tuples, vec
        std::wcout << i << std::endl;
        std::wcout << L"__x    =" << get<0>(saison.titres[i]) << std::endl;
        std::wcout << L"__t1   =[" << get<1>(saison.titres[i]) << L"]" << std::endl;
        std::wcout << L"__t2   =[" << get<2>(saison.titres[i]) << L"]" << std::endl;
        std::wcout << L"__t3   =[" << get<3>(saison.titres[i]) << L"]" << std::endl;
        j = get<4>(saison.titres[i]);
        std::wcout << L"__temps=[" << j.tm_hour*1 + j.tm_min << L"]" << std::endl;
        std::wcout << L"__p    =[" << get<5>(saison.titres[i]) << L"]" << std::endl;
    }
    return EXIT_SUCCESS;
}

/*int wmain(int argc, wchar_t* argv[])
{

    int i = 0;
    //
    init();
    //hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
    {
        return GetLastError();
    }
    //    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode))
    {
        return GetLastError();
    }
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode))
    {
        return GetLastError();
    }

    //    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    GetConsoleScreenBufferInfo(hOut, &csbiInfo);

    const std::wstring nomFichier1 = L"1024-08-31.txt"; // Erreur
    const std::wstring nomFichier2 = L"1624/08-31.txt"; // Erreur

    const std::wstring nomFichier3 = L"2022-08-31.txt";

    const std::wstring nomFichier4 = L"2022-08-31 .txt"; // Erreur
    const std::wstring nomFichier5 = L"2022-08-31Net.txt"; // Erreur

    const std::wstring nomFichier6 = L"2022-08-31_ Netflix.txt"; /// 
    const std::wstring nomFichier7 = L"2023-11-28_29_30_12-30_ abc.txt";
    const std::wstring nomFichier8 = L"2022-08-30_31.txt";
    const std::wstring nomFichier9 = L"2022-08-30_31 Netflix.txt";
    
    const std::wstring nomFichier10 = L"2022-08-30_31_2023-01-13 Netflix.txt";
    const std::wstring nomFichier11 = L"2022-08-31_30_2023-01-13 Netflix.txt"; // Pas ok !!!
    const std::wstring nomFichier12 = L"2023-08-30_31_2022-01-13 Netflix.txt"; // Pas ok !!!

    const std::wstring nomFichier13 = L"2023-08-30_09-01 Netflix.txt";

    const std::wstring nomFichier14 = L"2023-08-30_31_09-01_02 Netflix.txt";
    const std::wstring nomFichier15 = L"2023-08-30_09-12_12 Netflix.txt";
    const std::wstring nomFichier16 = L"2023-08-30_30_30_30_30_30_30_30_30_30_30_31_09-01_01_01_01_ Netflix.txt";
    
    std::vector<std::pair<std::vector<DateRecord>, std::wstring>>dates;

    afficher_Date_ou_Dates(nomFichier3, dates);
    afficher_Date_ou_Dates(nomFichier6, dates);
    afficher_Date_ou_Dates(nomFichier7, dates);
    afficher_Date_ou_Dates(nomFichier10, dates);
    afficher_Date_ou_Dates(nomFichier13, dates);
    afficher_Date_ou_Dates(nomFichier14, dates);
    afficher_Date_ou_Dates(nomFichier15, dates);
    afficher_Date_ou_Dates(nomFichier16, dates);

    std::size_t taille, taille2;
    taille = std::size(dates);
    //std::wcout << std::endl;
    std::wcout << L"Date(s) :" << std::endl;
    for (i = 0; i < taille; i++)
    {
        std::wcout << L"__" << i << std::endl;
        taille2 = std::size(dates[i].first);
        for (int j = 0; j < taille2; j++)
        {
            std::wcout << L"____" << j << std::endl;
            std::wcout << L"______date=[" << dates[i].first[j].date.tm_year + 1900 << L'/' << dates[i].first[j].date.tm_mon + 1 << L'/' << dates[i].first[j].date.tm_mday << L']' << std::endl;
            std::wcout << L"______someFlag=" << dates[i].first[j].someFlag << std::endl;
        }
        std::wcout << L"_____streaming=[" << dates[i].second << L"]" << std::endl;
    }
    std::wcout << std::endl;
    PrintDate_ou_Dates(dates);
    return EXIT_SUCCESS;
}*/

