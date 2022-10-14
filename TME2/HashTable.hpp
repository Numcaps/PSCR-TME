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
    public:
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

    private:
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
                int cnt = 0;
                //std::cout << "Section 1 ++ : " << std::endl;
                if (++lit_ == vit_->end())
                {
                    // si vit_ n'atteint pas la fin de la table et qu'il tombe toujours sur
                    // une case vide

                    // if(vit_->begin()->getK() == std::string("fees"))
                    //   std::cout << "OK!!!!!!!!" << std::endl ;
                    //std::cout << "Section 2 ++ : " << std::endl;
                    do
                    {
                        ++vit_;
                    } while ((vit_ != buckets_->end()) and (vit_->empty()));
                    //std::cout << "Section 3 ++ : " << std::endl;
                    
                    
                    //std::cout << "Section 4 ++ : " << std::endl;
                    lit_ = vit_->begin();
                }
                //std::cout << "Section 5 ++ : " << std::endl;
                return *this;
            }
            bool operator!=(const Iterator &other)
            {
                if (vit_ != buckets_->end())
                {
                    return (vit_ != other.vit_ and lit_ != other.lit_);
                }
                else
                {
                    return vit_ != other.vit_;
                }               
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

        // pb de invalid read of size
        void grow()
        {

            HashTable<K, V> tmp(capacity_ * 2);                           // construction d'un temporaire
            for (auto vit = buckets.begin(); vit != buckets.end(); ++vit) // itere sur les buckets
            {

                for (auto lit = vit->begin(); lit != vit->end(); ++lit) // itere sur les elements d'un bucket
                {
                    tmp.put(lit->getK(), lit->getV());
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
            //std::cout << "Section 1"<< std::endl;
            auto it = buckets.begin();
            //std::cout << "Section 2"<< std::endl;
            int ct = 0;
            while (it != buckets.end())
            {
                //std::cout << "Section 3 : "<< ct << std::endl;

                if (not(it->empty()))
                {
                    //std::cout << "Section 3.1 : "<< std::endl;
                    break;
                }
                ++ct;
                ++it;
            }
            //std::cout << "Section 4"<< std::endl;
            return Iterator(&buckets, it, it->begin());
        }

        Iterator end()
        {   
            int ct = 0;
            //std::cout << "Section 5 : "<< std::endl;
            size_t cpt = 0;
            auto vit = buckets.begin();
            //std::cout << "Section 6 : "<< std::endl;
            for (; vit != buckets.end(); ++vit)
            {
                auto lit = vit->begin();
                for (; lit != vit->end(); ++lit)
                {
                    ++cpt;
                }

                if (cpt == sz)
                {
                    break;
                }
            }
            //std::cout << "Section 7 : "<< std::endl;
            ++vit;            
            /* AWKWARD BUG, PRE-INCR IN CTOR ARGS NOT WORKING (VALUE NOT INCREMENTED)*/
            /* Valeur de lit aleatoire */
            return Iterator(&buckets, vit, (--vit)->begin());
        }
    };

}
#endif