
#pragma once

#include <vector>
#include <cereal/archives/xml.hpp>

#include <OgreViewport.h>
#include <OgreRenderTarget.h>
#include <OgreInput.h>
#include <OgreCameraMan.h>

#include "Simulation.h"

struct ViewportData {
    Ogre::String camName;
    Ogre::Real left, top, width, height, zorder;
    void from(Ogre::Viewport const *);
    template<class Ar>
    void serialize(Ar &a) {
        a(cereal::make_nvp("CameraName", camName),
        cereal::make_nvp("ZOrder", zorder),
        cereal::make_nvp("Left", left),
        cereal::make_nvp("Top", top),
        cereal::make_nvp("Width", width),
        cereal::make_nvp("Height", height));
    }
};

std::ostream& operator<<(std::ostream&, ViewportData const &);

class Player : public OgreBites::InputListener {
    Simulation *_sim{nullptr};
    std::vector<Ogre::Viewport*> _vps;
    OgreBites::CameraMan *_cm{nullptr};
public:
    struct CamManData {
        Ogre::String camName;
        int camStyle;
    };
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
            ViewportData vpd;
            ia(vpd);
            auto *cam = _sim->getSceneManager()->getCamera(vpd.camName);
            if (!cam)  {
                std::cout << "Player::load: Camera not found, viewport " << vpd.zorder << " not created!\n";
                continue;
            }
            std::cout << vpd << std::endl;
            auto *vp = addViewport(cam, vpd.zorder, vpd.left, vpd.top, vpd.width, vpd.height);
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
        oa(cereal::make_nvp("VieportNum", n));
        for (auto &vp : _vps) {
            ViewportData vpd;
            vpd.from(vp);
            oa(cereal::make_nvp("Viewport", vpd));
            /*oa(cereal::make_nvp("CamName", vp->getCamera()->getName()));
            oa(
                cereal::make_nvp("ZOrder", vp->getZOrder()),
               cereal::make_nvp("Left", vp->getLeft()),
               cereal::make_nvp("Top", vp->getTop()),
               cereal::make_nvp("Width", vp->getWidth()),
               cereal::make_nvp("Height", vp->getHeight())
            );*/
        }
        {
            auto name = _cm->getCamera()->getName();
            auto cs = _cm->getStyle();
            oa(cereal::make_nvp("ThirdCamName", name));
            oa(cereal::make_nvp("ThirdCamStyle", cs));
        }
    }
};
