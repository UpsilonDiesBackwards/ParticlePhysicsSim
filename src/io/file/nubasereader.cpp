//
// Created by Tayler Parsons on 14/04/2026.
//

#include "../../include/io/file/nubasereader.h"

#include <fstream>
#include <iostream>

#include "../../../include/application.h"
#include "../../../include/sim/atomtemplate.h"

void NubaseReader::ReadNubaseFile(std::string fileName) {
    std::ifstream ifs(fileName);
    std::string line;

    if (!ifs.is_open()) {
        std::cerr << "Could not open nubase file: " << fileName << std::endl;
        return;
    }

    const float BOHR_RADIUS_SCALE = 5.0f;

    while (std::getline(ifs, line)) {
        if (line.length() < 15) continue;
        if (line.length() < 16) continue;
        try {
            int massNum = std::stoi(line.substr(0, 3));

            int protons = std::stoi(line.substr(4, 3));
            int neutrons = massNum - protons;

            std::string symbol = line.substr(13, 3);

            symbol.erase(0, symbol.find_first_not_of(' '));
            size_t last = symbol.find_last_not_of(' ');
            if (last != std::string::npos) {
                symbol = symbol.substr(0, last + 1);
            }

            if (symbol.empty()) {
                symbol = (protons == 0) ? "n" : "unknown";
            }

            std::string fullName = std::to_string(massNum) + symbol;

            AtomTemplate atom;
            atom.name = fullName;
            atom.protons = protons;
            atom.neutrons = neutrons;
            atom.electrons = protons;
            atom.baseOrbitRadius = 6.0f;

            if (protons > 0) {
                atom.baseOrbitRadius = BOHR_RADIUS_SCALE;
            } else {
                atom.baseOrbitRadius = 0.0f;
            }

            atom.nucleusDensity = 1.0f + (0.1f * std::log10((float)massNum + 1));

            GET_APP.atomTemplates.push_back(atom);
        } catch (const std::exception& e) {
            return;
        }
    }
    ifs.close();
}

void NubaseReader::PrintDatabase() {
    std::cout << std::left << std::setw(10) << "Element"
              << std::setw(10) << "Protons"
              << std::setw(10) << "Neutrons"
              << "Electrons" << std::endl;
    std::cout << "------------------------------------------" << std::endl;

    for (const auto& atom : GET_APP.atomTemplates) {
        std::cout << std::left << std::setw(10) << atom.name
                  << std::setw(10) << atom.protons
                  << std::setw(10) << atom.neutrons
                  << atom.electrons << std::endl;
    }
}
