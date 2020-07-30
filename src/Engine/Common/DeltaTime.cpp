#include "Engine/Common/DeltaTime.h"

namespace Engine {
    bool DeltaTime::firstIter = true;
    auto DeltaTime::prev = std::chrono::high_resolution_clock::now();
    auto DeltaTime::cur = std::chrono::high_resolution_clock::now();
}