#include "GameActivityHandler.h"
#include "GameRunParams.h"
#include "Launcher.h"
#include <android/log.h>
#include <delauncher/engine/delEngineInstanceDirect.h>
#include <delauncher/game/delGameRunParams.h>
#include <delauncher/game/delGame.h>
#include <deremotelauncher/derlLauncherClient.h>

class RemoteLauncherHandler : public GameActivityHandler{
private:
    enum class State{
        idle,
        startGame,
        gameRunning,
        gameStopped
    };

    Launcher *pLauncher;
    delGame::Ref pGame;
    delGameRunParams pRunParams;
    State pState;

public:
    explicit RemoteLauncherHandler(Launcher *launcher) :
            pLauncher(launcher),
            pState(State::idle)
    {
    }

    void Command(BaseGameActivityAdapter &adapter, int32_t cmd) override{
        __android_log_print(ANDROID_LOG_INFO, "RemoteLauncherHandler", "command %d", (int)cmd);
    }

    void FrameUpdate(BaseGameActivityAdapter &adapter) override{
        try {
            switch(pState){
                case State::idle:
                    break;

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

    void StartGame(BaseGameActivityAdapter &adapter){
        __android_log_print(ANDROID_LOG_INFO, "RemoteLauncherHandler", "StartGame");
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

    void ProcessRunning(BaseGameActivityAdapter &adapter){
        if(pGame->IsRunning()){
            pGame->GetEngineInstance()->RunSingleFrameUpdate();
        }else{
            pState = State::gameStopped;
            adapter.QuitActivity();
        }
    }
};


extern "C"
JNIEXPORT jlong JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_RemoteLauncherHandler_createHandler(
JNIEnv *env, jobject thiz, jlong plauncher) {
    JniHelpers h(env);
    try {
        return (jlong)(intptr_t)(new RemoteLauncherHandler((Launcher*)(intptr_t)plauncher));
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
