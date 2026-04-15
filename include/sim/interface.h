//
// Created by tayler on 1/28/26.
//

#ifndef SIMULATION_INTERFACE_H
#define SIMULATION_INTERFACE_H
#include <map>

#include "particletypes.h"

class Interface {
public:

    void Show();

    ParticleType selectedType = ParticleType::ParticleType_Proton;
    static std::string selectedSymbol;

    int protonCount, neutronCount, electronCount, photonCount;

    std::map<std::string, std::vector<size_t>> elementGroups;
    std::vector<std::string> elementSymbols;

    void ResetParticleCounts();
};


#endif //SIMULATION_INTERFACE_H