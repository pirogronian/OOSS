
#include <iostream>
#include <fstream>
#include <OgreCameraMan.h>
#include <OgreOverlayManager.h>
#include "Application.h"

#include <physics/RigidBody.h>

using namespace std;

static const char DefaultSaveName[] {"defaultSave"};

class RTL : public Ogre::RenderTargetListener {
    Ogre::Viewport *_vp{nullptr};
    Ogre::SceneManager *_sceneMgr{nullptr};
public:
    RTL(Ogre::SceneManager *sm, Ogre::Viewport *vp) : _sceneMgr(sm), _vp(vp) {}
//     void preViewportUpdate(const Ogre::RenderTargetViewportEvent &e) {
//         if (e.source == _vp) {
//             _sceneMgr->addSpecialCaseRenderQueue(Ogre::RENDER_QUEUE_OVERLAY);
//             _sceneMgr->setSpecialCaseRenderQueueMode(Ogre::SceneManager::SCRQM_INCLUDE);
//         }
//     }
//     void postViewportUpdate(const Ogre::RenderTargetViewportEvent &e) {
// //         if (e.source == _vp)  _sceneMgr->removeSpecialCaseRenderQueue(Ogre::RENDER_QUEUE_OVERLAY);
//         _sceneMgr->clearSpecialCaseRenderQueues();
//     }
    void viewportAdded(const Ogre::RenderTargetViewportEvent &e) {
//         cout << "Viewport added\n";
        _vp->setClearEveryFrame(false);
    }

    void viewportRemoved(const Ogre::RenderTargetViewportEvent &e) {
        auto n = e.source->getTarget()->getNumViewports();
//         cout << "Viewport removed." << n << endl;
        if (n < 3) {
            _vp->setClearEveryFrame(true);
        }
    }
};

static const char AppStateFileName[] = "AppStateFileName";

Application::Application() : OgreBites::ApplicationContext("OOSS")
{
}

void Application::setup()
{
    OgreBites::ApplicationContext::setup();
    addInputListener(this);
    createDummyScene();

    {
        ifstream is(AppStateFileName);
        try {
            cereal::XMLInputArchive ia(is);
            ia(*this);
        } catch(cereal::Exception e) {
            cerr << e.what() << endl;
        }
    }

    _sceneMgr = getRoot()->createSceneManager();

    _sim = new Simulation(_sceneMgr);
    _sim->setRenderTarget(getRenderWindow());

    _imguiOverlay = new Ogre::ImGuiOverlay();

    _imguiOverlay->setZOrder(1024);
    _imguiOverlay->show();
    Ogre::OverlayManager::getSingleton().addOverlay(_imguiOverlay);

    _imguiListener = new OgreBites::ImGuiInputListener();

    auto *vp = getRenderWindow()->getViewportByZOrder(0);
//     vp->setClearEveryFrame(false);
    vp->setOverlaysEnabled(true);
    auto rec = vp->getActualDimensions();
    auto rtl = new RTL(_sceneMgr, vp);
    getRenderWindow()->addListener(rtl);
//     _frameLimit = 5;
//     _sim->populate();
}

void Application::shutdown()
{
    {
        ofstream os(AppStateFileName);
        cereal::XMLOutputArchive oa(os);
        oa(*this);
    }

    OgreBites::ApplicationContext::shutdown();
}

bool Application::frameStarted(const Ogre::FrameEvent &evt)
{
    OgreBites::ApplicationContext::frameStarted(evt);

    Ogre::ImGuiOverlay::NewFrame();

    if (_visibleUI.mainMenu)  updateMainMenu();
    if (_visibleUI.demoWindow)  ImGui::ShowDemoWindow();
    if (_visibleUI.simStats) updateSimStatsWindow();
    /*if (_visibleUI.populateWarning) */newBuiltinSimulationWarningWindow();

    _sim->update(evt.timeSinceLastFrame);

    ++_frameCounter;
    if (_frameLimit > 0 && _frameCounter > _frameLimit)
        getRoot()->queueEndRendering();

    return true;
}

/*bool Application::frameEnded(const Ogre::FrameEvent &evt) {
    OgreBites::ApplicationContext::frameEnded(evt);

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
//     cout << "Key pressed:" << evt.keysym.sym << ", " << evt.keysym.mod << endl;

    if (evt.keysym.sym == OgreBites::SDLK_ESCAPE)  getRoot()->queueEndRendering();
    if (evt.keysym.sym == OgreBites::SDLK_F1)  _visibleUI.mainMenu = !_visibleUI.mainMenu;

    if (_imguiListener->keyReleased(evt))
        return true;

    return true;
}

bool Application::keyReleased(const OgreBites::KeyboardEvent& evt)
{
    if (_imguiListener->keyReleased(evt))
        return true;

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
    bool newSimBuiltin {false};
    bool clearSim {false};
    bool loadSim {false};
    bool saveSim {false};

    bool showNodes = _sceneMgr->getDisplaySceneNodes();
    bool showBboxes = _sceneMgr->getShowBoundingBoxes();
    int mode = _sim->getPhysicsDebugDrawer()->getDebugMode();
    bool showPhysDebug = (mode == BtOgre::DebugDrawer::DBG_NoDebug ? false : true);
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Program")) {
            ImGui::MenuItem("Quit", "Esc", &quit);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Simulation")) {
            ImGui::MenuItem("New built-in", "", &newSimBuiltin);
            ImGui::MenuItem("Clear", "", &clearSim);
            ImGui::MenuItem("Load", "", &loadSim);
            ImGui::MenuItem("Save", "", &saveSim);
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
    if (newSimBuiltin)  newBuiltinSimulation();
    if (clearSim) _sim->clear();
    if (loadSim)  _sim->load(DefaultSaveName);
    if (saveSim)  _sim->save(DefaultSaveName);
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

void Application::newBuiltinSimulationWarningWindow() {
    if (ImGui::BeginPopupModal("Simulation not empty!", nullptr/*&_visibleUI.populateWarning*/)) {
        ImGui::Text("Simulation is not empty! Are you sure to proceed?");
        if (ImGui::Button("Yes")) {
            doNewBuiltinSimulation();
            closeOperationPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("No")) closeOperationPopup();
        ImGui::EndPopup();
    }
}

void Application::doNewBuiltinSimulation() {
    _sim->populate();
}

void Application::doClearSimulation() {
    _sim->clear();
}

void Application::doLoadSimulation() {
    _sim->load(DefaultSaveName);
}

void Application::doSaveSimulation() {
    _sim->save(DefaultSaveName);
}

void Application::newBuiltinSimulation() {
    if (_currentOp != NoOperation)  return;
    if (_sim->isEmpty()) {
        doNewBuiltinSimulation();
        return;
    }
    _currentOp = NewBuiltinSimulation;
    ImGui::OpenPopup("Simulation not empty!");
}

void Application::closeOperationPopup() {
    ImGui::CloseCurrentPopup();
    _currentOp = NoOperation;
}
