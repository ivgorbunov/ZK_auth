#pragma once

#include "prover.hpp"
#include <src/util/rand.hpp>
#include <random>
#include <src/util/bigint.hpp>

class FairProver : public IProver {
public:
    FairProver(size_t k): k(k) {
        std::cout << "P: PRIVATE KEY: ";
        for (size_t i = 0; i < k; i++) {
            s.push_back(GetRandomNumber());
            std::cout << s.back().get_value() << " ";
        }
        std::cout << std::endl << std::endl;
    }
    Message Init() override {
        std::vector<ModuledBigInt> I;

        std::cout << "P: Hello, my public key is: ";
        for (size_t i = 0; i < s.size(); i++) {
            auto inv = s[i].inversed();
            std::cout << inv.get_value() << " ";
            inv *= inv;
            if (rnd() % 2) {
                I.push_back(inv);
            } else {
                I.push_back(-inv);
            }
            //std::cout << I.back().get_value() << " ";
        }
        std::cout << std::endl;
        return {0, I, Respond::kProver};
    }

    Message Respond(Message message) override {
        size_t iter = message.iter;
        if (iter % 2 == 0) {
            R = GetRandomNumber();
            std::cout << "P: X = " << (R * R).get_value() << std::endl;
            return {iter, {R * R}, Respond::kProver};
        } else {
            ModuledBigInt now = R;
            std::cout << "P: Y = " << R.get_value();
            for (size_t i = 0; i < k; i++) {
                if (message.arr[i].get_value() == 1) {
                    now *= s[i];
                    std::cout << " * " << s[i].get_value();
                }
            }
            std::cout << " = " << now.get_value() << std::endl;
            return {iter, {now}, Respond::kProver};
        }
    }

private:
    size_t k;
    ModuledBigInt R;
    std::vector<ModuledBigInt> s;
    std::mt19937 rnd;
};