#include "GameActivityHandler.h"
#include "GameRunParams.h"
#include "Launcher.h"
#include <android/log.h>
#include <delauncher/engine/delEngineInstanceDirect.h>
#include <delauncher/game/delGameRunParams.h>
#include <delauncher/game/delGame.h>
#include <deremotelauncher/derlLauncherClient.h>
#include "RemoteLauncherHandler.h"

RemoteLauncherHandler::RemoteLauncherHandler(Launcher *launcher, delGame *game, const delGameRunParams &params) :
RunGameHandler(launcher, game, params)
{
}

void RemoteLauncherHandler::StartGame(BaseGameActivityAdapter &adapter){
    __android_log_print(ANDROID_LOG_INFO, "RemoteLauncherHandler", "StartGame");
    pState = State::gameRunning;

    const delEngineInstanceDirect::Factory::Ref factory(
            delEngineInstanceDirect::Factory::Ref::New(
                    new delEngineInstanceDirect::Factory()));

    deOSAndroid::sConfig osConfig(pLauncher->GetConfig().osConfig);
    factory->SetConfig(osConfig);

    pGame->SetVFSDelgaContainer(nullptr);
    pGame->SetDelgaFile("");

    pGame->StartGame(pRunParams, factory);
}

void RemoteLauncherHandler::GameExited(BaseGameActivityAdapter &adapter) {
    // notify client about quit
}


extern "C"
JNIEXPORT jlong JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_RemoteLauncherHandler_createHandler(
        JNIEnv *env, jobject thiz, jlong plauncher, jlong pgame, jobject pparams) {
    JniHelpers h(env);
    try {
        return (jlong)(intptr_t)(new RemoteLauncherHandler(
                (Launcher*)(intptr_t)plauncher,
                (delGame*)(intptr_t)pgame,
                GameRunParams(env).FromNative(pparams)));
    }catch(const deException &e){
        h.throwException(e);
        return 0L; // keep compiler happy. code never gets here
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_RemoteLauncherHandler_destroyHandler(
JNIEnv *env, jobject thiz, jlong phandler){
    delete (RemoteLauncherHandler*)(intptr_t)phandler;
}
