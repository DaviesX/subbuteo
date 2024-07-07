cmake_minimum_required(VERSION 3.16)

project(subbuteo)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED COMPONENTS
             json
             log
             program_options
             unit_test_framework)
find_package(Eigen3 REQUIRED)
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
    src/config.cpp
    src/control.cpp
    src/draw.cpp
    src/event_queue.cpp
    src/game_flow.cpp
    src/game.cpp
    src/log.cpp
    src/match.cpp
    src/physics.cpp)
set(LIBS
    Boost::json
    Boost::log
    Boost::program_options
    sfml-audio
    sfml-graphics
    sfml-system
    sfml-window)

add_library(game ${SRCS})
target_include_directories(game PUBLIC src)
target_link_libraries(game PUBLIC ${LIBS})

add_executable(${PROJECT_NAME} src/game_main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC src)
target_link_libraries(${PROJECT_NAME} PUBLIC game)

# Tests.
function(add_test TEST_NAME SOURCE_FILE_NAME)
    add_executable(${TEST_NAME} ${SOURCE_FILE_NAME})
    target_include_directories(${TEST_NAME} PRIVATE .)
    target_link_libraries(${TEST_NAME}
                          PRIVATE
                          ${PROJECT_NAME}_standalone
                          Boost::unit_test_framework)
endfunction()
