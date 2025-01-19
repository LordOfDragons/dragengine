#include "GameActivityHandler.h"
#include "GameRunParams.h"
#include "Launcher.h"
#include <thread>
#include <android/log.h>
#include <unordered_map>
#include <delauncher/engine/delEngineInstanceDirect.h>
#include <delauncher/game/delGameRunParams.h>
#include <delauncher/game/delGame.h>
#include <dragengine/common/file/decMemoryFileWriter.h>
#include <dragengine/systems/modules/input/deBaseInputModule.h>
#include <dragengine/logger/deLoggerConsole.h>
#include <dragengine/logger/deLoggerFile.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include "RunGameHandler.h"

JavaVM *vJavaVM = nullptr;

jint JNI_OnLoad(JavaVM *vm, void *reserved){
    vJavaVM = vm;
    return JNI_VERSION_1_6;
}

RunGameHandler::RunGameHandler(JNIEnv *env, Launcher *launcher, delGame *game,
    const delGameRunParams &params, jobject objListener) :
pLauncher(launcher),
pGame(game),
pRunParams(params),
pState(State::startGame),
pRequestStopGame(false),
pObjListener(env, objListener, true),
pClsListener(pObjListener, true),
pMetListenerStateChanged(env->GetMethodID(pClsListener, "stateChanged", "(I)V"))
{
    DEASSERT_NOTNULL(game)
    DEASSERT_NOTNULL(params.GetGameProfile())
    DEASSERT_NOTNULL(pMetListenerStateChanged)
}

RunGameHandler::~RunGameHandler(){
    JNIEnv *env = nullptr;
    vJavaVM->AttachCurrentThread(&env, nullptr);

    pObjListener.Dispose(env);
    pClsListener.Dispose(env);
}

void RunGameHandler::pSetState(RunGameHandler::State state) {
    if (state == pState) {
        return;
    }
    pState = state;
    pStateChanged();
}

void RunGameHandler::RequestStopGame(){
    pRequestStopGame = true;
}

const std::unordered_map<int32_t,const char*> vCommandName = {
    {UNUSED_APP_CMD_INPUT_CHANGED, "UNUSED_APP_CMD_INPUT_CHANGED"},
    {APP_CMD_INIT_WINDOW, "APP_CMD_INIT_WINDOW"},
    {APP_CMD_TERM_WINDOW, "APP_CMD_TERM_WINDOW"},
    {APP_CMD_WINDOW_RESIZED, "APP_CMD_WINDOW_RESIZED"},
    {APP_CMD_WINDOW_REDRAW_NEEDED, "APP_CMD_WINDOW_REDRAW_NEEDED"},
    {APP_CMD_CONTENT_RECT_CHANGED, "APP_CMD_CONTENT_RECT_CHANGED"},
    {APP_CMD_SOFTWARE_KB_VIS_CHANGED, "APP_CMD_SOFTWARE_KB_VIS_CHANGED"},
    {APP_CMD_GAINED_FOCUS, "APP_CMD_GAINED_FOCUS"},
    {APP_CMD_LOST_FOCUS, "APP_CMD_LOST_FOCUS"},
    {APP_CMD_CONFIG_CHANGED, "APP_CMD_CONFIG_CHANGED"},
    {APP_CMD_LOW_MEMORY, "APP_CMD_LOW_MEMORY"},
    {APP_CMD_START, "APP_CMD_START"},
    {APP_CMD_RESUME, "APP_CMD_RESUME"},
    {APP_CMD_SAVE_STATE, "APP_CMD_SAVE_STATE"},
    {APP_CMD_PAUSE, "APP_CMD_PAUSE"},
    {APP_CMD_STOP, "APP_CMD_STOP"},
    {APP_CMD_DESTROY, "APP_CMD_DESTROY"},
    {APP_CMD_WINDOW_INSETS_CHANGED, "APP_CMD_WINDOW_INSETS_CHANGED"},
    {APP_CMD_EDITOR_ACTION, "APP_CMD_EDITOR_ACTION"},
    {APP_CMD_KEY_EVENT, "APP_CMD_KEY_EVENT"},
    {APP_CMD_TOUCH_EVENT, "APP_CMD_TOUCH_EVENT"}};

void RunGameHandler::Command(BaseGameActivityAdapter &adapter, int32_t cmd){
    {
        const auto iter = vCommandName.find(cmd);
        __android_log_print(ANDROID_LOG_INFO, "RunGameHandler", "command %s(%d)",
            iter != vCommandName.cend() ? iter->second : "?", (int)cmd);
    }

    // graphic module responsibility:
    // - APP_CMD_WINDOW_RESIZED
    // - APP_CMD_WINDOW_REDRAW_NEEDED
    // - APP_CMD_CONTENT_RECT_CHANGED
    // - APP_CMD_CONFIG_CHANGED
    // - APP_CMD_WINDOW_INSETS_CHANGED

    switch(cmd){
    case APP_CMD_INIT_WINDOW: {
        size_t size;
        void *data;
        adapter.GetSavedState(size, data);
        __android_log_print(ANDROID_LOG_INFO, "RunGameHandler",
            "APP_CMD_INIT_WINDOW: savedState(%d bytes)", (int)size);

        if(size > 0 && data){
            pSavedState.TakeOver(new decMemoryFile("savedState"));
            decMemoryFileWriter::Ref::New(
                new decMemoryFileWriter(pSavedState, false))
                    ->Write(data, (int)size);
        }
        }break;

    case APP_CMD_GAINED_FOCUS:
        __android_log_print(ANDROID_LOG_INFO, "RunGameHandler",
            "APP_CMD_GAINED_FOCUS: hasEngine %d", pGame->HasEngineInstance());
        if(pGame->HasEngineInstance()){
            pGame->GetEngineInstance()->SetAppActive(true);
        }
        break;

    case APP_CMD_LOST_FOCUS:
        __android_log_print(ANDROID_LOG_INFO, "RunGameHandler",
            "APP_CMD_LOST_FOCUS: hasEngine %d", pGame->HasEngineInstance());
        if(pGame->HasEngineInstance()){
            pGame->GetEngineInstance()->SetAppActive(false);
        }
        break;

    case APP_CMD_RESUME:
        __android_log_print(ANDROID_LOG_INFO, "RunGameHandler",
            "APP_CMD_RESUME: hasEngine %d", pGame->HasEngineInstance());
        if(pGame->HasEngineInstance()){
            pGame->GetEngineInstance()->SetAppPaused(false);
        }
        break;

    case APP_CMD_SAVE_STATE:
        // TODO
        break;

    case APP_CMD_PAUSE:
        __android_log_print(ANDROID_LOG_INFO, "RunGameHandler",
            "APP_CMD_PAUSE: hasEngine %d", pGame->HasEngineInstance());
        if(pGame->HasEngineInstance()){
            pGame->GetEngineInstance()->SetAppPaused(true);
        }
        break;

    default:
        break;
    }
}

void RunGameHandler::FrameUpdate(BaseGameActivityAdapter &adapter){
    try {
        switch(pState){
            case State::startGame:
                StartGame(adapter);
                break;

            case State::gameRunning:
                if(pRequestStopGame){
                    pRequestStopGame = false;
                    StopGame(adapter);

                }else {
                    ProcessRunning(adapter);
                }
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
    pSetState(State::gameRunning);

    const delEngineInstanceDirect::Factory::Ref factory(
        delEngineInstanceDirect::Factory::Ref::New(
            new delEngineInstanceDirect::Factory()));
    pInitEngineInstanceFactory(factory);

    deOSAndroid::sConfig osConfig(pLauncher->GetConfig().osConfig);
    //osConfig.nativeWindow = adapter.GetNativeWindow();
    factory->SetConfig(osConfig);

    pGame->SetVFSDelgaContainer((deVFSContainer *) pLauncher->GetFDContainer());
    pInitGameForRun();

    pGame->StartGame(pRunParams, factory);
}

void RunGameHandler::ProcessRunning(BaseGameActivityAdapter &adapter){
    pGame->PulseChecking();
    if(pGame->IsRunning()){
        const ARect &contentRect = adapter.GetContentRect();
        pGame->GetEngineInstance()->UpdateContentRect(decBoundary(
                contentRect.left, contentRect.top,
                contentRect.right, contentRect.bottom));

        pGame->GetEngineInstance()->RunSingleFrameUpdate();

    }else{
        pSetState(State::gameStopped);
        GameExited(adapter);
    }
}

void RunGameHandler::StopGame(BaseGameActivityAdapter &adapter) {
    if(pGame->IsRunning()) {
        pGame->StopGame();
    }
    pSetState(State::gameStopped);
}

void RunGameHandler::GameExited(BaseGameActivityAdapter &adapter) {
    adapter.FinishActivity();
}

void RunGameHandler::pStateChanged() {
    if(!pObjListener){
        return;
    }

    JNIEnv *env = nullptr;
    vJavaVM->AttachCurrentThread(&env, nullptr);
    env->CallVoidMethod(pObjListener, pMetListenerStateChanged, (jint)GetState());
}

void RunGameHandler::pInitEngineInstanceFactory(delEngineInstanceDirect::Factory &factory) {
    pCreateEngineLogger();
    factory.SetEngineLogger(pEngineLogger);
}

void RunGameHandler::pInitGameForRun() {
}

void RunGameHandler::InputEvent(BaseGameActivityAdapter &adapter, const android_input_buffer &inputBuffer) {
    if(pGame->HasEngineInstance()) {
        pGame->GetEngineInstance()->InputEvent(inputBuffer);
    }
}

void RunGameHandler::ProcessCustomEvent(BaseGameActivityAdapter &adapter, const CustomEvent &event){
    switch(event.type){
        case CustomEvent::Type::backButton:
            //__android_log_print(ANDROID_LOG_INFO, "RunGameHandler", "custom event back button");
            adapter.FinishActivity();
            break;
    }
}

void RunGameHandler::pCreateEngineLogger() {
    pEngineLogger.TakeOver(new deLoggerChain);
    pEngineLogger->AddLogger(deLogger::Ref::New(new deLoggerConsole));

    decPath diskPath(decPath::CreatePathNative(pLauncher->GetPathLogs()));
    diskPath.AddUnixPath(pGame->GetLogFile());

    decPath filePath;
    filePath.AddComponent(diskPath.GetLastComponent());

    diskPath.RemoveLastComponent();

    const deVFSDiskDirectory::Ref diskDir(deVFSDiskDirectory::Ref::New(
            new deVFSDiskDirectory(diskPath)));

    pEngineLogger->AddLogger(deLogger::Ref::New(
            new deLoggerFile(decBaseFileWriter::Ref::New(
                    diskDir->OpenFileForWriting(filePath)))));
}


extern "C"
JNIEXPORT jlong JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_RunGameHandler_createHandler(
JNIEnv *env, jobject thiz, jlong plauncher, jlong pgame,
jobject pparams, jobject plistener) {
    JniHelpers h(env);
    try {
        return (jlong)(intptr_t)(new RunGameHandler(env,
            (Launcher*)(intptr_t)plauncher,
            (delGame*)(intptr_t)pgame,
            GameRunParams(env).FromNative(pparams),
            plistener));
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

extern "C"
JNIEXPORT jint JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_RunGameHandler_getState(
JNIEnv *env, jobject thiz, jlong phandler){
    return (jint)(((RunGameHandler*)(intptr_t)phandler)->GetState());
}

extern "C"
JNIEXPORT void JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_RunGameHandler_waitForState(
JNIEnv *env, jobject thiz, jlong phandler, jint pstate){
    RunGameHandler &handler = *((RunGameHandler*)(intptr_t)phandler);
    const auto state = (RunGameHandler::State)pstate;

    while(handler.GetState() != state){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_RunGameHandler_stopGame(
JNIEnv *env, jobject thiz, jlong phandler){
    ((RunGameHandler*)(intptr_t)phandler)->RequestStopGame();
}
