
#pragma once

#include "physics/BtOgre.h"

#include <physics/DynamicsWorld.h>

class RigidBody
{
    friend DynamicsWorld;
protected:
    Ogre::SceneNode *_sceneNode{nullptr};
    BtOgre::RigidBodyState *_rbmState{nullptr};
    btRigidBody *_bbody{nullptr};
    DynamicsWorld *_world{nullptr};
    std::size_t _worldIndex{std::numeric_limits<std::size_t>::max()};

    void setWorld(DynamicsWorld *w) { _world = w; }
    void setWorldIndex(std::size_t i) { _worldIndex - i; }
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
    void setSceneNode(Ogre::SceneNode*);
    void clearSceneNode();
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
};
