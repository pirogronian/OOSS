
#include <cassert>

#include "GravityCenter.h"

btVector3 GravityCenter::getPosition() const
{
    if (isType<btVector3>())
        return std::get<btVector3>(_position);
    if (isType<RigidBody*>()) {
        auto rb = std::get<RigidBody*>(_position);
        return rb->getTransform().getOrigin();
    }
    auto sn = std::get<Ogre::SceneNode*>(_position);
    return BtOgre::Convert::toBullet(sn->getPosition());
}

bool GravityCenter::checkOwner() const {
    return (isType<RigidBody*>() && isOwner(getOwner<RigidBody*>()) ||
        (isType<Ogre::SceneNode*>() && isOwner(getOwner<Ogre::SceneNode*>())) ||
        (isType<btVector3>()));
}

void GravityCenter::_clearOwnership(RigidBody *rb) {
    rb->setGravityCenter(nullptr);
}

void GravityCenter::_clearOwnership(Ogre::SceneNode *sn) {
    sn->getUserObjectBindings().eraseUserAny("GravityCenter");
}

void GravityCenter::_clearOwnership() {
    assert(checkOwner());
    if (isType<RigidBody*>()) { _clearOwnership(getOwner<RigidBody*>()); return; }
    if (isType<Ogre::SceneNode*>()) { _clearOwnership(getOwner<Ogre::SceneNode*>()); return; }
}

void GravityCenter::_setOwner(RigidBody *rb) {
    _position = rb;
    rb->setGravityCenter(this);
}

void GravityCenter::_setOwner(Ogre::SceneNode *sn) {
    _position = sn;
    sn->getUserObjectBindings().setUserAny("GravityCenter", this);
}

void GravityCenter::setOwner(RigidBody *rb) {
    _clearOwnership();
    _setOwner(rb);
}

void GravityCenter::setOwner(Ogre::SceneNode *sn) {
    _clearOwnership();
    _setOwner(sn);
}
