#include "prover.hpp"
#include <util/rand.hpp>
#include <random>
#include <util/bigint.hpp>

class FairProver : IProver {
    FairProver(size_t k): k(k) {
        for (size_t i = 0; i < k; i++) {
            // TODO Implement Random number gen
        }
    }
    Message Init() override {
        std::vector<BigInteger> I;
        for (size_t i = 0; i < s.size(); i++) {
            // TODO Implement Modulo arithmetic
        }
    }

    Message Resond(Message message) {
        size_t iter = message.iter;
        if (iter % 2 == 0) {
            // TODO Implement Random number gen
            return R * R;
        } else {
            BigInteger now = R;
            for (size_t i = 0; i < k; i++) {
                if (message.arr[i]) {
                    now *= s[i];
                }
            }
            return {iter, {now}, Respond::kProver};
        }
    }

private:
    size_t k;
    BigInteger R;
    std::vector<BigInteger> s;
    std::mt19937 rnd;
};