#include <fcntl.h>    /* For O_* constants */
#include <sys/stat.h> /* For mode constants */
#include <semaphore.h>
/**
 * Le but est de faire un round robin cf photo tablette
 * Exo a la place de Stack.h traite en cours
 * Dans un premier tant on utilise shared mem anonymous
 * puis named
 * idee de future/promise
 */

/* Classe Mult. Proc. Safe*/
// template <typename T>
// class Shared_result
// {

// private:
//     /* Protection contre les acces multi. proc. */
//     sem_t locker_;
//     T var_;

// public:
//     Shared_result();
//     ~Shared_result();
//     inline void set_result(const T &v)
//     {
//         sem_post(&locker_);
//         var_ = v;
//     }
//     inline T * get_result()
//     {
//         sem_wait(&locker_);
//         return &var_;
//     }
// };
// template <typename T>
// Shared_result<T>::Shared_result()
// {
//     /* Creation semaphore anonyme */
//     sem_init(&locker_, 1, 0);
// }

// template <typename T>
// Shared_result<T>::~Shared_result()
// {
//     sem_destroy(&locker_);
// }

template <typename T>
class Shared_result
{

private:
    /* Protection contre les acces multi. proc. */
    sem_t * locker_;
    T var_;

public:
    Shared_result();
    ~Shared_result();
    inline void set_result(const T &v)
    {
        sem_post(locker_);
        var_ = v;
    }
    inline T * get_result()
    {
        sem_wait(locker_);
        return &var_;
    }
};
template <typename T>
Shared_result<T>::Shared_result()
{
    /* Creation semaphore nomme */
    locker_ = sem_open("/monsem" , O_CREAT | O_RDWR, 0600, 0);
}

template <typename T>
Shared_result<T>::~Shared_result()
{
    sem_close(locker_);
    sem_unlink("/monsem");
}