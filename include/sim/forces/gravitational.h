//
// Created by tayler on 03/04/2026.
//

#ifndef SIMULATION_GRAVITATIONAL_H
#define SIMULATION_GRAVITATIONAL_H
#include "forceprovider.h"

class GravitationalForce : public IForceProvider {
public:
    void Apply(std::vector<Particle>& particles, float dT) override;
};

#endif //SIMULATION_GRAVITATIONAL_H