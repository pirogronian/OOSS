
#include <cmath>
#include <imgui/imgui.h>

#include "SimTimeControl.h"

using namespace std;
using namespace Ogre;

SimTimeControl::SimTimeControl(Simulation *s, bool *v) : _sim(s), _v(v) {}

bool SimTimeControl::frameStarted(const Ogre::FrameEvent &e) {
    if (!*_v) return false;
    if (ImGui::Begin("Time control", _v)) {
        ImGui::Text("Time: %f", _sim->getSimulationTime());
        if (_sim->isPaused()) {
            if (ImGui::Button("Resume"))  _sim->resume();
        } else {
            if (ImGui::Button("Pause"))  _sim->pause();
        }
    }
    double ts = _sim->getTimeScale();
    if (ImGui::Button("2x slower")) {
        _mt -= 1;
        _sim->setTimeScale(pow(2, _mt));
    }
    ImGui::SameLine(); ImGui::Text("%f", ts); ImGui::SameLine();
    if (ImGui::Button("2x faster")) {
        _mt += 1;
        _sim->setTimeScale(pow(2, _mt));
    }
    ImGui::End();
    return true;
}
