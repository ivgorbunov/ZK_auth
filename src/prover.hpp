#pragma once

#include "message.hpp"

struct IProver {
    virtual Message Init() = 0;
    virtual Message Respond(Message) = 0;
};