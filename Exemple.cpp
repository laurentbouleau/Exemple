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

class exception_sp
{
    std::wstring message;
public:
    exception_sp() : message(L"aaaaaaaaaa") {}
    std::wstring get_message() const { return message; }
};

void test_sp(wchar_t d)
{
    if (d != L' ')
    {
        throw exception_sp();
    }
    return;
}


/*class exception_test_sp_et_npos_ou_pas_isblank
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
}*/


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
    ;
    ;
    ;
    int firstYear = 0, firstMon = 0, firstDay = 0;
    std::size_t taille{};
    int i = 0;
    do
    {
        std::wcout << L"strRestant=[" << strRestant << L"]" << std::endl;
        if (i == 10)
            break;
        if (strRestant[0] == sp)
        {
            std::wcout << L"strRestant=[" << strRestant << L"] ???" << std::endl;
            strRestant = strRestant.substr(1);
            std::wcout << L"strRestant=[" << strRestant << L"]" << std::endl;
            // date_sp

            /*try
            {
                test_sp_et_npos_ou_pas_isblank(strRestant[0], isblank(strRestant[1]));
            }
            catch (exception_test_sp_et_npos_ou_pas_isblank e)
            {
                std::wcout << L"Exception a été capturée : " << e.get_message() << std::endl;
                exit(1);
            }*/
            /*try
            {
                test_sp(strRestant[0]);
            }
            catch (exception_test_sp e)
            {
                std::wcout << L"Exception a été capturée : " << e.get_message() << std::endl;
                exit(1);
            }*/
            assert(strRestant[0] == sp && L"sp !!!");
            if (strRestant[0] != wstring::npos)
            {
                streaming = strRestant;
            }
            strRestant = L"";
            break;
        }
        //else if (!isblank(strRestant[0]))
        //{
        //    std::wcout << L"Erreur [" << strRestant << L"] !!!" << std::endl;
        //    exit(1);
        //}
        //else if (
        //    iswblank(strRestant[0])
        //    )
        //{
        //    std::wcout << L"a-zA-Z0-9" << std::endl;
        //    assert(iswblank(strRestant[0]) && L"isalnum pla pla pla !!!");
        //}
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
            firstYear < y
            )
        {
            std::wcout << L"year + mon + mday" << std::endl;
            assert(firstYear < y && L"L'année aaaaa");
            firstYear = y;
            assert(firstMon < m && L"Le mois aaaaa");
            firstMon = m;
            assert(firstDay <= d && L"Le jours aaaaa");
            firstDay = d;
            std::wcout << L"y ?" << std::endl;
            //taille = std::size(dates);
            //std::wcout << L"taille=" << taille << std::endl;
            dr.push_back(DateRecord{ 0 });
            dr[i].date.tm_year = y - 1900;
            dr[i].date.tm_mon = m - 1;
            dr[i].date.tm_mday = d;
            taille = std::size(dr);
            //std::wcout << L"taille=" << taille << std::endl;
            std::wcout << L"i=" << i << std::endl;
            strRestant = strRestant.substr(10);
            std::wcout << L"  dr[" << i << L"].date=" << dr[i].date.tm_year + 1900 << L'/' << dr[i].date.tm_mon + 1 << L'/' << dr[i].date.tm_mday << std::endl;
            std::wcout << L"  dr[" << i << L"].someFlag=" << dr[i].someFlag << std::endl;
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
            //firstYear == y //&&
            firstMon < m
            )
        {
            std::wcout << L"mon + mday" << std::endl;
            assert(firstMon < m && L"Le mois aaaaa");
            firstMon = m;
            std::wcout << L"m ?" << std::endl;
            dr.push_back(DateRecord{ 0 });
            dr[i].date.tm_year = firstYear - 1900;
            dr[i].date.tm_mon = m - 1;
            dr[i].date.tm_mday = d;
            assert(firstDay <= d && L"Le jours aaaaa");
            firstDay = d;
            strRestant = strRestant.substr(5);
            //std::wcout << L"taille=" << taille << std::endl;
            std::wcout << L"i=" << i << std::endl;
            std::wcout << L"  dr[" << i << L"].date=" << dr[i].date.tm_year + 1900 << L'/' << dr[i].date.tm_mon + 1 << L'/' << dr[i].date.tm_mday << std::endl;
            std::wcout << L"  dr[" << i << L"].someFlag=" << dr[i].someFlag << std::endl;
            if (strRestant[0] == tiret_bas)
            {
                dr[i].someFlag = true;
                strRestant = strRestant.substr(1);
            }
            i++;
            continue;
        }
        // mday
        if (
            /*firstYear != 0 &&*/
            (d = std::stoi(strRestant.substr(0, 2))) && checkday(firstMon, d, firstYear)
            &&
            firstDay <= d
            )
        {
            std::wcout << L"mday" << std::endl;
            //assert(firstYear < y && L"L'année aaaaa");
            //assert(firstMon < m && L"Le mois aaaaa");
            //if(firstYear < y && firstMon < m)
            firstDay = d;
            assert(firstDay <= d && L"Le jours aaaaa");
            std::wcout << L"d ?" << std::endl;
            dr.push_back(DateRecord{ 0 });
            dr[i].date.tm_year = firstYear - 1900;
            dr[i].date.tm_mon = firstMon - 1;
            dr[i].date.tm_mday = d;
            strRestant = strRestant.substr(2);
            //std::wcout << L"taille=" << taille << std::endl;
            std::wcout << L"i=" << i << std::endl;
            std::wcout << L"  dr[" << i << L"].date=" << dr[i].date.tm_year + 1900 << L'/' << dr[i].date.tm_mon + 1 << L'/' << dr[i].date.tm_mday << std::endl;
            std::wcout << L"  dr[" << i << L"].someFlag=" << dr[i].someFlag << std::endl;
            if (strRestant[0] == tiret_bas)
            {
                dr[i].someFlag = true;
                strRestant = strRestant.substr(1);
            }
            i++;
            continue;
        }
        else
        {
            std::wcout << L"??? strRestant=[" << strRestant << L"]" << std::endl;
            std::wcout << L"firstYear=" << firstYear << L", firstMon=" << firstMon << L", firstDay=" << firstDay << std::endl;
            std::wcout << L"y=" << y << L", m=" << m << L", d=" << d << std::endl;
            std::wcout << L"Erreur !!!" << std::endl;
            exit(1);
            }
        i++;
    } while (strRestant.length() > 0);
    return std::pair<std::vector<DateRecord>, std::wstring>(dr, streaming);
}
/*std::pair<std::vector<DateRecord>, std::wstring> ExtraireInfosDepuisNomDeFichier(const std::wstring& nomFichier)
{
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");

    std::size_t pos;
    pos = nomFichier.length();
    std::wstring strRestant = nomFichier.substr(0, pos - 4);
    assert(strRestant.length() > 9 && L"Nom de fichier trop court pour avoir au moins une date");

    std::vector<DateRecord> dates;
    std::wstring streaming = L"";

    int y, m, d;
    wchar_t sp = L' ', tiret = L'-', tiret_bas = L'_';
    bool yearFlag = false, monFlag = false, dayFlag = false;
    int firstYear = 0, firstMon = 0, firstDay = 0;

    DateRecord dr;
    std::tm dateFichier;

    y = stoi(strRestant.substr(0, 4));
    m = std::stoi(strRestant.substr(5, 2));
    d = std::stoi(strRestant.substr(8, 2));

    dateFichier.tm_year = y - 1900;
    dateFichier.tm_mon = m - 1;
    dateFichier.tm_mday = d;

    dr.date = dateFichier;
    dr.someFlag = false;
    std::wcout << L"aaa" << std::endl;

    dates.push_back(dr);
    int i = 0;
    std::size_t taille{};
    do
    {
        std::wcout << L"strRestant=[" << strRestant << L"]" << std::endl;
        if (i == 10)
            break;
        if (strRestant[0] == sp)
        {
            std::wcout << L"strRestant=[" << strRestant << L"] ???" << std::endl;
            strRestant = strRestant.substr(1);
            std::wcout << L"strRestant=[" << strRestant << L"]" << std::endl;
            // date_sp
            try
            {
                test_sp_et_npos_ou_pas_isblank(strRestant[0], !isblank(strRestant[1]));
            }
            catch (exception_test_sp_et_npos_ou_pas_isblank e)
            {
                std::wcout << L"Exception a été capturée : " << e.get_message() << std::endl;
                exit(1);
            }
            if (strRestant[0] != wstring::npos)
            {
                streaming = strRestant;
            }
            strRestant = L"";
        }
        // year + mon + day
        if (strRestant[0] != tiret_bas &&
            !yearFlag && !monFlag && !dayFlag &&
            checkyear(y) &&
            strRestant[4] == tiret &&
            checkmonth(m) &&
            strRestant[7] == tiret &&
            checkday(m, d, y) &&
            firstYear < y)
        {
            std::wcout << L"year + mon + day :" << std::endl;
            assert(firstYear < y && L"L'année aaaaa");
            firstYear = y;
            assert(firstMon < m && L"Le mois aaaaa");
            firstMon = m;
            assert(firstDay <= d && L"Le jours aaaaa");
            firstDay = d;
            std::wcout << L"y ?" << std::endl;
            //taille = std::size(dates);
            //std::wcout << L"taille=" << taille << std::endl;
            taille = std::size(dates);
            std::wcout << L"taille=" << taille << std::endl;
            std::wcout << L"i=" << i << std::endl;
            strRestant = strRestant.substr(10);
            std::wcout << L"  dates[" << i << L"].date=" << dates[i].date.tm_year + 1900 << L'/' << dates[i].date.tm_mon + 1 << L'/' << dates[i].date.tm_mday << std::endl;
            std::wcout << L"  dates[" << i << L"].someFlag=" << dates[i].someFlag << std::endl;
            if (strRestant[0] == tiret_bas)
            {
                dates[i].someFlag = true;
                std::wcout << L"  dates[" << i << L"].someFlag=" << dates[i].someFlag << std::endl;
                strRestant = strRestant.substr(1);
            }
            yearFlag = true; monFlag = true; dayFlag = true;
        }
        // year + mon - day
        if (strRestant[0] != tiret_bas &&
            !yearFlag && !monFlag && !dayFlag &&
            //checkyear(y) &&
            strRestant[4] == tiret &&
            checkmonth(m) &&
            strRestant[7] == tiret &&
            checkday(m, d, y) &&
            firstMon < m)
        {
            std::wcout << L"mon + day :" << std::endl;
            assert(firstMon < m && L"Le mois aaaaa");
            firstMon = m;
            assert(firstDay <= d && L"Le jours aaaaa");
            firstDay = d;
            std::wcout << L"y ?" << std::endl;
            //taille = std::size(dates);
            //std::wcout << L"taille=" << taille << std::endl;
            taille = std::size(dates);
            std::wcout << L"taille=" << taille << std::endl;
            std::wcout << L"i=" << i << std::endl;
            strRestant = strRestant.substr(10);
            std::wcout << L"  dates[" << i << L"].date=" << dates[i].date.tm_year + 1900 << L'/' << dates[i].date.tm_mon + 1 << L'/' << dates[i].date.tm_mday << std::endl;
            std::wcout << L"  dates[" << i << L"].someFlag=" << dates[i].someFlag << std::endl;
            if (strRestant[0] == tiret_bas)
            {
                dates[i].someFlag = true;
                std::wcout << L"  dates[" << i << L"].someFlag=" << dates[i].someFlag << std::endl;
                strRestant = strRestant.substr(1);
            }
            yearFlag = true; monFlag = true; dayFlag = true;
        }
        std::wcout << L"Ok : [" << strRestant << L"]" << std::endl;
        i++;

    } while (strRestant.length() > 0);
    if (!yearFlag || !monFlag || !dayFlag)
    {
        std::wcout << L"zzzz" << std::endl;
        dates.clear();
    }
    return std::pair<std::vector<DateRecord>, std::wstring>(dates, streaming);
}*/
int wmain(int argc, wchar_t* argv[])
{

    int i;
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


    //const std::wstring nomFichier = L"1024-08-31.txt";
    //const std::wstring nomFichier = L"1624/08-31.txt";

    //const std::wstring nomFichier = L"2022-08-31.txt";

    //const std::wstring nomFichier = L"2022-08-31 .txt";
    //const std::wstring nomFichier = L"2022-08-31Net.txt";

    //const std::wstring nomFichier = L"2022-08-31 Netflix.txt";
    //const std::wstring nomFichier = L"2022-08-31_ Netflix.txt";
    const std::wstring nomFichier = L"2023-11-28_29_30_12-30 abc.txt";
    //const std::wstring nomFichier = L"2022-08-30_31.txt";
    //const std::wstring nomFichier = L"2022-08-30_31 Netflix.txt";
    
    //const std::wstring nomFichier = L"2022-08-30_31_2023-01-13 Netflix.txt";
    //const std::wstring nomFichier = L"2022-08-31_30_2023-01-13 Netflix.txt"; // Pas ok !!!
    //const std::wstring nomFichier = L"2023-08-30_31_2022-01-13 Netflix.txt"; // Pas ok !!!

    //const std::wstring nomFichier = L"2023-08-30_09-01 Netflix.txt";

    //const std::wstring nomFichier = L"2023-08-30_31_09-01_02 Netflix.txt";
    //const std::wstring nomFichier = L"2023-08-30_09-12_12 Netflix.txt";
 
    int y, m, d;
    y = stoi(nomFichier.substr(0, 4));
    m = std::stoi(nomFichier.substr(5, 2));
    d = std::stoi(nomFichier.substr(8, 2));
    std::wcout << L"y=" << y << std::endl;
    std::wcout << L"m=" << m << std::endl;
    std::wcout << L"d=" << d << std::endl;

    
    std::pair<std::vector<DateRecord>, std::wstring>dates;
    dates = ExtraireInfosDepuisNomDeFichier(nomFichier);
    i = 0;
    std::wcout << std::endl << L"Date :" <<  std::endl;
    std::vector<DateRecord>::iterator iter;
    if (i == 0)
    {
        for (iter = dates.first.begin(); iter != dates.first.end(); iter++, i++)
        {
            std::wcout << L"__" << i << std::endl;
            std::wcout << L"____date=[" << (*iter).date.tm_year + 1900 << L'/' << (*iter).date.tm_mon + 1 << L'/' << (*iter).date.tm_mday << L']' << std::endl;
            std::wcout << L"____someFlag=" << (*iter).someFlag << std::endl;
        }
        std::wcout << L"streaming=[" << dates.second << L"]" << std::endl;
    }
    else
        std::wcout << L"__Erreur dates !!!" << std::endl;
    return EXIT_SUCCESS;
}

/*int wmain()
{
    // Set output mode to handle virtual terminal sequences
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
    {
        return GetLastError();
    }

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode))
    {
        return GetLastError();
    }

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode))
    {
        return GetLastError();
    }

    // Try some Set Graphics Rendition (SGR) terminal escape sequences
    wprintf(L"\x1b[31mThis text has a red foreground using SGR.31.\r\n");
    wprintf(L"\x1b[1mThis text has a bright (bold) red foreground using SGR.1 to affect the previous color setting.\r\n");
    wcout << L"\x1b[1mThis text has a bright (bold) red foreground using SGR.1 to affect the previous color setting." << endl;
    wprintf(L"\x1b[mThis text has returned to default colors using SGR.0 implicitly.\r\n");
    wprintf(L"\x1b[34;46mThis text shows the foreground and background change at the same time.\r\n");
    wprintf(L"\x1b[0mThis text has returned to default colors using SGR.0 explicitly.\r\n");
    wprintf(L"\x1b[31;32;33;34;35;36;101;102;103;104;105;106;107mThis text attempts to apply many colors in the same command. Note the colors are applied from left to right so only the right-most option of foreground cyan (SGR.36) and background bright white (SGR.107) is effective.\r\n");
    wprintf(L"\x1b[39mThis text has restored the foreground color only.\r\n");
    wprintf(L"\x1b[49mThis text has restored the background color only.\r\n");

    wcout << L"\n\n";

    int j = 0, k = 0, n;

    wstring Textes = L"\x1b[94;1mChaine : \x1b[37;0mThis text has restored the background color only.";
    n = Textes.length();
    //wstring S_T_1 = L"\x1b[94;1m";
    //wstring S_t_1 = L"\x1b[37;0m";
    wstring o;
    int oo;

    for (j = k; k < n; k++)
    {
        if (Textes[k] == L'\x1b')
        {
            o = L'\x1b'; // Textes[k];
            oo = 1;
            do
            {
                o += Textes[k + oo];
                oo++;
            } while (Textes[k + oo] != L'm');
            o += L'm';
            wcout << o;
            //                wcout << L'm';
            //                oo++;
            k = k + oo;
            //n = n + oo;
            //wcout << L"XXXXX";
            continue;
        }
        wcout << Textes[k];
        //l++;
    }


    return 0;
}*/


