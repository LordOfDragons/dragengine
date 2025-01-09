#include <game-activity/native_app_glue/android_native_app_glue.h>
#include <cstdint>
#include <mutex>
#include <dragengine/common/exceptions.h>
#include "GameActivityHandler.h"

static class GameActivityAdapter : public BaseGameActivityAdapter{
private:
    android_app *pApp = nullptr;
    std::mutex pMutex;
    GameActivityHandler *pHandler = nullptr;
    JNIEnv *pJniEnv = nullptr;

public:
    GameActivityAdapter() = default;
    ~GameActivityAdapter() override{
        SetHandler(nullptr);
    }

    void SetHandler(GameActivityHandler *handler){
        std::lock_guard guard(pMutex);
        pHandler = (GameActivityHandler*)(intptr_t)handler;
    }

    void GameLoop(android_app *app){
        app->userData = this;
        app->onAppCmd = pCBOnAppCmd;
        app->textInputState = 0;
        pApp = app;

        app->activity->vm->AttachCurrentThread(&pJniEnv, nullptr);

        while(true){
            int events;
            android_poll_source *source;

            // mHasFocus && mIsVisible && mHasWindow => timeoutMillis = 0
            // otherwise => timeoutMillis = -1
            while((ALooper_pollOnce(0, nullptr, &events, (void**)&source)) >= 0){
                std::lock_guard guard(pMutex);

                // process message
                if(source){
                    source->process(source->app, source);
                }
                if(app->destroyRequested){
                    return;
                }
            }

            std::lock_guard guard(pMutex);
            if(pHandler){
                pHandler->FrameUpdate(*this);
            }
        }

        app->activity->vm->DetachCurrentThread();
        pJniEnv = nullptr;

        app->textInputState = 0;
        app->onAppCmd = nullptr;
        app->userData = nullptr;
        pApp = nullptr;
    }

    ANativeWindow * GetNativeWindow() override{
        return pApp ? pApp->window : nullptr;
    }

    void QuitActivity() override{
        if(pApp) {
            GameActivity_finish(pApp->activity);
        }
    }

private:
    static void pCBOnAppCmd(android_app *app, int32_t cmd){
        ((GameActivityAdapter*)app->userData)->pHandleCommand(cmd);
    }

    void pHandleCommand(int32_t cmd){
        // callback is triggered while the mutex is held
        //std::lock_guard guard(pMutex);
        if(pHandler){
            pHandler->Command(*this, cmd);
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
