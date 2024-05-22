#include "prover.hpp"
#include "verificator.hpp"

bool try_connect(IProver* prover) {
    Verificator* verificator = Verificator::GetInstance();
    verificator->Init(prover->Init());
    Message now{0, {}, Respond::kContinue};
    int cnt = 0;
    while (now.resp == Respond::kContinue) {
        if (cnt % 2 == 0) {
            std::cout << "-------------------------\n";
            std::cout << "Iteration: " << cnt / 2 << std::endl;
            std::cout << "-------------------------\n";

        }
        cnt++;
        auto was = now.iter;
        now = prover->Respond(now);
        if (now.iter != was || now.resp != Respond::kProver) { // In case he is trying to fool us
            return false;
        }
        now = verificator->Respond(now);
    }
    return now.resp == Respond::kSuccess;
}