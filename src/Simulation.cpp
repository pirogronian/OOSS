
#include <fstream>
#include <cereal/archives/xml.hpp>

#include <OgreShaderGenerator.h>

#include "Simulation.h"
#include "Player.h"

#include <physics/RigidBody.h>
#include <physics/GravityCenter.h>

using namespace std;
using namespace Ogre;

namespace cereal {
template<class Ar>
void serialize(Ar &a, Ogre::ColourValue &cv) {
    a(cv.r, cv.g, cv.b, cv.a);
}
}

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
    if (!_pl->getCameraMan())  _pl->createCameraMan();

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

    auto cm = _pl->getCameraMan();
    cm->getCamera()->attachObject(cam);

    cm->setStyle(OgreBites::CS_ORBIT);
//     cout << __FILE__ << " (" << __LINE__ << ")\n";
    cm->setYawPitchDist(Ogre::Degree(45), Ogre::Degree(45), 120);
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

bool Simulation::load(const filesystem::path &name) {
    cout << "Simulation::load(" << name << ")\n";
    if (!filesystem::is_directory(name)) {
        cout << "Slot not exists, exiting!\n";
        return false;
    }
    auto sceneFile = name / "scene.scene";
    if (!filesystem::is_regular_file(sceneFile)) {
        cout << "Scene file doesn not exists, exiting!\n";
        return false;
    }
    _sceneMgr->getRootSceneNode()->loadChildren(sceneFile);
    _empty = false;

    ifstream is(name / "simulation.xml");
    {
        try {
            cereal::XMLInputArchive ia(is);
            ia(*_pl);
            ColourValue al;
            ia(al);
            _sceneMgr->setAmbientLight(al);
        } catch(cereal::Exception e) {
            cerr << e.what() << endl;
            return false;
        }
    }

    return true;
}

bool Simulation::save(const filesystem::path &name) const {
    cout << "Simulation::save(" << name << ")\n";
    if (!filesystem::is_directory(name)) {
        cout << "Slot not exists, creating!\n";
        if (!filesystem::create_directories(name)) {
            cout << "Creating slot failed, exiting!\n";
            return false;
        }
    }
    if (_empty)  return false;
    _sceneMgr->getRootSceneNode()->saveChildren(name / "scene.scene");

    ofstream os(name / "simulation.xml");
    cereal::XMLOutputArchive oa(os);
    oa(cereal::make_nvp("Player", *_pl));
    ColourValue al = _sceneMgr->getAmbientLight();
    oa(cereal::make_nvp("AmbientLight", al));

    return true;
}
