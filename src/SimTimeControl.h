
#pragma once

#include "SubModule.h"

#include "Simulation.h"

class SimTimeControl : public SubModule {
    Simulation *_sim;
    int _mt {0};
public:
    bool visible{true};
    SimTimeControl(Simulation *s);
    bool frameStarted(const Ogre::FrameEvent &);
};
