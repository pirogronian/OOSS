
#include <iostream>
#include <OgreCameraMan.h>
#include "Application.h"

#include <physics/RigidBody.h>

using namespace std;

Application::Application() : OgreBites::ApplicationContext("OOSS")
{
}

void Application::setup()
{
    OgreBites::ApplicationContext::setup();
    addInputListener(this);

    _sceneMgr = getRoot()->createSceneManager();

    _sim = new Simulation(_sceneMgr);
    getRenderWindow()->addViewport(_sim->getMainCamera());

    Ogre::ImguiManager::createSingleton();
    Ogre::ImguiManager::getSingleton().init(_sceneMgr);
    _imguiListener = Ogre::ImguiManager::getSingletonPtr()->getInputListener();

    _cammanListener = _sim->getMainCameraMan();
    
//     _frameLimit = 5;
//     _sim->populate();
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

    if (_visibleUI.mainMenu)  updateMainMenu();
    if (_visibleUI.demoWindow)  ImGui::ShowDemoWindow();
    updateSimStatsWindow();

    _sim->update(evt.timeSinceLastFrame);

    ++_frameCounter;
    if (_frameLimit > 0 && _frameCounter > _frameLimit)
        getRoot()->queueEndRendering();

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

    if (evt.keysym.sym == OgreBites::SDLK_ESCAPE)  getRoot()->queueEndRendering();
    if (evt.keysym.sym == OgreBites::SDLK_F1)  _visibleUI.mainMenu = !_visibleUI.mainMenu;

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

bool Application::touchMoved(const OgreBites::TouchFingerEvent& evt)
{
    if (_imguiListener->touchMoved(evt))
        return true;

    if (_cammanListener->touchMoved(evt))
        return true;

    return true;
}

bool Application::touchPressed(const OgreBites::TouchFingerEvent& evt)
{
    if (_imguiListener->touchPressed(evt))
        return true;

    if (_cammanListener->touchPressed(evt))
        return true;

    return true;
}

bool Application::touchReleased(const OgreBites::TouchFingerEvent& evt)
{
    if (_imguiListener->touchReleased(evt))
        return true;

    if (_cammanListener->touchReleased(evt))
        return true;

    return true;
}

void Application::updateMainMenu()
{
    bool quit {false};
    bool newsim {false};
    bool clrsim {false};
    
    bool showNodes = _sceneMgr->getDisplaySceneNodes();
    bool showBboxes = _sceneMgr->getShowBoundingBoxes();
    int mode = _sim->getPhysicsDebugDrawer()->getDebugMode();
    bool showPhysDebug = (mode == BtOgre::DebugDrawer::DBG_NoDebug ? false : true);
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Program")) {
            ImGui::MenuItem("Populate simulation", "", &newsim);
            ImGui::MenuItem("Clear simulation", "", &clrsim);
            ImGui::MenuItem("Quit", "Esc", &quit);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
            ImGui::MenuItem("Show sim stats", "", &_visibleUI.simStats);
            ImGui::MenuItem("Show nodes", "", &showNodes);
            ImGui::MenuItem("Show boxes", "", &showBboxes);
            ImGui::MenuItem("Show physics", "", &showPhysDebug);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    if (newsim) _sim->populate();
    if (clrsim) _sim->clear();
    if (quit) getRoot()->queueEndRendering();
    
    _sceneMgr->setDisplaySceneNodes(showNodes);
    _sceneMgr->showBoundingBoxes(showBboxes);
    if (showPhysDebug) mode = BtOgre::DebugDrawer::DBG_MAX_DEBUG_DRAW_MODE;
    else mode = BtOgre::DebugDrawer::DBG_NoDebug;
    _sim->getPhysicsDebugDrawer()->setDebugMode(mode);
}

void Application::updateSimStatsWindow()
{
    if (ImGui::Begin("SimStats", &_visibleUI.simStats)) {
        if (ImGui::BeginTable("SimStats", 2)) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("Rigid bodies num:");
            ImGui::TableNextColumn();
            ImGui::Text("%i", _sim->getDynamicsWorld().rigidBodiesNumber());
            ImGui::EndTable();
        }
    }
    ImGui::End();
}
