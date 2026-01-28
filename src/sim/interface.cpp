//
// Created by tayler on 1/28/26.
//

#include "../../include/sim/interface.h"

#include "imgui.h"
#include "../../include/application.h"
#include "../../include/sim/system.h"

const char* GetParticleName(ParticleType type) {
    switch (type) {
        case ParticleType::ParticleType_Electron: return "Electron";
        case ParticleType::ParticleType_Proton:   return "Proton";
        case ParticleType::ParticleType_Neutron:  return "Neutron";
        case ParticleType::ParticleType_Photon:   return "Photon";
        default: return "Unknown";
    }
}

void Interface::Show() {
    static ParticleType selectedType = ParticleType::ParticleType_Proton;

    ImGui::SetNextWindowSize(ImVec2(320, 560));

    ImGui::Begin("Control Panel", NULL, ImGuiWindowFlags_None);

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

    ImGui::Separator();

    ImGui::Text("Playback");
    ImGui::Separator();

    ImGui::Text("Stats");

    ImGui::Text("Camera Coords: X %.2f, Y %.2f", GET_APP.getCamera()->GetPosition().x, GET_APP.getCamera()->GetPosition().y);
    ImGui::Text("Zoom: %.2f %%", GET_APP.camera->GetZoom());
    ImGui::Text("Cursor Coords: X %.2f, Y %.2f", GET_APP.inputManager.getMouseX(), GET_APP.inputManager.getMouseY());

    ImGui::Text("Particle Counts (P/N/E,Ph): %d / %d / %d / %d", protonCount, neutronCount, electronCount, photonCount);

    ImGui::Separator();

    ImGui::End();
}
