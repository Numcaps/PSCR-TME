
#include "List.h" // FAUTE : omission de fichier d'en-tete
namespace pr {

// ******************* Chainon
Chainon::Chainon (const std::string & data, Chainon * next):data(data),next(next) {};

size_t Chainon::length() {
	size_t len = 1;
	if (next != nullptr) {
		len += next->length();
	}
	return length();
}

void Chainon::print (std::ostream & os) const { // FAUTE : omission de "const"
	os << data ;
	if (next != nullptr) {
		os << ", ";
	}
	next->print(os);
}

// ******************  List
const std::string & List::operator[] (size_t index) const  {
	Chainon * it = tete;
	for (size_t i=0; i < index ; i++) {
		it = it->next;
	}
	return it->data;
}

void List::push_back (const std::string& val) {
	if (tete == nullptr) {
		tete = new Chainon(val);
	} else {
		Chainon * fin = tete;
		while (fin->next) {
			fin = fin->next;
		}
		fin->next = new Chainon(val);
	}
}

//void List::push_front (const std::string& val) { // FAUTE : declaration superflue
//	tete = new Chainon(val,tete);
//}

bool List::empty() { // FAUTE : omission de l'opérateur de resolution de portee
	return tete == nullptr;
}

size_t List::size() const {
	if (tete == nullptr) {
		return 0;
	} else {
		return tete->length();
	}
}

} // namespace pr

std::ostream & pr::operator<< (std::ostream & os, const pr::List & vec) // FAUTE : omission operateur de resolution de portee PR
{
	os << "[";
	if (vec.tete != nullptr) {
		vec.tete->print (os) ;
	}
	os << "]";
	return os;
}

