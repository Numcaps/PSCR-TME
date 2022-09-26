#include "List.h"
#include <string>
#include <iostream>
#include <cstring>

int main () {

	std::string abc = "abc";
	//char * str = new char [3]; // FAUTE : omission '\0' donc taille du tableau = 4
	char * str = new char[4];
	str[0] = 'a';
	str[1] = 'b';
	str[2] = 'c';
	str[3] = '\0';
	int i = 0;
	//size_t i = 0; // FAUTE : mauvais type car dans la boucle i doit etre
				// decremente jusqu'a qu'il soit negatif

	if (! strcmp (str, abc.c_str())) {
		std::cout << "Equal !";
	}

	pr::List list;
	list.push_front(abc);
	list.push_front(abc);

	std::cout << "Liste : " << list << std::endl;
	std::cout << "Taille : " << list.size() << std::endl;

	// Affiche à l'envers
	for (i= list.size() - 1 ; i >= 0 ; i--) {
		std::cout << "elt " << i << ": " << list[i] << std::endl;
	}

	// FAUTE : str est un tableau de char et donc seul la liberation du pointeur
	// est necessaire
	// liberer les char de la chaine
//	for (char *cp = str ; *cp ; cp++) {
//		delete cp;
//	}
	// et la chaine elle meme
	delete [] str; // FAUTE : omission [] car str est un tableau

}
