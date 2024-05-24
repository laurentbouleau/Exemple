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
#include <map>
//#include <stdio.h>
#include <locale>
#include <algorithm>
#include <codecvt>
//#include <sstream>
#include <regex>

#include <filesystem> // C++17 standard header file name
//using namespace std;

void initialiser_Avec(std::filesystem::path const& m_cheminFichier, std::vector<std::pair<std::wstring, std::wstring>>& m_avec);
void initialiser_Image(std::filesystem::path const& m_cheminFichier, std::vector<std::wstring>& m_images);
//void initialiser_Titre_Original(std::filesystem::path const& m_cheminFichier, std::vector<std::wstring>& m_titre_original);
