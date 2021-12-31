
#include <iostream>
#include <fstream>
#include <OgreCameraMan.h>
#include <OgreOverlayManager.h>
#include "Application.h"

#include <physics/RigidBody.h>

#include "Player.h"

#include "LoadSaveModal.h"
#include "LoadModal.h"

using namespace std;
using namespace Ogre;
using namespace OgreBites;

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
    _pl = _sim->getPlayer();
    _stc = new SimTimeControl(_sim, &_visibleUI.timeControl);
    _si = new SceneInspector(_sim, &_visibleUI.sceneInspector);

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
//     pollEvents();

    Ogre::ImGuiOverlay::NewFrame();

    if (_visibleUI.mainMenu)  updateMainMenu();
    if (_visibleUI.demoWindow)  ImGui::ShowDemoWindow();
    if (_visibleUI.simStats) updateSimStatsWindow();
    _stc->frameStarted(evt);
    _si->frameStarted(evt);

    if (_mdp)
        if (_mdp->isActive())  _mdp->frameStarted(evt);
    else {
        delete _mdp;
        _mdp = nullptr;
    }
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

void Application::frameRendered(const FrameEvent &e) {
    return _smm.frameRendered(e);
}

bool Application::textInput(const TextInputEvent &e) {
    if (_imguiListener->textInput(e))  return true;

    return _smm.textInput(e);
}

bool Application::mouseMoved (const OgreBites::MouseMotionEvent &evt)
{
    if (_imguiListener->mouseMoved(evt))
        return true;

    if (_pl->mouseMoved(evt))
        return true;

    return _smm.mouseMoved(evt);
}

bool Application::mousePressed (const OgreBites::MouseButtonEvent &evt)
{
    if (_imguiListener->mousePressed(evt))
        return true;

    if (_pl->mousePressed(evt))
        return true;

    return _smm.mousePressed(evt);
}

bool Application::mouseReleased (const OgreBites::MouseButtonEvent &evt)
{
    if (_imguiListener->mouseReleased(evt))
        return true;

    if (_pl->mouseReleased(evt))
        return true;

    return _smm.mouseReleased(evt);
}

bool Application::mouseWheelRolled (const OgreBites::MouseWheelEvent &evt)
{
    if (_imguiListener->mouseWheelRolled(evt))
        return true;

    if (_pl->mouseWheelRolled(evt))
        return true;

    return _smm.mouseWheelRolled(evt);
}

bool Application::keyPressed(const OgreBites::KeyboardEvent& evt)
{
//     cout << "Key pressed:" << evt.keysym.sym << ", " << evt.keysym.mod << endl;

    if (evt.keysym.sym == OgreBites::SDLK_ESCAPE)  getRoot()->queueEndRendering();
    if (evt.keysym.sym == OgreBites::SDLK_F1)  _visibleUI.mainMenu = !_visibleUI.mainMenu;

    if (_imguiListener->keyReleased(evt))
        return true;

    return _smm.keyPressed(evt);
}

bool Application::keyReleased(const OgreBites::KeyboardEvent& evt)
{
    if (_imguiListener->keyReleased(evt))
        return true;

    return _smm.keyReleased(evt);
}

bool Application::touchMoved(const OgreBites::TouchFingerEvent& evt)
{
    if (_imguiListener->touchMoved(evt))
        return true;

    if (_pl->touchMoved(evt))
        return true;

    return _smm.touchMoved(evt);
}

bool Application::touchPressed(const OgreBites::TouchFingerEvent& evt)
{
    if (_imguiListener->touchPressed(evt))
        return true;

    if (_pl->touchPressed(evt))
        return true;

    return _smm.touchPressed(evt);
}

bool Application::touchReleased(const OgreBites::TouchFingerEvent& evt)
{
    if (_imguiListener->touchReleased(evt))
        return true;

    if (_pl->touchReleased(evt))
        return true;

    return _smm.touchReleased(evt);
}

void Application::updateMainMenu()
{
    bool quit {false};
    bool newSimBuiltin {false};
    bool clearSim {false};
    bool loadSim {false};
    bool loadSimSlot {false};
    bool saveSim {false};
    bool saveSimSlot {false};

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
            ImGui::MenuItem("Load from", "", &loadSimSlot);
            ImGui::MenuItem("Save", "", &saveSim);
            ImGui::MenuItem("Save to", "", &saveSimSlot);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
            ImGui::MenuItem("Show sim stats", "", &_visibleUI.simStats);
            ImGui::MenuItem("Sim time control", "", &_visibleUI.timeControl);
            ImGui::MenuItem("Scene inspector", "", &_visibleUI.sceneInspector);
            ImGui::MenuItem("Nodes", "", &showNodes);
            ImGui::MenuItem("Boxes", "", &showBboxes);
            ImGui::MenuItem("Physics", "", &showPhysDebug);
            ImGui::MenuItem("ImGui demo", "", &_visibleUI.demoWindow);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    if (newSimBuiltin)  newBuiltinSimulation();
    if (clearSim) _sim->clear();
    if (loadSim)  loadSimulation();
    if (loadSimSlot)  loadSimulationSlot();
    if (saveSim)  doSaveSimulation();
    if (saveSimSlot) saveSimulationSlot();
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
            ImGui::Text("%i", _sim->getDynamicsWorld()->rigidBodiesNumber());
            ImGui::EndTable();
        }
    }
    ImGui::End();
}

void Application::doNewBuiltinSimulation() {
    try {
        _sim->populate();
    } catch (Ogre::ItemIdentityException e) {
        cout << e.what() << endl;
    }
}

void Application::doClearSimulation() {
    _sim->clear();
}

void Application::doLoadSimulation() {
    try {
        auto path = getSavePath();
        _sim->load(path / DefaultSaveName);
    } catch (Ogre::ItemIdentityException e) {
        cout << e.what() << endl;
    }
}

void Application::doSaveSimulation() {
    auto path = getSavePath();
    _sim->save(path / DefaultSaveName);
}

void Application::newBuiltinSimulation() {
    assert(_mdp == nullptr);
    if (_sim->isEmpty())  _sim->populate();
    else _mdp = new LoadModal("", _sim, string(), true);
}

void Application::loadSimulation() {
    if (_sim->isEmpty()) {
        doLoadSimulation();
        return;
    }
    _mdp = new LoadModal(getSavePath(), _sim, DefaultSaveName);
}

void Application::loadSimulationSlot() {
    assert(_mdp == nullptr);
    if (_sim->isEmpty()) {
        _mdp = new LoadSaveModal(getSavePath(), _sim, LoadSaveModal::Load);
    } else _mdp = new LoadModal(getSavePath(), _sim);
}

void Application::saveSimulationSlot() {
    assert(_mdp == nullptr);
    _mdp = new LoadSaveModal(getSavePath(), _sim, LoadSaveModal::Save);
}

filesystem::path Application::getSavePath(bool create) {
    auto path = getSaveSubdirName();
    if (create)  FileSystemLayer::createDirectory(path);
    return path;
}
