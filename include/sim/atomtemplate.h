//
// Created by Tayler Parsons on 14/04/2026.
//

#ifndef SIMULATION_ATOMTEMPLATE_H
#define SIMULATION_ATOMTEMPLATE_H

#include <string>

struct AtomTemplate {
    std::string name;
    int protons;
    int neutrons;
    int electrons;
    float nucleusDensity = 0.5;
    float baseOrbitRadius = 5.0f;
};

#endif //SIMULATION_ATOMTEMPLATE_H
