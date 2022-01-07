
#include "RigidBody.h"

using namespace BtOgre;

void RigidBody::syncTransform()
{
    if (!_bbody || !_sceneNode)  return;
    auto pos = Convert::toBullet(_sceneNode->getPosition());
    auto rot = Convert::toBullet(_sceneNode->getOrientation());
    btTransform tr(rot, pos);
    _bbody->setWorldTransform(tr);
}

void RigidBody::syncInertia()
{
    if (!_bbody)  return;
    auto m = _bbody->getMass();
    _bbody->setMassProps(m, btVector3(m, m, m));
}

void RigidBody::setMass(btScalar m)
{
    if (!_bbody)  return;
    _bbody->setMassProps(m, btVector3(m, m, m));
}

void RigidBody::setBtRigidBody(btRigidBody *body)
{
    clearBtRigidBody();
    _bbody = body;
    _bbody->setUserPointer(this);
    _bbody->setUserIndex(_worldIndex);
    _bbody->setActivationState(DISABLE_DEACTIVATION);
    _bbody->setMotionState(_rbmState);
    _btrbIndex = _bbody->getWorldArrayIndex();
    syncInertia();
    syncTransform();
    if (_world)
        _world->getBtWorld().addRigidBody(_bbody);
}

void RigidBody::clearBtRigidBody()
{
    if (_bbody)
    {
        _bbody->setUserPointer(nullptr);
        _bbody->setMotionState(nullptr);
        if (_world) _world->getBtWorld().removeRigidBody(_bbody);
        _bbody = nullptr;
    }
}

void RigidBody::setSceneNode(Ogre::SceneNode *n)
{
    clearSceneNode();
    _sceneNode = n;
    _nodeName = _sceneNode->getName();
    _rbmState = new BtOgre::RigidBodyState(_sceneNode);
    if (_bbody)  _bbody->setMotionState(_rbmState);
}

void RigidBody::clearSceneNode()
{
    if (_bbody && _sceneNode)
    {
        _bbody->setMotionState(nullptr);
        delete _rbmState;
        _rbmState = nullptr;
    }
    _sceneNode = nullptr;
}

void dump(const RigidBody *rb) {
    dump(rb->getBtRigidBody());
}
