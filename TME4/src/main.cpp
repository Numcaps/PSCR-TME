#include "Banque.h"

using namespace std;

const int NB_THREAD = 10;
size_t SOLDEINITIAL = 50;
size_t K = 10; /* Nombre de comptes*/

void Q1(pr::Banque &b, size_t i, size_t j, size_t m)
{
	b.transfert(i, j, m);
	std::this_thread::sleep_for (std::chrono::milliseconds(20));

}
int main()
{
	/* Creation de la banque */
	pr::Banque b(K, SOLDEINITIAL);
	size_t i = rand() % 99 + 1;
	size_t j = rand() % 99 + 1;
	size_t m = rand() % 99 + 1;
	vector<thread> threads;
	// TODO : creer des threads qui font ce qui est demandé
	for (size_t c = 0; c < 1000; c++)
	{
		threads.emplace_back(Q1, std::ref(b), i, j, m);
	}

	for (auto &t : threads)
	{
		t.join();
	}

	// TODO : tester solde = NB_THREAD * JP
	return 0;
}
