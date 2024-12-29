#include <memory>

#include <jni.h>
#include <android/native_window_jni.h>
#include <android/native_window_jni.h>
#include <android/log.h>

#include <delauncher/delLauncher.h>
#include <delauncher/engine/modules/delEngineModule.h>
#include <delauncher/engine/modules/parameter/delEMParameter.h>
#include <delauncher/game/delGame.h>
#include <delauncher/game/fileformat/delFileFormat.h>
#include <delauncher/game/icon/delGameIcon.h>

#include "JniHelper.h"
#include "EngineModule.h"
#include "Game.h"

static JavaVM *vJavaVM = nullptr;

jint JNI_OnLoad(JavaVM *vm, void *reserved){
    vJavaVM = vm;
    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_Launcher_createLauncher(
        JNIEnv *env, jobject thiz, jobject config) {
    //__android_log_print(ANDROID_LOG_ERROR, "Launcher", "Checkpoint %d", __LINE__);
    JniObjectClass clsConfig(env, config);

    delLauncher::sConfig delConfig{};

    delConfig.loggerSource = clsConfig.GetFieldString("loggerSource").Get(config);
    delConfig.engineLogFileTitle = clsConfig.GetFieldString("engineLogFileTitle").Get(config);
    delConfig.pathLauncher = clsConfig.GetFieldString("pathLauncher").Get(config);
    delConfig.pathGames = clsConfig.GetFieldString("pathLauncherGames").Get(config);
    delConfig.pathConfig = clsConfig.GetFieldString("pathLauncherConfig").Get(config);

    delConfig.osConfig.pathEngine = clsConfig.GetFieldString("pathEngine").Get(config);
    delConfig.osConfig.pathConfig = clsConfig.GetFieldString("pathEngineConfig").Get(config);
    delConfig.osConfig.pathCache = clsConfig.GetFieldString("pathEngineCache").Get(config);

    delConfig.osConfig.javavm = vJavaVM;

    {
        const JniFieldObject fid(env, clsConfig, "surface", "Landroid/view/SurfaceView;");
        auto js = fid.Get(config);
        if(js) {
            delConfig.osConfig.nativeWindow = ANativeWindow_fromSurface(env, js);
        }
        //ANativeWindow_release();
    }

    delLauncher *launcher = nullptr;
    try {
        launcher = new delLauncher(delConfig);
        launcher->AddFileLogger("delauncher");
        launcher->Prepare();
        return (jlong)(intptr_t)launcher;

    } catch(...) {
        delete launcher;
        throw;
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_Launcher_destroyLauncher(
        JNIEnv *env, jobject thiz, jlong launcher) {
    delete (delLauncher*)(intptr_t)launcher;
}

extern "C"
JNIEXPORT jobjectArray JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_Launcher_getEngineModules(JNIEnv *env, jobject thiz, jlong plauncher) {
    const delLauncher &launcher = *((delLauncher*)(intptr_t)plauncher);
    return EngineModule(env).Convert(launcher.GetEngine().GetModules());
}

extern "C"
JNIEXPORT jobjectArray JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_Launcher_getGames(JNIEnv *env, jobject thiz, jlong plauncher) {
    const delLauncher &launcher = *((delLauncher *) (intptr_t) plauncher);
    return Game(env).Convert(launcher.GetGameManager().GetGames());
}
