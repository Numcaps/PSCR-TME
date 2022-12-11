#include <unistd.h>
#include <iostream>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{

    /* Les fd sont conserves apres un exec */
    if (argc < 2)
        perror("Missing arguments !");

    char *command1[256];
    char *command2[256];
    /* Creation du tableau contenant les fd tab[0]=read tab[1]=write */
    int pipefd[2];
    int oldfd, newfd;
    memset(command1, 0, 256 * sizeof(char *));
    memset(command2, 0, 256 * sizeof(char *));
    bool next_com = false;

    /* Lecture des commandes */
    int j = 0;
    for (int i = 1; i < argc; i++)
    {
        if (!strcmp("|", argv[i]))
        {
            next_com = true;
            continue;
        }
        if (next_com)
        {
            command2[j] = argv[i];
            j++;
        }
        else
        {
            command1[i - 1] = argv[i];
        }
    }
    std::cout << "[DEBUG] command1 : ";
    for (int i = 0; command1[i] != nullptr; i++)
    {
        std::cout << command1[i];
    }
    std::cout << std::endl
              << "[DEBUG] command2 : ";
    for (int i = 0; command2[i] != nullptr; i++)
    {
        std::cout << command2[i];
    }
    std::cout << std::endl;

    /* Creation du tube*/
    std::cout << "Pipe creation ..." << std::endl;
    if (pipe(pipefd) == -1)
    {
        perror("Pipe failed to create !");
    }

    /* Creation du reader */
    if (fork() == 0)
    {
        std::cout << "Reader process created !" << std::endl;
        /* Changement de stdin sur la sortie du pipe */
        oldfd = STDIN_FILENO;
        newfd = pipefd[0];
        dup2(oldfd, newfd);
        /* Execution de la commande 2 */
        execv(command2[0], command2);
        perror("Failed to execv in reader process !");
        exit(1);
    }

    
    if (fork() == 0)
    {
        /* Changement de stdout sur l'entree du pipe */
        oldfd = STDOUT_FILENO;
        newfd = pipefd[1];
        dup2(oldfd, newfd);
        execv(command1[0], command1);
        perror("Failed to execv in writer process ! ");
    }
    /* Attend le writer */
    

    return 0;
}
