
#pragma once

#include <vector>

#include <OgreInput.h>

class SubModule : public OgreBites::InputListener {
protected:
    bool _active {false};
public:
    bool isActive() const { return _active; }
    virtual void setActive(bool a) { _active = a; }
    virtual bool frameStarted(const Ogre::FrameEvent &);
};

class SubModuleManager : public OgreBites::InputListener {
    std::vector<SubModule*> _v;
public:
    SubModuleManager() = delete;
    SubModuleManager(std::size_t n) : _v(n, nullptr) {}
    void registerSubModule(std::size_t i, SubModule *sm) { _v[i] = sm; }
    SubModule *getSubModule(std::size_t i) const { return _v[i]; }
    void unregisterSubModule(std::size_t i) { _v[i] = nullptr; }

    bool axisMoved(const OgreBites::AxisEvent &);
    bool buttonPressed(const OgreBites::ButtonEvent &);
    bool buttonReleased(const OgreBites::ButtonEvent &);
    virtual bool frameStarted(const Ogre::FrameEvent &);
    void frameRendered(const Ogre::FrameEvent &);
    bool keyPressed(const OgreBites::KeyboardEvent &);
    bool keyReleased(const OgreBites::KeyboardEvent &);
    bool mouseMoved(const OgreBites::MouseMotionEvent &);
    bool mousePressed(const OgreBites::MouseButtonEvent &);
    bool mouseReleased(const OgreBites::MouseButtonEvent &);
    bool mouseWheelRolled(const OgreBites::MouseWheelEvent &);
    bool textInput(const OgreBites::TextInputEvent &);
    bool touchMoved(const OgreBites::TouchFingerEvent &);
    bool touchPressed(const OgreBites::TouchFingerEvent &);
    bool touchReleased(const OgreBites::TouchFingerEvent &);
};
