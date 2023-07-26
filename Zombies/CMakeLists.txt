cmake_minimum_required(VERSION 3.20)

project(sev-zombies LANGUAGES CXX)

add_executable(sev-zombies
    Actor.cpp
    Actor.h
    Animation.cpp
    Animation.h
    Audio.cpp
    Audio.h
    BloodSplatter.cpp
    BloodSplatter.h
    Door.cpp
    Door.h
    Game.cpp
    Game.h
    GameLayer.cpp
    GameLayer.h
    Grenade.cpp
    Grenade.h
    GrenadeLauncher.cpp
    GrenadeLauncher.h
    HealthBar.cpp
    HealthBar.h
    Layer.cpp
    Layer.h
    Main.cpp
    MenuLayer.cpp
    MenuLayer.h
    PathFinding.cpp
    PathFinding.h
    Pistol.cpp
    Pistol.h
    Player.cpp
    Player.h
    Projectile.cpp
    Projectile.h
    ScoreFloatingText.cpp
    ScoreFloatingText.h
    Shotgun.cpp
    Shotgun.h
    Space.cpp
    Space.h
    Text.cpp
    Text.h
    Tile.cpp
    Tile.h
    Weapon.cpp
    Weapon.h
    WeaponIcon.cpp
    WeaponIcon.h
    WeaponLocation.cpp
    WeaponLocation.h
    Window.cpp
    Window.h
    Zombie.cpp
    Zombie.h
)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
target_link_libraries(sev-zombies
    PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)
target_link_libraries(sev-zombies PRIVATE $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>)
target_link_libraries(sev-zombies PRIVATE $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>)
target_link_libraries(sev-zombies PRIVATE $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>)

if(EMSCRIPTEN)
    set_target_properties(sev-zombies PROPERTIES SUFFIX ".html")
    target_link_options(sev-zombies PRIVATE --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/res@/res)
    target_link_options(sev-zombies PRIVATE -sINITIAL_MEMORY=67108864)
endif()