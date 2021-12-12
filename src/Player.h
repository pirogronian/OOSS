
#pragma once

#include <vector>

#include <OgreViewport.h>
#include <OgreRenderTarget.h>
#include <OgreInput.h>
#include <OgreCameraMan.h>

#include "Simulation.h"

class Player : public OgreBites::InputListener {
    Simulation *_sim{nullptr};
    std::vector<Ogre::Viewport*> _vps;
    OgreBites::CameraMan *_cm{nullptr};
public:
    Player(Simulation *s) : _sim(s) {}

    Simulation *getSimulation() { return _sim; }
    Simulation const *getSimulation() const { return _sim; }

    Ogre::RenderTarget *getRenderTarget();
    const Ogre::RenderTarget *getRenderTarget() const;

    void createCameraMan(OgreBites::CameraStyle = OgreBites::CS_FREELOOK, Ogre::SceneNode* = nullptr);
    void deleteCameraMan();
    OgreBites::CameraMan *getCameraMan() { return _cm; }
    OgreBites::CameraMan const *getCameraMan() const { return _cm; }

    Ogre::Viewport *addViewport(Ogre::Camera *, int = 0, Ogre::Real = 0, Ogre::Real = 0, Ogre::Real = 1, Ogre::Real = 1);
    int getNumViewports() const { return _vps.size(); }
    const std::vector<Ogre::Viewport*>& getViewports() const { return _vps; }
    int findViewportIndex(Ogre::Viewport*);
    bool removeViewport(int);
    void removeAllViewports();
    void clear();

    bool mouseMoved (const OgreBites::MouseMotionEvent &evt);
    bool mousePressed (const OgreBites::MouseButtonEvent &evt);
    bool mouseReleased (const OgreBites::MouseButtonEvent &evt);
    bool mouseWheelRolled (const OgreBites::MouseWheelEvent &evt);
    bool touchMoved (const OgreBites::TouchFingerEvent &evt);
    bool touchPressed (const OgreBites::TouchFingerEvent &evt);
    bool touchReleased (const OgreBites::TouchFingerEvent &evt);

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
        {
            deleteCameraMan();
            Ogre::String name;
            OgreBites::CameraStyle cs;
            ia(name);
            ia(cs);
            createCameraMan(cs, _sim->getSceneManager()->getSceneNode(name));
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
        {
            auto name = _cm->getCamera()->getName();
            auto cs = _cm->getStyle();
            oa(name);
            oa(cs);
        }
    }
};
