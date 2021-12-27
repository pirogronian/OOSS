
#pragma once

#include <set>
#include <string>
#include <filesystem>

#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>

#include <physics/DynamicsWorld.h>

class Player;

class Simulation
{
    Ogre::SceneManager *_sceneMgr{nullptr};
    Ogre::RenderTarget *_rt{nullptr};
    DynamicsWorld _world;
    BtOgre::DebugDrawer *_debugDrawer{nullptr};
    bool _debugDraw{true};
    bool _empty{true};
    Player *_pl{nullptr};
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
    Ogre::RenderTarget *getRenderTarget() { return _rt; }
    Ogre::RenderTarget const *getRenderTarget() const { return _rt; }
    void setRenderTarget(Ogre::RenderTarget *rt) { _rt = rt; }
    Player *getPlayer() { return _pl; }
    Player const *getPlayer() const { return _pl; }

    void createDummyVieport();
    void destroyDummyVieport();
    void loadBuiltin(const std::string&);
    void populate();
    void clear();
    void clearRigidBodies(bool);
    void clearGravityCenters(bool);
    void clearCameras();

    bool load(const std::filesystem::path &);
    bool save(const std::filesystem::path &) const;
};
