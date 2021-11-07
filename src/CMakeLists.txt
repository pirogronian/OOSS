
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

target_include_directories(OOSS SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(OOSS PRIVATE
    Application.h
    Application.cpp
    Simulation.h
    Simulation.cpp
    physics/BtOgre.h
    physics/BtOgre.cpp
    physics/RigidBody.h
    physics/RigidBody.cpp
    physics/DynamicsWorld.h
    physics/DynamicsWorld.cpp
    main.cpp
)

target_link_libraries(OOSS OgreMain OgreBites)

target_link_libraries(OOSS ${BULLET_LIBRARIES})
