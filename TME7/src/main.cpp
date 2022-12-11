#include "Shared.h"
#include <sys/mman.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
/**
 * OK?
 * sem nomme oblige pointeur
 * chaque proc cree un shared_res dans l'espace partage ?
*/
int * sp;
int main(int argc, char const *argv[])
{
    int fd;
    /* Creer le segment monshm, ouverture en R/W */
    if ((fd = shm_open("/monshm", O_RDWR | O_CREAT, 0600)) == -1)
    {
        perror("shm_open");
        exit(1);
    }
    /* Allouer au segment une taille pour stocker un entier */
    if (ftruncate(fd, sizeof(Shared_result<int>)) == -1)
    {
        perror("ftruncate");
        exit(1);
    }
    /* “mapper” le segment en R/W partagé */
    if ((sp = (int *)mmap(NULL, sizeof(Shared_result<int>), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }
    

    Shared_result<int> * sr = new (sp)Shared_result<int>();
    if (fork()==0)
    {
        std::cout <<"fils"<< *(sr->get_result())<< std::endl;
    }
    
    sr->set_result(5);
    std::cout << "pere"<<*(sr->get_result())<< std::endl;
    return 0;
}
