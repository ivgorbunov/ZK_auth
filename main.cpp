#include <iostream>

#include "src/channel.hpp"
#include "src/fair_prover.hpp"


signed main() {
    FairProver fp(3);
    std::cout << try_connect(&fp);
}