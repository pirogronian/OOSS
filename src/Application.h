
// #include <Ogre.h>
#include <OgreApplicationContext.h>

#include <ImguiManager.h>
// #include <physics/DynamicsWorld.h>

#include <Simulation.h>

class Application :  public OgreBites::ApplicationContext, public OgreBites::InputListener
{
    Simulation *_sim;
    OgreBites::InputListener *_imguiListener;
    OgreBites::InputListener *_cammanListener;
public:
    Application();
    void setup();
    void shutdown();
    bool frameStarted(const Ogre::FrameEvent &);
//     void frameRendered (const Ogre::FrameEvent &evt)
    bool keyPressed (const OgreBites::KeyboardEvent &evt);
    bool keyReleased (const OgreBites::KeyboardEvent &evt);
    bool mouseMoved (const OgreBites::MouseMotionEvent &evt);
    bool mousePressed (const OgreBites::MouseButtonEvent &evt);
    bool mouseReleased (const OgreBites::MouseButtonEvent &evt);
    bool mouseWheelRolled (const OgreBites::MouseWheelEvent &evt);
//     bool touchMoved (const OgreBites::TouchFingerEvent &evt);
//     bool touchPressed (const OgreBites::TouchFingerEvent &evt);
//     bool touchReleased (const OgreBites::TouchFingerEvent &evt);
};
