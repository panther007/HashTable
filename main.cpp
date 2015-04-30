/*
 * Don't modify this, except for your own testing.
 */

#include "HashTable.hpp"
#include <functional>
#include <limits>
#include <iostream>

class Name {
        friend std::ostream &operator<<(std::ostream &os, const Name &n) {
            return os << n.m_name;
        }
    public:
        Name(const std::string &n) : m_name(n) {}
        Name &operator=(const Name &) = delete;
        unsigned int hash() const {
            return s_hash(m_name)%std::numeric_limits<unsigned int>::max();
        }
    private:
        const std::string m_name;
        static std::hash<std::string> s_hash;
};

std::hash<std::string> Name::s_hash;

class Address {
        friend std::ostream &operator<<(std::ostream &os, const Address &a) {
            return os << a.m_number << " " << a.m_street;
        }
    public:
        Address(int n, const std::string &s) : m_number(n), m_street(s) {}
        Name &operator=(const Name &) = delete;
    private:
        const int m_number;
        const std::string m_street;
};

int
main() {

    using ht_t = cs540::HashTable<Name, Address>;
    ht_t ht;
    auto print = [&]() {
        std::cout << "----" << std::endl;
        // The range-based for is doing the below.
        for (ht_t::Iterator it = ht.begin(); it != ht.end(); ++it) {
        //for (auto &e : ht) {
            //std::cout << "Name: " << e.first << ", Address: " << e.second << std::endl;
            std::cout << "Name: " << it->first << ", Address: " << it->second << std::endl;
        }
    };

    ht.insert(std::make_pair(Name("Joe1"), Address(11, "Main Street")));
    print();
    ht.insert(std::make_pair(Name("Joe2"), Address(22, "Main Street")));
    print();
    ht.insert(std::make_pair(Name("Joe3"), Address(33, "Main Street")));
    print();
    ht.insert(std::make_pair(Name("Joe4"), Address(44, "Main Street")));
    print();
    ht.insert(std::make_pair(Name("Joe5"), Address(55, "Main Street")));
    print();
    ht.insert(std::make_pair(Name("Joe6"), Address(66, "Main Street")));
    print();
    ht.insert(std::make_pair(Name("Joe7"), Address(77, "Main Street")));
    print();

    //auto ht2(ht); // Make this a compile-time error.
}
