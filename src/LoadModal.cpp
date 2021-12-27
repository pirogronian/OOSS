
#include <imgui/imgui.h>

#include "LoadModal.h"

using namespace std;
using namespace Ogre;

LoadModal::LoadModal(const filesystem::path &p, Simulation *s, string sn, bool bi) : _path(p), _sim(s), _sn(sn), _builtin(bi) {
    ImGui::OpenPopup("Simulation not empty!");
    _active = true;
}

void LoadModal::_close() {
    ImGui::CloseCurrentPopup();
    _active = false;
}

void LoadModal::_load() {
    _sim->load(_path / _sn);
}

bool LoadModal::frameStarted(const Ogre::FrameEvent &e) {
    if (ImGui::BeginPopupModal("Simulation not empty!")) {
        ImGui::Text("Simulation is not empty. Are you sure to load another one?");
        ImGui::Checkbox("Clear before load", &_clr);
        if (ImGui::Button("Yes")) {
            if (!_builtin && _sn.empty()) {
                cout << "LoadModal: starting LoadSaveModal\n";
                _lsmp = new LoadSaveModal(_path, _sim, LoadSaveModal::Load, _clr);
            } else {
                cout << "LoadModal: load default or builtin\n";
                if (_clr)  _sim->clear();
                if (_builtin) _sim->populate();
                else _load();
                _close();
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("No")) {
            _close();
        }
        if (_lsmp) {
            if (_lsmp->isActive())  _lsmp->frameStarted(e);
            else {
                delete _lsmp;
                _close();
            }
        }
        ImGui::EndPopup();
    }

    return true;
}
