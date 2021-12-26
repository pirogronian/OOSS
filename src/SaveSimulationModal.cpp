
#include <cstring>
#include <iostream>
#include <imgui/imgui.h>

#include "SaveSimulationModal.h"

using namespace std;
using namespace std::filesystem;

SaveSimulationModal::SaveSimulationModal(const path& p, Simulation *s) : _sim(s) {
    _flist.path = p;
    _flist.setFileTypeMask(FileList::Directory);
    _flist.refresh();
    _active = true;
    _chosen.reserve(1024);
    _edit[0] = '\0';
    ImGui::OpenPopup("Choose save slot");
}

bool SaveSimulationModal::frameStarted(const Ogre::FrameEvent &e) {
    if (ImGui::BeginPopupModal("Choose save slot")) {
        _chosen = _edit;
        ImGui::Text(_flist.path.string().data());
        if (ImGui::Button("Refresh")) {
            _flist.refresh();
        }
        for (auto const &p : _flist.getEntries()) {
            if (ImGui::Selectable(p.first.data(), false, ImGuiSelectableFlags_DontClosePopups)) {
                _chosen = p.first;
                strcpy(_edit, _chosen.data());
            }
        }
        ImGui::Separator();
        ImGui::Text(_chosen.data());
        if (ImGui::InputText("SlotName", _edit, 1024, ImGuiInputTextFlags_EnterReturnsTrue)) {
            _sim->save(_flist.path.string() + _chosen);
            ImGui::CloseCurrentPopup();
            _active = false;
        }
        ImGui::Separator();
        if (ImGui::Button("Ok")) {
            _sim->save(_flist.path.string() + _chosen);
            ImGui::CloseCurrentPopup();
            _active = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
            _active = false;
        }
        ImGui::EndPopup();
    }

    return true;
}
