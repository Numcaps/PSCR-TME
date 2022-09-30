#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include <vector>
#include <string>

int main()
{
	using namespace std;
	using namespace std::chrono;

	ifstream input = ifstream("/users/nfs/Etu5/3809215/Documents/PSCR/PSCR-TME/TME2/WarAndPeace.txt");

	auto start = steady_clock::now();
	cout << "Parsing War and Peace" << endl;

	size_t nombre_lu = 0;
	// prochain mot lu
	string word;
	// une regex qui reconnait les caractères anormaux (négation des lettres)
	regex re(R"([^a-zA-Z])");

	// Q1
	vector<string> encountered_words;
	int count_diff_num = 0;

	// Q2
	vector<pair<string,int>> occur_count;


	while (input >> word)
	{
		// élimine la ponctuation et les caractères spéciaux
		word = regex_replace(word, re, "");
		// passe en lowercase
		transform(word.begin(), word.end(), word.begin(), ::tolower);

		// word est maintenant "tout propre"
		/*if (nombre_lu % 100 == 0)
			// on affiche un mot "propre" sur 100
			cout << nombre_lu << ": " << word << endl;*/
		nombre_lu++;

		vector<string>::iterator it;
		for (it = encountered_words.begin(); it != encountered_words.end(); ++it)
		{
			if (word == *it)
			{
				occur_count.push_back()
				break;
			}	
		}

		if (it == encountered_words.end())
		{
			// ajout de tout les mots dans le vecteur
			encountered_words.push_back(word);
			++count_diff_num;
		}
	}
	

	input.close();

	cout << "Finished Parsing War and Peace" << endl;

	auto end = steady_clock::now();
	cout << "Parsing took "
		 << duration_cast<milliseconds>(end - start).count()
		 << "ms.\n";

	cout << "Found a total of " << nombre_lu << " words." << endl;
	cout << "Count of different words : " << count_diff_num << endl;
	return 0;
}
