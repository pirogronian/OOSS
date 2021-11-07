
#include <OgreShaderGenerator.h>

#include "Simulation.h"

#include <physics/RigidBody.h>

Simulation::Simulation(Ogre::Root *root)
{
    _root = root;
    _sceneMgr = _root->createSceneManager();
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
    auto body = new RigidBody(ogrenode, 0, shape1);
    _world.addRigidBody(body);

    auto shape2 = BtOgre::StaticMeshToShapeConverter(fishEnt).createConvex();
    body = new RigidBody(fishNode, 1, shape2);
    _world.addRigidBody(body);
}

void Simulation::update(double t)
{
    _world.getBtWorld().stepSimulation(t, 10);

    if(_debugDraw)
       	_debugDrawer->update();
}
