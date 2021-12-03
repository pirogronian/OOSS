
#include <iostream>
#include <OgreCameraMan.h>
#include <OgreOverlayManager.h>
#include "Application.h"

#include <physics/RigidBody.h>

using namespace std;

void printViewports(Ogre::RenderTarget *rt) {
    cout << "Vieports: " << rt->getNumViewports() << endl;
    for (int i = 0; i < 1024; i++) {
        auto vp = rt->getViewport(i);
        if (!vp)  continue;
        cout << "Vp[" << i << "] at " << vp->getZOrder() << endl;
    }
}

class RTL : public Ogre::RenderTargetListener {
    Ogre::Viewport *_vp{nullptr};
    Ogre::SceneManager *_sceneMgr{nullptr};
public:
    RTL(Ogre::SceneManager *sm, Ogre::Viewport *vp) : _sceneMgr(sm), _vp(vp) {}
    void preViewportUpdate(const Ogre::RenderTargetViewportEvent &e) {
        if (e.source == _vp) {
            _sceneMgr->addSpecialCaseRenderQueue(Ogre::RENDER_QUEUE_OVERLAY);
            _sceneMgr->setSpecialCaseRenderQueueMode(Ogre::SceneManager::SCRQM_INCLUDE);
        }
    }
    void postViewportUpdate(const Ogre::RenderTargetViewportEvent &e) {
//         if (e.source == _vp)  _sceneMgr->removeSpecialCaseRenderQueue(Ogre::RENDER_QUEUE_OVERLAY);
        _sceneMgr->clearSpecialCaseRenderQueues();
    }
};

Application::Application() : OgreBites::ApplicationContext("OOSS")
{
}

void Application::setup()
{
    OgreBites::ApplicationContext::setup();
    addInputListener(this);
    createDummyScene();

    _sceneMgr = getRoot()->createSceneManager();

    _sim = new Simulation(_sceneMgr);
    _sim->setRenderWindow(getRenderWindow());

    _imguiOverlay = new Ogre::ImGuiOverlay();

    _imguiOverlay->setZOrder(1024);
    _imguiOverlay->show();
    Ogre::OverlayManager::getSingleton().addOverlay(_imguiOverlay);

    _imguiListener = new OgreBites::ImGuiInputListener();

    auto *vp = getRenderWindow()->getViewportByZOrder(0);
    vp->setClearEveryFrame(false);
    vp->setOverlaysEnabled(true);
    auto rec = vp->getActualDimensions();
    auto rtl = new RTL(_sceneMgr, vp);
//     getRenderWindow()->addListener(rtl);
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

    Ogre::ImGuiOverlay::NewFrame();

    if (_visibleUI.mainMenu)  updateMainMenu();
    if (_visibleUI.demoWindow)  ImGui::ShowDemoWindow();
    if (_visibleUI.simStats) updateSimStatsWindow();
    /*if (_visibleUI.populateWarning) */updatePopulateWarningWindow();

    _sim->update(evt.timeSinceLastFrame);

    ++_frameCounter;
    if (_frameLimit > 0 && _frameCounter > _frameLimit)
        getRoot()->queueEndRendering();

    return true;
}

/*bool Application::frameEnded(const Ogre::FrameEvent &evt) {
//     cout << "frameEnded()\n";
    
    OgreBites::ApplicationContext::frameEnded(evt);

//     Ogre::ImguiManager::getSingleton().endFrame();
    
    return true;
}*/ 

bool Application::mouseMoved (const OgreBites::MouseMotionEvent &evt)
{
    if (_imguiListener->mouseMoved(evt))
        return true;

    if (_sim->mouseMoved(evt))
        return true;

    return true;
}

bool Application::mousePressed (const OgreBites::MouseButtonEvent &evt)
{
    if (_imguiListener->mousePressed(evt))
        return true;

    if (_sim->mousePressed(evt))
        return true;

    return true;
}

bool Application::mouseReleased (const OgreBites::MouseButtonEvent &evt)
{
    if (_imguiListener->mouseReleased(evt))
        return true;

    if (_sim->mouseReleased(evt))
        return true;

    return true;
}

bool Application::mouseWheelRolled (const OgreBites::MouseWheelEvent &evt)
{
    if (_imguiListener->mouseWheelRolled(evt))
        return true;

    if (_sim->mouseWheelRolled(evt))
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

//     if (_cammanListener->keyReleased(evt))
//         return true;

    return true;
}

bool Application::keyReleased(const OgreBites::KeyboardEvent& evt)
{
    if (_imguiListener->keyReleased(evt))
        return true;

//     if (_cammanListener->keyReleased(evt))
//         return true;

    return true;
}

bool Application::touchMoved(const OgreBites::TouchFingerEvent& evt)
{
    if (_imguiListener->touchMoved(evt))
        return true;

    if (_sim->touchMoved(evt))
        return true;

    return true;
}

bool Application::touchPressed(const OgreBites::TouchFingerEvent& evt)
{
    if (_imguiListener->touchPressed(evt))
        return true;

    if (_sim->touchPressed(evt))
        return true;

    return true;
}

bool Application::touchReleased(const OgreBites::TouchFingerEvent& evt)
{
    if (_imguiListener->touchReleased(evt))
        return true;

    if (_sim->touchReleased(evt))
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
            ImGui::MenuItem("Show ImGui demo", "", &_visibleUI.demoWindow);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    if (newsim) {
        if (_sim->isEmpty())  _sim->populate();
        else ImGui::OpenPopup("Simulation already populated!");
    }
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

void Application::updatePopulateWarningWindow() {
    if (ImGui::BeginPopupModal("Simulation already populated!", nullptr/*&_visibleUI.populateWarning*/)) {
        ImGui::Text("Simulation is already populated! Are you sure to proceed?");
        if (ImGui::Button("Yes")) {
            _sim->populate();
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("No")) ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }
}
