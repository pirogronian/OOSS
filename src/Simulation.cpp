
#include <fstream>
#include <cereal/archives/xml.hpp>

#include <OgreShaderGenerator.h>

#include "Simulation.h"
#include "Player.h"

#include <physics/RigidBody.h>
#include <physics/GravityCenter.h>

using namespace std;

Simulation::Simulation(Ogre::SceneManager *sceneMgr)
{
    _sceneMgr = sceneMgr;
    Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
    shadergen->addSceneManager(_sceneMgr);
    _debugDrawer = new BtOgre::DebugDrawer(_sceneMgr->getRootSceneNode(), &_world.getBtWorld());

    _pl = new Player(this);
}

void Simulation::update(double delta)
{
//     dump(_world);
    _world.stepSimulation(delta);

    if(_debugDraw)
       	_debugDrawer->update();
}

void Simulation::populate()
{
    _empty = false;

    auto *ogreEnt = _sceneMgr->createEntity("ogrehead.mesh");
    auto *ogrenode = _sceneMgr->getRootSceneNode()->createChildSceneNode();
    ogrenode->attachObject(ogreEnt);

    auto *fishEnt = _sceneMgr->createEntity("fish.mesh");
    auto *fishNode = _sceneMgr->getRootSceneNode()->createChildSceneNode();
    fishNode->attachObject(fishEnt);
    fishNode->setPosition(0, 50, 0);

    _sceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

    auto shape1 = BtOgre::StaticMeshToShapeConverter(ogreEnt).createTrimesh();
    auto body1 = new RigidBody(ogrenode, 0, shape1);
    _world.addRigidBody(body1);

    auto shape2 = BtOgre::StaticMeshToShapeConverter(fishEnt).createConvex();
//    auto bbody2 = new btRigidBody(1, nullptr, shape2);
    auto body2 = new RigidBody(fishNode, 1, shape2);
//    auto body2 = new RigidBody(fishNode, bbody2);
    _world.addRigidBody(body2);

    body2->getBtRigidBody()->setLinearVelocity(btVector3(7, 0, 0));

    auto gc = new GravityCenter(body1);
    gc->setFactor(1000);
    _world.addGravityCenter(gc);

    _world.setGlobalGravity(btVector3(0, 0, 0));

    Ogre::Camera *cam = _sceneMgr->createCamera("TestCamera");
    cam->setNearClipDistance(0.5);
    cam->setAutoAspectRatio(true);
    auto vp = _pl->addViewport(cam, -1);
    vp->setOverlaysEnabled(false);

    auto *camnode = _sceneMgr->getRootSceneNode()->createChildSceneNode();
    camnode->attachObject(cam);

    _currentCamMan = new OgreBites::CameraMan(camnode);
    _currentCamMan->setStyle(OgreBites::CS_ORBIT);
    _currentCamMan->setYawPitchDist(Ogre::Degree(45), Ogre::Degree(45), 120);
}


bool Simulation::mouseMoved(const OgreBites::MouseMotionEvent &evt) {
    if (_currentCamMan)  return _currentCamMan->mouseMoved(evt);
    return false;
}

bool Simulation::mousePressed(const OgreBites::MouseButtonEvent &evt) {
    if (_currentCamMan)  return _currentCamMan->mousePressed(evt);
    return false;
}

bool Simulation::mouseReleased(const OgreBites::MouseButtonEvent &evt) {
    if (_currentCamMan)  return _currentCamMan->mouseReleased(evt);
    return false;
}

bool Simulation::mouseWheelRolled(const OgreBites::MouseWheelEvent &evt) {
    if (_currentCamMan)  return _currentCamMan->mouseWheelRolled(evt);
    return false;
}

bool Simulation::touchMoved (const OgreBites::TouchFingerEvent &evt) {
    if (_currentCamMan)  return _currentCamMan->touchMoved(evt);
    return false;
}

bool Simulation::touchPressed (const OgreBites::TouchFingerEvent &evt) {
    if (_currentCamMan)  return _currentCamMan->touchPressed(evt);
    return false;
}

bool Simulation::touchReleased (const OgreBites::TouchFingerEvent &evt) {
    if (_currentCamMan)  return _currentCamMan->touchReleased(evt);
    return false;
}

void Simulation::clear()
{
    clearGravityCenters(false);
    clearRigidBodies(false);
    _sceneMgr->clearScene();
    _pl->clear();
    clearCameras();

    // Clearing the scene detaches internal ManualObject, so it has to be attached again.
    _debugDrawer->attach(_sceneMgr->getRootSceneNode());
    _empty = true;
}

void Simulation::clearRigidBodies(bool clearnodes)
{
    for (std::size_t i = 0; i < _world.getMaxRigidBodyIndex(); i++) {
        RigidBody *rb = _world.getRigidBody(i);
        if (!rb)  continue;
        _world.removeRigidBody(rb);
        if (clearnodes) {
            Ogre::SceneNode *sn = rb->getSceneNode();
            if (sn)  _sceneMgr->destroySceneNode(sn);
        }
        delete rb;
    }
}

void Simulation::clearGravityCenters(bool clearnodes) {
    for (std::size_t i = 0; i < _world.getMaxGravityCenterIndex(); i++) {
        GravityCenter *gr = _world.getGravityCenter(i);
        if (!gr) continue;
        _world.removeGravityCenter(gr);
        if (clearnodes && gr->isType<Ogre::SceneNode*>()) {
            Ogre::SceneNode *sn = gr->getOwner<Ogre::SceneNode*>();
            if (sn)  _sceneMgr->destroySceneNode(sn);
        }
    }
}

void Simulation::clearCameras() {
    for (auto &cam : _sceneMgr->getCameras()) {
        _sceneMgr->destroyCamera(cam.second->getName());
    }
}

bool Simulation::load(const string &name) {
    _sceneMgr->getRootSceneNode()->loadChildren(name + ".scene");

    ifstream is(name + ".xml");
    try {
            cereal::XMLInputArchive ia(is);
            ia(*_pl);
        } catch(cereal::Exception e) {
            cerr << e.what() << endl;
            return false;
        }

    return true;
}

bool Simulation::save(const string &name) const {
    _sceneMgr->getRootSceneNode()->saveChildren(name + ".scene");

    ofstream os(name + ".xml");
    cereal::XMLOutputArchive oa(os);
    oa(cereal::make_nvp("Player", *_pl));

    return true;
}
