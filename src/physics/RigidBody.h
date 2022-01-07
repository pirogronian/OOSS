
#pragma once

#include <cereal/cereal.hpp>

#include "physics.h"

#include "physics/BtOgre.h"

#include <physics/DynamicsWorld.h>

class GravityCenter;

class Simulation;

class RigidBody
{
    friend DynamicsWorld;
    friend GravityCenter;
    friend Simulation;
protected:
    Ogre::SceneNode *_sceneNode{nullptr};
    Ogre::String _nodeName;
    BtOgre::RigidBodyState *_rbmState{nullptr};
    btRigidBody *_bbody{nullptr};
    int _btrbIndex{-1};
    DynamicsWorld *_world{nullptr};
    std::size_t _worldIndex{std::numeric_limits<std::size_t>::max()};

    mutable GravityCenter *_gc{nullptr};
    void setWorld(DynamicsWorld *w) { _world = w; }
    void setWorldIndex(std::size_t i) {
        _worldIndex = i;
        if (_bbody) _bbody->setUserIndex(_worldIndex);
    }
    void setGravityCenter(GravityCenter *gc) { _gc = gc; }
public:
    std::size_t getWorldIndex() const { return _worldIndex; }
    btRigidBody *getBtRigidBody() { return _bbody; }
    void setMass(btScalar);
    btTransform getTransform() const { return _bbody ? _bbody->getWorldTransform() : btTransform(); }
    void syncTransform();
    void syncInertia();
    const btRigidBody *getBtRigidBody() const { return _bbody; }
    void setBtRigidBody(btRigidBody*);
    void clearBtRigidBody();
    Ogre::SceneNode *getSceneNode() { return _sceneNode; }
    const Ogre::SceneNode *getSceneNode() const { return _sceneNode; }
    void setSceneNode(Ogre::SceneNode*);
    void clearSceneNode();
    GravityCenter *getGravityCenter() const { return _gc; }
//     GravityCenter *getGravityCenter() { return _gc; }

    RigidBody() = default;
    RigidBody(Ogre::SceneNode *node, btRigidBody *body)
    {
        setSceneNode(node);
        setBtRigidBody(body);
    }
    RigidBody(Ogre::SceneNode *node, btScalar mass, btCollisionShape *shape)
    {
        setSceneNode(node);
        setBtRigidBody(new btRigidBody(mass, nullptr, shape));
    }

    void applyForce(const btVector3 &f, const btVector3& offset = btVector3(0, 0, 0)) {
        if (!_bbody)  return;
        _bbody->applyForce(f, offset);
    }
    void applyTorque(const btVector3& rot)
    {
        if (!_bbody)  return;
        _bbody->applyTorque(rot);
    }
    void applyTorqueLocal(const btVector3& rot)
    {
        if (!_bbody)  return;
        _bbody->applyTorque(_bbody->getWorldTransform().getBasis() * rot);
    }
    void applyForceLocal(const btVector3& f, const btVector3& offset = btVector3(0, 0, 0))
    {
        if (!_bbody)  return;
        auto &tr = _bbody->getWorldTransform().getBasis();
        _bbody->applyForce(tr * f, tr * offset);
    }
    btVector3 getAppliedForce() const { return _bbody ? _bbody->getTotalForce() : btVector3(0, 0, 0); }
    btCollisionWorld::ClosestRayResultCallback rayTestClosest(const btVector3& start,
                        const btVector3& stop) const
    {
        auto tr = getTransform();
        if (!_world)  return btCollisionWorld::ClosestRayResultCallback(start, stop);
        return _world->rayTestClosest(tr * start, tr * stop);
    }
    btCollisionWorld::ClosestRayResultCallback rayTestClosest(const btVector3& stop) const
    {
        return rayTestClosest(btVector3(0, 0, 0), stop);
    }

    template<class Ar>
    void serialize(Ar &a) {
        a(cereal::make_nvp("NodeName", _nodeName),
          cereal::make_nvp("btRigidBodyIndex", _worldIndex),
          cereal::make_nvp("btRigidBodyWorldIndex", _btrbIndex));
    }
};

void dump(const btRigidBody *);
void dump(const RigidBody *);
