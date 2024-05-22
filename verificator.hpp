#include <message.hpp>
#include <random>

struct Verificator {
public:

    void Init(Message message) {
        k = message.arr.size();
        for (size_t i = 0; i < k; i++) {
            public_key[i] = message.arr[i];
        }
    }
    Message Respond(Message message) {
        size_t iter = message.iter;
        if (iter % 2 == 0) {
            X = message.arr[0];
            regenerate();
            return {iter + 1, last_query, Respond::kContinue};
        } else {
            BigInteger Y = message.arr[0];
            BigInteger accum = Y * Y;
            for (size_t i = 0; i < k; i++) {
                if (last_query[i])
                    accum *= public_key[i];
            }
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
            last_query[i] = (rnd() % 2);
        }
    }

    static Verificator* GetInstance() {
        if (self == nullptr) {
            self = new Verificator{};
        }
        return self;
    }
private:
    static Verificator* self;
    size_t k{0};
    const size_t limit = 64;
    BigInteger X{0};
    std::mt19937 rnd{123};
    std::vector<BigInteger> last_query;
    std::vector<BigInteger> public_key;
};