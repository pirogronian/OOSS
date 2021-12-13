
#include <OgreApplicationContext.h>
// #include <ImguiManager.h>
#include <OgreImGuiOverlay.h>
#include <OgreImGuiInputListener.h>

#include <cereal/archives/xml.hpp>

#include <Simulation.h>

class Application :  public OgreBites::ApplicationContext, public OgreBites::InputListener
{
    enum Operations {
        NoOperation = 0,
        NewBuiltinSimulation,
        ClearSimulation,
        LoadSimulation,
        SaveSimulation
    };
    enum ModalDialog {
        NoDialog = 0,
        SimulationNotEmpty
    };
    struct VisibleUI {
        bool demoWindow {false};
        bool mainMenu {true};
        bool simStats {false};
        bool populateWarning {false};
        template<class Archive>
        void serialize(Archive &archv) {
            archv(CEREAL_NVP(demoWindow), CEREAL_NVP(mainMenu), CEREAL_NVP(simStats));
        }
    };
    Ogre::SceneManager *_sceneMgr;
    Simulation *_sim;
    Player *_pl;
    OgreBites::ImGuiInputListener *_imguiListener;
    Ogre::ImGuiOverlay *_imguiOverlay;
    int _frameCounter {0};
    int _frameLimit{-1};
    Operations _co{Operations::NoOperation};
    ModalDialog _md {ModalDialog::NoDialog};
    VisibleUI _visibleUI;
public:
    Application();
    void setup();
    void shutdown();
    bool frameStarted(const Ogre::FrameEvent &);
//     bool frameEnded(const Ogre::FrameEvent &);
//     void frameRendered (const Ogre::FrameEvent &evt)
    bool keyPressed (const OgreBites::KeyboardEvent &evt);
    bool keyReleased (const OgreBites::KeyboardEvent &evt);
    bool mouseMoved (const OgreBites::MouseMotionEvent &evt);
    bool mousePressed (const OgreBites::MouseButtonEvent &evt);
    bool mouseReleased (const OgreBites::MouseButtonEvent &evt);
    bool mouseWheelRolled (const OgreBites::MouseWheelEvent &evt);
    bool touchMoved (const OgreBites::TouchFingerEvent &evt);
    bool touchPressed (const OgreBites::TouchFingerEvent &evt);
    bool touchReleased (const OgreBites::TouchFingerEvent &evt);

    void updateMainMenu();
    void updateSimStatsWindow();

    void dispatchModalDialog();
    void simulationNotEmptyModalDialog();

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
    void saveSimulation();

    void closeOperationPopup();
};
