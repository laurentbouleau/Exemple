//#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
//#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING 1


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
#include <locale>
#include <algorithm>
#include <codecvt>
#include <tuple>

#include <filesystem> // C++17 standard header file name
//#include <experimental/filesystem> // Header file for pre-standard implementation
//using namespace std::experimental::filesystem::v1;
using namespace std;
namespace fs = std::filesystem;

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

const std::vector<std::pair<std::wstring, std::wstring>>lire_paireCleValeur_depuisFichierTxt(std::wstring const& nomFichier, std::wstring separeteur)
{
    std::string contenuFichier{ u8"" };
    std::string ligneCourante{ u8"" };
    std::vector<std::pair<std::wstring, std::wstring>> clevaleurs;

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
    converti += L"\n";

    //std::wcout << L"lire_paireCleValeur_depuisFichierTxt" << std::endl;
    //std::wcout << L"converti{" << converti << L"}" << L"\n" << std::endl;
    std::size_t pos = converti.length();

    if (pos == std::wstring::npos)
        return clevaleurs;

    std::size_t pos2 = 0;
    while (pos = converti.find(L"\n"))
    {
        if (converti[0] != converti.length())
        {
            pos2 = converti.find(L"...");
            if (pos2 != std::wstring::npos && converti[0] == L'.' && converti[1] == L'.' && converti[2] == L'.')
            {
                //std::wcout << L"aaa" << std::endl;
                clevaleurs.push_back(std::make_pair(L"…", L""));
                return clevaleurs;
            }
            pos2 = converti.find(L"…");
            if (pos2 != std::wstring::npos && converti[0] == L'…')
            {
                //std::wcout << L"bbb" << std::endl;
                clevaleurs.push_back(std::make_pair(L"…", L""));
                return clevaleurs;
            }
            pos2 = converti.find(L".");
            if (pos2 != std::wstring::npos && converti[0] == L'…')
            {
                //std::wcout << L"ccc" << std::endl;
                clevaleurs.push_back(std::make_pair(L"…", L""));
                return clevaleurs;
            }
            pos2 = converti.find(separeteur);
            if (pos2 == std::wstring::npos && pos != std::wstring::npos)
            {
                //std::wcout << L"ddd" << std::endl;
                clevaleurs.push_back(std::make_pair(converti.substr(0, pos - 1), L""));
            }
            else if (pos2 == 0)
            {
                //std::wcout << L"eee" << std::endl;
                clevaleurs.push_back(std::make_pair(L"", converti.substr(3, (pos - 1) - 2)));
            }
            else if (pos > pos2)
            {
                //std::wcout << L"eee" << std::endl;
                //std::wcout << converti.substr(0, pos2) << separeteur << converti.substr(pos2 + 3, pos - (pos2 + 3)) << std::endl;
                clevaleurs.push_back(std::make_pair(converti.substr(0, pos2), converti.substr(pos2 + 3, pos - (pos2 + 3))));
            }
            else
            {
                //std::wcout << L"fff" << std::endl;
                break;
            }
            converti = converti.substr(pos + 1);
            //std::wcout << L"ggg" << std::endl;

        }
        else
            break;
    }
    /*std::vector<std::pair<std::wstring, std::wstring>> clevaleurs{};
    std::vector<std::wstring> items = lire_fichierTxt(nomFichier, { separeteur });

    if (items.size() % 2 != 0)
    {
        throw std::runtime_error("Le fichier '" + wstr_to_u8(nomFichier) + "' contient un nombre impair d'élément.");
    }

    for (int i = 0; i < items.size(); i += 2)
    {
        clevaleurs.push_back(std::make_pair(items[i], items[i + 1]));
    }*/

    return clevaleurs;
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
    std::wcout << L"nomFichier={" << nomFichier << L'}' << std::endl;
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
    std::wcout << L"converti={" << converti << L'}' << std::endl;
    return converti;
}

inline std::wstring space(int i, wchar_t espace)
{
    std::wstring wstr = L"";
    //for (auto ===> Pas trouvé !!!)
    for (int j = 0; j < i; j++)
        wstr += espace;
    return wstr;
}

void Console_Lire(HANDLE hOut, const std::wstring& wstr, int taille_indentation, wchar_t caractere_indentation = L' ')
{
    DWORD numberOfBytesWritten = 0;

    std::wstring indentation(taille_indentation, caractere_indentation);

    BOOL res;

    if (taille_indentation > 0)
    {
        res = WriteFile(hOut, &indentation[0], (DWORD)(indentation.size() * sizeof(wchar_t)), &numberOfBytesWritten, NULL);
        assert(res == TRUE);
    }

    res = WriteFile(hOut, &wstr[0], (DWORD)(wstr.size() * sizeof(wchar_t)), &numberOfBytesWritten, NULL);
    assert(res == TRUE);
}

void test_date_tire(wchar_t d)
{
    if (d != L'-')
    {
        throw exception_date_tiret();
    }
    return;
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
// # void Saison::afficher_Dossier(fs::path const& nomDossier)                                                                                          #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::afficher_Dossier(fs::path const& nomDossier)
{
    std::wcout << L"Dossier=[" << nomDossier << L']' << std::endl;
    auto nd = fs::path(nomDossier).wstring();
    assert(nd.length() > 0 && L"Nom de dossier vide");
    std::size_t pos = 0;
    std::wstring wstr = nd.substr(pos);
    assert(nd.length() > 9 && L"Nom de fichier trop court pour avoir au moins une date");

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
    std::wcout << L"dossier.first ok !!!" << std::endl;
    std::wcout << L"dossier.second=[" << dossier.second << L']' << L" ok !!!" << std::endl;
    std::wcout << L"Dossier(1)=[" << nd << L']' << std::endl;
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::afficher_Fichier(fs::path const& nomDossier)                                                                                          #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::afficher_Fichier(fs::path const& nomFichier)
{
    auto nf = fs::path(nomFichier).wstring();
    std::wcout << L"nf=[" << nf << L']' << std::endl;
    auto pos = nf.find_last_of(L"\\");
    pos++;
    auto t = nf.substr(pos);

    if (t == L"_you_.txt")
    {
        std::wcout << L"_you_.txt ok !!!" << std::endl;
        return;
    }

    pos = t.length();
    auto filename = t;
    t = t.substr(0, pos - 4);
    //pos = pos - 4;
    pos -= 4;
    std::wcout << L"pos=" << pos << std::endl;
    if (!(std::isdigit(t[0])))
    {
        // Avec
        if (filename == L"Avec.txt")
        {
            std::wcout << L"----> Avec.txt" << std::endl;
            afficher_Avec(nomFichier);
            return;
        }
        // Chaîne d'origine -
        if (filename == L"Chaîne d'origine.txt")
        {
            //afficher_Chaine(filename, nomFichier, d_chaine[I]);
            return;
        }
        // DVD
        if (filename == L"DVD.txt")
        {
            //D_DVD[I] = true;
            return;
        }
        // Note
        if (filename == L"Note.txt")
        {
            //afficher_Note(t, nomFichier, I);
            return;
        }
        // Titre
        if (filename == L"Titre.txt")
        {
        }
        return;
        // Image
        //if (nomImage == JGP_ || nomImage == PNG_ || nomImage == WEBP_)
        //{
            //d_image[I]
            //i = afficher_Image(I, nomFichier);
    //            ::afficher_Image(nomFichier, d_image[I]);
        //    return;
        //}
        // Erreur !
        if (t != L"")
        {
            //E.afficher_X(-1, nomFichier, L'{' + t + L".txt} !!!");
            //return EXIT_FAILURE;
        }
    }
    else
    {
        //
        if (
            (t[0] == L'1' || t[0] == L'2' || t[0] == L'3' || t[0] == L'4' || t[0] == L'5' || t[0] == L'6' || t[0] == L'7' || t[0] == L'8' || t[0] == L'9')
            && t[1] == L'x'
            )
        {
            afficher_Date_ou_Dates(nomFichier);
        }
        if (
            (t[0] == L'1' || t[0] == L'2' || t[0] == L'3' || t[0] == L'4' || t[0] == L'5' || t[0] == L'6' || t[0] == L'7' || t[0] == L'8' || t[0] == L'9')
            &&
            (std::isdigit(t[1]))
            && t[2] == L'x'
            )
        {
            afficher_Date_ou_Dates(nomFichier);
        }
        if (int j = std::stoi(t, 0))
        {
            //std::wstring t2 = std::to_wstring(j);
            auto t2 = std::to_wstring(j);
            if (t == t2)
            {
                std::wcout << L"Episodes !!! ok !!!" << std::endl;
                return;
            }
        }
    }
}


const void afficher_Avec(fs::path const& nomFichier)
{
    //assert((a_filename == createdBy_filename) && L"Erreur !!! Avec... !");
    auto nf = fs::path(nomFichier).wstring();
    std::wcout << L"nf=" << nf << std::endl;
    assert(nf.length() > 0 && L"Nom de fichier vide");
    avec = lire_paireCleValeur_depuisFichierTxt(nf, L" : ");
    assert((avec.size() != 0));
}


// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::afficher_Episodes(fs::path const& nomFichier)                                                                                         #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::afficher_Episodes(fs::path const& nomFichier)
{
    auto nf = fs::path(nomFichier).wstring();
    assert(nf.length() > 0 && L"Nom de fichier vide");

    std::wcout << L"nf=" << nf << std::endl;

    std::size_t pos;
    pos = nf.find_last_of(L"\\");
    pos++;
    std::wstring wstr = nf.substr(pos);

    episodes.first = std::stoi(wstr, &pos);
    std::wcout << L"episodes.first=[" << episodes.first << L']' << std::endl;

    std::wcout << L"ddd [" << fs::current_path() << L']' << std::endl;
    try
    {
        wstr = lire_fichierTxt(std::wstring(nomFichier));
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

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::afficher_Titres(fs::path const& nomFichier)                                                                                           #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::afficher_Titres(fs::path const& nomFichier)
{
    std::wcout << L"===> afficher_Titre() = nomFichier=[" << nomFichier << L']' << std::endl;
    //system("PAUSE");
    auto nf = fs::path(nomFichier).wstring();
    if (nf.length() == 0)
        return;

    std::vector<std::wstring> t = lire_fichierTxt(nf, {L"\n"});
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
    titres.push_back(make_tuple(x, t1, t2, t3, tm_temps, t[2]));
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Saison::afficher_Date_ou_Dates(fs::path const& nomFichier)                                                                                    #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Saison::afficher_Date_ou_Dates(fs::path const& nomFichier)
{
    auto nf = fs::path(nomFichier).wstring();
    assert(nf.length() > 0 && L"Nom de fichier vide");
    std::wcout << L"nf=[" << nf << L']' << std::endl;
    std::size_t pos;
    pos = nf.find_last_of(L"\\");
    pos++;
    std::wstring strRestant = nf.substr(pos);
    pos = strRestant.length();
    strRestant = strRestant.substr(0, pos - 4);
    std::wcout << L"ttt [" << strRestant << L']' << std::endl;
    pos = strRestant.length();
    std::wcout << L"uuuu [" << strRestant << L']' << std::endl;
    assert((strRestant.length() < (9 + episodes.first + 1)) && L"Nom de fichier trop court pour avoir au moins une date");

    //std::wcout << L"aaa" << std::endl;
    pos = 0;
    int x = std::stoi(strRestant, &pos);
    //if (saison_x == x && x >= 1000)
    if(episodes.first == x && x >= 1000)
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
    date_ou_dates.push_back(make_tuple(x, dr, streaming));
    afficher_Titres(nomFichier);
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # void Serie::PrintDate_ou_Dates()                                                                                                                   #
// #                                                                                                                                                    #
// ######################################################################################################################################################

void Serie::PrintDate_ou_Dates()
{
    if (affichage_Date_ou_Dates /* && dates.size() > 0*/)
    {
        std::vector<std::wstring>keyColor{ L"\x1b[94;1m", L"\x1b[38;2;0;255;0m" }; // keyColor[0] (bleu) et keyColor[1] (vert)
        std::wstring valuesColor = L"\x1b[38;2;255;255;255m"; // Blanc
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


int wmain(int argc, wchar_t* argv[])
{

    //int i = 0;
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

    std::wstring t;
    const wchar_t* p{ L".\\Exemple.txt" };
    std::filesystem::path filePath(p);
    try
    {
        t = lire_fichierTxt(filePath);
    }
    catch (runtime_error const& exception)
    {
        std::wcout << L"Erreur : " << exception.what() << std::endl;
    }
    const std::wstring racine = t + L"Better Call Saul.[2015-2022]";
    std::wcout << L"racine=[" << racine << L']' << std::endl;
    Serie serie(racine);
    std::wcout << L"serie.racine=[" << serie.getRacine() << L"]" << std::endl;
    //system("PAUSE");
    std::wcout << L"directory_iterator :" << std::endl;
    for (const auto& entry_serie : fs::directory_iterator((serie.getRacine())))
    {
        std::wcout << L"[" << entry_serie << L']' << std::endl;
        if (entry_serie.is_regular_file())
        {
            std::wcout << L"===> is_regular_file(1)=[" << entry_serie << L']' << std::endl;
            std::wcout << std::endl;
            continue;
        }
        if (fs::is_directory(entry_serie))
        {
            Saison saison;
            auto const& nomDossier = entry_serie.path().filename();
            saison.afficher_Dossier(nomDossier);
            for (const auto& entry_saison : fs::directory_iterator(entry_serie))
            {
                auto const& nomFichier = entry_saison.path().filename();
                std::wcout << L"nomFichier=[" << nomFichier << L']' << std::endl;
                if (entry_saison.is_regular_file() && saison.episode == 0) 
                {
                    saison.afficher_Episodes(entry_saison);
                    saison.episode = 1;
                    std::wcout << L"Episodes ok !!!" << std::endl;
                    //system("PAUSE");
                    std::wcout << std::endl;
                    break;
                }
                if (saison.episode == 0)
                {
                    std::wcout << L"Pas d'episodes !!!" << std::endl;
                    exit(1);
                }
            }
            for (const auto& entry_saison : fs::directory_iterator(entry_serie))
            {
                if (entry_saison.is_regular_file())
                {
                    std::wcout << L"===> is_regular_file(2)=[" << entry_saison << L']' << std::endl;
                    saison.afficher_Fichier(entry_saison);
                }
            }
            std::wcout << std::endl;
            std::wcout << std::endl;
            std::wcout << L"saison :" << std::endl;
            auto s = saison.titres.size(); //to get the size of the vector
            std::tm j;
            for (auto i = 0; i < s; i++)
            { //to print the elements stored in vector of tuples, vec
                std::wcout << i << std::endl;
                std::wcout << L"__x    =" << get<0>(saison.titres[i]) << std::endl;
                std::wcout << L"__t1   =[" << get<1>(saison.titres[i]) << L"]" << std::endl;
                std::wcout << L"__t2   =[" << get<2>(saison.titres[i]) << L"]" << std::endl;
                std::wcout << L"__t3   =[" << get<3>(saison.titres[i]) << L"]" << std::endl;
                j = get<4>(saison.titres[i]);
                std::wcout << L"__temps=[" << j.tm_hour * 1 + j.tm_min << L"]" << std::endl;
                std::wcout << L"__p    =[" << get<5>(saison.titres[i]) << L"]" << std::endl;
            }
            serie.saisons.push_back(saison);
            saison.~saison();
            std::wcout << L"saison ok !!!" << std::endl;
        }
    }
    std::wcout << L"Ok !!!" << std::endl;
    return EXIT_SUCCESS;
}
