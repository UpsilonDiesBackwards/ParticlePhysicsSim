//
// Created by Tayler Parsons on 05/04/2026.
//

#ifndef SIMULATION_WEAK_H
#define SIMULATION_WEAK_H
#include "forceprovider.h"


class WeakForce : public IForceProvider {
    void Apply(std::vector<Particle> &particles, float dT) override;
};



#endif //SIMULATION_WEAK_H
