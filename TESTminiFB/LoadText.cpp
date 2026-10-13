#include "LoadText.h"
#include "Compilateur.h"

void LoadText::Print()
{
    for (std::string e : text)
    {
        cout << e << endl;
    }
}

std::vector<std::string> LoadText::LoadTxt(const char* _path)
{
    std::ifstream fichier(_path);
    if (!fichier.is_open())
    {
        cout << "Impossible d'ouvrir le fichier" << endl;
    }

    std::string line;
    while (std::getline(fichier, line))
    {
        text.push_back(line);
    }

    //Print();
    //cout << endl;

    return text;
}
