
# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html.
# For more examples on how to use CMake, see https://github.com/android/ndk-samples.

# Sets the minimum CMake version required for this project.
cmake_minimum_required(VERSION 3.22.1)

# Declares the project name. The project name can be accessed via ${ PROJECT_NAME},
# Since this is the top level CMakeLists.txt, the project name is also accessible
# with ${CMAKE_PROJECT_NAME} (both CMake variables are in-sync within the top level
# build script scope).
project("launcherinternal")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
#
# In this top level CMakeLists.txt, ${CMAKE_PROJECT_NAME} is used to define
# the target library name; in the sub-module's CMakeLists.txt, ${PROJECT_NAME}
# is preferred for the same purpose.
#
# In order to load a library into your app from Java/Kotlin, you must call
# System.loadLibrary() and pass the name of the library defined here;
# for GameActivity/NativeActivity derived applications, the same library name must be
# used in the AndroidManifest.xml file.
add_library(${CMAKE_PROJECT_NAME} SHARED
    # List C/C++ source files with relative paths to this CMakeLists.txt.
        Launcher.cpp
        JniHelper.cpp
        EngineModule.cpp
        Game.cpp
        GameProfile.cpp
        FDFileReader.cpp
        FDVFSContainer.cpp)

include_directories("${CMAKE_SOURCE_DIR}/../../../../../shared/build/include"
    "${CMAKE_SOURCE_DIR}/../../../../../../dragengine/build/include")

# Specifies libraries CMake should link to your target library. You
# can link libraries from various origins, such as libraries defined in this
# build script, prebuilt third-party libraries, or Android system libraries.
target_link_libraries(${CMAKE_PROJECT_NAME}
    ${CMAKE_SOURCE_DIR}/../jniLibs/${CMAKE_ANDROID_ARCH_ABI}/libdelauncher.so
    ${CMAKE_SOURCE_DIR}/../jniLibs/${CMAKE_ANDROID_ARCH_ABI}/libdragengine.so
    android
    log)



# GameActivity adapter
find_package(game-activity REQUIRED CONFIG)

get_target_property(game-activity-include
        game-activity::game-activity
        INTERFACE_INCLUDE_DIRECTORIES)

include_directories(${game-activity-include})

set(CMAKE_SHARED_LINKER_FLAGS
    "${CMAKE_SHARED_LINKER_FLAGS} -u \
    Java_com_google_androidgamesdk_GameActivity_initializeNativeCode")

add_library("game_activity_adapter" SHARED
        GameActivityAdapter.cpp
)

target_link_libraries("game_activity_adapter"
    game-activity::game-activity_static
    android
    log)



# RunGameHandler
add_library("run_game_handler" SHARED
    JniHelper.cpp
    RunGameHandler.cpp
    GameRunParams.cpp
)

include_directories(
        ${game-activity-include}
        "${CMAKE_SOURCE_DIR}/../../../../../shared/build/include"
        "${CMAKE_SOURCE_DIR}/../../../../../../dragengine/build/include")

target_link_libraries("run_game_handler"
    ${CMAKE_SOURCE_DIR}/../jniLibs/${CMAKE_ANDROID_ARCH_ABI}/libdelauncher.so
    ${CMAKE_SOURCE_DIR}/../jniLibs/${CMAKE_ANDROID_ARCH_ABI}/libdragengine.so
    android
    log)



# RemoteLauncherHandler
add_library("remote_launcher_handler" SHARED
    JniHelper.cpp
    RemoteLauncherHandler.cpp
    RunGameHandler.cpp
    RemoteLauncherClient.cpp
    GameRunParams.cpp
)

include_directories(
        ${game-activity-include}
        "${CMAKE_SOURCE_DIR}/../../../../../shared/build/include"
        "${CMAKE_SOURCE_DIR}/../../../../../../dragengine/build/include"
        "${CMAKE_SOURCE_DIR}/../../../../../../../extern/deremotelauncher/build/deremotelauncher-android-armv8-nightly/include",
        "${CMAKE_SOURCE_DIR}/../../../../../../../extern/denetwork/build/denetworkcpp-android-armv8-nightly/include")

target_link_libraries("remote_launcher_handler"
    ${CMAKE_SOURCE_DIR}/../jniLibs/${CMAKE_ANDROID_ARCH_ABI}/libdelauncher.so
    ${CMAKE_SOURCE_DIR}/../jniLibs/${CMAKE_ANDROID_ARCH_ABI}/libdragengine.so
    ${CMAKE_SOURCE_DIR}/../../../../../../../extern/deremotelauncher/build/deremotelauncher-android-armv8-nightly/lib/libderemotelauncher.a
    ${CMAKE_SOURCE_DIR}/../../../../../../../extern/denetwork/build/denetworkcpp-android-armv8-nightly/lib/libdenetwork.a
    android
    log)



# sanitizer
#target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC -fsanitize=address -fno-omit-frame-pointer)
#set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES LINK_FLAGS -fsanitize=address)
#target_compile_options("game_activity_adapter" PUBLIC -fsanitize=address -fno-omit-frame-pointer)
#set_target_properties("game_activity_adapter" PROPERTIES LINK_FLAGS -fsanitize=address)
#target_compile_options("run_game_handler" PUBLIC -fsanitize=address -fno-omit-frame-pointer)
#set_target_properties("run_game_handler" PROPERTIES LINK_FLAGS -fsanitize=address)
#target_compile_options("remote_launcher_handler" PUBLIC -fsanitize=address -fno-omit-frame-pointer)
#set_target_properties("remote_launcher_handler" PROPERTIES LINK_FLAGS -fsanitize=address)
