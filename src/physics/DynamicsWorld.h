
#pragma once

#include <physics/BtOgre.h>

#include <utils/AutoIndexer.h>

class RigidBody;

class DynamicsWorld
{
protected:
    btDefaultCollisionConfiguration _config;
    btCollisionDispatcher _dispatcher{&_config};
    btDbvtBroadphase _broadphase;
    btSequentialImpulseConstraintSolver _solver;
    btDiscreteDynamicsWorld _world{&_dispatcher, &_broadphase, &_solver, &_config};
    int _maxSubSteps{128};
    btScalar _minStepDelta{1./60.};
    AutoIndexer<RigidBody*> _bodies;
public:
    btDiscreteDynamicsWorld &getBtWorld() { return _world; }
    const btDiscreteDynamicsWorld &getBtWorld() const { return _world; }
    bool addRigidBody(RigidBody*);
    bool removeRigidBody(RigidBody*);
    btCollisionWorld::ClosestRayResultCallback rayTestClosest(const btVector3& start, const btVector3& stop) const
    {
        btCollisionWorld::ClosestRayResultCallback callback(start, stop);
        _world.rayTest(start, stop, callback);
        return callback;
    }
};
