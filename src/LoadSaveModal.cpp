
#include <cstring>
#include <iostream>
#include <imgui/imgui.h>

#include "LoadSaveModal.h"

using namespace std;
using namespace std::filesystem;

LoadSaveModal::LoadSaveModal(const path& p, Simulation *s, Type t, bool clr) : _sim(s), _type(t), _clr(clr) {
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

void LoadSaveModal::_doLoad() {
    if (_clr) _sim->clear();
    _sim->load(_flist.path / _chosen);
    _doClose();
}

void LoadSaveModal::_doSave() {
    _sim->save(_flist.path / _chosen);
    _doClose();
}

void LoadSaveModal::_doClose() {
    ImGui::CloseCurrentPopup();
    _active = false;
}

bool LoadSaveModal::frameStarted(const Ogre::FrameEvent &e) {
    if (ImGui::BeginPopupModal(_cpt)) {
        _chosen = _edit;
        bool close = false;
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
            if (_type == Save) {
                if (_flist.contains(_chosen))
                    ImGui::OpenPopup("Overwrite?");
                else _doSave();
            }
            else
                _doLoad();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
            _active = false;
        }
        bool save = false;
        if (ImGui::BeginPopupModal("Overwrite?")) {
            ImGui::Text("Following slot already exists. Are you sure to overwrite it?");
            ImGui::Text(_chosen.data());
            ImGui::Separator();
            if (ImGui::Button("Yes")) {
                ImGui::CloseCurrentPopup();
                save = true;
            }
            ImGui::SameLine();
            if (ImGui::Button("No")) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        if (save)  _doSave();
        ImGui::EndPopup();
    }

    return true;
}
