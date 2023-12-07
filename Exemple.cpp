#include "pch.h"

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


#include <filesystem> // C++17 standard header file name
//#include <experimental/filesystem> // Header file for pre-standard implementation
//namespace fs = std::filesystem;

using namespace std;
//using namespace std::experimental::filesystem::v1;

HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
DWORD dwMode = 0;
CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
int X;

void init()
{
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    GetConsoleScreenBufferInfo(hOut, &csbiInfo);
    _wsetlocale(LC_ALL, L"fr");
    //std::locale("fr_FR.utf8");
    X = csbiInfo.srWindow.Right + 1;
    //
}

class exception_test_sp_et_npos_ou_pas_isblank
{
    std::wstring message;
public:
    exception_test_sp_et_npos_ou_pas_isblank() : message(L"Espace invalide !!!") {}
    std::wstring get_message() const { return message; }
};

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

struct DateRecord
{
    std::tm date{ 0 };
    bool someFlag{ false };
};

std::pair<std::vector<DateRecord>, std::wstring> ExtraireInfosDepuisNomDeFichier(const std::wstring& nomFichier)
{
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");

    std::size_t pos;
    pos = nomFichier.length();
    std::wstring strRestant = nomFichier.substr(0, pos - 4);
    assert(strRestant.length() > 9 && L"Nom de fichier trop court pour avoir au moins une date");

    std::vector<DateRecord> dr;
    std::wstring streaming = L"";

    //DateRecord dr;
    //std::tm date tm;
    //tm.tm_year = y - 1900;
    //tm.tm_mon = m - 1;
    //tm.tm_mday = d;
    //dr.date = tm;
    //dr.someFlag = false;
    //dates.push_back(dr);

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
                exit(1);
            }
            strRestant = strRestant.substr(1);
            // try cath !!!
            // Explique-moi ??? 
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
        exit(1);
    } while (strRestant.length() > 0);
    return std::pair<std::vector<DateRecord>, std::wstring>(dr, streaming);
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

    const std::wstring nomFichier1 = L"1024-08-31.txt"; // Erreur
    const std::wstring nomFichier2 = L"1624/08-31.txt"; // Erreur

    const std::wstring nomFichier3 = L"2022-08-31.txt";

    const std::wstring nomFichier4 = L"2022-08-31 .txt"; // Erreur
    const std::wstring nomFichier5 = L"2022-08-31Net.txt"; // Erreur

    const std::wstring nomFichier6 = L"2022-08-31_ Netflix.txt"; /// 
    const std::wstring nomFichier7 = L"2023-11-28_29_30_12-30 abc.txt";
    const std::wstring nomFichier8 = L"2022-08-30_31.txt";
    const std::wstring nomFichier9 = L"2022-08-30_31 Netflix.txt";
    
    const std::wstring nomFichier10 = L"2022-08-30_31_2023-01-13 Netflix.txt";
    const std::wstring nomFichier11 = L"2022-08-31_30_2023-01-13 Netflix.txt"; // Pas ok !!!
    const std::wstring nomFichier12 = L"2023-08-30_31_2022-01-13 Netflix.txt"; // Pas ok !!!

    const std::wstring nomFichier13 = L"2023-08-30_09-01 Netflix.txt";

    const std::wstring nomFichier14 = L"2023-08-30_31_09-01_02 Netflix.txt";
    const std::wstring nomFichier15 = L"2023-08-30_09-12_12 Netflix.txt";
 
    std::pair<std::vector<DateRecord>, std::wstring>dates;
    std::vector<std::pair<std::vector<DateRecord>, std::wstring>>dates_pla_pla_pla;
    dates = ExtraireInfosDepuisNomDeFichier(nomFichier14);
    dates_pla_pla_pla.push_back(dates);
    dates = ExtraireInfosDepuisNomDeFichier(nomFichier3);
    dates_pla_pla_pla.push_back(dates);
    dates = ExtraireInfosDepuisNomDeFichier(nomFichier7);
    dates_pla_pla_pla.push_back(dates);
    std::size_t taille, taille2;
    taille = std::size(dates_pla_pla_pla);
    std::wcout << std::endl;
    std::wcout << L"dates_pla_pla_pla :" << std::endl;
    for (i = 0; i < taille; i++)
    {
        std::wcout << L"__" << i << std::endl;
        taille2 = std::size(dates_pla_pla_pla[i].first);
        for (int j = 0; j < taille2; j++)
        {
            std::wcout << L"____" << j << std::endl;
            std::wcout << L"______date=[" << dates_pla_pla_pla[i].first[j].date.tm_year + 1900 << L'/' << dates_pla_pla_pla[i].first[j].date.tm_mon + 1 << L'/' << dates_pla_pla_pla[i].first[j].date.tm_mday << L']' << std::endl;
            std::wcout << L"______someFlag=" << dates_pla_pla_pla[i].first[j].someFlag << std::endl;
        }
        std::wcout << L"_____streaming=[" << dates_pla_pla_pla[i].second << L"]" << std::endl;
    }
    return EXIT_SUCCESS;
}
