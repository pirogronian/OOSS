
#include <OgreApplicationContext.h>
// #include <ImguiManager.h>
#include <OgreImGuiOverlay.h>
#include <OgreImGuiInputListener.h>

#include <Simulation.h>

class Application :  public OgreBites::ApplicationContext, public OgreBites::InputListener
{
    Ogre::SceneManager *_sceneMgr;
    Simulation *_sim;
    OgreBites::ImGuiInputListener *_imguiListener;
    Ogre::ImGuiOverlay *_imguiOverlay;
    int _frameCounter {0};
    int _frameLimit{-1};
    struct VisibleUI {
        bool demoWindow {false};
        bool mainMenu {true};
        bool simStats {false};
        bool populateWarning {false};
    };
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
    void updatePopulateWarningWindow();
};
