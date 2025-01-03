#include <game-activity/native_app_glue/android_native_app_glue.h>
#include <cstdint>
#include <mutex>
#include "GameActivityHandler.h"

static class GameActivityAdapter{
private:
    android_app *pApp = nullptr;
    std::mutex pMutex;

    GameActivityHandler *pHandler = nullptr;
    bool pHandlerChanged = false;

public:
    GameActivityAdapter() = default;
    ~GameActivityAdapter(){
        SetHandler(nullptr);
    }

    void SetHandler(GameActivityHandler *handler){
        std::lock_guard guard(pMutex);
        if(pHandler){
            delete pHandler;
        }
        pHandler = (GameActivityHandler*)(intptr_t)handler;
        pHandlerChanged = true;
    }

    void GameLoop(android_app *app){
        app->userData = this;
        app->onAppCmd = pCBOnAppCmd;
        app->textInputState = 0;
        pApp = app;

        while(true){
            int events;
            android_poll_source *source;

            // check for handler changed. calls Activated() on the handler
            {
                std::lock_guard guard(pMutex);
                pCheckHandlerChanged();
            }

            // mHasFocus && mIsVisible && mHasWindow => timeoutMillis = 0
            // otherwise => timeoutMillis = -1
            while((ALooper_pollOnce(0, nullptr, &events, (void**)&source)) >= 0){
                std::lock_guard guard(pMutex);

                // check for handler changed. calls Activated() on the handler
                pCheckHandlerChanged();

                // process message
                if(source){
                    source->process(source->app, source);
                }
                if(app->destroyRequested){
                    return;
                }
            }
        }

        app->textInputState = 0;
        app->onAppCmd = nullptr;
        app->userData = nullptr;
        pApp = nullptr;
    }

private:
    static void pCBOnAppCmd(android_app *app, int32_t cmd){
        ((GameActivityAdapter*)app->userData)->pHandleCommand(cmd);
    }

    void pHandleCommand(int32_t cmd){
        // callback is triggered while the mutex is held
        //std::lock_guard guard(pMutex);
        if(pHandler){
            pHandler->Command(pApp, cmd);
        }
    }

    void pCheckHandlerChanged(){
        if(!pHandlerChanged) {
            return;
        }
        pHandlerChanged = false;

        if(pHandler){
            pHandler->Activated();
        }
    }
} vState;

extern "C" void android_main(android_app *app){
    vState.GameLoop(app);
}

extern "C"
JNIEXPORT void JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_GameActivityAdapter_setHandler(
JNIEnv *env, jobject thiz, jlong handler){
    vState.SetHandler((GameActivityHandler*)(intptr_t)handler);
}
