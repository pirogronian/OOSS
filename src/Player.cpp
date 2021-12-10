
#include "Player.h"

using namespace std;
using namespace Ogre;

bool Player::addViewport(Camera *cam, int z, Real l, Real t, Real w, Real h) {
    Viewport *vp = _rt->addViewport(cam, z, l, t, w, h);
    _vps.push_back(vp);

    return true;
}

bool Player::removeViewport(int i) {
    if (i >= _vps.size())  return false;

    Viewport *vp = _vps[i];
    _vps.erase(_vps.begin() + i);
    _rt->removeViewport(vp->getZOrder());

    return true;
}

int Player::findViewportIndex(Viewport *vp) {
    for (int i = 0; i < _vps.size(); i++)
        if (_vps[i] == vp)  return i;

    return -1;
}
