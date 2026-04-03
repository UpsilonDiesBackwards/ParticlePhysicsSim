//
// Created by tayler on 03/04/2026.
//

#ifndef SIMULATION_GRAVITATIONALFORCE_H
#define SIMULATION_GRAVITATIONALFORCE_H
#include "forceprovider.h"


class GravitationalForce : public IForceProvider {
public:
    float G = 0.1f;

    void Apply(std::vector<Particle>& particles, float dT) override;
};


#endif //SIMULATION_GRAVITATIONALFORCE_H