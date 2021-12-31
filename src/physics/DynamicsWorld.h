
#pragma once

#include <filesystem>

#include "physics.h"

#include <physics/BtOgre.h>

#include <utils/AutoIndexer.h>

class RigidBody;

class GravityCenter;

class DynamicsWorld
{
protected:
    btDefaultCollisionConfiguration _config;
    btCollisionDispatcher _dispatcher{&_config};
    btDbvtBroadphase _broadphase;
    btSequentialImpulseConstraintSolver _solver;
    btDiscreteDynamicsWorld _world{&_dispatcher, &_broadphase, &_solver, &_config};
    int _maxSubSteps{10};
    btScalar _minStepDelta{1./60.};
    AutoIndexer<RigidBody*> _bodies;
    AutoIndexer<GravityCenter*> _gcenters;

    void _update(RigidBody*, btScalar);
    void _updateRigidBodies(btScalar);
public:
    btDiscreteDynamicsWorld &getBtWorld() { return _world; }
    const btDiscreteDynamicsWorld &getBtWorld() const { return _world; }
    bool addRigidBody(RigidBody*);
    bool removeRigidBody(RigidBody*);
    RigidBody *getRigidBody(size_t i) { return _bodies.get(i, nullptr); }
    const RigidBody *getRigidBody(size_t i) const { return _bodies.get(i, nullptr); }
    size_t rigidBodiesNumber() const { return _bodies.itemsNumber(); }
    size_t getMaxRigidBodyIndex() const { return _bodies.maxIndex(); }

    bool addGravityCenter(GravityCenter*);
    bool removeGravityCenter(GravityCenter*);
    GravityCenter *getGravityCenter(size_t i) { return _gcenters.get(i, nullptr); }
    const GravityCenter *getGravityCenter(size_t i) const { return _gcenters.get(i, nullptr); }
    size_t gravityCentersNumber() const { return _gcenters.itemsNumber(); }
    size_t getMaxGravityCenterIndex() const { return _gcenters.maxIndex(); }

    void setGlobalGravity(const btVector3& v) { _world.setGravity(v); }
    btVector3 getGlobalGravity() const { return _world.getGravity(); }
    void setMinStepDelta(btScalar d) { _minStepDelta = d; }
    btScalar getMinStepDelta() const { return _minStepDelta; }
    btScalar getMaxSubSteps() const { return _maxSubSteps; }
    void setMaxSubSteps(btScalar m) { _maxSubSteps = m; }
    void stepSimulation(btScalar d);

    btCollisionWorld::ClosestRayResultCallback rayTestClosest(const btVector3& start, const btVector3& stop) const
    {
        btCollisionWorld::ClosestRayResultCallback callback(start, stop);
        _world.rayTest(start, stop, callback);
        return callback;
    }

    void loadPhysics(std::filesystem::path const &);
    void savePhysics(std::filesystem::path const &);
};

void dump(const DynamicsWorld *);
inline void dump(const DynamicsWorld& w) { dump(&w); }
