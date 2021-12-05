
#pragma once

#include <set>
#include <string>

#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreInput.h>
#include <OgreCameraMan.h>

#include <physics/DynamicsWorld.h>

class Simulation : public OgreBites::InputListener
{
    Ogre::SceneManager *_sceneMgr{nullptr};
    Ogre::RenderWindow *_rendWin{nullptr};
    OgreBites::CameraMan *_currentCamMan{nullptr};
    DynamicsWorld _world;
    BtOgre::DebugDrawer *_debugDrawer{nullptr};
    bool _debugDraw{true};
    bool _empty{true};
    std::set<int> _vps;
public:
    Simulation(Ogre::SceneManager *);
    bool isEmpty() const { return _empty; }
    void update(double);
    bool getDebugDraw() const { return _debugDraw; }
    void setDebugDraw(bool d) { _debugDraw = d; }
    BtOgre::DebugDrawer *getPhysicsDebugDrawer() { return _debugDrawer; }
    DynamicsWorld &getDynamicsWorld() { return _world; }
    Ogre::SceneManager *getSceneManager() { return _sceneMgr; }
    const Ogre::SceneManager *getSceneManager() const { return _sceneMgr; }
    void setRenderWindow(Ogre::RenderWindow *w) { _rendWin = w; }
    OgreBites::CameraMan *getCurrentCameraMan() { return _currentCamMan; }
    const OgreBites::CameraMan *getCurrentCameraMan() const { return _currentCamMan; }
    Ogre::Viewport *addViewport(Ogre::Camera *cam, int z) {
        auto *vp = _rendWin->addViewport(cam, z);
        _vps.insert(z);
        return vp;
    }

    bool mouseMoved (const OgreBites::MouseMotionEvent &evt);
    bool mousePressed (const OgreBites::MouseButtonEvent &evt);
    bool mouseReleased (const OgreBites::MouseButtonEvent &evt);
    bool mouseWheelRolled (const OgreBites::MouseWheelEvent &evt);
    bool touchMoved (const OgreBites::TouchFingerEvent &evt);
    bool touchPressed (const OgreBites::TouchFingerEvent &evt);
    bool touchReleased (const OgreBites::TouchFingerEvent &evt);

    void createDummyVieport();
    void destroyDummyVieport();
    void populate();
    void clear();
    void clearRigidBodies(bool);
    void clearGravityCenters(bool);
    void clearViewports(bool);
    void clearCameras();

    bool load(const std::string &);
    bool save(const std::string &) const;
};
