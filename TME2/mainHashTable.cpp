#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include <vector>
#include <string>
#include <algorithm>
#include "HashTable.hpp"

int main()
{
    using namespace std;
    using namespace std::chrono;

    ifstream input = ifstream("/home/num/Documents/PSCR-TME/TME2/WarAndPeace.txt");
    auto start = steady_clock::now();
    std::cout << "Parsing War and Peace" << endl;

    size_t nombre_lu = 0;
    // prochain mot lu
    string word;
    // une regex qui reconnait les caractères anormaux (négation des lettres)
    regex re(R"([^a-zA-Z])");

    // Q1

    int count_diff_num = 0;

    // Q10
    pr::HashTable<string, int> occur_count(1000);

    std::cout << "-----Starting text parsing-----" << std::endl;
    while (input >> word)
    {
        // std::cout << "Iteration number :" << nombre_lu <<std::endl;
        //  std::cout << occur_count.capacity()<< std::endl;
        //   élimine la ponctuation et les caractères spéciaux
        word = regex_replace(word, re, "");
        // passe en lowercase
        transform(word.begin(), word.end(), word.begin(), ::tolower);

        nombre_lu++;

        int *val = occur_count.get(word);

        // ajout du mot dans la table
        // if (occur_count.size() >= 0.8 * occur_count.capacity())
        //      occur_count.grow(); // on test si la taille de la table est inferieur a sa capacite
        //  sinon on l'agrandit
        if (val) // si word est deja dans la table
        {
            occur_count.put(word, ++(*val));
        }
        else
        {
            occur_count.put(word, 1);
            ++count_diff_num;
        }
    }

    input.close();
    std::cout << "-----Finish text parsing-----" << std::endl;
    auto end = steady_clock::now();
    std::cout << "Parsing took "
              << duration_cast<milliseconds>(end - start).count()
              << "ms.\n";

    std::cout << "Found a total of " << nombre_lu << " words." << endl;
    std::cout << "Count of different words : " << count_diff_num << endl;

    // parcours du vecteur pour trouver "war", "peace", "toto"
    std::cout << "-----Starting count occurences-----\n\n";
    std::cout << "Occurences of \"war\" : " << *(occur_count.get("war")) << endl;
    std::cout << "Occurences of \"peace\" : " << *(occur_count.get("peace")) << endl;
    int *a = occur_count.get("toto");
    size_t b = a ? *a : 0;
    std::cout << "Occurences of \"toto\" : " << b << endl;
    std::cout << "Table capacity : " << occur_count.capacity() << std::endl; // vecteur des entrees
    std::cout << "-----Finish count occurences-----\n\n";

    
    // recopie du contenu de HT dans le vector
    std::cout << "-----Starting copy HT-----\n\n";
    std::cout << "RECOPIE" << std::endl;
    std::vector<pair<string, int>> v_entry;
    pr::HashTable<string, int>::Iterator it = occur_count.begin();

    auto e = occur_count.end();
    for (; it != e; ++it)
    {
        v_entry.push_back(pair<string, int>((*it).getK(), (*it).getV()));
    }

    std::cout << "-----Finish count HT-----\n\n";
    std::cout << v_entry.size() << std::endl;
    std::cout << occur_count.size() << std::endl;

    // Tri du vector
    std::sort(v_entry.begin(), v_entry.end(), 
    [](pair<string, int> & a, pair<string, int> & b){
        return a.second>b.second;
    });
    for (auto it = v_entry.begin(); it != v_entry.end(); ++it)
    {
        std::cout << "["<< it->first << ", " << it->second << " ]";
    }
    std::cout << std::endl;
    return 0;
}
