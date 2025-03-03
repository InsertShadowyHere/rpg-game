cmake_minimum_required(VERSION 3.30)
project(rpg_game)

set(CMAKE_CXX_STANDARD 20)
set(SOURCE_FILES
        main.cpp
)
add_executable(rpg_game ${SOURCE_FILES})
set(CLIENT_FILES
        client.cpp
)
add_executable(client ${CLIENT_FILES})



include_directories(/usr/local/include)
find_package(SFML 3.0.0 COMPONENTS System Window Graphics Network Audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(rpg_game SFML::System SFML::Window SFML::Graphics SFML::Audio SFML::Network)
target_link_libraries(client SFML::System SFML::Window SFML::Graphics SFML::Audio SFML::Network)