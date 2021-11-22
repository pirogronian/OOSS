
#include <OgreShaderGenerator.h>

#include "Simulation.h"

#include <physics/RigidBody.h>
#include <physics/GravityCenter.h>

Simulation::Simulation(Ogre::SceneManager *sceneMgr)
{
    _sceneMgr = sceneMgr;
    Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
    shadergen->addSceneManager(_sceneMgr);
    _debugDrawer = new BtOgre::DebugDrawer(_sceneMgr->getRootSceneNode(), &_world.getBtWorld());

    _mainCam = _sceneMgr->createCamera("MainCamera");
    _mainCam->setNearClipDistance(0.5);
    _mainCam->setAutoAspectRatio(true);
    auto *camnode = _sceneMgr->getRootSceneNode()->createChildSceneNode();
    camnode->attachObject(_mainCam);

    auto *ogreEnt = _sceneMgr->createEntity("ogrehead.mesh");
    auto *ogrenode = _sceneMgr->getRootSceneNode()->createChildSceneNode();
    ogrenode->attachObject(ogreEnt);

    auto *fishEnt = _sceneMgr->createEntity("fish.mesh");
    auto *fishNode = _sceneMgr->getRootSceneNode()->createChildSceneNode();
    fishNode->attachObject(fishEnt);
    fishNode->setPosition(0, 50, 0);

    _sceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

    _mainCamMan = new OgreBites::CameraMan(camnode);
    _mainCamMan->setStyle(OgreBites::CS_ORBIT);
    _mainCamMan->setYawPitchDist(Ogre::Degree(45), Ogre::Degree(45), 120);

    auto shape1 = BtOgre::StaticMeshToShapeConverter(ogreEnt).createTrimesh();
    auto body1 = new RigidBody(ogrenode, 0, shape1);
    _world.addRigidBody(body1);

    auto shape2 = BtOgre::StaticMeshToShapeConverter(fishEnt).createConvex();
//    auto bbody2 = new btRigidBody(1, nullptr, shape2);
    auto body2 = new RigidBody(fishNode, 1, shape2);
//    auto body2 = new RigidBody(fishNode, bbody2);
    _world.addRigidBody(body2);

//     body2->getBtRigidBody()->setLinearVelocity(btVector3(-1, 0, 0));

    auto gc = new GravityCenter(body1);
    gc->setFactor(1000);
    _world.addGravityCenter(gc);

    _world.setGlobalGravity(btVector3(0, 0, 0));

//     body2->getBtRigidBody()->applyCentralImpulse(btVector3(91, 0, 0));
//     body2->getBtRigidBody()->applyTorqueImpulse(btVector3(92, 0, 0));
//     body2->applyForce(btVector3(0, -7, 0));
//     body2->getBtRigidBody()->integrateVelocities(0.1);
//     dump(body2->getBtRigidBody());
//     body2->applyTorque(btVector3(94, 0, 0));
}

void Simulation::update(double delta)
{
//     dump(_world);
    _world.stepSimulation(delta);

    if(_debugDraw)
       	_debugDrawer->update();
}
