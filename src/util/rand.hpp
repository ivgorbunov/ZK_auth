#pragma once

#include "moduled_bigint.hpp"


std::mt19937 rnd(134092830);

ModuledBigInt GetRandomNumber() {
    ModuledBigInt md{1};
    ModuledBigInt ans{0};

    do {
        for (size_t i = 0; i < 100; i++) {
            ans = (ans + (md * BigInteger(rnd())));
            md *= BigInteger(10);
        }
    } while ((ans.value % P == 0) || (ans.value % Q == 0));
    //std::cout << "-> " << ans.value << std::endl;
    return ans;
}