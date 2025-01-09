#include <chrono>
#include <mutex>
#include <thread>
#include <atomic>
#include <jni.h>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <deremotelauncher/derlLauncherClient.h>
#include <deremotelauncher/derlProtocol.h>
#include <denetwork/denLogger.h>
#include <delauncher/delLauncher.h>
#include <dragengine/logger/deLogger.h>
#include <android/log.h>
#include "JniHelper.h"
#include "RemoteLauncherClient.h"

static JavaVM *vJavaVM = nullptr;

jint JNI_OnLoad(JavaVM *vm, void *reserved){
    vJavaVM = vm;
    return JNI_VERSION_1_6;
}

// Class RemoteLauncherClient::ClientLogger
/////////////////////////////////////////////

RemoteLauncherClient::ClientLogger::ClientLogger(RemoteLauncherClient &owner) :
pOwner(owner){
}

void RemoteLauncherClient::ClientLogger::Log(LogSeverity severity, const std::string &message){
    pOwner.AddLogs(severity, message);
}


// Class RemoteLauncherClient::EngineLogger
/////////////////////////////////////////////

RemoteLauncherClient::EngineLogger::EngineLogger(RemoteLauncherClient &owner, denLogger &clientLogger) :
pOwner(owner),
pClientLogger(clientLogger){
}

void RemoteLauncherClient::EngineLogger::LogInfo(const char *source, const char *message){
    std::stringstream ss;
    ss << "[" << source << "] " << message;
    pClientLogger.Log(denLogger::LogSeverity::info, ss.str());

    pOwner.SendLog(denLogger::LogSeverity::info, source, message);
}

void RemoteLauncherClient::EngineLogger::LogWarn(const char *source, const char *message){
    std::stringstream ss;
    ss << "[" << source << "] " << message;
    pClientLogger.Log(denLogger::LogSeverity::warning, ss.str());

    pOwner.SendLog(denLogger::LogSeverity::warning, source, message);
}

void RemoteLauncherClient::EngineLogger::LogError( const char *source, const char *message ){
    std::stringstream ss;
    ss << "[" << source << "] " << message;
    pClientLogger.Log(denLogger::LogSeverity::error, ss.str());

    pOwner.SendLog(denLogger::LogSeverity::error, source, message);
}


// Class RemoteLauncherClient
///////////////////////////////

RemoteLauncherClient::RemoteLauncherClient(JNIEnv *env, jobject objListener) :
pObjListener(env, objListener, true),
pClsListener(env, objListener, true),
pMetListenerAddLogs(env->GetMethodID(pClsListener, "addLogs", "(ILjava/lang/String;)V")),
pMetListenerUpdateUIState(env->GetMethodID(pClsListener, "updateUIState", "()V")),
pMetListenerRequestProfileNames(env->GetMethodID(pClsListener, "requestProfileNames", "()V")),
pMetListenerRequestDefaultProfileName(env->GetMethodID(pClsListener, "requestDefaultProfileName", "()V")),
pMetListenerStartApplication(env->GetMethodID(pClsListener, "startApplication",
      "(L" JPATH_BASE "RemoteLauncherClientRunParameters;)V")),
pMetListenerStopApplication(env->GetMethodID(pClsListener, "stopApplication", "()V")),
pMetListenerKillApplication(env->GetMethodID(pClsListener, "killApplication", "()V")),

pClsRunParameters(env, JPATH_BASE "RemoteLauncherClientRunParameters", true),
pFldRunParametersGameConfig(pClsRunParameters.GetFieldString("gameConfig")),
pFldRunParametersProfileName(pClsRunParameters.GetFieldString("profileName")),
pFldRunParametersArguments(pClsRunParameters.GetFieldString("arguments")),

pLastTime(std::chrono::steady_clock::now()),
pLauncher(nullptr)
{
    DEASSERT_NOTNULL(pMetListenerAddLogs)

    pLogger = std::make_shared<ClientLogger>(*this);
    SetLogger(pLogger);

    pEngineLogger.TakeOver(new EngineLogger(*this, *pLogger));

    pThreadUpdater = std::make_shared<std::thread>([&](){
        while(!pExitUpdaterThread){
            pFrameUpdate();
        }
    });
}

RemoteLauncherClient::~RemoteLauncherClient(){
    if(pThreadUpdater){
        pExitUpdaterThread = true;
        pThreadUpdater->join();
    }

    StopTaskProcessors();

    if(pEngineLogger && pLauncher && pLauncher->GetLogger()){
        pLauncher->GetLogger()->RemoveLogger(pEngineLogger);
        pEngineLogger = nullptr;
    }

    JNIEnv *env = nullptr;
    vJavaVM->AttachCurrentThread(&env, nullptr);

    pObjListener.Dispose(env);
    pClsListener.Dispose(env);
    pClsRunParameters.Dispose(env);
}

void RemoteLauncherClient::SetLauncher(delLauncher *launcher){
    if(launcher == pLauncher){
        return;
    }

    if(pLauncher){
        pLauncher->GetLogger()->RemoveLogger(pEngineLogger);
    }

    pLauncher = launcher;

    if(launcher){
        launcher->GetLogger()->AddLogger(pEngineLogger);
    }
}

void RemoteLauncherClient::AddLogs(denLogger::LogSeverity severity, const std::string &logs){
    if(!pObjListener){
        return;
    }

    JNIEnv *env = nullptr;
    vJavaVM->AttachCurrentThread(&env, nullptr);
    jstring jlogs = JniHelpers(env).convertString(logs.c_str());
    env->CallVoidMethod(pObjListener, pMetListenerAddLogs, (jint)severity, jlogs);
    env->DeleteLocalRef(jlogs);
}

bool RemoteLauncherClient::IsDisconnected(){
    const std::lock_guard guard(pMutexClient);
    return GetConnectionState() == denConnection::ConnectionState::disconnected;
}

void RemoteLauncherClient::ConnectToHost(const char *name, const char *pathDataDir, const char *address){
    const std::lock_guard guard(pMutexClient);
    if(GetConnectionState() != denConnection::ConnectionState::disconnected){
        return;
    }

    SetName(name);
    SetPathDataDir(pathDataDir);

    ConnectTo(address);
}

void RemoteLauncherClient::DisconnectFromHost(){
    const std::lock_guard guard(pMutexClient);
    if(GetConnectionState() != denConnection::ConnectionState::disconnected){
        Disconnect();
    }
}

std::unique_ptr<std::string> RemoteLauncherClient::GetSystemProperty(const std::string &property){
    if(property == derlProtocol::SystemPropertyNames::propertyNames){
        std::stringstream names;
        names << derlProtocol::SystemPropertyNames::profileNames << "\n";
        names << derlProtocol::SystemPropertyNames::defaultProfile;
        return std::make_unique<std::string>(names.str());

    }else if(property == derlProtocol::SystemPropertyNames::profileNames){
        if(pObjListener) {
            JNIEnv *env = nullptr;
            vJavaVM->AttachCurrentThread(&env, nullptr);
            env->CallVoidMethod(pObjListener, pMetListenerRequestProfileNames);
        }
        return nullptr;

    }else if(property == derlProtocol::SystemPropertyNames::defaultProfile){
        if(pObjListener) {
            JNIEnv *env = nullptr;
            vJavaVM->AttachCurrentThread(&env, nullptr);
            env->CallVoidMethod(pObjListener, pMetListenerRequestDefaultProfileName);
        }
        return nullptr;

    }else{
        return std::make_unique<std::string>();
    }
}

void RemoteLauncherClient::SendSystemProperty(const std::string &property, const std::string &value){
    const std::lock_guard guard(pMutexClient);
    derlLauncherClient::SendSystemProperty(property, value);
}

void RemoteLauncherClient::pFrameUpdate(){
    {
        const std::lock_guard guard(pMutexClient);

        const std::chrono::steady_clock::time_point now(std::chrono::steady_clock::now());
        const int64_t elapsed_us = std::chrono::duration_cast<std::chrono::microseconds>(now - pLastTime).count();
        const float elapsed = (float)elapsed_us / 1e6f;
        pLastTime = now;

        Update(elapsed);
    }

    std::this_thread::yield();
    //std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

void RemoteLauncherClient::StartApplication(const derlRunParameters &params){
    if(!pObjListener){
        return;
    }

    JNIEnv *env = nullptr;
    vJavaVM->AttachCurrentThread(&env, nullptr);

    const JniObject objParams(pClsRunParameters.New(env));
    pFldRunParametersGameConfig.Set(env, objParams, params.GetGameConfig().c_str());
    pFldRunParametersProfileName.Set(env, objParams, params.GetProfileName().c_str());
    pFldRunParametersArguments.Set(env, objParams, params.GetArguments().c_str());
    env->CallVoidMethod(pObjListener, pMetListenerStartApplication, objParams.CallArgument());
}

void RemoteLauncherClient::StopApplication(){
    if(!pObjListener){
        return;
    }

    JNIEnv *env = nullptr;
    vJavaVM->AttachCurrentThread(&env, nullptr);
    env->CallVoidMethod(pObjListener, pMetListenerStopApplication);
}

void RemoteLauncherClient::KillApplication(){
    if(!pObjListener){
        return;
    }

    JNIEnv *env = nullptr;
    vJavaVM->AttachCurrentThread(&env, nullptr);
    env->CallVoidMethod(pObjListener, pMetListenerKillApplication);
}

void RemoteLauncherClient::SetRunStatus(RunStatus status){
    if(status == GetRunStatus()){
        return;
    }

    derlLauncherClient::SetRunStatus(status);
    pNotifyUpdateUIState();
}

void RemoteLauncherClient::SendLog(denLogger::LogSeverity severity, const std::string &source, const std::string &log){
    const std::lock_guard guard(pMutexClient);
    derlLauncherClient::SendLog(severity, source, log);
}

void RemoteLauncherClient::OnConnectionEstablished(){
    pNotifyUpdateUIState();
}

void RemoteLauncherClient::OnConnectionFailed(denConnection::ConnectionFailedReason reason){
    pNotifyUpdateUIState();
}

void RemoteLauncherClient::OnConnectionClosed(){
    pNotifyUpdateUIState();
}

void RemoteLauncherClient::pNotifyUpdateUIState(){
    if(!pObjListener){
        return;
    }

    JNIEnv *env = nullptr;
    vJavaVM->AttachCurrentThread(&env, nullptr);
    env->CallVoidMethod(pObjListener, pMetListenerUpdateUIState);
}


// JNI Bridge
///////////////

extern "C"
JNIEXPORT jlong JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_RemoteLauncherClient_createClient(
JNIEnv *env, jobject thiz, jobject objListener){
    JniHelpers h(env);
    try {
        return (jlong)(intptr_t)(new RemoteLauncherClient(env, objListener));
    }catch(const deException &e){
        h.throwException(e);
        return 0L; // keep compiler happy. code never gets here
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_RemoteLauncherClient_destroyClient(
JNIEnv *env, jobject thiz, jlong pclient){
    delete (RemoteLauncherClient*)(intptr_t)pclient;
}

extern "C"
JNIEXPORT void JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_RemoteLauncherClient_connectToHost(
JNIEnv *env, jobject thiz, jlong pclient,
jstring pname, jstring ppathDataDir, jstring paddress){
    JniHelpers h(env);
    try {
        RemoteLauncherClient &client = *((RemoteLauncherClient*)pclient);
        const decString name(h.convertString(pname));
        const decString pathDataDir(h.convertString(ppathDataDir));
        const decString address(h.convertString(paddress));
        client.ConnectToHost(name, pathDataDir, address);
    }catch(const deException &e){
        h.throwException(e);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_RemoteLauncherClient_disconnectFromHost(
JNIEnv *env, jobject thiz, jlong pclient){
    JniHelpers h(env);
    try {
        ((RemoteLauncherClient*)pclient)->DisconnectFromHost();
    }catch(const deException &e){
        h.throwException(e);
    }
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_RemoteLauncherClient_isDisconnected(
JNIEnv *env, jobject thiz, jlong pclient){
    JniHelpers h(env);
    try {
        return ((RemoteLauncherClient*)pclient)->IsDisconnected();
    }catch(const deException &e){
        h.throwException(e);
        return false; // keep compiler happy. code never gets here
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_RemoteLauncherClient_setLauncher(
JNIEnv *env, jobject thiz, jlong pclient, jlong plauncher){
    JniHelpers h(env);
    try {
        ((RemoteLauncherClient*)pclient)->SetLauncher(((delLauncher*)plauncher));
    }catch(const deException &e){
        h.throwException(e);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_RemoteLauncherClient_sendSystemProperty(
JNIEnv *env, jobject thiz, jlong pclient, jstring pproperty, jstring pvalue){
    JniHelpers h(env);
    try {
        const std::string property(h.convertString(pproperty));
        const std::string value(h.convertString(pvalue));
        ((RemoteLauncherClient*)pclient)->SendSystemProperty(property, value);
    }catch(const deException &e){
        h.throwException(e);
    }
}

