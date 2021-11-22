
#include <iostream>

#include <utils/DumpHelper.h>

#include "DynamicsWorld.h"

#include "RigidBody.h"
#include "GravityCenter.h"

using namespace std;

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

void DynamicsWorld::_update(RigidBody *rb, btScalar delta) {
    for(std::size_t i = 0; i < _gcenters.size(); i++)
    {
        if (_gcenters.contains(i)) {
            GravityCenter *gc = _gcenters.get(i);
            gc->actOn(rb);
        }
    }
}

void DynamicsWorld::_updateRigidBodies(btScalar delta) {
    for(std::size_t i = 0; i < _bodies.size(); i++)
    {
        if (_bodies.contains(i)) {
            RigidBody *rb = _bodies.get(i);
            _update(rb, delta);
        }
    }
}

void DynamicsWorld::stepSimulation(btScalar d)
{
    _updateRigidBodies(d);
    _world.stepSimulation(d, _maxSubSteps, _minStepDelta);
}

void dump(const DynamicsWorld *dw) {
    Dump.begin("DynamicsWorld");
    size_t i = 0, c = 0, n = dw->rigidBodiesNumber();
    Dump.putParam("RigidBodies number:", n);
    while(c < n) {
        const RigidBody *rb = dw->getRigidBody(i);
//         cout << i << ", " << c << endl;
        i++;
        if (!rb)  continue;
        c++;
        dump(rb);
    }
    Dump.end();
}
