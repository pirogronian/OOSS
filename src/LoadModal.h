
#pragma once

#include "Simulation.h"

#include "LoadSaveModal.h"

class LoadModal : public SubModule {
    std::string _sn;
    std::filesystem::path _path;
    Simulation *_sim;
    LoadSaveModal *_lsmp{nullptr};
    bool _clr = false;
    void _close();
    void _load();
public:
    LoadModal(const std::filesystem::path&, Simulation*, std::string sn = std::string());
    bool frameStarted(const Ogre::FrameEvent &);
};
