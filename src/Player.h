
#pragma once

#include <vector>

#include <OgreViewport.h>
#include <OgreRenderTarget.h>

class Player {
    Ogre::RenderTarget *_rt{nullptr};
    std::vector<Ogre::Viewport*> _vps;
public:
    Player(Ogre::RenderTarget *rt) : _rt(rt) {}
    Ogre::RenderTarget *getRenderTarget() { return _rt; }
    const Ogre::RenderTarget *getRenderTarget() const { return _rt; }
    void setRenderTarget(Ogre::RenderTarget *rt) { _rt = rt; } // should it remove all viewports from previous target?
    Ogre::Viewport *addViewport(Ogre::Camera *, int = 0, Ogre::Real = 0, Ogre::Real = 0, Ogre::Real = 1, Ogre::Real = 1);
    int getNumViewports() const { return _vps.size(); }
    const std::vector<Ogre::Viewport*>& getViewports() const { return _vps; }
    int findViewportIndex(Ogre::Viewport*);
    bool removeViewport(int);
    void removeAllViewports();
    void clear();
};
