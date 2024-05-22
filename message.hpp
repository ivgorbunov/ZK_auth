#include <util/bigint.hpp>
#include <variant>

enum class Respond {
    kFailed,
    kSuccess,
    kContinue,
    kProver
};

struct Message {
    size_t iter;
    std::vector<BigInteger> arr;
    Respond resp;
};