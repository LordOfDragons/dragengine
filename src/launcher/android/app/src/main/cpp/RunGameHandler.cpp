#include "GameActivityHandler.h"
#include "GameRunParams.h"
#include <android/log.h>
#include <delauncher/game/delGameRunParams.h>
#include <delauncher/game/delGame.h>

class RunGameHandler : public GameActivityHandler{
private:
    delGame::Ref pGame;
    delGameRunParams pRunParams;

public:
    explicit RunGameHandler(delGame *game, const delGameRunParams &params) :
    pGame(game),
    pRunParams(params){
    }

    ~RunGameHandler() override = default;

    void Activated() override{
        __android_log_print(ANDROID_LOG_INFO, "RunGameHandler", "activated");
    }

    void Command(android_app *app, int32_t cmd) override{
        __android_log_print(ANDROID_LOG_INFO, "RunGameHandler", "command %d", (int)cmd);
    }
};


extern "C"
JNIEXPORT jlong JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_RunGameHandler_createHandler(
JNIEnv *env, jobject thiz, jobject pgame, jobject pparams) {
    return (jlong)(intptr_t)(new RunGameHandler((delGame*)pgame,
        GameRunParams(env).FromNative(pparams)));
}
