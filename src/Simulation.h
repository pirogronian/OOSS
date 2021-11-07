
#pragma once

#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreCameraMan.h>

#include <physics/DynamicsWorld.h>

class Simulation
{
    Ogre::Root *_root{nullptr};
    Ogre::SceneManager *_sceneMgr;
    Ogre::Camera *_mainCam{nullptr};
    OgreBites::CameraMan *_mainCamMan{nullptr};
    DynamicsWorld _world;
    BtOgre::DebugDrawer *_debugDrawer{nullptr};
    bool _debugDraw{true};
public:
    Simulation(Ogre::Root *);
    void update(double);
    bool getDebugDraw() const { return _debugDraw; }
    void setDebugDraw(bool d) { _debugDraw = d; }
    Ogre::Camera *getMainCamera() { return _mainCam; }
    const Ogre::Camera *getMainCamera() const { return _mainCam; }
    OgreBites::CameraMan *getMainCameraMan() { return _mainCamMan; }
    const OgreBites::CameraMan *getMainCameraMan() const { return _mainCamMan; }
};