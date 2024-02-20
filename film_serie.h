#pragma once
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING


#include "Exemple.h"
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
//using namespace std;

const void afficher_Avec(std::filesystem::path const& cheminFichier, std::vector<std::pair<std::wstring, std::wstring>>& avec);
const void afficher_Image(std::filesystem::path const& cheminFichier, std::vector<std::wstring>& images);
const bool afficher_Min(std::wstring& t);
const bool afficher_Temps(std::wstring ttt);
