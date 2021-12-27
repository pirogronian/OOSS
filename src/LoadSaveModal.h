
#pragma once

#include <filesystem>

#include <utils/FileList.h>
#include "Simulation.h"
#include "SubModule.h"

class LoadSaveModal : public SubModule {
public:
    enum Type {
        Load,
        Save
    };
    Simulation *_sim;
    FileList _flist;
    std::string _chosen;
    char _edit[1024];
    Type _type;
    const char *_cpt;
    bool _clr{false};
    void _doLoad();
    void _doSave();
    void _doClose();
public:
    LoadSaveModal(const std::filesystem::path&, Simulation *, Type, bool = false);
    bool frameStarted(const Ogre::FrameEvent &);
};
