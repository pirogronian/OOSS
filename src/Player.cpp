
#include "Player.h"

using namespace std;
using namespace Ogre;

Viewport *Player::addViewport(Camera *cam, int z, Real l, Real t, Real w, Real h) {
    if (z >= 0 || _rt->hasViewportWithZOrder(z))  return nullptr;
    Viewport *vp = _rt->addViewport(cam, z, l, t, w, h);
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

    Viewport *vp = _vps[i];
    _vps.erase(_vps.begin() + i);
    _rt->removeViewport(vp->getZOrder());

    return true;
}

void Player::removeAllViewports() {
    while(_vps.size()) {
        auto it = _vps.back();
        _rt->removeViewport(it->getZOrder());
        _vps.pop_back();
    }
}

void Player::clear() {
    removeAllViewports();
}
