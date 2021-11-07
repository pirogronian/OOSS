
// #include <Ogre.h>
#include <OgreApplicationContext.h>

// #include <physics/DynamicsWorld.h>

#include <Simulation.h>

class Application :  public OgreBites::ApplicationContext, public OgreBites::InputListener
{
    Simulation *_sim;
public:
    Application();
    void setup();
    void shutdown();
    bool frameStarted(const Ogre::FrameEvent &);
    bool keyPressed(const OgreBites::KeyboardEvent&);
};
