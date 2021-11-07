
#include "DynamicsWorld.h"

#include "RigidBody.h"

bool DynamicsWorld::addRigidBody(RigidBody *b)
{
    if (_bodies.contains(b->getWorldIndex()))  return false;
    b->setWorld(this);
    if (b->getBtRigidBody())  _world.addRigidBody(b->getBtRigidBody());
    auto i = _bodies.add(b);
    b->setWorldIndex(i);
    b->_world = this;
    return true;
}

bool DynamicsWorld::removeRigidBody(RigidBody *b)
{
    if (!_bodies.contains(b->getWorldIndex()))  return false;
    b->setWorld(nullptr);
    if (b->getBtRigidBody())  _world.removeRigidBody(b->getBtRigidBody());
    _bodies.remove(b->getWorldIndex());
    b->setWorldIndex(std::numeric_limits<std::size_t>::max());
    b->_world = nullptr;
    return true;
}
