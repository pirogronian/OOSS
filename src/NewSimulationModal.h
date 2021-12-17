
#pragma once

#include "SubModule.h"

#include "Simulation.h"

class NewSimulationModal : public SubModule {
    Simulation *_sim;
public:
    NewSimulationModal(Simulation *s);
    bool frameStarted(const Ogre::FrameEvent&);
};
