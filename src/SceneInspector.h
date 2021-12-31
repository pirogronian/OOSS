
#pragma once

#include "SubModule.h"

#include "Simulation.h"

class SceneInspector : public SubModule {
    Simulation *_sim;
    bool *_v;
public:
    SceneInspector(Simulation *s, bool * = nullptr);
    bool frameStarted(const Ogre::FrameEvent &);
    void updateTreeNode(Ogre::SceneNode const *);
};
