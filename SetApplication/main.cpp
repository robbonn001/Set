#include <iostream>
#include "BitField.h"
#include "Set.h"

void sieve(Set& set) {
    for (size_t i = 2;i <= set.maxPower();++i) {
        set.insert(i);
    }
    for (size_t i = 2; i * i <= set.maxPower();++i) {
        if (!set.isMember(i)) continue;
        for (size_t j = i*i; j <= set.maxPower();j += i) {
            set.erase(j);
        }
    }
}

int main() {
    Set set(1000);
    sieve(set);
    std::cout << set;
}