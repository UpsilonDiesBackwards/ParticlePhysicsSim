//
// Created by tayler on 1/28/26.
//

#include "../../include/sim/interface.h"

#include "imgui.h"
#include "../../include/application.h"
#include "../../include/sim/system.h"

const char* GetParticleName(ParticleType type) {
    switch (type) {
        case ParticleType::ParticleType_Proton:   return "Proton (1)";
        case ParticleType::ParticleType_Neutron:  return "Neutron (2)";
        case ParticleType::ParticleType_Electron: return "Electron (3)";
        case ParticleType::ParticleType_Photon:   return "Photon (4)";
        default: return "Unknown";
    }
}

const char* GetPlayState(PlayState state) {
    switch (state) {
        case PlayState_PLAY:  return "PLAY";
        case PlayState_STOP:  return "STOP";
        case PlayState_STEP:  return "STEP";
        case PlayState_PAUSE: return "PAUSE";
        default:              return "UNKNOWN";
    }
}

std::string Interface::selectedSymbol = "H";

void Interface::Show() {
    ImGui::SetNextWindowSize(ImVec2(320, 560));
    ImGui::Begin("Control Panel", NULL, ImGuiWindowFlags_NoNavInputs);

    ImGui::Text("Simulation");

    if (ImGui::BeginCombo("Particle", GetParticleName(selectedType))) {
        for (int i = 0; i < (int)ParticleType::ParticleType_NumItems; i++) {
            ParticleType currentItem = (ParticleType)i;

            bool isSelected = (selectedType == currentItem);

            if (ImGui::Selectable(GetParticleName(currentItem), isSelected)) {
                selectedType = currentItem;
                GET_APP.GetInstance().targetParticleType = selectedType;
            }

            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    ImGui::Checkbox("Spawn Atom Mode", &GET_APP.spawnAtomMode);

    if (ImGui::BeginCombo("Element", selectedSymbol.c_str())) {
        for (const auto& sym : elementSymbols) {
            std::string label = sym.empty() ? "???" : sym;
            std::string uniqueID = label + "##" + sym;

            if (ImGui::Selectable(uniqueID.c_str(), selectedSymbol == sym)) {
                selectedSymbol = sym;
                GET_APP.targetAtomTemplate = GET_APP.atomTemplates[elementGroups[selectedSymbol][0]];
            }
        }
        ImGui::EndCombo();
    }

    if (ImGui::BeginCombo("Isotope", GET_APP.targetAtomTemplate.name.c_str())) {
        const auto& indices = elementGroups[selectedSymbol];
        for (size_t idx : indices) {
            const auto& atom = GET_APP.atomTemplates[idx];

            std::string uniqueID = atom.name + "##" + std::to_string(idx);

            bool isSelected = (GET_APP.targetAtomTemplate.name == atom.name &&
                               GET_APP.targetAtomTemplate.protons == atom.protons);

            if (ImGui::Selectable(uniqueID.c_str(), isSelected)) {
                GET_APP.targetAtomTemplate = atom;
            }
        }
        ImGui::EndCombo();
    }

    ImGui::Text("Selection: %s (P: %d, N: %d)",
                GET_APP.targetAtomTemplate.name.c_str(),
                GET_APP.targetAtomTemplate.protons,
                GET_APP.targetAtomTemplate.neutrons);

    if (ImGui::Button("Clear all Particles")) {
        GET_APP.GetInstance().simulationSystem.ClearAllParticles();
        ResetParticleCounts();
    }

    ImGui::SameLine();

    if (ImGui::Button("Camera to Origin")) {
        GET_APP.camera->SetPosition(glm::vec2(0.0f, 0.0f));
        GET_APP.camera->SetZoom(1.0f);
    }

    ImGui::Separator();

    ImGui::Text("Playback");

    ImGui::Text("Play State: %s", GetPlayState(GET_APP.playState));

    if (GET_APP.playState == PlayState_PLAY) {
        if (ImGui::Button("Pause")) GET_APP.playState = PlayState_PAUSE;
    } else {
        if (ImGui::Button("Play"))  GET_APP.playState = PlayState_PLAY;
    }

    ImGui::SameLine();

    if (ImGui::Button("Stop")) GET_APP.playState = PlayState_STOP;

    ImGui::SameLine();

    bool isPaused = (GET_APP.playState == PlayState_PAUSE);
    if (!isPaused) {
        ImGui::BeginDisabled();
    }
    if (ImGui::Button("Step")) {
        GET_APP.playState = PlayState_STEP;
    }
    if (!isPaused) {
        ImGui::EndDisabled();
    }

    ImGui::Separator();

    ImGui::Text("Simulation Settings");
    ImGui::SliderFloat("Simulation Speed", &GET_APP.simulationSystem.properties.timeScale, 0.1f, 10.0f);

    ImGui::Separator();

    ImGui::Text("Stats");

    int currentFPS = GET_APP.time.GetCurrentFPS();

    if (currentFPS < 30) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "FPS: %d (Critical)", currentFPS);
    } else if (currentFPS < 60) {
        ImGui::TextColored(ImVec4(1.0f, 0.6f, 0.0f, 1.0f), "FPS: %d (Fair)", currentFPS); // Orange
    } else {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "FPS: %d (Good)", currentFPS);
    }

    // Collapsable Graph
    if (ImGui::CollapsingHeader("Performance Graph")) {
        float currentFPS = static_cast<float>(GET_APP.time.GetCurrentFPS());
        ImVec4 plotColor;

        if (currentFPS < 30.0f) {
            plotColor = ImVec4(1.0f, 0.2f, 0.2f, 1.0f); // Red
        } else if (currentFPS < 60.0f) {
            plotColor = ImVec4(1.0f, 0.6f, 0.0f, 1.0f); // Orange
        } else {
            plotColor = ImVec4(0.2f, 1.0f, 0.2f, 1.0f); // Green
        }

        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, plotColor);
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.05f, 0.05f, 0.05f, 0.54f));

        ImGui::PlotHistogram(
            "##FPS_Histogram",
            GET_APP.time.GetFPSHistory(),
            GET_APP.time.GetHistorySize(),
            0,
            "FPS History",
            0.0f,
            144.0f,
            ImVec2(0, 80)
        );

        ImGui::PopStyleColor(2);
    }

    ImGui::Text("Coords: X %.2f, Y %.2f", GET_APP.getCamera()->GetPosition().x, GET_APP.getCamera()->GetPosition().y);
    ImGui::Text("Zoom: %.2f %%", GET_APP.camera->GetZoom());
    ImGui::Text("Cursor Coords: X %.2f, Y %.2f", GET_APP.inputManager.getMouseX(), GET_APP.inputManager.getMouseY());

    ImGui::BeginDisabled();
    ImGui::Checkbox("Cursor Captured?", &GET_APP.captureMouse);
    ImGui::EndDisabled();

    ImGui::Text("Particle Counts (P/N/E,Ph): %d / %d / %d / %d", protonCount, neutronCount, electronCount, photonCount);

    ImGui::Separator();
    ImGui::Text("Particle Colors");

    glm::vec4& protonColor   = GET_APP.simulationSystem.GetParticleColor(ParticleType::ParticleType_Proton);
    glm::vec4& neutronColor  = GET_APP.simulationSystem.GetParticleColor(ParticleType::ParticleType_Neutron);
    glm::vec4& electronColor = GET_APP.simulationSystem.GetParticleColor(ParticleType::ParticleType_Electron);
    glm::vec4& photonColor   = GET_APP.simulationSystem.GetParticleColor(ParticleType::ParticleType_Photon);

    // Proton
    if (ImGui::ColorEdit4("Proton Color", (float*)&protonColor)) {
        GET_APP.simulationSystem.UpdateParticleColors(ParticleType::ParticleType_Proton, protonColor);
    }

    // Neutron
    if (ImGui::ColorEdit4("Neutron Color", (float*)&neutronColor)) {
        GET_APP.simulationSystem.UpdateParticleColors(ParticleType::ParticleType_Neutron, neutronColor);
    }

    // Electron
    if (ImGui::ColorEdit4("Electron Color", (float*)&electronColor)) {
        GET_APP.simulationSystem.UpdateParticleColors(ParticleType::ParticleType_Electron, electronColor);
    }

    // Photon
    if (ImGui::ColorEdit4("Photon Color", (float*)&photonColor)) {
        GET_APP.simulationSystem.UpdateParticleColors(ParticleType::ParticleType_Photon, photonColor);
    }

    ImGui::Separator();

    ImGui::Checkbox("Show Trails", &GET_APP.showTrails);

    glm::vec4& protonTrailColor   = GET_APP.simulationSystem.GetParticleTrailColor(ParticleType::ParticleType_Proton);
    glm::vec4& neutronTrailColor  = GET_APP.simulationSystem.GetParticleTrailColor(ParticleType::ParticleType_Neutron);
    glm::vec4& electronTrailColor = GET_APP.simulationSystem.GetParticleTrailColor(ParticleType::ParticleType_Electron);
    glm::vec4& photonTrailColor   = GET_APP.simulationSystem.GetParticleTrailColor(ParticleType::ParticleType_Photon);

    // Proton
    if (ImGui::ColorEdit4("Proton Trail", (float*)&protonTrailColor)) {
        GET_APP.simulationSystem.UpdateTrailColors(ParticleType::ParticleType_Proton, protonTrailColor);
    }

    // Neutron
    if (ImGui::ColorEdit4("Neutron Trail", (float*)&neutronTrailColor)) {
        GET_APP.simulationSystem.UpdateTrailColors(ParticleType::ParticleType_Neutron, neutronTrailColor);
    }

    // Electron
    if (ImGui::ColorEdit4("Electron Trail", (float*)&electronTrailColor)) {
        GET_APP.simulationSystem.UpdateTrailColors(ParticleType::ParticleType_Electron, electronTrailColor);
    }

    // Photon
    if (ImGui::ColorEdit4("Photon Trail", (float*)&photonTrailColor)) {
        GET_APP.simulationSystem.UpdateTrailColors(ParticleType::ParticleType_Photon, photonTrailColor);
    }

    if (ImGui::Button("Reset Particle Colors")) {
        GET_APP.simulationSystem.ResetParticleColors();
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset Trail Colors")) {
        GET_APP.simulationSystem.ResetTrailColors();
    }


    ImGui::Separator();
    ImGui::Text("Physics Settings");

    if (ImGui::InputInt("Collision Steps", &GET_APP.collisionResolutionCount)) {
        if (GET_APP.collisionResolutionCount < 1) GET_APP.collisionResolutionCount = 1;
        if (GET_APP.collisionResolutionCount > 50) GET_APP.collisionResolutionCount = 50;
    }

    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Higher values = more stable physics, but lower FPS.");
    }

    ImGui::End();
}

void Interface::ResetParticleCounts() {
    protonCount = 0;
    neutronCount = 0;
    electronCount = 0;
    photonCount = 0;
}
