#pragma once

#include <src/util/moduled_bigint.hpp>
#include <variant>

enum class Respond {
    kFailed,
    kSuccess,
    kContinue,
    kProver
};

struct Message {
    size_t iter;
    std::vector<ModuledBigInt> arr;
    Respond resp;
};