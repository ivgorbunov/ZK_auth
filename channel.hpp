#include "prover.hpp"
#include "verificator.hpp"

bool try_connect(IProver* prover) {
    Verificator* verificator = Verificator::GetInstance();
    verificator->Init(prover->Init());
    Message now{0, {}, Respond::kContinue};
    while (now.resp == Respond::kContinue) {
        auto was = now.iter;
        now = prover->Respond(now);
        if (now.iter != was || now.resp != Respond::kProver) { // In case he is trying to fool us
            return false;
        }
        now = verificator->Respond(now);
    }
    return now.resp == Respond::kSuccess;
}