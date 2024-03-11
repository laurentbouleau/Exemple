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

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const void afficher_Avec(fs::path const& cheminFichier, std::vector<std::pair<std::wstring, std::wstring>>& avec)                                  #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const void afficher_Avec(fs::path const& cheminFichier, std::vector<std::pair<std::wstring, std::wstring>>& avec)
{
    auto nomFichier = cheminFichier.filename().wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    avec = lire_paireCleValeur_depuisFichierTxt(cheminFichier.wstring(), L" : ");
    assert((avec.size() != 0));
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const void afficher_Chaine(fs::path const& cheminFichier, std::wstring& chaine)                                                                    #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const void afficher_Chaine(fs::path const& cheminFichier, std::wstring& chaine)
{ // Chaîne
    auto nomFichier = cheminFichier.filename().wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    chaine = lire_fichierTxt(cheminFichier.wstring());
    assert((chaine.size() != 0));
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const void afficher_Image(fs::path const& cheminFichier, std::vector<std::wstring>& images)                                                        #
// #                                                                                                                                                    #
// ######################################################################################################################################################

const void afficher_Image(fs::path const& cheminFichier, std::vector<std::wstring>& images)
{
    auto nomFichier = cheminFichier.filename().wstring();
    assert(nomFichier.length() > 0 && L"Nom de fichier vide");
    //std::size_t pos_txt = nf.find_last_of(L"\\");
    //assert((pos_txt != std::wstring::npos) && L"Erreur Image(s) !!!");
    //pos_txt++;
    //images.push_back(nomFichier);
    images.push_back(nomFichier);
}

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const bool afficher_Min(std::wstring& t)                                                                                                           #
// #                                                                                                                                                    #
// ######################################################################################################################################################

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

// ######################################################################################################################################################
// #                                                                                                                                                    #
// # const bool afficher_Temps(std::wstring ttt)                                                                                                        #
// #                                                                                                                                                    #
// ######################################################################################################################################################

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