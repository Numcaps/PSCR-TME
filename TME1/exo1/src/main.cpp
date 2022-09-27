#include "List.h"
#include <string>
#include <iostream>
#include <cstring>
#include "TD1.hpp"
#include "String.hpp"

int main() {

	std::string abc = "abc";
	//char * str = new char [3]; // FAUTE : omission '\0' donc taille du tableau = 4
	char *str = new char[4];
	str[0] = 'a';
	str[1] = 'b';
	str[2] = 'c';
	str[3] = '\0';
	int i = 0;
	//size_t i = 0; // FAUTE : mauvais type car dans la boucle i doit etre
	// decremente jusqu'a qu'il soit negatif

	if (!strcmp(str, abc.c_str())) {
		std::cout << "Equal !";
	}

	pr::List list;
	list.push_front(abc);
	list.push_front(abc);

	std::cout << "Liste : " << list << std::endl;
	std::cout << "Taille : " << list.size() << std::endl;

	// Affiche à l'envers
	for (i = list.size() - 1; i >= 0; i--) {
		std::cout << "elt " << i << ": " << list[i] << std::endl;
	}

	// FAUTE : str est un tableau de char et donc seul la liberation du pointeur
	// est necessaire
	// liberer les char de la chaine
	//	for (char *cp = str ; *cp ; cp++) {
	//		delete cp;
	//	}
	// et la chaine elle meme
	delete[] str; // FAUTE : omission [] car str est un tableau

	// Test TD1
	const char *hw = "Hello World!";
	const char *bw = "Bye World!";
	std::cout << "Comparaison hw et bw : "<<pr::compare(hw, bw) << std::endl;
	std::cout << "Comparaison bw et hw : "<<pr::compare(bw, hw) << std::endl;
	std::cout << "Comparaison hw et hw : "<<pr::compare(hw, hw) << std::endl;
	bw = "";
	std::cout << "Comparaison bw null terminated only : "<<pr::compare(bw, bw) << std::endl;
	std::cout << pr::length(hw);
	char *news = pr::newcopy(hw);
	std::cout << news << std::endl;;
	delete[] news;


	// Test String class
	String s1("Hello World!"); // appel constructeur a un arg
	String s2("Bip Bip Boup Boup !");
	String s3(s2); // appel constructeur par copie
	std::cout << "Affichage de s1 : "<< s1 << std::endl; // impression grace a redef de <<
	std::cout << "Affichage de s2 et s3 : " << s2 << ", " << s3 <<std::endl;
	std::cout << "Affichage des adresses de s2 et s3 : "<< &s2 << ", " << &s3<<std::endl;
	s3 = s1;
	std::cout << "Affichage de s3 apres affectation avec s1 : " << s3 <<std::endl;
	std::cout << "Affichage des adresses de s1 et s3 : "<< &s1 << ", " << &s3<<std::endl;
	std::cout << "Test d'egalite entre s3 et s1 : " << (s3 == s1) << std::endl;
	std::cout << "Test d'egalite entre s3 et s2 : " << (s3 == s2) << std::endl;
	std::cout << "Test relation d'ordre < entre s1 et s2 : " << (s1<s2) << std::endl;
	std::cout << "Test relation d'ordre < entre s2 et s1 : " << (s2<s1) << std::endl;


	return 0;

}
