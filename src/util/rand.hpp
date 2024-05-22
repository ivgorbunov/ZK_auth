#pragma once

#include "moduled_bigint.hpp"


std::mt19937 rnd(134092830);

ModuledBigInt GetRandomNumber() {
    ModuledBigInt md{1};
    ModuledBigInt ans{0};

    do {
        for (size_t i = 0; i < 100; i++) {
            ans = (ans + (md * BigInteger(rnd())));
        }
    } while (ans == P || ans == Q || ans == BigInteger{0});
    //std::cout << "-> " << ans.value << std::endl;
    return ans;
}