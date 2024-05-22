#include <iostream>

#include "src/channel.hpp"
#include "src/fair_prover.hpp"


signed main() {

    for (int i = 0; i < 100; i++) {
        FairProver fp(10);
        assert(try_connect(&fp));
    }
}