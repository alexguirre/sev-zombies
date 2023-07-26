# sev-zombies

Simple zombie-shooter game made for an intro to game development university course.

## Compiling

1. Install vcpkg:
    ```sh
    git clone https://github.com/Microsoft/vcpkg.git
    .\vcpkg\bootstrap-vcpkg.bat
    ```
2. Install dependencies:
    ```sh
    vcpkg install --triplet=x64-windows-static sdl2 sdl2-ttf sdl2-mixer sdl2-image
    ```
3. Run CMake:
    ```sh
    mkdir build
    cd build
    cmake ../Zombies/ -DCMAKE_TOOLCHAIN_FILE="../vcpkg/scripts/buildsystems/vcpkg.cmake" -DVCPKG_TARGET_TRIPLET=x64-windows-static
    ```
4. Compile:
    ```sh
    cmake --build . --config Release
    ```

## Compiling with Emscripten

1. Install vcpkg:
    ```sh
    git clone https://github.com/Microsoft/vcpkg.git
    .\vcpkg\bootstrap-vcpkg.bat
    ```
2. Install dependencies:
    ```sh
    vcpkg install --triplet=wasm32-emscripten sdl2 sdl2-ttf sdl2-mixer sdl2-image
    ```
3. Run CMake:
    ```sh
    mkdir build-em
    cd build-em
    emcmake cmake ../Zombies/ -DCMAKE_TOOLCHAIN_FILE="../vcpkg/scripts/buildsystems/vcpkg.cmake" -DVCPKG_TARGET_TRIPLET=wasm32-emscripten -DVCPKG_CHAINLOAD_TOOLCHAIN_FILE="%EMSDK%/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake"
    ```
4. Compile:
    ```sh
    cmake --build . --config Release
    ```