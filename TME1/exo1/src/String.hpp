/*
 * String.hpp
 *
 *  Created on: 26 sept. 2022
 *      Author: num
 */

#ifndef STRING_HPP_
#define STRING_HPP_

#include <cstddef>
#include <iostream>

class String  {
private :
	const char * str;

public :
	String(const char * s); // constructeur
	String(const String& s); // constructeur par copie
	size_t length() const;
	friend std::ostream & operator<<(std::ostream& os, const String& s);
	String& operator=(const String& s1);
	~String(); // destructeur
};

std::ostream & operator<<(std::ostream& os, const String& s);


#endif /* STRING_HPP_ */
