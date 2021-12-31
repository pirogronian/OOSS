
#include <cmath>
#include <imgui/imgui.h>

#include "SceneInspector.h"

using namespace std;
using namespace Ogre;

SceneInspector::SceneInspector(Simulation *s, bool *v) : _sim(s), _v(v) {}

bool SceneInspector::frameStarted(const Ogre::FrameEvent &e) {
    if (!*_v) return false;
    if (ImGui::Begin("Scene inspector", _v)) {
        updateTreeNode(_sim->getSceneManager()->getRootSceneNode());
    }
    ImGui::End();
    return true;
}

void SceneInspector::updateTreeNode(Ogre::SceneNode const *node) {
    if (ImGui::TreeNode(reinterpret_cast<const void*>(node), "%s", node->getName().data())) {
        auto children = node->getChildren();
        for (auto child : children)
            updateTreeNode(dynamic_cast<Ogre::SceneNode const *>(child));
        ImGui::TreePop();
    }
}
