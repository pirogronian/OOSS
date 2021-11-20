
#include "DynamicsWorld.h"

#include "RigidBody.h"
#include "GravityCenter.h"

bool DynamicsWorld::addRigidBody(RigidBody *b)
{
    if (_bodies.contains(b->getWorldIndex()))  return false;
    b->setWorld(this);
    if (b->getBtRigidBody())  _world.addRigidBody(b->getBtRigidBody());
    auto i = _bodies.add(b);
    b->setWorldIndex(i);
    b->_world = this;
    if (b->_bbody) b->_bbody->setUserIndex(i);
    return true;
}

bool DynamicsWorld::removeRigidBody(RigidBody *b)
{
    if (!_bodies.contains(b->getWorldIndex()))  return false;
    b->setWorld(nullptr);
    if (b->getBtRigidBody())  _world.removeRigidBody(b->getBtRigidBody());
    _bodies.remove(b->getWorldIndex());
    b->setWorldIndex(std::numeric_limits<std::size_t>::max());
    return true;
}

bool DynamicsWorld::addGravityCenter(GravityCenter *gc)
{
    if (_gcenters.contains(gc->getWorldIndex()))  return false;
    auto i = _gcenters.add(gc);
    gc->_setWorldIndex(i);
    gc->_setWorld(this);
    return true;
}

bool DynamicsWorld::removeGravityCenter(GravityCenter *gc)
{
    if (!_gcenters.contains(gc->getWorldIndex()))  return false;
    gc->_setWorld(nullptr);
    _gcenters.remove(gc->getWorldIndex());
    gc->_setWorldIndex(std::numeric_limits<std::size_t>::max());
    return true;
}

void DynamicsWorld::stepSimulation(btScalar d)
{
    _world.stepSimulation(d, _maxSubSteps, _minStepDelta);
}
