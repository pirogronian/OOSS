
#include <iostream>
#include <imgui/imgui.h>

#include "SaveSimulationModal.h"

using namespace std;
using namespace std::filesystem;

SaveSimulationModal::SaveSimulationModal(const path& p, Simulation *s) : _sim(s) {
    _flist.path = p;
    _flist.dir = true;
    _flist.refresh();
    _active = true;
    ImGui::OpenPopup("Choose save slot");
}

bool SaveSimulationModal::frameStarted(const Ogre::FrameEvent &e) {
    if (ImGui::BeginPopupModal("Choose save slot")) {
        ImGui::Text(_flist.path.string().data());
        for (auto const &p : _flist.getEntries()) {
            if (ImGui::Selectable(p.first.data())) {
                _chosen = p.first;
            }
        }
        ImGui::Separator();
        ImGui::Text(_chosen.data());
        ImGui::Separator();
        if (ImGui::Button("Ok")) {
            cout << "SaveSimulationModal::Ok\n";
            ImGui::CloseCurrentPopup();
            _active = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Refresh")) {
            _flist.refresh();
        }
        ImGui::EndPopup();
    }

    return true;
}
