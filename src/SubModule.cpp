
#include "SubModule.h"

using namespace Ogre;
using namespace OgreBites;

#define EventHandler(eh, et) \
bool SubModuleManager::eh(const et &e) { \
    for (auto *sm : _v) { \
        if (sm == nullptr)  continue; \
        if (!sm->isActive())  continue; \
        if (sm->eh(e))  return true; \
    } \
    return false; \
}

void SubModuleManager::frameRendered(const FrameEvent &e) {
    for (auto *sm : _v) {
        if (sm == nullptr)  continue;
        if (!sm->isActive())  continue;
        sm->frameRendered(e);
    }
}

EventHandler(axisMoved, AxisEvent)
EventHandler(buttonPressed, ButtonEvent)
EventHandler(buttonReleased, ButtonEvent)
EventHandler(keyPressed, KeyboardEvent)
EventHandler(keyReleased, KeyboardEvent)
EventHandler(mouseMoved, MouseMotionEvent)
EventHandler(mousePressed, MouseButtonEvent)
EventHandler(mouseReleased, MouseButtonEvent)
EventHandler(mouseWheelRolled, MouseWheelEvent)
EventHandler(textInput, TextInputEvent)
EventHandler(touchMoved, TouchFingerEvent)
EventHandler(touchPressed, TouchFingerEvent)
EventHandler(touchReleased, TouchFingerEvent)
