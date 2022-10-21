#include "Banque.h"
#include <iostream>
using namespace std;

const int NB_THREAD = 100;
int SOLDEINITIAL = 50;
size_t K = 2; /* Nombre de comptes*/

void Q1(pr::Banque &b)
{
	size_t i = rand() % K; /* ATTENTION INDICE DOIT CORRESPONDRE AU NOMBRE DE COMPTE */
	size_t j = rand() % K;
	size_t m = rand() % K + 1;
	for (size_t c = 0; c < 10000; c++)
	{
		b.transfert(i, j, m);
		std::cout << "Transfert de " << m << " $, de "<<i<<" a "<< j<<std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 20 + 1));
	}

}

int main()
{
	/* Creation de la banque */
	pr::Banque b(K, SOLDEINITIAL);

	vector<thread> threads;
	//TODO : creer des threads qui font ce qui est demandé
	for (size_t c = 0; c < NB_THREAD; c++)
	{
		threads.emplace_back(Q1, std::ref(b));
	}
	
	for (auto &t : threads)
	{
		t.join();
	}

	// TODO : tester solde = NB_THREAD * JP
	return 0;
}
