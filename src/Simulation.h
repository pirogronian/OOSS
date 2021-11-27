
#pragma once

#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreCameraMan.h>

#include <physics/DynamicsWorld.h>

class Simulation
{
    Ogre::SceneManager *_sceneMgr{nullptr};
    Ogre::Camera *_mainCam{nullptr};
    OgreBites::CameraMan *_mainCamMan{nullptr};
    DynamicsWorld _world;
    BtOgre::DebugDrawer *_debugDrawer{nullptr};
    bool _debugDraw{true};
public:
    Simulation(Ogre::SceneManager *);
    void update(double);
    bool getDebugDraw() const { return _debugDraw; }
    void setDebugDraw(bool d) { _debugDraw = d; }
    BtOgre::DebugDrawer *getPhysicsDebugDrawer() { return _debugDrawer; }
    DynamicsWorld &getDynamicsWorld() { return _world; }
    Ogre::SceneManager *getSceneManager() { return _sceneMgr; }
    const Ogre::SceneManager *getSceneManager() const { return _sceneMgr; }
    Ogre::Camera *getMainCamera() { return _mainCam; }
    const Ogre::Camera *getMainCamera() const { return _mainCam; }
    OgreBites::CameraMan *getMainCameraMan() { return _mainCamMan; }
    const OgreBites::CameraMan *getMainCameraMan() const { return _mainCamMan; }

    void populate();
    void clear();
    void clearRigidBodies(bool);
    void clearGravityCenters(bool);
};
