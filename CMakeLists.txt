cmake_minimum_required(VERSION 3.26)
# Configurando o padrão c++
set(CMAKE_CXX_STANDARD 11)

# INIT NO SCREEN BUGFIX
# Em alguns casos, o programa pode não renderizar a tela. Por isso temos
# as seguintes linhas. Caso encontre algum problema, comente-as.
set(CMAKE_SKIP_RPATH ON)
set(CMAKE_SKIP_RPATH ON CACHE BOOL "Skip RPATH" FORCE)
# END NO SCREEN BUGFIX
####### DO NOT EDIT ABOVE

# Configurando o nome e executável do projeto
set(PROJECT_NAME "jogo-final")
project(${PROJECT_NAME})
add_executable(${PROJECT_NAME}
        Source/Main.cpp
        Source/Math.cpp
        Source/Random.cpp
        Source/Actors/Actor.cpp
        Source/Components/Component.cpp
        Source/Game.cpp
        Source/PlayerProcessor.cpp
        Source/PlayerProcessor.h
        Source/InputProcess.cpp
        Source/InputProcess.h
        Source/Components/DrawComponents/DrawComponent.cpp
        Source/Components/DrawComponents/DrawComponent.h
        Source/Components/RigidBodyComponent.cpp
        Source/Components/RigidBodyComponent.h
        Source/Components/ColliderComponents/CircleColliderComponent.cpp
        Source/Components/ColliderComponents/CircleColliderComponent.h
        Source/Json.h
        Source/Components/DrawComponents/DrawSpriteComponent.cpp
        Source/Components/DrawComponents/DrawSpriteComponent.h
        Source/Components/DrawComponents/DrawPolygonComponent.cpp
        Source/Components/DrawComponents/DrawPolygonComponent.h
        Source/Actors/Mario.cpp
        Source/Actors/Mario.h
        Source/Components/ColliderComponents/AABBColliderComponent.cpp
        Source/Components/ColliderComponents/AABBColliderComponent.h
        Source/Components/DrawComponents/DrawAnimatedComponent.cpp
        Source/Components/DrawComponents/DrawAnimatedComponent.h
        Source/Actors/Block.cpp
        Source/Actors/Block.h
        Source/Actors/Goomba.cpp
        Source/Actors/Goomba.h
        Source/Actors/Spawner.cpp
        Source/Actors/Spawner.h
        Source/Actors/Ball.cpp
        Source/Actors/Ball.h
        Source/Actors/Point.cpp
        Source/Actors/Point.h
        Source/Timer.h
        Source/Music.cpp
        Source/Music.h
        Source/MusicManager.cpp
        Source/MusicManager.h
        Source/Scenes/Menu.cpp
        Source/Scenes/Menu.h
        Source/Scenes/Overworld.cpp
        Source/Scenes/Overworld.h
        Source/Scenes/Single.cpp
        Source/Scenes/Single.h
        Source/Scenes/Scene.cpp
        Source/Scenes/Scene.h
        Source/Font.cpp
        Source/Font.h
        Source/Components/DrawComponents/DrawTextComponent.cpp
        Source/Components/DrawComponents/DrawTextComponent.h

)

####### DO NOT EDIT BELLOW
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} SDL2_image::SDL2_image ${SDL2_MIXER_LIBRARIES} -lSDL2_ttf  -lSDL2_mixer pthread)
