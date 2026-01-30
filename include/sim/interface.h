//
// Created by tayler on 1/28/26.
//

#ifndef SIMULATION_INTERFACE_H
#define SIMULATION_INTERFACE_H
#include "particletypes.h"


class Interface {
public:
    void Show();

    ParticleType selectedType = ParticleType::ParticleType_Proton;

    int protonCount, neutronCount, electronCount, photonCount;
};


#endif //SIMULATION_INTERFACE_H