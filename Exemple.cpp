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

static inline void rtrim(std::wstring& s)
{
    s.erase(find_if(s.rbegin(), s.rend(), [](int ch)
        {
            return !isspace(ch);
        }).base(), s.end());
}

const void Console_Lire_txt(std::wstring lire)
{
    std::size_t i = X / lire.length();
    int j = X % lire.length();
    int k;
    for (k = 0; k < i; k++)
        std::wcout << lire;
    k = 0;
    while (k < j)
    {
        std::wcout << (wchar_t)lire[k];
        k++;
    }
}

inline std::wstring space(int i, wchar_t espace)
{
    std::wstring wstr = L"";
    //for (auto ===> Pas trouvé !!!)
    for (int j = 0; j < i; j++)
        wstr += espace;
    return wstr;
}



const int Console_Lire_txt(std::wstring Textes, int x1, int x2)
{
#if Console_Lire_txt_OK_ == 1
    std::wcout << L"const int Console_Lire_txt(" << Textes << L", " << x1 << L", " << x2 << L") : " << std::endl;
#endif

    if (Textes == L"" && X == 0)
        return 0;
    rtrim(Textes);
    //
    bool ligne_vide = false;
    bool retrait = false;
    wchar_t entree = L'\n';

    int I, x;
    int i, j, k, n = 0;
    int l, m;
    std::size_t N;
    // // // //  //
    Textes += entree;
    // // // //  //
    int p = 0;
    std::vector<std::wstring> P;
    std::vector<int> PP;
    std::wstring o;
    int oo = 0;
    N = Textes.length();
    i = 0;
    while (i < N)
    {
        if (Textes[i] == L'\x1b')
        {
            j = i;
            o = L'\x1b';
            j++;
            while (Textes[j] != L'm' || j >= N)
            {
                o += Textes[j];
                j++;
            }
            if (j >= N)
            {
                //E.afficher_X(-1, L"::Console_Lire_txt()", L"j >= N !!!");
                //return -1;
                return EXIT_FAILURE;
            }
            o += L'm';
            P.push_back(o);
            PP.push_back(i);
            Textes = Textes.replace(i, o.length(), L"");
            p++;
            N = Textes.length();
        }
        i++;
    }
    std::vector <std::wstring>::iterator iter;
    std::wstring wstr;
    x = x1;
    I = X - x;
    j = 0;
    if (I < N)
        n = I;
    else
        n = (int)N;
    std::vector <int>::iterator iter2;
    iter = P.begin();
    iter2 = PP.begin();
    oo = 0;
    while (j < N)
    {
        // Ok
        if (Textes[j] == entree && retrait == true)
        {
            ligne_vide = true;
            //retrait = true;
            goto _ok_;
        }
        // Ok
        for (i = 0; i < x; i++)
#if Console_Lire_txt_ == 1
            std::wcout << L'_';
#else
            std::wcout << L' ';
#endif
        //converti = converti.substr(pos_found + sep_found.length());
        // std::wcout << 




        // Ok
        while (Textes[j] == L' ')
        {
            j++;
            n++;
            if (n > N)
                n = (int)N;
        }
        // Ok
        k = j;
        for (j = k; k < n; k++)
        {
            if (Textes[k] == entree)
            {
                break;
            }
        }
        if (k != n)
        {
            retrait = true;
            n = k;
        }
        else
        { // ???
            ligne_vide = false;
            retrait = false;
            while (Textes[n] != L' ')
            {
                n--;
            }
        }
        // Ok
        k = j;
        l = 0;
        for (j = k; k < n; k++)
        {
            if (p != 0 && k == *iter2)
            {
                o = *iter;
                std::wcout << o;
                wstr = o;
                iter++;
                iter2++;
                oo++;
            }
            std::wcout << Textes[k];
            l++;
        }
        // Ok
        j = k;
        m = X - (x + l);
        for (i = 0; i < m; i++)
        {
#if Console_Lire_txt_ == 1
            std::wcout << L'*';
#else
            std::wcout << L' ';
#endif
        }
        // Ok
    _ok_:
        if (ligne_vide == true && retrait == true)
        {
            ligne_vide = false;
            j++;
            n++;
            if (n > N)
                n = (int)N;
            x = x1;
            continue;
        }
        else if (ligne_vide == false && retrait == true)
        {
            retrait = false;
            x = x1;
        }
        else
        {
            ligne_vide = false;
            retrait = false;
            x = x2;
        }
        j = n;
        if (p != 0
            &&
            j == *iter2)
        {
            o = *iter;
            std::wcout << o;
            wstr = o;
            iter++;
            iter2++;
            oo++;
        }
        j++;
        // x ?
        n += (X - x);
        if (n > N)
        {
            n = (int)N;
        }
    }
    if (p != 0 && PP.back() <= N)
    {
        std::wcout << P.back() << endl;
        wstr = P.back();
    }
#if Console_Lire_txt_OK_ == 1
    std::wcout << L"const int Console_Lire_txt() : Ok !" << std::endl;
#endif
    return EXIT_SUCCESS;
}


//std::string wstr_to_u8(std::wstring uneWString)
//{
//    return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(uneWString);
//}

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

/*bool checkyear(int y)
{
    if (!(1582 <= y))
        assert(y >= 1582 && L"L'année invalide !");
        //return false;
    return true;
}*/
bool checkyear(int y)
{
    return 1582 <= y;
}
/*bool checkmonth(int m)
{
    if (!(1 <= m && m <= 12))
        assert((1 <= m && m <= 12) && L"Le moi invalide !");
    //return false;
    return true;
}*/
bool checkmonth(int m)
{
    return (1 <= m && m <= 12);
}

/*bool checkday(int m, int d, int y)
{
    if (!(1 <= d && d <= 31))
        return false;
    if ((d == 31) && (m == 2 || m == 4 || m == 6 || m == 9 || m == 11))
         return false;
    if ((d == 30) && (m == 2))
         return false;
    if ((m == 2) && (d == 29) && (y % 4 != 0))
        return false;
    if ((m == 2) && (d == 29) && (y % 400 == 0))
        return true;
    if ((m == 2) && (d == 29) && (y % 100 == 0))
        return false;
    if ((m == 2) && (d == 29) && (y % 4 == 0))
        return true;
    return true;
}*/
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

/*std::vector<std::pair<std::pair<std::tm, bool>, std::wstring>> ExtraireInfosDepuisNomDeFichier(const std::wstring& nomFichier)
{
    //Ici !!!
    std::vector<std::pair<std::pair<std::tm, bool>, std::wstring>> dates;
    //Ici !!!
    //Exemple : 
    // 2023-01-01 TF1
    // 2023-01-02_03 Netflix
    // 2023-01-04_02-01 Netflix
    // 2023-03-01_02_03_ Netflix
    return dates;
}*/

/*const int afficher_Date_1(std::wstring& wstr, std::vector<std::pair<std::tm, bool>>& date, std::wstring& streaming)
{
    if (wstr == L"")
    {
        return -1;
    }
    std::size_t pos;
    pos = wstr.length();
    wstr = wstr.substr(0, pos - 4); // .txt
    std::wcout << L"wstr=" << wstr << std::endl;
    if (pos < 10)
    {
        assert(L"La date invalide !");
    }
    std::tm tm;

    // std::vector<std::pair<std::tm, bool>>& date
    // 2023-01-01 TF1
    // 2023-01-02_03 Netflix
    // 2023-01-04_02-01 Netflix
    // 2023-03-01_02_03_ Netflix
    int year = 0, mon = 0, mday = 0, y = 0, m = 0, d = 0;
    int first_year = 0, first_mon = 0, first_mday = 0;

    wchar_t tiret1 = L'-', tiret2 = L'-', sp = L' ', tiret_bas = L'_';
    bool a_year = false, a_mon = false, a_mday = false, a_tiret1 = false, a_tiret2 = false, a_tiret_bas = false, a_sp = false, b_bbb = false;
    //std::size_t pos_year, pos_mon, pos_mday;
    int i = 0;
    do
    {
        std::wcout << L"Ok !!!" << std::endl;
        if (wstr[0] == std::wstring::npos || wstr[0] == sp)
            break;
        std::wcout << L"Ok qqqq!!!" << std::endl;
        // tiret_bas
        if (wstr[0] == tiret_bas)
        {
            if (iswalpha(wstr[0]))
            {
                std::wcout << L"ttt [" << wstr[0] << L"]" << std::endl;
                assert(L"La date ou le tiret ou le tiret bas invalide !");
            }


        }
        //pos = 0;
        // year + mon + mday
        if (!a_year && !a_mon && !a_mday && 
            ParseYear(y = std::stoi(wstr.substr(0, 4))) &&
            ParseTiret(wstr[4]) &&
            ParseMonth(m = std::stoi(wstr.substr(5, 2))) &&
            ParseTiret(wstr[7]) &&
            ParseDay(m, d = std::stoi(wstr.substr(8, 2)), y) &&
            //(wstr[11] == std::wstring::npos || wstr[11] == tiret_bas || wstr[11] == L' ') &&
            first_year < y
            )
        {
            std::wcout << L"year ???" << std::endl;
            a_year = true;
            //a_tiret1 = true;
            year = y;
            mon = m;
            mday = d;
            tm.tm_year = year - 1900;
            tm.tm_mon = mon - 1;
            tm.tm_mday = mday;
            wstr = wstr.substr(10);
            std::wcout << L"year=[" << wstr[0] << L"]" << std::endl;
            std::wcout << L"    date=" << tm.tm_year + 1900 << L"/" << tm.tm_mon + 1 << L"/" << tm.tm_mday << L", true" << std::endl;
            if (wstr[0] == tiret_bas)
            {
                date.push_back(std::make_pair(tm, true));
                first_year = year;
                //wstr = wstr.substr(1);
                y = m = d = 0;
                std::wcout << y << L" " << m << L" " << d << std::endl;
                continue;
            }
            if(wstr[0] == std::wstring::npos)
            {
                date.push_back(std::make_pair(tm, false));
                break;
            }
            else
                assert(L"La date ou le tiret ou le tiret bas invalide !");

        }
        // tiret1
        //if (a_year && !a_tiret1 && !a_mon && !a_tiret2 && !a_mday && ParseTiret(wstr[0]))
        //{
        //    a_tiret1 = true;
        //    std::wcout << L"Ok !!!" << std::endl;
        //    wstr = wstr.substr(1);
        //    //continue;
        //}
        // mon + mday
        if (a_year && !a_mon && !a_mday &&
            first_year == year &&
            ParseMonth(m = std::stoi(wstr.substr(0, 2))) &&
            ParseTiret(wstr[2]) &&
            ParseDay(m, d = std::stoi(wstr.substr(2, 2)), y) &&
            first_mon < m
            )
        {
            std::wcout << L"mon ???" << std::endl;
            a_mon = true;
            mon = m;
            mday = d;
            tm.tm_mon = mon - 1;
            tm.tm_mday = mday;
            wstr = wstr.substr(5);
            std::wcout << L"yyy [" << wstr[0] << L"]" << std::endl;
            if (wstr[0] == tiret_bas)
            {
                date.push_back(std::make_pair(tm, true));
                first_mon = mon;
                wstr = wstr.substr(1);
                if (iswalpha(wstr[0]))
                    assert(L"Le moi ou le jour ou le tiret ou le tiret bas invalide !");
                if (wstr[0] == std::wstring::npos || (wstr[0] == sp && wstr[1] == iswalpha(wstr[1])))
                    break;
                //continue;
            }
            else
            {
                date.push_back(std::make_pair(tm, false));
                break;
            }
            //continue;
        }
        // tire2
        //if (a_year && a_tiret1 && a_mon && !a_tiret2 && !a_mday && ParseTiret(wstr[0]))
        //{
        //    a_tiret2 = true;
        //    wstr = wstr.substr(1);
        //    //continue;
        //}
        // mday
        std::wcout << L"bbbbb" << std::endl;
        if (a_year && a_mon && !a_mday &&
            ParseDay(m, d = std::stoi(wstr.substr(0, 2)), y) &&
            first_mday <= d
            )
        {
            std::wcout << L"mday ???" << std::endl;
            a_mday = true;
            mday = d;
            tm.tm_mday = mday;
            std::wcout << L"    tm.tm_mday=" << tm.tm_mday << std::endl;
            wstr = wstr.substr(2);
            std::wcout << L"uuu [" << wstr[0] << L"]" << std::endl;
            if (wstr[0] == tiret_bas)
            {
                date.push_back(std::make_pair(tm, true));
                first_mday = mday;
                wstr = wstr.substr(1);
                std::wcout << L"uuu [" << wstr[0] << L"]" << std::endl;
                if (iswalpha(wstr[0]))
                    assert(L"Le jour ou le tiret ou le tiret bas invalide !");
                //if (wstr[0] == std::wstring::npos || (wstr[0] == sp && wstr[1] == iswalpha(wstr[1])))
                //    break;
                //continue;
            }
            else
            {
                date.push_back(std::make_pair(tm, false));
                break;
            }
            //date.tm_mon = mon - 1;
            //date.tm_mday = mday;
            //continue;
        }
        std::wcout << L"wstr=[" << wstr << L"]" << std::endl;
        if (wstr[0] == sp)
        {
            std::wcout << L"L'espasse !!!" << std::endl;
            break;
        }


    } while (wstr[0] != std::wstring::npos);// || wstr[0] == sp));
    if (wstr[0] == sp)
    {
        wstr = wstr.substr(1);
        if (wstr[0] == sp)
        {
            assert(L"L'espasse invalide 2 !");
        }
    }
    if (wstr[0] == std::wstring::npos)
        streaming = L"";
    else
    {
        streaming = wstr;
    }
    std::vector<pair<std::tm, bool>>::iterator iter;
    std::wcout << L"Dates(s) :" << std::endl;
    for (iter = date.begin(); iter != date.end(); iter++)
    {
        std::wcout << L"    date=" << (*iter).first.tm_year + 1900 <<
            L"/" << (*iter).first.tm_mon + 1 <<
            L"/" << (*iter).first.tm_mday <<
            L", bool=" << (*iter).second << std::endl;
    }
    std::wcout << L"    streaming=[" << streaming << L"]" << std::endl;
    std::wcout << L"Ok : Fin" << std::endl;
    return EXIT_SUCCESS;
}*/

/*std::pair<std::vector<DateRecord>, std::wstring> ExtraireInfosDepuisNomDeFichier(const std::wstring& nomFichier)
{
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    
    std::size_t pos;
    pos = nomFichier.length();
    std::wstring strRestant = nomFichier.substr(0, pos - 4);
    assert(strRestant.length() > 9 && L"Nom de fichier trop court pour avoir au moins une date");

    std::vector<DateRecord> dates;
    //std::vector<DateRecord>::iterator iter;
    DateRecord date{ 0 };

    wchar_t sp = L' ', tiret = L'-', tiret_bas = L'_';
    int y, m, d;
    y = m = d = 0;
    int firstYear = 0, firstMon = 0, firstDay = 0;
    bool yearFlag, monFlag, dayFlag;
    yearFlag = monFlag = dayFlag = false;
    int i = 0;
    do
    {
        std::wcout << L"strRestant=" << strRestant << std::endl;
        if (strRestant[0] == sp)
            break;

        //y = std::stoi(strRestant.substr(0, 4));
        std::wcout << L"y=" << y << std::endl;
        //int m = std::stoi(wstr.substr(5, 2));
        //int d = std::stoi(wstr.substr(8, 2));
 
        //checkyear(y);
        //checkmonth(m);
        ///checkday(m, d, y);

        // year + mon + mday
        std::wcout << L"year + mon + mday" << std::endl;
        if (!yearFlag && !monFlag && !dayFlag
            && (y = stoi(strRestant.substr(0, 4))) && checkyear(y)
            && strRestant[4] == tiret
            && (m = std::stoi(strRestant.substr(5, 2))) && checkmonth(m)
            && strRestant[7] == tiret
            && ((d = std::stoi(strRestant.substr(8, 2))) && checkday(m, d, y))
            && firstYear < y)
        {
            std::wcout << L"y ?" << std::endl;
            date.date.tm_year = y - 1900;
            date.date.tm_mon = m - 1;
            date.date.tm_mday = d;
            strRestant = strRestant.substr(10);
            if (strRestant[0] == tiret_bas)
            {
                date.someFlag = true;
                dates.push_back(date);
                std::wcout << L"i=" << i << std::endl;
                std::wcout << L"  dates[" << i << L"].date=" << dates[i].date.tm_year + 1900 << L'/' << dates[i].date.tm_mon + 1 << L'/' << dates[i].date.tm_mday << std::endl;
                std::wcout << L"  dates[" << i << L"].someFlag=" << dates[i].someFlag << std::endl;
                i++;
                strRestant = strRestant.substr(1);
                firstYear = y;
                std::wcout << L"firstYear=" << firstYear << std::endl;
                y = m = d = 0;
                //continue;
            }
            else
            {
                date.someFlag = false;
                dates.push_back(date);
                std::wcout << L"i=" << i << std::endl;
                std::wcout << L"  dates[" << i << L"].date=" << dates[i].date.tm_year + 1900 << L'/' << dates[i].date.tm_mon + 1 << L'/' << dates[i].date.tm_mday << std::endl;
                std::wcout << L"  dates[" << i << L"].someFlag=" << dates[i].someFlag << std::endl;
                i++;
                break;
            }
        }
        else
        {
            std::wcout << L"year pas ok !" << std::endl;
            assert(firstYear > y && L"aaa");
            break;
        }
        // mon + mday
        std::wcout << L"mon + mday" << std::endl;
        if (!monFlag && !dayFlag
            && firstYear != 0
            && (m = std::stoi(strRestant.substr(0, 2))) && checkmonth(m)
            && strRestant[2] == tiret
            && (d = std::stoi(strRestant.substr(3, 2))) && checkday(m, d, y)
            && firstMon < m)
        {
            std::wcout << L"m ?" << std::endl;
            date.date.tm_year = y - 1900;
            date.date.tm_mon = m - 1;
            date.date.tm_mday = d;
            strRestant = strRestant.substr(5);
            if (strRestant[0] == tiret_bas)
            {
                date.someFlag = true;
                dates.push_back(date);
                std::wcout << L"i=" << i << std::endl;
                std::wcout << L"  dates[" << i << L"].date=" << dates[i].date.tm_year + 1900 << L'/' << dates[i].date.tm_mon + 1 << L'/' << dates[i].date.tm_mday << std::endl;
                std::wcout << L"  dates[" << i << L"].someFlag=" << dates[i].someFlag << std::endl;
                i++;
                strRestant = strRestant.substr(1);
                firstMon = m;
                m = d = 0;
                //continue;
            }
            else
            {
                date.someFlag = false;
                dates.push_back(date);
                std::wcout << L"i=" << i << std::endl;
                std::wcout << L"  dates[" << i << L"].date=" << dates[i].date.tm_year + 1900 << L'/' << dates[i].date.tm_mon + 1 << L'/' << dates[i].date.tm_mday << std::endl;
                std::wcout << L"  dates[" << i << L"].someFlag=" << dates[i].someFlag << std::endl;
                i++;
                break;
            }
        }
        else
        {
            std::wcout << L"mon pas ok !" << std::endl;
            assert(firstMon > m && L"aaa");
           break;
        }
        // mday
        std::wcout << L"mday" << std::endl;
        if (firstYear != 0
            && (m = std::stoi(strRestant.substr(0, 2))) && checkmonth(m)
            && firstDay <= d)
        {
            std::wcout << L"d ?" << std::endl;
            date.date.tm_year = y - 1900;
            date.date.tm_mon = m - 1;
            date.date.tm_mday = d;
            strRestant = strRestant.substr(2);
            if (strRestant[0] == tiret_bas)
            {
                date.someFlag = true;
                dates.push_back(date);
                std::wcout << L"i=" << i << std::endl;
                std::wcout << L"  dates[" << i << L"].date=" << dates[i].date.tm_year + 1900 << L'/' << dates[i].date.tm_mon + 1 << L'/' << dates[i].date.tm_mday << std::endl;
                std::wcout << L"  dates[" << i << L"].someFlag=" << dates[i].someFlag << std::endl;
                i++;
                strRestant = strRestant.substr(1);
                firstDay = d;
                m = d = 0;
                continue;
            }
            else
            {
                date.someFlag = false;
                dates.push_back(date);
                std::wcout << L"i=" << i << std::endl;
                std::wcout << L"  dates[" << i << L"].date=" << dates[i].date.tm_year + 1900 << L'/' << dates[i].date.tm_mon + 1 << L'/' << dates[i].date.tm_mday << std::endl;
                std::wcout << L"  dates[" << i << L"].someFlag=" << dates[i].someFlag << std::endl;
                i++;
                break;
            }
        }
        else
        {
            std::wcout << L"day pas ok !" << std::endl;
            assert(firstDay > d && L"aaa");
           break;
        }
        //if (!yearFlag)
        //    yearFlag;
        //if (!monFlag)
        //    monFlag;
        //if (!dayFlag)
        //    dayFlag;
        std::wcout << L"firstYear=" << firstYear << std::endl;
        system("PAUSE");
        //exit(EXIT_FAILURE);
    } while (strRestant.length() > 0 || strRestant[0] == sp);
    std::wstring streaming = L"";
    if (strRestant[0] == sp)
    {
        strRestant = strRestant.substr(1);
        if(strRestant[0] == sp)
            assert(L"L'espace (2) invalide !");
        if (streaming[0] != wstring::npos)
            streaming = strRestant;
    }
    return std::pair<std::vector<DateRecord>, std::wstring>(dates, streaming);
}*/

std::pair<std::vector<DateRecord>, std::wstring> ExtraireInfosDepuisNomDeFichier(const std::wstring& nomFichier)
{
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");

    std::size_t pos;
    pos = nomFichier.length();
    std::wstring strRestant = nomFichier.substr(0, pos - 4);
    assert(strRestant.length() > 9 && L"Nom de fichier trop court pour avoir au moins une date");

    std::vector<DateRecord> dates{};
    //std::vector<DateRecord>::iterator iter;
    DateRecord date{ 0 };

    wchar_t sp = L' ', tiret = L'-', tiret_bas = L'_';
    int y, m, d;
    y = m = d = 0;
    int firstYear = 0, firstMon = 0, firstDay = 0;
    bool yearFlag, monFlag, dayFlag;
    yearFlag = monFlag = dayFlag = false;
    std::size_t taille{};
    int i = 0;
    do
    {
        std::wcout << L"strRestant=" << strRestant << std::endl;
        if (strRestant[0] == sp)
            break;

        //y = std::stoi(strRestant.substr(0, 4));
        //std::wcout << L"y=" << y << std::endl;
        //int m = std::stoi(wstr.substr(5, 2));
        //int d = std::stoi(wstr.substr(8, 2));

        //checkyear(y);
        //checkmonth(m);
        ///checkday(m, d, y);

        // year + mon + mday
        std::wcout << L"year + mon + mday" << std::endl;
        if (strRestant[0] != tiret_bas &&
            !yearFlag && !monFlag && !dayFlag &&
            (y = stoi(strRestant.substr(0, 4))) && checkyear(y) &&
            strRestant[4] == tiret &&
            (m = std::stoi(strRestant.substr(5, 2))) && checkmonth(m) &&
            strRestant[7] == tiret &&
            ((d = std::stoi(strRestant.substr(8, 2))) && checkday(m, d, y)))
        {
            assert(firstYear < y && L"L'année aaaaa");
            firstYear = y;
            std::wcout << L"y ?" << std::endl;
            date.date.tm_year = y - 1900;
            date.date.tm_mon = m - 1;
            date.date.tm_mday = d;
            strRestant = strRestant.substr(10);
            date.someFlag = false;
            std::wcout << L"taille=" << taille << std::endl;
            taille = std::size(dates);
            dates.push_back(date);
            std::wcout << L"  dates[" << i << L"].date=" << dates[i].date.tm_year + 1900 << L'/' << dates[i].date.tm_mon + 1 << L'/' << dates[i].date.tm_mday << std::endl;
            std::wcout << L"  dates[" << i << L"].someFlag=" << dates[i].someFlag << std::endl;
            taille = std::size(dates);
            std::wcout << L"taille=" << taille << std::endl;
            std::wcout << L"i=" << i << std::endl;
            //taille++;
            i++;
            std::wcout << L"i=" << i << std::endl;
            if (strRestant[0] == sp /* || strRestant[0] != iswdigit(strRestant[0])*/)
                break;
            std::wcout << L"year qqq" << std::endl;
            //continue;
        }
        /*else
        {
            std::wcout << L"year pas ok !" << std::endl;
            assert(firstYear > y && L"aaa");
            break;
        }*/
        // mon + mday
        std::wcout << L"mon + mday" << std::endl;
        if (strRestant[0] == tiret_bas &&
            firstYear != 0 &&
            (m = std::stoi(strRestant.substr(1, 2))) && checkmonth(m) &&
            strRestant[3] == tiret &&
            (d = std::stoi(strRestant.substr(4, 2))) && checkday(m, d, y))
        {
            assert(firstMon < m && L"Le mois aaaaa");
            std::wcout << L"m ?" << std::endl;
            date.date.tm_year = y - 1900;
            date.date.tm_mon = m - 1;
            date.date.tm_mday = d;
            strRestant = strRestant.substr(5);
            date.someFlag = false;
            dates.push_back(date);
            std::wcout << L"taille=" << taille << std::endl;
            std::wcout << L"  dates[" << taille << L"].date=" << dates[taille].date.tm_year + 1900 << L'/' << dates[i].date.tm_mon + 1 << L'/' << dates[i].date.tm_mday << std::endl;
            std::wcout << L"  dates[" << taille << L"].someFlag=" << dates[taille].someFlag << std::endl;
            taille++;
            i++;
            if ((pos = strRestant.length()) == 0 || strRestant[0] == sp || strRestant[0] != iswdigit(strRestant[0]))
                break;
            //continue;
        }
        /*else
        {
            std::wcout << L"mon pas ok !" << std::endl;
            assert(firstMon > m && L"aaa");
            break;
        }*/
        // mday
        std::wcout << L"mday" << std::endl;
        if (firstYear != 0 &&
            strRestant[0] == tiret_bas &&
            (d = std::stoi(strRestant.substr(1, 2))) && checkday(m, d, y))
        {
            assert(firstDay <= d && L"Le jours aaaaa");
            std::wcout << L"d ?" << std::endl;
            date.date.tm_year = y - 1900;
            date.date.tm_mon = m - 1;
            date.date.tm_mday = d;
            strRestant = strRestant.substr(2);
            date.someFlag = false;
            dates.push_back(date);
            std::wcout << L"taille=" << taille << std::endl;
            std::wcout << L"  dates[" << taille << L"].date=" << dates[taille].date.tm_year + 1900 << L'/' << dates[i].date.tm_mon + 1 << L'/' << dates[i].date.tm_mday << std::endl;
            std::wcout << L"  dates[" << taille << L"].someFlag=" << dates[taille].someFlag << std::endl;
            taille++;
            i++;
            if ((pos = strRestant.length()) == 0 || strRestant[0] == sp || strRestant[0] != iswdigit(strRestant[0]))
                break;
            //continue;
        }
        /*else
        {
            std::wcout << L"day pas ok !" << std::endl;
            assert(firstDay > d && L"aaa");
            break;
        }*/
        //exit(EXIT_FAILURE);
        // _
        std::wcout << L"_" << std::endl;
        if (
            strRestant[0] == tiret_bas &&
            strRestant[1] != wstring::npos
            )
        {
            taille = std::size(dates);
            std::wcout << L"taille=" << taille << std::endl;
            std::wcout << L"_" << std::endl;
            //std::wcout << L"  dates[" << i - 1 << L"].someFlag=" << dates[i].someFlag << std::endl;
            dates[i - 1].someFlag = true;
            //std::wcout << L"  dates[" << taille - 1 << L"].someFlag=" << dates[i].someFlag << std::endl;
            //dates.at(0) = 
            strRestant = strRestant.substr(1);
            continue;
        }
        else
        {
            std::wcout << L"pla pla pla" << std::endl;
            std::wcout << L"  dates[" << taille - 1 << L"].date=" << dates[i].date.tm_year + 1900 << L'/' << dates[i].date.tm_mon + 1 << L'/' << dates[i].date.tm_mday << std::endl;

            break;
        }
    } while (strRestant.length() > 0 || strRestant[0] == sp);
    std::wstring streaming = L"";
    if (strRestant[0] == sp)
    {
        strRestant = strRestant.substr(1);
        if (strRestant[0] == sp)
            assert(L"L'espace (2) invalide !");
        if (streaming[0] != wstring::npos)
            streaming = strRestant;
    }
    return std::pair<std::vector<DateRecord>, std::wstring>(dates, streaming);
}

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

    //SetConsoleTitle(u8"You");	// Réglage du titre
    std::size_t pos;
    std::wstring wstr;
    std::wcout << L"Ok :" << std::endl;
    pos = wstr.length();
    //wstr = L"2022-08-31 Netflix.txt";
    wstr = L"2022-08-31_ Netflix.txt";
    //wstr = L"2022-08-30 Net.txt";
    //wstr = L"2022-08-30_31.txt";
    //wstr = L"2022-08-30_31_2023-01-13 Netflix.txt";
    //wstr = L"2023-08-30_09-01 Netflix.txt";
    std::wcout << L"pos=" << pos << std::endl;
    std::wcout << L"wstr=" << wstr << std::endl;
    const std::wstring nomFichier = wstr;
    //afficher_Date_1(wstr, date, streaming);
    std::pair<std::vector<DateRecord>, std::wstring>dates;
    dates = ExtraireInfosDepuisNomDeFichier(nomFichier);
    i = 0;
    std::wcout << L"Date :" << std::endl;
    std::vector<DateRecord>::iterator iter;
    for (iter = dates.first.begin(); iter != dates.first.end(); iter++, i++)
    {
        std::wcout << L"__" << i <<  std::endl;
        std::wcout << L"____year=" << (*iter).date.tm_year + 1900 << std::endl;
        std::wcout << L"____mon=" << (*iter).date.tm_mon + 1 << std::endl;
        std::wcout << L"____mday=" << (*iter).date.tm_mday << std::endl;
        std::wcout << L"____someFlag=" << (*iter).someFlag << std::endl;
    }
    std::wcout << L"streaming=[" << dates.second << L"]" << std::endl;
    //std::wcout << L"__streaming=[" << streaming << L"]" << std::endl;
    //date.pop_back();
    //wstr = L"2022-08-01_01_01_05_05_08.txt";
    //afficher_Date_1(wstr, date, titre);
    //std::wcout << L"date.tm_year=" << date[0].first.tm_year << std::endl;
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


