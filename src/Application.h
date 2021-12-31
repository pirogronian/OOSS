
#include <OgreApplicationContext.h>
#include <OgreImGuiOverlay.h>
#include <OgreImGuiInputListener.h>

#include <cereal/archives/xml.hpp>

#include "SubModule.h"
#include <Simulation.h>
#include <SimTimeControl.h>
#include <SceneInspector.h>

class Application :  public OgreBites::ApplicationContext, public OgreBites::InputListener
{
    enum SubModules {
        DummyModule = 0,
        NewSimulationModule = 1,
        LoadSimulationModule = 2,
        SaveSimulationModule = 3,
        MaxModulesNumber
    };
    struct VisibleUI {
        bool demoWindow {false};
        bool mainMenu {true};
        bool simStats {false};
        bool timeControl{true};
        bool sceneInspector{false};
        bool populateWarning {false};
        template<class Archive>
        void serialize(Archive &archv) {
            archv(CEREAL_NVP(demoWindow), CEREAL_NVP(mainMenu), CEREAL_NVP(simStats), CEREAL_NVP(timeControl), CEREAL_NVP(sceneInspector));
        }
    };
    Ogre::SceneManager *_sceneMgr;
    Simulation *_sim;
    Player *_pl;
    OgreBites::ImGuiInputListener *_imguiListener;
    Ogre::ImGuiOverlay *_imguiOverlay;
    SubModuleManager _smm{MaxModulesNumber};
    SubModule *_mdp{nullptr};
    int _frameCounter {0};
    int _frameLimit{-1};
    VisibleUI _visibleUI;
    SimTimeControl *_stc;
    SceneInspector *_si;
public:
    Application();
    void setup();
    void shutdown();
    bool frameStarted(const Ogre::FrameEvent &);
//     bool frameEnded(const Ogre::FrameEvent &);
    void frameRendered (const Ogre::FrameEvent &);
    bool textInput(const OgreBites::TextInputEvent &);
    bool keyPressed (const OgreBites::KeyboardEvent &);
    bool keyReleased (const OgreBites::KeyboardEvent &);
    bool mouseMoved (const OgreBites::MouseMotionEvent &);
    bool mousePressed (const OgreBites::MouseButtonEvent &);
    bool mouseReleased (const OgreBites::MouseButtonEvent &);
    bool mouseWheelRolled (const OgreBites::MouseWheelEvent &);
    bool touchMoved (const OgreBites::TouchFingerEvent &);
    bool touchPressed (const OgreBites::TouchFingerEvent &);
    bool touchReleased (const OgreBites::TouchFingerEvent &);

    void updateMainMenu();
    void updateSimStatsWindow();

    template<class Archive>
    void serialize(Archive &archv) {
        archv(cereal::make_nvp("VisibleUI", _visibleUI));
    }

    void doNewBuiltinSimulation();
    void doClearSimulation();
    void doLoadSimulation();
    void doSaveSimulation();

    void newBuiltinSimulation();
    void clearSimulation();
    void loadSimulation();
    void loadSimulationSlot();
    void saveSimulation();
    void saveSimulationSlot();

    void closeOperationPopup();

    Ogre::String getSaveSubdirName() const { return "saves"; }
    std::filesystem::path getSavePath(bool = true);
};
