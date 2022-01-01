
#set(OGRE_BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ogre)

#set(OGRE_STATIC TRUE)
#set(OGRE_BUILD_COMPONENT_PYTHON FALSE)
#set(OGRE_BUILD_COMPONENT_JAVA FALSE)
#set(OGRE_BUILD_COMPONENT_CSHARP FALSE)
#set(OGRE_BUILD_SAMPLES FALSE)
#set(OGRE_BUILD_TOOLS FALSE)
#set(OGRE_BUILD_XSIEXPORTER FALSE)
#set(OGRE_CONFIG_DOUBLE TRUE)
#set(OGRE_INSTALL_TOOLS FALSE)
#set(OGRE_INSTALL_DOCS FALSE)
#set(OGRE_PROFILING TRUE)
#set(OGRE_BUILD_PLUGIN_CG FALSE)
#set(OGRE_GLSUPPORT_USE_EGL FALSE)
#add_subdirectory(ogre)

#find_package(OGRE 13 REQUIRED PATHS ${OGRE_BUILD_DIR} NOT DEFAULT COMPONENTS Bites RTShaderSystem)

find_package(OGRE 13 REQUIRED COMPONENTS Bites RTShaderSystem)

file(COPY ${OGRE_CONFIG_DIR}/resources.cfg DESTINATION ${CMAKE_BINARY_DIR})

add_executable(OOSS)

target_include_directories(OOSS SYSTEM PUBLIC ${OGRE_INCLUDE_DIRS})
target_include_directories(OOSS SYSTEM PUBLIC ${OGRE_INCLUDE_DIRS}/Bites)
target_include_directories(OOSS SYSTEM PUBLIC ${OGRE_INCLUDE_DIRS}/RTShaderSystem)

find_package(Bullet REQUIRED)
target_include_directories(OOSS SYSTEM PUBLIC ${BULLET_INCLUDE_DIRS})

find_package(Cereal REQUIRED)

target_include_directories(OOSS SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui)

target_include_directories(OOSS SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ogre-imgui)

target_include_directories(OOSS SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_definitions(-DIMGUI_DISABLE_OBSOLETE_FUNCTIONS)
add_definitions(-DHAVE_OGRE_BITES)

set(IMGUI_SRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imconfig.h
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.h
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_demo.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp
        #${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_internal.h
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_tables.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp
        #${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_rectpack.cpp
        #${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_textedit.cpp
        #${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_truetype.cpp
        #${CMAKE_CURRENT_SOURCE_DIR}/imgui/misc/
)

target_sources(OOSS PRIVATE
    ${IMGUI_SRCS}
    Application.h
    Application.cpp
    SubModule.h
    SubModule.cpp
    Simulation.h
    Simulation.cpp
    Player.h
    Player.cpp
    LoadSaveModal.h
    LoadSaveModal.cpp
    LoadModal.h
    LoadModal.cpp
    SimTimeControl.h
    SimTimeControl.cpp
    SceneInspector.h
    SceneInspector.cpp
    physics/physics.h
    physics/physics.cpp
    physics/BtOgre.h
    physics/BtOgre.cpp
    physics/RigidBody.h
    physics/RigidBody.cpp
    physics/GravityCenter.h
    physics/GravityCenter.cpp
    physics/DynamicsWorld.h
    physics/DynamicsWorld.cpp
    utils/DumpHelper.h
    utils/DumpHelper.cpp
    utils/FileList.h
    utils/FileList.cpp
    main.cpp
)

target_link_libraries(OOSS OgreMain OgreBites)

add_definitions     ( ${BULLET_DEFINITIONS} )
include_directories ( ${BULLET_INCLUDE_DIRS} )
link_directories    ( ${BULLET_LIBRARY_DIRS} )

target_link_libraries(OOSS ${BULLET_LIBRARIES})
target_link_libraries(OOSS BulletWorldImporter)

add_subdirectory(tests)
