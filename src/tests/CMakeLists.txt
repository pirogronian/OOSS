
add_executable(AutoIndexerTest)

target_sources(AutoIndexerTest PRIVATE
    AutoIndexerTest.cpp
)

target_include_directories(AutoIndexerTest SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/src)
