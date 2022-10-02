#ifndef HASH_TABLE_
#define HASH_TABLE_
#include <cstddef>
#include <vector>
#include <forward_list>

namespace pr
{

    template <typename K, typename V>
    class HashTable
    {
    private:
        class Entry
        {
        public:
            const K key;
            V value;
            Entry(const K k, V v) : key(k), value(v){} // liste initialisation obligatoire car key = const
           
        };
        typedef std::vector<std::forward_list<Entry>> buckets_t;
        buckets_t buckets;

    public:
        // CTOR
        HashTable(size_t init = 100)
        {
            buckets.reserve(init); // alloue espace memoire de taille init

            // initialisation avec des listes vides
            for (auto it = buckets.begin(); it != buckets.end(); ++it)
            {
                *it = std::forward_list<Entry>();
            }
        }

        V *get(const K &key)
        {
            size_t h = std::hash<K>()(key);                                  // calcule de l'index
            for (auto it = buckets[h].begin(); it != buckets[h].end(); ++it) // sinon
            {
                if (key == it->key)
                    return &(it->value); // renvoie l'adresse
            }
            return nullptr; // si on arrive ici alors la cle n'existe pas
        }

        bool put(const K &key, const V &value)
        {

            size_t h = std::hash<K>()(key); // calcule de l'index
            bool res;

            for (auto it = buckets[h].begin(); it != buckets[h].end(); ++it)
            {
                if (key == it->key)
                {
                    it->value = value;
                    res = true;
                    break;
                }
            }
            // la pair n'existe pas dans la  liste
            buckets[h].push_front(Entry(key, value));
            res = false;
            return res;
        }
        size_t size() const
        {
            size_t cpt = 0;
            for (auto it = buckets.begin(); it != buckets.end(); ++it)
            {
                if (!it->empty()) // si la liste n'est pas vide
                    ++cpt;
            }
            return cpt;
        }
    };

}
#endif