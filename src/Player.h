
#pragma once

#include <vector>

#include <OgreViewport.h>
#include <OgreRenderTarget.h>

class Player {
    Ogre::RenderTarget *_rt{nullptr};
    std::vector<Ogre::Viewport*> _vps;
public:
    Player(Ogre::RenderTarget *rt) : _rt(rt) {}
    bool addViewport(Ogre::Camera *, int = 0, Ogre::Real = 0, Ogre::Real = 0, Ogre::Real = 0, Ogre::Real = 0);
    const std::vector<Ogre::Viewport*>& getViewports() const { return _vps; }
    int findViewportIndex(Ogre::Viewport*);
    bool removeViewport(int);
};
