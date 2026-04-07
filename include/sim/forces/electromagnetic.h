//
// Created by Tayler Parsons on 05/04/2026.
//

#ifndef SIMULATION_ELECTROMAGNETIC_H
#define SIMULATION_ELECTROMAGNETIC_H
#include "forceprovider.h"


class ElectromagneticForce : public IForceProvider {
    void Apply(std::vector<Particle> &particles, float dT) override;
};



#endif //SIMULATION_ELECTROMAGNETIC_H
