#pragma once

#include "message.hpp"
#include <random>
#include <iostream>

struct Verificator {
public:

    void Init(Message message) {
        k = message.arr.size();
        public_key.resize(k);
        last_query.resize(k);
        for (size_t i = 0; i < k; i++) {
            public_key[i] = message.arr[i];
        }
        std::cout << "V: Public key received\n";
    }
    Message Respond(Message message) {
        size_t iter = message.iter;
        if (iter % 2 == 0) {
            X = message.arr[0];
            std::cout << "V: random vector is: ";
            regenerate();
            return {iter + 1, last_query, Respond::kContinue};
        } else {
            ModuledBigInt Y = message.arr[0];
            ModuledBigInt accum = Y * Y;
            std::cout << "V: Checking that X is equal to " << Y.value << " * " << Y.value;
            for (size_t i = 0; i < k; i++) {
                if (last_query[i].value) {
                    accum *= public_key[i];
                    std::cout << " * " << public_key[i].value;
                }
            }
            std::cout << " = " << accum.value << std::endl;
            bool good = (X == accum) || (X == -accum);
            if (!good) {
                return {iter + 1, {}, Respond::kFailed};
            } else {
                if (iter >= limit) {
                    return {iter + 1, {}, Respond::kSuccess};
                } else {
                    return {iter + 1, {}, Respond::kContinue};
                }
            }
        }

    }

    void regenerate() {
        std::vector<long long> ans;
        for (size_t i = 0; i < k; i++) {
            last_query[i] = ModuledBigInt(rnd() % 2);
            std::cout << last_query[i].value << " ";
        }
        std::cout << std::endl;
    }

    static Verificator* GetInstance() {
        static Verificator god;
        return &god;
    }
private:
    size_t k{0};
    const size_t limit = 64;
    ModuledBigInt X{0};
    std::mt19937 rnd{123};
    std::vector<ModuledBigInt> last_query;
    std::vector<ModuledBigInt> public_key;
};