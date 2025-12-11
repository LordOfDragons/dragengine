#include "GameActivityHandler.h"
#include "GameRunParams.h"
#include "Launcher.h"
#include <android/log.h>
#include <delauncher/engine/delEngineInstanceDirect.h>
#include <delauncher/game/delGameRunParams.h>
#include <delauncher/game/delGame.h>
#include <deremotelauncher/derlLauncherClient.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/logger/deLoggerFile.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include "RemoteLauncherHandler.h"
#include "RemoteLauncherClient.h"

extern JavaVM *vJavaVM;

RemoteLauncherHandler::RemoteLauncherHandler(JNIEnv *env, RemoteLauncherClient *client,
    Launcher *launcher, delGame *game, const delGameRunParams &params, jobject objListener) :
RunGameHandler(env, launcher, game, params, objListener),
pClient(client){
}

RemoteLauncherHandler::~RemoteLauncherHandler() = default;

void RemoteLauncherHandler::GameExited(BaseGameActivityAdapter &adapter) {
}

void RemoteLauncherHandler::pInitGameForRun() {
    pGame->SetVFSDelgaContainer(nullptr);
    pGame->SetDelgaFile("");
}

void RemoteLauncherHandler::pCreateEngineLogger(){
    pEngineLogger.TakeOver(new deLoggerChain);
    pEngineLogger->AddLogger(pClient->GetEngineLogger());

    decPath diskPath(decPath::CreatePathNative(pLauncher->GetPathLogs()));
    diskPath.AddUnixPath(pGame->GetLogFile());

    decPath filePath;
    filePath.AddComponent(diskPath.GetLastComponent());

    diskPath.RemoveLastComponent();

    const deVFSDiskDirectory::Ref diskDir(deVFSDiskDirectory::Ref::NewWith(diskPath));

    pEngineLogger->AddLogger(deLoggerFile::Ref::NewWith(diskDir->OpenFileForWriting(filePath)));
}


extern "C"
JNIEXPORT jlong JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_RemoteLauncherHandler_createHandler(
JNIEnv *env, jobject thiz, jlong pclient, jlong plauncher,
jlong pgame, jobject pparams, jobject plistener) {
    JniHelpers h(env);
    try {
        return (jlong)(intptr_t)(new RemoteLauncherHandler(env,
                (RemoteLauncherClient*)(intptr_t)pclient,
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

extern "C"
JNIEXPORT void JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_RemoteLauncherHandler_waitForState(
JNIEnv *env, jobject thiz, jlong phandler, jint pstate){
    RemoteLauncherHandler &handler = *((RemoteLauncherHandler*)(intptr_t)phandler);
    const auto state = (RunGameHandler::State)pstate;

    while(handler.GetState() != state){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_RemoteLauncherHandler_stopGame(
JNIEnv *env, jobject thiz, jlong phandler){
    ((RemoteLauncherHandler*)(intptr_t)phandler)->RequestStopGame();
}
