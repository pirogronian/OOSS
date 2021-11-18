
#include <iostream>
#include <OgreCameraMan.h>
#include "Application.h"

// #include <physics/BtOgre.h>
#include <physics/RigidBody.h>

using namespace std;

Application::Application() : OgreBites::ApplicationContext("OOSS")
{
}

void Application::setup()
{
    OgreBites::ApplicationContext::setup();
    addInputListener(this);

    _sim = new Simulation(getRoot());
    getRenderWindow()->addViewport(_sim->getMainCamera());

    Ogre::ImguiManager::createSingleton();
    Ogre::ImguiManager::getSingleton().init(_sim->getSceneManager());
    _imguiListener = Ogre::ImguiManager::getSingletonPtr()->getInputListener();

    _cammanListener = _sim->getMainCameraMan();
}

void Application::shutdown()
{
    OgreBites::ApplicationContext::shutdown();
}

bool Application::frameStarted(const Ogre::FrameEvent &evt)
{
    OgreBites::ApplicationContext::frameStarted(evt);

    Ogre::ImguiManager::getSingleton().newFrame(
            evt.timeSinceLastFrame,
            Ogre::Rect(0, 0, getRenderWindow()->getWidth(), getRenderWindow()->getHeight()));

    ImGui::ShowDemoWindow();

    _sim->update(evt.timeSinceLastFrame);

    return true;
}

bool Application::mouseMoved (const OgreBites::MouseMotionEvent &evt)
{
    if (_imguiListener->mouseMoved(evt))
        return true;

    if (_cammanListener->mouseMoved(evt))
        return true;

    return true;
}

bool Application::mousePressed (const OgreBites::MouseButtonEvent &evt)
{
    if (_imguiListener->mousePressed(evt))
        return true;

    if (_cammanListener->mousePressed(evt))
        return true;

    return true;
}

bool Application::mouseReleased (const OgreBites::MouseButtonEvent &evt)
{
    if (_imguiListener->mouseReleased(evt))
        return true;

    if (_cammanListener->mouseReleased(evt))
        return true;

    return true;
}

bool Application::mouseWheelRolled (const OgreBites::MouseWheelEvent &evt)
{
    if (_imguiListener->mouseWheelRolled(evt))
        return true;

    if (_cammanListener->mouseWheelRolled(evt))
        return true;

    return true;
}

bool Application::keyPressed(const OgreBites::KeyboardEvent& evt)
{
    cout << "Key pressed:" << evt.keysym.sym << ", " << evt.keysym.mod << endl;

    if (evt.keysym.sym == OgreBites::SDLK_ESCAPE)
    {
        getRoot()->queueEndRendering();
    }

    if (_imguiListener->keyReleased(evt))
        return true;

    if (_cammanListener->keyReleased(evt))
        return true;

    return true;
}

bool Application::keyReleased(const OgreBites::KeyboardEvent& evt)
{
    if (_imguiListener->keyReleased(evt))
        return true;

    if (_cammanListener->keyReleased(evt))
        return true;

    return true;
}
