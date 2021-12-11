
#pragma once

#include <vector>

#include <OgreViewport.h>
#include <OgreRenderTarget.h>

#include "Simulation.h"

class Player {
    Simulation *_sim;
    std::vector<Ogre::Viewport*> _vps;
public:
    Player(Simulation *s) : _sim(s) {}
    Simulation *getSimulation() { return _sim; }
    Simulation const *getSimulation() const { return _sim; }
    Ogre::RenderTarget *getRenderTarget();
    const Ogre::RenderTarget *getRenderTarget() const;
    Ogre::Viewport *addViewport(Ogre::Camera *, int = 0, Ogre::Real = 0, Ogre::Real = 0, Ogre::Real = 1, Ogre::Real = 1);
    int getNumViewports() const { return _vps.size(); }
    const std::vector<Ogre::Viewport*>& getViewports() const { return _vps; }
    int findViewportIndex(Ogre::Viewport*);
    bool removeViewport(int);
    void removeAllViewports();
    void clear();

    template<class Ar>
    void load(Ar &ia) {
        int vpn = 0;
        ia(vpn);
        while(vpn) {
            Ogre::String camname;
            int z = 0;
            Ogre::Real l, t, w, h;
            ia(camname, z, l, t, w, h);
            auto *cam = _sim->getSceneManager()->getCamera(camname);
            if (!cam)  continue;
            auto *vp = addViewport(cam, z, l, t, w, h);
            --vpn;
        }
    }

    template<class Ar>
    void save(Ar &oa) const {
        int n = _vps.size();
        oa(n);
        for (auto &vp : _vps) {
            oa(vp->getCamera()->getName());
            oa(
                vp->getZOrder(),
               vp->getLeft(),
               vp->getTop(),
               vp->getWidth(),
               vp->getHeight()
            );
        }
    }
};
