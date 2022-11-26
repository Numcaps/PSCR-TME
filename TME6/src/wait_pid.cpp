#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <chrono>

int wait_till_pid(pid_t pid)
{
    bool sleep_more = true;
    pid_t fils;
    while (sleep_more)
    {
        fils = wait(nullptr);
        if (fils == pid)
        {
            return pid;
        }
        if (fils == -1)
        {
            break;
        }
    }
    return -1;
}

int wait_till_pid(pid_t pid, int sec)
{
    using namespace std::chrono;
    pid_t watchdog;
    /* Watchdog */
    if ((watchdog = fork()) == 0)
    {
        auto t1 = high_resolution_clock::now();
        auto t2 = high_resolution_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
        while (time_span.count() < sec)
        {
            t2 = high_resolution_clock::now();
            time_span = duration_cast<duration<double>>(t2 - t1);
        }
        exit(0);
    }
    /* ------------------------------------------------------------------------ */
    bool sleep_more = true;
    pid_t fils;
    while (sleep_more)
    {
        fils = wait(nullptr);
        if (fils == pid)
        {
            return pid;
        }
        if (fils == -1)
        {
            return -1;
        }
        if (fils == watchdog)
        {
            break;
        }
    }
    return 0;
}

void handler(int signum, siginfo_t *info, void *ucontext)
{
    switch (signum)
    {
    case SIGALRM:
        std::cout << "KO ! " << info->si_pid << std::endl;
        exit(0);
        break;
    case SIGCHLD:

        if (info->si_status == 4)
        {
            std::cout << "OK ! " << info->si_pid << std::endl;
            exit(info->si_pid);
        }

        break;
    default:
        break;
    }
}
void wait_till_pid_alarm(pid_t pid, int sec)
{
    struct sigaction act;
    sigset_t sig_proc;

    /* Initialise a 0 */
    sigemptyset(&sig_proc);

    /* Mise en place du handler */
    act.sa_handler = nullptr;
    act.sa_sigaction = handler;
    act.sa_flags = SA_SIGINFO;
    act.sa_mask = sig_proc;
    sigaction(SIGALRM, &act, nullptr);
    sigaction(SIGCHLD, &act, nullptr);

    /* Alarm */
    alarm(sec);

    while (true)
        pause();
}
int main(int argc, char const *argv[])
{
    pid_t fils, cible;

    for (int i = 0; i < 5; i++)
    {
        if ((fils = fork()) == 0)
        {
            if (i == 2)
            {
                sleep(3);
                exit(4);
            }

            exit(0);
        }
        if (i == 2)
        {
            cible = fils;
        }
    }
    wait_till_pid_alarm(cible, 1);
    /* cible = wait_till_pid(cible, 3);
    if (cible > 0)
    {
        std::cout << "OK !" << cible << std::endl;
    }
    else
    {
        std::cout << "KO !" << cible << std::endl;
    } */
    return 0;
}
