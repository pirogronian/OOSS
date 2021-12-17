
#pragma once

#include <filesystem>

#include <utils/FileList.h>
#include "Simulation.h"
#include "SubModule.h"

class SaveSimulationModal : public SubModule {
    Simulation *_sim;
    FileList _flist;
public:
    SaveSimulationModal(const std::filesystem::path&, Simulation *);
    bool frameStarted(const Ogre::FrameEvent &);
};
