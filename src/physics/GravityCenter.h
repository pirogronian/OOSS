
#pragma once

#include <variant>
#include <OgreSceneNode.h>
#include "RigidBody.h"
#include "DynamicsWorld.h"

class GravityCenter
{
    btScalar _factor{1};
    std::variant<btVector3, RigidBody*, Ogre::SceneNode*> _position{btVector3(0, 0, 0)};
    bool _autoClear{true};

    friend DynamicsWorld;

    DynamicsWorld *_world{nullptr};
    std::size_t _worldIndex{std::numeric_limits<std::size_t>::max()};

    static void _clearOwnership(RigidBody*);
    static void _clearOwnership(Ogre::SceneNode*);
    void _clearOwnership();
    void _setWorld(DynamicsWorld *w) { _world = w; }
    void _setWorldIndex(std::size_t i) { _worldIndex = i; }
    void _setOwner(RigidBody *);
    void _setOwner(Ogre::SceneNode *);
public:
    btScalar getFactor() const { return _factor; }
    void setFactor(btScalar f) { _factor = f; }
    std::size_t getWorldIndex() const { return _worldIndex; }
    bool getAutoClear() const { return _autoClear; }
    void setAutoClear(bool ac) { _autoClear = ac; }
    bool isOwner(const RigidBody *rb) const { return rb->getGravityCenter() == this; }
    bool isOwner(const Ogre::SceneNode *sn) const {
        Ogre::Any any = sn->getUserObjectBindings().getUserAny("GravityCenter");
        return any.has_value() && Ogre::any_cast<GravityCenter*>(any) == this;
    }
    template<typename T>
    bool isType() const { return std::holds_alternative<T>(_position); }
    template<typename T>
    T getOwner() { return std::get<T>(_position); }
    template<typename T>
    const T getOwner() const { return std::get<T>(_position); }
    bool checkOwner() const;
    btVector3 getPosition() const;
    void reset() { _clearOwnership(); _position = btVector3(0, 0, 0); }
    void setPosition(btVector3 v);
    void setOwner(RigidBody*);
    void setOwner(Ogre::SceneNode*);
    void actOn(RigidBody*);
    GravityCenter() = default;
    GravityCenter(btVector3 v) : _position(v) {}
    GravityCenter(RigidBody *rb) { _setOwner(rb); }
    GravityCenter(Ogre::SceneNode *sn) { _setOwner(sn); }
    ~GravityCenter() { if (_autoClear) _clearOwnership(); }
};

class GravityCenterOwner
{
    GravityCenter *_gc{nullptr};
public:
//     void setGravityCenter(GravityCenter)
};
