
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
    addInputListener(_sim->getMainCameraMan());

}

void Application::shutdown()
{
    OgreBites::ApplicationContext::shutdown();
}

bool Application::frameStarted(const Ogre::FrameEvent &evt)
{
    OgreBites::ApplicationContext::frameStarted(evt);

    _sim->update(evt.timeSinceLastFrame);

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
