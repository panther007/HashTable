/*
 * This is a hash table.  Each bucket is a circular, doubly-linked list with
 * sentinel.  There is also a sentinel bucket at the very end with one dummy
 * link.  To allow the list sentinel to not have any data, a base class with links
 * but no data is used.
 *
 * To make debugging easier, the hash table is statically sized to 5.
 *
 * Read the code over carefully before you do anything.
 */

#ifndef CS540_HASH_TABLE_HPP
#define CS540_HASH_TABLE_HPP



#include <utility>
#include <array>
#include <cstdio>
#include <iostream>

namespace cs540 {



template <typename K, typename M>
class HashTable {
    public:
        using ValueType = std::pair<const K, M>;
    private:
        class Bucket;
        class Link;
        class LinkData;
    public:
        class Iterator {
                friend class HashTable;
            public:
                Iterator(Bucket *b, Link *ld) : m_bucket(b), m_current(ld) {}
                Iterator(Bucket *b) : Iterator(b, b->m_head.m_next) {}
                friend bool operator!=(const Iterator &it1, const Iterator &it2) {
                    return it1.m_current != it2.m_current;
                }
                Iterator &operator++() {
                  m_current = m_current->m_next;
                  if (m_current == &m_bucket->m_head) {
                    m_bucket = find_next_nonempty_bucket(++m_bucket);
                    m_current = m_bucket->m_head.m_next;
                  }
                  return *this;
                }
                Iterator operator++(int) {
                    Iterator Temp = *this;
                    ++(*this);
                    return Temp;
                }
                ValueType &operator*() {
                    return static_cast<LinkData *>(m_current)->m_value;
                }
                ValueType *operator->() {
                    return &(*(*this));
                }
            private:
                Bucket *m_bucket;
                Link *m_current;
        };
    private:
        class Link {
            friend class HashTable;
            // Initialized to point to itself, circularly.
            Link() : m_next(this), m_prev(this) {}
            Link *m_next, *m_prev;
        };
        class LinkData : public Link {
            friend class HashTable;
            LinkData(const ValueType &v) : Link(),  m_value(v) {
            }
            ValueType m_value;
        };
        // Each bucket has a doubly-lined, circular list with sentinel.
        class Bucket {
            friend class HashTable;
            void append(Link *);
            Link m_head;
        };
    public:
        HashTable() : m_table(new Bucket[sz + 1]) {
            // begin() is initialized to point to the first nonempty bucket, so
            // we create the sentinel with one dummy Link.
            m_table[sz].append(new Link);
        }
        HashTable(const HashTable&) = delete;
        ~HashTable() {
          for(int i=0;i<sz;i++){
            Bucket *b = &m_table[i];
            while (b->m_head.m_next != &b->m_head) {
              Link *l = b->m_head.m_next;
              b->m_head.m_next = l->m_next;
              l->m_next->m_prev = l->m_prev;
              delete static_cast<LinkData *>(l);
            }
          }
          delete m_table[sz].m_head.m_next;
          delete [] m_table; 
        }
    public:
        Iterator begin() { 
          return Iterator(find_next_nonempty_bucket(&m_table[0])); 
        }
        Iterator end() { 
          return Iterator(&(m_table[sz])); 
        }
        Iterator insert(const ValueType &);
    private:
        // This function is very useful.
        static Bucket *find_next_nonempty_bucket(Bucket *b);
    private:
        constexpr static int sz = 5;
        Bucket *m_table;
    private:
};

template <typename K, typename M>
typename HashTable<K, M>::Iterator
HashTable<K, M>::insert(const std::pair<const K, M> &p) {
    unsigned int ind = p.first.hash()%sz;
    LinkData *ld = new LinkData(p);
    m_table[ind].append(ld);
    return {&m_table[ind], ld};
}

template <typename K, typename M>
void
HashTable<K, M>::Bucket::append(Link *ld) {
    ld->m_next = &m_head;
    ld->m_prev = m_head.m_prev;
    m_head.m_prev->m_next = ld;
    m_head.m_prev = ld;
}

template <typename K, typename M>
typename HashTable<K, M>::Bucket *
HashTable<K, M>::find_next_nonempty_bucket(Bucket *b) {
    // Skip empty buckets.
    while (b->m_head.m_next == &b->m_head) {
        b++;
    }
    return b;
}


} // namespace cs540

#endif
