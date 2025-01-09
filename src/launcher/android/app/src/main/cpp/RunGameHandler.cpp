#include "GameActivityHandler.h"
#include "GameRunParams.h"
#include "Launcher.h"
#include <android/log.h>
#include <delauncher/engine/delEngineInstanceDirect.h>
#include <delauncher/game/delGameRunParams.h>
#include <delauncher/game/delGame.h>
#include "RunGameHandler.h"

RunGameHandler::RunGameHandler(Launcher *launcher, delGame *game, const delGameRunParams &params) :
pLauncher(launcher),
pGame(game),
pRunParams(params),
pState(State::startGame)
{
    DEASSERT_NOTNULL(game)
    DEASSERT_NOTNULL(params.GetGameProfile())
}

void RunGameHandler::Command(BaseGameActivityAdapter &adapter, int32_t cmd){
    __android_log_print(ANDROID_LOG_INFO, "RunGameHandler", "command %d", (int)cmd);
}

void RunGameHandler::FrameUpdate(BaseGameActivityAdapter &adapter){
    try {
        switch(pState){
            case State::startGame:
                StartGame(adapter);
                break;

            case State::gameRunning:
                ProcessRunning(adapter);
                break;

            case State::gameStopped:
                break;
        }

    }catch(const deException &e){
        JniHelpers::logException(e);
    }
}

void RunGameHandler::StartGame(BaseGameActivityAdapter &adapter){
    __android_log_print(ANDROID_LOG_INFO, "RunGameHandler", "StartGame");
    pState = State::gameRunning;

    const delEngineInstanceDirect::Factory::Ref factory(
        delEngineInstanceDirect::Factory::Ref::New(
            new delEngineInstanceDirect::Factory()));

    deOSAndroid::sConfig osConfig(pLauncher->GetConfig().osConfig);
    //osConfig.nativeWindow = adapter.GetNativeWindow();
    factory->SetConfig(osConfig);

    pGame->SetVFSDelgaContainer((deVFSContainer*)pLauncher->GetFDContainer());

    pGame->StartGame(pRunParams, factory);
}

void RunGameHandler::ProcessRunning(BaseGameActivityAdapter &adapter){
    if(pGame->IsRunning()){
        pGame->GetEngineInstance()->RunSingleFrameUpdate();

    }else{
        pState = State::gameStopped;
        GameExited(adapter);
    }
}

void RunGameHandler::GameExited(BaseGameActivityAdapter &adapter) {
    adapter.QuitActivity();
}


extern "C"
JNIEXPORT jlong JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_RunGameHandler_createHandler(
JNIEnv *env, jobject thiz, jlong plauncher, jlong pgame, jobject pparams) {
    JniHelpers h(env);
    try {
        return (jlong)(intptr_t)(new RunGameHandler(
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
Java_ch_dragondreams_delauncher_launcher_internal_RunGameHandler_destroyHandler(
JNIEnv *env, jobject thiz, jlong phandler){
    delete (RunGameHandler*)(intptr_t)phandler;
}
