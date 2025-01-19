#include <game-activity/native_app_glue/android_native_app_glue.h>
#include <cstdint>
#include <mutex>
#include <queue>
#include <dragengine/common/exceptions.h>
#include <android/window.h>
#include "GameActivityHandler.h"
#include "CustomEvent.h"

JavaVM *vJavaVM = nullptr;

jint JNI_OnLoad(JavaVM *vm, void *reserved){
    vJavaVM = vm;
    return JNI_VERSION_1_6;
}

#define JPATH_BASE "ch/dragondreams/delauncher/launcher/internal/"

class GameActivityAdapter : public BaseGameActivityAdapter{
private:
    android_app *pApp = nullptr;
    std::mutex pMutex;
    GameActivityHandler *pHandler = nullptr;
    JNIEnv *pJniEnv = nullptr;
    std::queue<CustomEvent> pCustomEvents;

    jclass pClsCustomEvent;
    jfieldID pFldCustomEventType;

public:
    GameActivityAdapter() :
    pClsCustomEvent(nullptr),
    pFldCustomEventType(0){
    }

    ~GameActivityAdapter() override{
        JNIEnv *env = nullptr;
        vJavaVM->AttachCurrentThread(&env, nullptr);

        SetHandler(nullptr);

        if(pClsCustomEvent) {
            env->DeleteGlobalRef(pClsCustomEvent);
        }
    }

    void SetHandler(GameActivityHandler *handler){
        std::lock_guard guard(pMutex);
        pHandler = (GameActivityHandler*)(intptr_t)handler;
    }

    void SendCustomEvent(JNIEnv *env, jobject objEvent){
        if(!pClsCustomEvent){
            pClsCustomEvent = (jclass)env->NewGlobalRef(env->FindClass(JPATH_BASE "CustomEvent"));
            pFldCustomEventType = env->GetFieldID(pClsCustomEvent, "type", "I");
        }

        CustomEvent event;
        event.type = (CustomEvent::Type)env->GetIntField(objEvent, pFldCustomEventType);

        const std::lock_guard guard(pMutex);
        pCustomEvents.push(event);
    }

    void GameLoop(android_app *app){
        app->userData = this;
        app->onAppCmd = pCBOnAppCmd;
        app->textInputState = 0;
        pApp = app;

        app->activity->vm->AttachCurrentThread(&pJniEnv, nullptr);

        // receive all kinds of input events (null filter)
        android_app_set_motion_event_filter(app, nullptr);

        while(true){
            int events;
            android_poll_source *source;

            // mHasFocus && mIsVisible && mHasWindow => timeoutMillis = 0
            // otherwise => timeoutMillis = -1
            while((ALooper_pollOnce(0, nullptr, &events, (void**)&source)) >= 0){
                std::lock_guard guard(pMutex);
                if(source){
                    source->process(source->app, source);
                }
                if(app->destroyRequested){
                    return;
                }
            }

            // process pending key and motion events. requires swapping input buffers to
            // not miss events while processing inputBuffer.
            android_input_buffer *inputBuffer = android_app_swap_input_buffers(pApp);
            if(inputBuffer){
                pHandler->InputEvent(*this, *inputBuffer);
                if(inputBuffer->keyEventsCount != 0){
                    android_app_clear_key_events(inputBuffer);
                }
                if (inputBuffer->motionEventsCount != 0) {
                    android_app_clear_motion_events(inputBuffer);
                }
            }

            // process custom events
            std::lock_guard guard(pMutex);
            while(!pCustomEvents.empty()){
                if(pHandler){
                    pHandler->ProcessCustomEvent(*this, pCustomEvents.front());
                    pCustomEvents.pop();
                }
            }

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

    const ARect & GetContentRect() override{
        static const ARect dummyRect{};
        return pApp ? pApp->contentRect : dummyRect;
    }

    void FinishActivity() override{
        if(pApp) {
            GameActivity_finish(pApp->activity);
        }
    }

    void EnableFullScreenMode(bool enable) override{
        const int flags = AWINDOW_FLAG_KEEP_SCREEN_ON
                | AWINDOW_FLAG_TURN_SCREEN_ON
                | AWINDOW_FLAG_FULLSCREEN
                | AWINDOW_FLAG_SHOW_WHEN_LOCKED;

        if(enable) {
            GameActivity_setWindowFlags(pApp->activity, flags, 0);

        }else{
            GameActivity_setWindowFlags(pApp->activity, 0, flags);
        }
    }

    void GetSavedState(size_t &size, void *&data) override{
        size = pApp->savedStateSize;
        data = pApp->savedState;
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

extern "C"
JNIEXPORT void JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_GameActivityAdapter_sendCustomEvent(
JNIEnv *env, jobject thiz, jobject pevent){
    vState.SendCustomEvent(env, pevent);
}
