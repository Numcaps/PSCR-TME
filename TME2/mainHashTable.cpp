#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include <vector>
#include <string>
#include "HashTable.hpp"

int main()
{
    using namespace std;
    using namespace std::chrono;

    ifstream input = ifstream("/home/num/Documents/PSCR-TME/TME2/WarAndPeace.txt");
    ofstream MyFile("filename.txt");

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
    pr::HashTable<string, int> occur_count(5);

    while (input >> word)
    {
        //std::cout << occur_count.capacity()<< std::endl;
        // élimine la ponctuation et les caractères spéciaux
        word = regex_replace(word, re, "");
        // passe en lowercase
        transform(word.begin(), word.end(), word.begin(), ::tolower);
        MyFile<<word<<" ";
        // word est maintenant "tout propre"
        /*if (nombre_lu % 100 == 0)
            // on affiche un mot "propre" sur 100
            std::cout << nombre_lu << ": " << word << endl;*/
        nombre_lu++;

        int *val = occur_count.get(word);
        // ajout du mot dans la table
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

    std::cout << "Finished Parsing War and Peace" << endl;

    occur_count.print();

    auto end = steady_clock::now();
    std::cout << "Parsing took "
              << duration_cast<milliseconds>(end - start).count()
              << "ms.\n";

    std::cout << "Found a total of " << nombre_lu << " words." << endl;
    std::cout << "Count of different words : " << count_diff_num << endl;

    // parcours du vecteur pour trouver "war", "peace", "toto"

    std::cout << "Occurences of \"war\" : " << *(occur_count.get("war")) << endl;
    std::cout << "Occurences of \"peace\" : " << *(occur_count.get("peace")) << endl;
    int *a = occur_count.get("toto");
    size_t b = a ? *a : 0;
    std::cout << "Occurences of \"toto\" : " << b << endl;
    std::cout << "Table capacity : " << occur_count.capacity() << std::endl;    // vecteur des entrees
    std::vector<pair<string,int>> v_entry ;
    
    return 0;
}
