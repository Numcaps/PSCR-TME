/*
 * TD1.cpp
 *
 *  Created on: 25 sept. 2022
 *      Author: num
 */

#include "TD1.hpp"
using namespace std;

size_t pr::length(const char * s)
{
	const char * cp = s;
	for(cp=s; *cp; ++cp)
	{
		/*NOP*/
	}
	return cp-s;
}


char * pr::newcopy(const char * s)
{
	size_t sz = pr::length(s);
	char * news = new char[sz+1];
	memcpy(news,s,sz+1);
	return news;
}

int pr::compare(const char * s1, const char * s2)
{
	if(pr::length(s1)<pr::length(s2)) return -1;
	else if (pr::length(s1)>pr::length(s2)) return 1;
	else
	{
		while(*s1!='\0' and (*s1-*s2)==0)
		{
			++s1;
			++s2;
		}
		return *s1-*s2;
	}
}
