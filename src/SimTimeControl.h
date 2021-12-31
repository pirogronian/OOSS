
#pragma once

#include "SubModule.h"

#include "Simulation.h"

class SimTimeControl : public SubModule {
    Simulation *_sim;
    int _mt {0};
    bool *_v;
public:
    SimTimeControl(Simulation *s, bool * = nullptr);
    bool frameStarted(const Ogre::FrameEvent &);
};
