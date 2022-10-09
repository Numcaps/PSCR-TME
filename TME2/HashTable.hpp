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

            const K key;
            V value;

        public:
            Entry(const K k, V v) : key(k), value(v) {} // liste initialisation obligatoire car key = const
            // accessors
            const K &getK()
            {
                return key;
            }

            V &getV()
            {
                return value;
            }
        };

        typedef std::vector<std::forward_list<Entry>> buckets_t;
        buckets_t buckets;
        size_t sz;
        size_t capacity_;

        public:
        // iterator
        class Iterator
        {
            // attributes
            typename HashTable::buckets_t *buckets_;          // pointeur sur le buckets
            typename buckets_t::iterator vit_;                // iterateur sur la liste explore actuellement
            typename std::forward_list<Entry>::iterator lit_; // pointeur sur l'objet Entry
        public:
            Iterator(typename HashTable::buckets_t *b, typename buckets_t::iterator v, typename std::forward_list<Entry>::iterator l) : buckets_(b), vit_(v), lit_(l) {}

            Iterator &operator++()
            {
                if (lit_ != vit_->end())
                {
                    lit_++;
                }
                else
                {
                    // si vit_ n'atteint pas la fin de la table et qu'il tombe toujour sur
                    // une case vide
                    for (; (vit_ != buckets->end()) and vit_->empty(); ++vit_)
                    {
                    }
                    lit_ = *vit_;
                }
                return lit_;
            }
            bool operator!=(const Iterator &other)
            {
                return (vit_ != other.vit_ and lit_ != other.lit_);
            }

            Entry &operator*()
            {
                return *lit_;
            }
        };

    public:
        // CTOR
        HashTable(size_t init_capa = 100) : capacity_(init_capa)
        {
            sz = 0;
            buckets.reserve(init_capa); // alloue espace memoire de taille init

            // initialisation avec des listes vides

            for (size_t i = 0; i < capacity_; i++)
            {
                buckets.push_back(std::forward_list<Entry>());
            }
        }

        V *get(const K &key)
        {

            size_t h = std::hash<K>()(key) % capacity_;
            // calcule de l'index
            for (auto it = buckets[h].begin(); it != buckets[h].end(); ++it) // sinon
            {
                if (key == it->getK())
                    return &(it->getV()); // renvoie l'adresse
            }
            return nullptr; // si on arrive ici alors la cle n'existe pas
        }

        bool put(const K &key, const V &value)
        {
            if (sz >= 0.8 * capacity_)
                grow(); // on test si la taille de la table est inferieur a sa capacite
            //  sinon on l'agrandit

            size_t h = std::hash<K>()(key) % capacity_; // calcule de l'index

            for (auto it = buckets[h].begin(); it != buckets[h].end(); ++it)
            {
                if (key == it->getK())
                {
                    it->getV() = value;

                    return true;
                }
            }
            // la pair n'existe pas dans la  liste
            ++sz; // incremente le nombre d'item de la hashtable
            buckets[h].push_front(Entry(key, value));

            return false;
        }
        size_t size() const
        {
            return sz;
        }

        size_t capacity() const
        {
            return capacity_;
        }

        void grow()
        {

            HashTable<K, V> tmp(capacity_ * 2); // construction d'un temporaire
            for (auto &b : buckets)             // itere sur les buckets
            {
                for (Entry &e : b) // itere sur les elements d'un bucket
                {
                    tmp.put(e.getK(), e.getV());
                }
            }
            *this = tmp;
        }

        void print()
        {
            for (auto b : buckets)
            {
                if (!b.empty())
                {
                    std::cout << "{";
                    for (auto e : b)
                    {
                        std::cout << "<" << e.getK() << ", " << e.getV() << ">, ";
                    }
                    std::cout << "}" << std::endl;
                }
            }
        }

        Iterator begin()
        {
            // recuperation du pointeur sur le premier bucket non vide
            auto it = buckets.begin();
            while (it != buckets.end())
            {
                if (!it->empty())
                {
                    break;
                }

                ++it;
            }
            return Iterator(&buckets, it, it->begin());
        }
        
    };

}
#endif