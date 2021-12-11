
#include "Simulation.h"

#include "Player.h"

using namespace std;
using namespace Ogre;

RenderTarget *Player::getRenderTarget() { return _sim->getRenderTarget(); }
RenderTarget const *Player::getRenderTarget() const { return _sim->getRenderTarget(); }

Viewport *Player::addViewport(Camera *cam, int z, Real l, Real t, Real w, Real h) {
    auto rt = getRenderTarget();
    if (z >= 0 || rt->hasViewportWithZOrder(z))  return nullptr;
    Viewport *vp = rt->addViewport(cam, z, l, t, w, h);
    _vps.push_back(vp);

    return vp;
}

int Player::findViewportIndex(Viewport *vp) {
    for (int i = 0; i < _vps.size(); i++)
        if (_vps[i] == vp)  return i;

    return -1;
}

bool Player::removeViewport(int i) {
    if (i >= _vps.size())  return false;

    auto rt = getRenderTarget();
    Viewport *vp = _vps[i];
    _vps.erase(_vps.begin() + i);
    rt->removeViewport(vp->getZOrder());

    return true;
}

void Player::removeAllViewports() {
    auto rt = getRenderTarget();
    while(_vps.size()) {
        auto it = _vps.back();
        rt->removeViewport(it->getZOrder());
        _vps.pop_back();
    }
}

void Player::clear() {
    removeAllViewports();
    deleteCameraMan();
}

void Player::createCameraMan() {
    auto *sm = _sim->getSceneManager();
    auto *node = sm->getRootSceneNode()->createChildSceneNode("3rdPersonCameraNode");
    _cm = new OgreBites::CameraMan(node);
}

void Player::restoreCameraMan() {
    auto *sn = _sim->getSceneManager()->getSceneNode("3rdPersonCameraNode");
    _cm = new OgreBites::CameraMan(sn);
}

void Player::deleteCameraMan() {
    delete _cm;
    _cm = nullptr;
}

bool Player::mouseMoved(const OgreBites::MouseMotionEvent &evt) {
    if (!_cm)  return false;
    return _cm->mouseMoved(evt);
}

bool Player::mousePressed(const OgreBites::MouseButtonEvent &evt) {
    if (!_cm)  return false;
    return _cm->mousePressed(evt);
}

bool Player::mouseReleased(const OgreBites::MouseButtonEvent &evt) {
    if (!_cm)  return false;
    return _cm->mouseReleased(evt);
}

bool Player::mouseWheelRolled(const OgreBites::MouseWheelEvent &evt) {
    if (!_cm)  return false;
    return _cm->mouseWheelRolled(evt);
}

bool Player::touchMoved (const OgreBites::TouchFingerEvent &evt) {
    if (!_cm)  return false;
    return _cm->touchMoved(evt);
}

bool Player::touchPressed (const OgreBites::TouchFingerEvent &evt) {
    if (!_cm)  return false;
    return _cm->touchPressed(evt);
}

bool Player::touchReleased (const OgreBites::TouchFingerEvent &evt) {
    if (!_cm)  return false;
    return _cm->touchReleased(evt);
}
