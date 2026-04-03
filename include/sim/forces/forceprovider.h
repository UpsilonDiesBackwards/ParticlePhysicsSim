//
// Created by tayler on 03/04/2026.
//

#ifndef SIMULATION_FORCEPROVIDER_H
#define SIMULATION_FORCEPROVIDER_H
#include <vector>

#include "../particle.h"

class IForceProvider {
public:
    virtual ~IForceProvider() = default;
    virtual void Apply(std::vector<Particle>& particles, float dT) = 0;
};

#endif //SIMULATION_FORCEPROVIDER_H