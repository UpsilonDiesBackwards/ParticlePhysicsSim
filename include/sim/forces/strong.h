//
// Created by Tayler Parsons on 05/04/2026.
//

#ifndef SIMULATION_STRONG_H
#define SIMULATION_STRONG_H

#include "forceprovider.h"
#include "../../application.h"

class StrongForce : public IForceProvider {
    const float Gs = GET_APP.simulationSystem.properties.Gn * 10;
    const float range = 7.5f;

    void Apply(std::vector<Particle> &particles, float dT) override;
};



#endif //SIMULATION_STRONG_H
