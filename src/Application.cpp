
#include <iostream>
#include <OgreCameraMan.h>
#include "Application.h"

// #include <physics/BtOgre.h>
#include <physics/RigidBody.h>

using namespace std;

Application::Application() : OgreBites::ApplicationContext("OgreD")
{
}

void Application::setup()
{
    OgreBites::ApplicationContext::setup();
    addInputListener(this);

    _sceneMgr = getRoot()->createSceneManager();

    Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
    shadergen->addSceneManager(_sceneMgr);

    _mainCam = _sceneMgr->createCamera("MainCamera");
    _mainCam->setNearClipDistance(0.5);
    _mainCam->setAutoAspectRatio(true);
    auto *camnode = _sceneMgr->getRootSceneNode()->createChildSceneNode();
//     camnode->setPosition(0, 0, 150);
    camnode->attachObject(_mainCam);

    auto *ogreEnt = _sceneMgr->createEntity("ogrehead.mesh");
    auto *ogrenode = _sceneMgr->getRootSceneNode()->createChildSceneNode();
    ogrenode->attachObject(ogreEnt);

    auto *fishEnt = _sceneMgr->createEntity("fish.mesh");
    auto *fishNode = _sceneMgr->getRootSceneNode()->createChildSceneNode();
    fishNode->attachObject(fishEnt);
    fishNode->setPosition(0, 50, 0);
    
    _sceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

    getRenderWindow()->addViewport(_mainCam);

    auto *camMan = new OgreBites::CameraMan(camnode);
    camMan->setStyle(OgreBites::CS_ORBIT);
    camMan->setYawPitchDist(Ogre::Degree(45), Ogre::Degree(45), 120);
    addInputListener(camMan);

    auto shape1 = BtOgre::StaticMeshToShapeConverter(ogreEnt).createTrimesh();
    auto btBody = new btRigidBody(0, nullptr, shape1);
    auto body = new RigidBody();
    body->setSceneNode(ogrenode);
    body->setBtRigidBody(btBody);
    _world.addRigidBody(body);

    auto shape2 = BtOgre::StaticMeshToShapeConverter(fishEnt).createConvex();
    btBody = new btRigidBody(1, nullptr, shape2);
    body = new RigidBody();
    body->setSceneNode(fishNode);
    body->setBtRigidBody(btBody);
    _world.addRigidBody(body);
    
    
    _debugDrawer = new BtOgre::DebugDrawer(_sceneMgr->getRootSceneNode(), &_world.getBtWorld());
}

void Application::shutdown()
{
    OgreBites::ApplicationContext::shutdown();
}

bool Application::frameStarted(const Ogre::FrameEvent &evt)
{
    OgreBites::ApplicationContext::frameStarted(evt);

    //Update Bullet world. Don't forget the debugDrawWorld() part!
    _world.getBtWorld().stepSimulation(evt.timeSinceLastFrame, 10);

    if(_debugDraw)
       	_debugDrawer->update();
    return true;
}

bool Application::keyPressed(const OgreBites::KeyboardEvent& evt)
{
    cout << "Key pressed:" << evt.keysym.sym << ", " << evt.keysym.mod << endl;

    if (evt.keysym.sym == OgreBites::SDLK_ESCAPE)
    {
        getRoot()->queueEndRendering();
    }
    return true;
}
