#include <unistd.h>
#include <signal.h>
#include <cstdlib>
#include <sys/types.h>
#include "rsleep.h"
#include <iostream>
/* Nous allons construire une application où deux processus s'envoient respectivement des signaux avec
kill pour simuler un combat (Vador et Luke). Le principe est que chaque combattant (processus)
alterne entre une phase de défense où il se protège (en ignorant les signaux), et une phase d'attaque
où il envoie un signal (coup de sabre laser) à l'adversaire (attaque) mais devient en contrepartie
vulnérable aux signaux pendant un moment.
On va modéliser également des points de vie, c'est-à-dire que le fait de recevoir un signal quand
on est vulnérable (dans sa phase d'attaque) va décrémenter un compteur (initialement trois vies).
Quand le compteur atteint 0, le processus meurt et rend la valeur 1. Quand l'attaquant détecte la
mort de son adversaire, c'est-à-dire que son pid n'existe plus (ce qui cause une exception sur kill),
il meurt également et rend la valeur 0.
Le temps passé en phase de défense (signal masqué) ou d'attaque (signal reçu) est aléatoire et
compris entre 0.3 et 1 seconde.

Défnir les trois fonctions suivantes :
• void attaque (pid_t adversaire) :
 La phase d'attaque commence par installer un gestionnaire pour le signal SIGINT, qui
décrémente les  points de vie  du processus et affiche le nombre de points restants. Si
0 est atteint il affiche que le processus se termine, et celui-ci se termine en retournant 1.
 Ensuite le processus envoie un signal SIGINT à l'adversaire ; si cette invocation échoue,
on suppose que l'adversaire a déjà perdu, et le processus sort avec la valeur 0 ;
 Ensuite le processus s'endort pour une durée aléatoire.

• void defense()
 La phase de défense consiste à désarmer le signal SIGINT en positionnant son action à
SIG_IGN ;
 Ensuite le processus s'endort pour une durée aléatoire.

• void combat(pid_t adversaire) : boucle indéfniment sur une défense suivie d'une attaque et
invoquez-la dans le corps des deux fils. */

/**
 * Points de vie du proc (var global)
 */
int pdv = 3;

void handler_attaque(int signum)
{
    switch (signum)
    {
    case SIGCHLD:
        /* NOP */
        break;
    case SIGINT:
        pdv--;
        std::cout << getpid() << " se fait attaquer ! " << pdv << " points de vie restant." << std::endl;
        if (!pdv)
        {
            std::cout << "Fin du processus " << getpid() << " ..." << std::endl;
            exit(1);
        }
    default:
        break;
    }
}

void handler_defense(int signum)
{
    std::cout << getpid() << " se defend !" << std::endl;
}

void attaque(pid_t adversaire)
{
    /**
     * Mise en place du gestionnaire de signal
     */
    struct sigaction act;
    sigset_t sig_proc;

    /* Initialise a 0 */
    sigemptyset(&sig_proc);

    /* Mise en place du handler */
    act.sa_handler = handler_attaque;
    act.sa_flags = SA_NOCLDWAIT;
    act.sa_mask = sig_proc;
    sigaction(SIGINT, &act, nullptr);
    /**
     * Necessaire pour eviter que un des processus reste
     * zombie et bloque celui vivant (car attend un signal
     * d'un proc deja mort)
     */
    sigaction(SIGCHLD, &act, nullptr);
    //std::cout << getpid() << " attaque !" << std::endl;
    if (kill(adversaire, SIGINT) < 0)
    {
        std::cout << "L'adversaire de " << getpid() << " est mort. "
                  << getpid() << " est le gagnant !" << std::endl;
        exit(0);
    }

    randsleep();
}

void defense()
{
    struct sigaction act;
    sigset_t sig_proc;

    /* Initialise a 0 */
    sigemptyset(&sig_proc);

    /* Mise en place de handler_defense */
    act.sa_handler = SIG_IGN;
    act.sa_flags = 0;
    act.sa_mask = sig_proc;
    sigaction(SIGINT, &act, nullptr);
    //std::cout << getpid() << " se defend (1) !" << std::endl;
    /* Masquage de SIGINT durant le sleep */
    // sigset_t sig_mask;
    // sigemptyset(&sig_mask);
    // sigaddset(&sig_mask, SIGINT);
    // sigprocmask(SIG_SETMASK, &sig_mask, nullptr);
  
    randsleep();
    // sigsuspend(&sig_mask);
    //std::cout << getpid() << " se defend (2) !" << std::endl;
}
void combat(pid_t adversaire)
{
    while (true)
    {
        defense();
        attaque(adversaire);
    }
}

int main(int argc, char const *argv[])
{
    pid_t luke;

    
    if ((luke = fork()) == 0)
    {
        srand(time(NULL) ^ (getpid()<<16));
        combat(getppid());
    }
    srand(time(NULL) ^ (getpid()<<16));
    combat(luke);

    return 0;
}
