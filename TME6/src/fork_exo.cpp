#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>

int main()
{
	const int N = 3;
	int cpt = 1;
	int count_i;
	
	std::cout << "main pid=" << getpid() << std::endl;

	for (int i = 1, j = N; i <= N && j == N && fork() == 0; i++)
	{
		cpt = 0;
		std::cout << " i:j " << i << ":" << j << std::endl;
		for (int k = 1; k <= i && j == N; k++)
		{
			cpt++;
			if (fork() == 0)
			{
				cpt = 0;
				j = 0;
				std::cout << " k:j " << k << ":" << j << std::endl;
			}
		}
		cpt++;
		count_i = i;
	}
	/**
	 * A la derniere iteration, le fork n'est execute
	 * a cause de l'evaluation paresseuse
	*/
	if (count_i==N)
	{
		cpt--; 

	}
	
	
	std::string k ;
	std::cin >> k;
	std::cout << cpt << " " << getpid() << std::endl;
	for (int i = 0; i < cpt; i++)
	{
		int status;
		wait(&status);
	}

	return 0;
}
