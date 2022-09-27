/*
 * String.cpp
 *
 *  Created on: 26 sept. 2022
 *      Author: num
 */
#include "String.hpp"
#include  "TD1.hpp"

String::String(const char * s)
{
	str = pr::newcopy(s);
} //constructeur

String::String(const String& s)
{
	str = pr::newcopy(s.str);
}

size_t String::length() const
{
	return pr::length(this->str);
}

std::ostream & operator<<(std::ostream& os, const String& s)
{
	return std::cout << s.str;
}

String& String::operator=(const String& s1)
{
	if (this == &s1) return *this; // pour eviter s=s
	delete [] str;
	str = pr::newcopy(s1.str);
	return *this;

}

bool String::operator<(const String& a) const
{
	return pr::compare(this->str, a.str) < 0 ? true : false;
}

bool operator==(const String& a, const String& b)
{
	return pr::compare(a.str, b.str) == 0 ? true : false;
}

 String::~String()
 {
	 delete [] str;
 }
