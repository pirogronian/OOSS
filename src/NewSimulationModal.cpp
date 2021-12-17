
#include <imgui/imgui.h>

#include "NewSimulationModal.h"

using namespace Ogre;
using namespace OgreBites;

NewSimulationModal::NewSimulationModal(Simulation *s) : _sim(s) {
    _active = true;
    ImGui::OpenPopup("Simulation not empty");
}

bool NewSimulationModal::frameStarted(const FrameEvent &e) {
    if (ImGui::BeginPopupModal("Simulation not empty")) {
        ImGui::Text("Simulation is not empty! Are you sure to load new simulation?");
        if (ImGui::Button("Yes")) {
            _active = false;
            ImGui::CloseCurrentPopup();
            _sim->populate();
        }
        ImGui::SameLine();
        if (ImGui::Button("No")) {
            ImGui::CloseCurrentPopup();
            _active = false;
        }
        ImGui::EndPopup();
    }
    return true;
}
