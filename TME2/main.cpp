#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include <vector>
#include <string>
#include "utils.hpp"
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
	// vector<string> encountered_words;
	int count_diff_num = 0;

	// Q3
	vector<pair<string, int>> occur_count;

	while (input >> word)
	{
		// élimine la ponctuation et les caractères spéciaux
		word = regex_replace(word, re, "");
		// passe en lowercase
		transform(word.begin(), word.end(), word.begin(), ::tolower);

		// word est maintenant "tout propre"
		/*if (nombre_lu % 100 == 0)
			// on affiche un mot "propre" sur 100
			std::cout << nombre_lu << ": " << word << endl;*/
		nombre_lu++;

		// cherche si la pair est deja presente
		vector<pair<string, int>>::iterator it;
		for (it = occur_count.begin(); it != occur_count.end(); ++it)
		{
			if ((*it).first == word)
			{
				++(*it).second;
				break;
			}
		}

		// la pair n'est pas presente
		if (it == occur_count.end())
		{
			// creation d'une nouvelle pair
			pair<string, int> new_pair(word, 1);

			// insertion de la pair
			occur_count.push_back(new_pair);
			++count_diff_num;
		}
	}

	input.close();

	std::cout << "Finished Parsing War and Peace" << endl;

	auto end = steady_clock::now();
	std::cout << "Parsing took "
		 << duration_cast<milliseconds>(end - start).count()
		 << "ms.\n";

	std::cout << "Found a total of " << nombre_lu << " words." << endl;
	std::cout << "Count of different words : " << count_diff_num << endl;

	// parcour du vecteur pour trouver "war", "peace", "toto"
	vector<pair<string, int>>::iterator it;
	for (it = occur_count.begin(); it != occur_count.end(); ++it)
	{
		if (!strcmp((*it).first.c_str(), "war"))
			std::cout << "Occurences of \"war\" : " << (*it).second << endl;
		else if (!strcmp((*it).first.c_str(), "peace"))
			std::cout << "Occurences of \"peace\" : " << (*it).second << endl;
		else if (!strcmp((*it).first.c_str(), "toto"))
			std::cout << "Occurences of \"toto\" : " << (*it).second << endl;
	}
	
	
	//test des fonctions count et count_if_equal
	std::cout << pr::count(occur_count.begin(),occur_count.end()) << std::endl;
	
	return 0;
}
