
#include <Ogre.h>
#include <OgreApplicationContext.h>

#include <physics/DynamicsWorld.h>

class Application :  public OgreBites::ApplicationContext, public OgreBites::InputListener
{
    Ogre::SceneManager *_sceneMgr{nullptr};
    Ogre::Camera *_mainCam{nullptr};
    DynamicsWorld _world;
    BtOgre::DebugDrawer *_debugDrawer{nullptr};
    bool _debugDraw{true};
public:
    Application();
    void setup();
    void shutdown();
    bool frameStarted(const Ogre::FrameEvent &);
    bool keyPressed(const OgreBites::KeyboardEvent&);
};
