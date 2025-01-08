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

static JavaVM *vJavaVM = nullptr;

jint JNI_OnLoad(JavaVM *vm, void *reserved){
    vJavaVM = vm;
    return JNI_VERSION_1_6;
}

class RemoteLauncherClient : public derlLauncherClient{
private:
    class ClientLogger : public denLogger{
    private:
        RemoteLauncherClient &pOwner;

    public:
        explicit ClientLogger(RemoteLauncherClient &owner) : pOwner(owner){}
        void Log(LogSeverity severity, const std::string &message) override{
            pOwner.AddLogs(severity, message);
        }
    };

    class EngineLogger : public deLogger{
    private:
        RemoteLauncherClient &pOwner;
        denLogger &pClientLogger;

    public:
        EngineLogger(RemoteLauncherClient &owner, denLogger &clientLogger) :
        pOwner(owner), pClientLogger(clientLogger){}

        void LogInfo(const char *source, const char *message) override{
            std::stringstream ss;
            ss << "[" << source << "] " << message;
            pClientLogger.Log(denLogger::LogSeverity::info, ss.str());

            pOwner.SendLog(denLogger::LogSeverity::info, source, message);
        }

        void LogWarn(const char *source, const char *message) override{
            std::stringstream ss;
            ss << "[" << source << "] " << message;
            pClientLogger.Log(denLogger::LogSeverity::warning, ss.str());

            pOwner.SendLog(denLogger::LogSeverity::warning, source, message);
        }

        void LogError( const char *source, const char *message ) override{
            std::stringstream ss;
            ss << "[" << source << "] " << message;
            pClientLogger.Log(denLogger::LogSeverity::error, ss.str());

            pOwner.SendLog(denLogger::LogSeverity::error, source, message);
        }
    };

    ClientLogger::Ref pLogger;
    jobject pObjListener;
    jclass pClsListener;
    jmethodID pMetListenerAddLogs;
    jmethodID pMetListenerUpdateUIState;
    jmethodID pMetListenerRequestProfileNames;
    jmethodID pMetListenerRequestDefaultProfileName;
    jmethodID pMetListenerStartApplication;
    jmethodID pMetListenerStopApplication;
    jmethodID pMetListenerKillApplication;

    JniClass pClsRunParameters;
    const JniFieldString pFldRunParametersGameConfig;
    const JniFieldString pFldRunParametersProfileName;
    const JniFieldString pFldRunParametersArguments;

    std::chrono::steady_clock::time_point pLastTime;

    std::shared_ptr<std::thread> pThreadUpdater;
    std::atomic<bool> pExitUpdaterThread = false;
    std::mutex pMutexClient;

    delLauncher *pLauncher;
    deLogger::Ref pEngineLogger;

    bool pIsRunning;


public:
    RemoteLauncherClient(JNIEnv *env, jobject objListener) :
    pObjListener(env->NewGlobalRef(objListener)),
    pClsListener(env->GetObjectClass(objListener)),
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
    pLauncher(nullptr),
    pIsRunning(false)
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

    ~RemoteLauncherClient() override{
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

        if(pObjListener){
            env->DeleteGlobalRef(pObjListener);
        }
        pClsRunParameters.Dispose(env);
    }

    void SetLauncher(delLauncher *launcher){
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

    inline bool IsRunning() const{
        return pIsRunning;
    }

    void AddLogs(denLogger::LogSeverity severity, const std::string &logs) {
        JNIEnv *env = nullptr;
        vJavaVM->AttachCurrentThread(&env, nullptr);
        jstring jlogs = JniHelpers(env).convertString(logs.c_str());
        env->CallVoidMethod(pObjListener, pMetListenerAddLogs, (jint)severity, jlogs);
        env->DeleteLocalRef(jlogs);
    }

    bool IsDisconnected(){
        const std::lock_guard guard(pMutexClient);
        return GetConnectionState() == denConnection::ConnectionState::disconnected;
    }

    void ConnectToHost(const char *name, const char *pathDataDir, const char *address){
        const std::lock_guard guard(pMutexClient);
        if(GetConnectionState() != denConnection::ConnectionState::disconnected){
            return;
        }

        SetName(name);
        SetPathDataDir(pathDataDir);

        ConnectTo(address);
    }

    void DisconnectFromHost(){
        const std::lock_guard guard(pMutexClient);
        if(GetConnectionState() != denConnection::ConnectionState::disconnected){
            Disconnect();
        }
    }

    std::unique_ptr<std::string> GetSystemProperty(const std::string &property) override{
        if(property == derlProtocol::SystemPropertyNames::propertyNames){
            std::stringstream names;
            names << derlProtocol::SystemPropertyNames::profileNames << "\n";
            names << derlProtocol::SystemPropertyNames::defaultProfile;
            return std::make_unique<std::string>(names.str());

        }else if(property == derlProtocol::SystemPropertyNames::profileNames){
            JNIEnv *env = nullptr;
            vJavaVM->AttachCurrentThread(&env, nullptr);
            env->CallVoidMethod(pObjListener, pMetListenerRequestProfileNames);
            return nullptr;

        }else if(property == derlProtocol::SystemPropertyNames::defaultProfile){
            JNIEnv *env = nullptr;
            vJavaVM->AttachCurrentThread(&env, nullptr);
            env->CallVoidMethod(pObjListener, pMetListenerRequestDefaultProfileName);
            return nullptr;

        }else{
            return std::make_unique<std::string>();
        }
    }

    void SendSystemProperty(const std::string &property, const std::string &value){
        const std::lock_guard guard(pMutexClient);
        derlLauncherClient::SendSystemProperty(property, value);
    }

    void pFrameUpdate(){
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

    void StartApplication(const derlRunParameters &params) override{
        JNIEnv *env = nullptr;
        vJavaVM->AttachCurrentThread(&env, nullptr);

        const JniObject objParams(pClsRunParameters.New(env));
        pFldRunParametersGameConfig.Set(env, objParams, params.GetGameConfig().c_str());
        pFldRunParametersProfileName.Set(env, objParams, params.GetProfileName().c_str());
        pFldRunParametersArguments.Set(env, objParams, params.GetArguments().c_str());
        env->CallVoidMethod(pObjListener, pMetListenerStartApplication, objParams.CallArgument());
    }

    void StopApplication() override{
        JNIEnv *env = nullptr;
        vJavaVM->AttachCurrentThread(&env, nullptr);
        env->CallVoidMethod(pObjListener, pMetListenerStopApplication);
    }

    void KillApplication() override{
        JNIEnv *env = nullptr;
        vJavaVM->AttachCurrentThread(&env, nullptr);
        env->CallVoidMethod(pObjListener, pMetListenerKillApplication);
    }

    void SetRunStatus(RunStatus status){
        derlLauncherClient::SetRunStatus(status);
    }

    void SendLog(denLogger::LogSeverity severity, const std::string &source, const std::string &log){
        const std::lock_guard guard(pMutexClient);
        derlLauncherClient::SendLog(severity, source, log);
    }

    void OnConnectionEstablished() override{
        pNotifyUpdateUIState();
    }

    void OnConnectionFailed(denConnection::ConnectionFailedReason reason) override{
        pNotifyUpdateUIState();
    }

    void OnConnectionClosed() override{
        pNotifyUpdateUIState();
    }

private:
    void pNotifyUpdateUIState(){
        JNIEnv *env = nullptr;
        vJavaVM->AttachCurrentThread(&env, nullptr);
        env->CallVoidMethod(pObjListener, pMetListenerUpdateUIState);
    }
};


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
JNIEXPORT jboolean JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_RemoteLauncherClient_isRunning(
JNIEnv *env, jobject thiz, jlong pclient){
    JniHelpers h(env);
    try {
        return ((RemoteLauncherClient*)pclient)->IsRunning();
    }catch(const deException &e){
        h.throwException(e);
        return false; // keep compiler happy. code never gets here
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

