#include "GameActivityHandler.h"
#include "GameRunParams.h"
#include "Launcher.h"
#include <android/log.h>
#include <delauncher/engine/delEngineInstanceDirect.h>
#include <delauncher/game/delGameRunParams.h>
#include <delauncher/game/delGame.h>
#include <deremotelauncher/derlLauncherClient.h>
#include "RemoteLauncherHandler.h"
#include "RemoteLauncherClient.h"

RemoteLauncherHandler::RemoteLauncherHandler(RemoteLauncherClient *client,
    Launcher *launcher, delGame *game, const delGameRunParams &params) :
RunGameHandler(launcher, game, params),
pClient(client){
}

void RemoteLauncherHandler::GameExited(BaseGameActivityAdapter &adapter) {
}

void RemoteLauncherHandler::pStateChanged() {
    switch(GetState()){
    case State::gameRunning:
        pClient->SetRunStatus(derlLauncherClient::RunStatus::running);
        break;

    default:
        pClient->SetRunStatus(derlLauncherClient::RunStatus::stopped);
    }
}

void RemoteLauncherHandler::pInitEngineInstanceFactory(delEngineInstanceDirect::Factory &factory) {
    factory.SetEngineLogger(pClient->GetEngineLogger());
}

void RemoteLauncherHandler::pInitGameForRun() {
    pGame->SetVFSDelgaContainer(nullptr);
    pGame->SetDelgaFile("");
}


extern "C"
JNIEXPORT jlong JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_RemoteLauncherHandler_createHandler(
JNIEnv *env, jobject thiz, jlong pclient, jlong plauncher, jlong pgame, jobject pparams) {
    JniHelpers h(env);
    try {
        return (jlong)(intptr_t)(new RemoteLauncherHandler(
                (RemoteLauncherClient*)(intptr_t)pclient,
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

extern "C"
JNIEXPORT jint JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_RemoteLauncherHandler_getState(
JNIEnv *env, jobject thiz, jlong phandler){
    return (jint)(((RemoteLauncherHandler*)(intptr_t)phandler)->GetState());
}
