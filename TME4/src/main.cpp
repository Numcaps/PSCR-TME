#include "Banque.h"

using namespace std;

const int NB_THREAD = 10;
int SOLDEINITIAL = 50;
size_t K = 10; /* Nombre de comptes*/

void Q1(pr::Banque &b)
{
	size_t i = rand() % 99 + 1;
	size_t j = rand() % 99 + 1;
	size_t m = rand() % 99 + 1;
	for (size_t c = 0; c < 1000; c++)
	{
		b.transfert(i, j, m);
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
}
int main()
{
	/* Creation de la banque */
	pr::Banque b(K, SOLDEINITIAL);

	vector<thread> threads;
	// TODO : creer des threads qui font ce qui est demandé
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
