
#include <cstring>
#include <iostream>
#include <imgui/imgui.h>

#include "LoadSaveModal.h"

using namespace std;
using namespace std::filesystem;

LoadSaveModal::LoadSaveModal(const path& p, Simulation *s, Type t) : _sim(s), _type(t) {
    _flist.path = p;
    _flist.setFileTypeMask(FileList::Directory);
    _flist.refresh();
    _active = true;
    _chosen.reserve(1024);
    _edit[0] = '\0';
    if (_type == Load)  _cpt = "Choose slot to load";
    else _cpt = "Choose or type slot to save";
    ImGui::OpenPopup(_cpt);
}

bool LoadSaveModal::frameStarted(const Ogre::FrameEvent &e) {
    if (ImGui::BeginPopupModal(_cpt)) {
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
        if (_type == Load)
            ImGui::Text(_chosen.data());
        else {
            if (ImGui::InputText("SlotName", _edit, 1024, ImGuiInputTextFlags_EnterReturnsTrue)) {
                _sim->save(_flist.path.string() + _chosen);
                ImGui::CloseCurrentPopup();
                _active = false;
            }
        }
        ImGui::Separator();
        if (ImGui::Button("Ok")) {
            if (_type == Save)
                _sim->save(_flist.path.string() + _chosen);
            else
                _sim->load(_flist.path.string() + _chosen);
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
