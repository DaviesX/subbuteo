cmake_minimum_required(VERSION 3.16)

project(subbuteo)

set(CMAKE_CXX_STANDARD 20)

find_package(box2d REQUIRED)
find_package(gflags REQUIRED)
find_package(glog REQUIRED)
find_program(iwyu_path NAMES include-what-you-use iwyu REQUIRED)
find_package(rapidjson REQUIRED)
find_package(SFML REQUIRED COMPONENTS
             audio
             graphics
             system
             window)

if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -fPIC -fsanitize=address -g")
    set(LINK_FLAGS "-fsanitize=address")
elseif(${CMAKE_BUILD_TYPE} STREQUAL Release)
    set(CMAKE_CXX_FLAGS "-fPIC -Ofast -flto=auto")
    set(LINK_FLAGS "-Ofast -flto=auto")
endif()

set(SRCS
    src/agent_human.cpp
    src/agent.cpp
    src/camera.cpp
    src/config.cpp
    src/control.cpp
    src/coordinate.cpp
    src/draw.cpp
    src/drawable.cpp
    src/event_queue.cpp
    src/game_flow.cpp
    src/game.cpp
    src/log.cpp
    src/match.cpp
    src/scene.cpp)
set(LIBS
    box2d::box2d
    gflags
    glog::glog
    sfml-audio
    sfml-graphics
    sfml-system
    sfml-window)

add_library(game ${SRCS})
target_include_directories(game PUBLIC src)
target_link_libraries(game PUBLIC ${LIBS})
set_property(TARGET game PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})

add_executable(${PROJECT_NAME} src/game_main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC src)
target_link_libraries(${PROJECT_NAME} PUBLIC game)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})

# Tests.
function(add_test TEST_NAME SOURCE_FILE_NAME)
    add_executable(${TEST_NAME} ${SOURCE_FILE_NAME})
    target_include_directories(${TEST_NAME} PRIVATE .)
    target_link_libraries(${TEST_NAME}
                          PRIVATE
                          ${PROJECT_NAME}_standalone
                          Boost::unit_test_framework)
endfunction()
