#ifndef DELAUNCHER_REMOTELAUNCHERCLIENT_H
#define DELAUNCHER_REMOTELAUNCHERCLIENT_H

#include <jni.h>
#include <deremotelauncher/derlLauncherClient.h>
#include <dragengine/logger/deLogger.h>
#include "JniHelper.h"

class delLauncher;

class RemoteLauncherClient : public derlLauncherClient{
private:
    class ClientLogger : public denLogger{
    private:
        RemoteLauncherClient &pOwner;

    public:
        explicit ClientLogger(RemoteLauncherClient &owner);
        void Log(LogSeverity severity, const std::string &message) override;
    };

    class EngineLogger : public deLogger{
    private:
        RemoteLauncherClient &pOwner;
        denLogger &pClientLogger;

    public:
        EngineLogger(RemoteLauncherClient &owner, denLogger &clientLogger);

        void LogInfo(const char *source, const char *message) override;
        void LogWarn(const char *source, const char *message) override;
        void LogError(const char *source, const char *message) override;
    };

    ClientLogger::Ref pLogger;
    JniObject pObjListener;
    JniObjectClass pClsListener;
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


public:
    RemoteLauncherClient(JNIEnv *env, jobject objListener);
    ~RemoteLauncherClient() override;

    inline const deLogger::Ref &GetEngineLogger() const{ return pEngineLogger; }
    void SetLauncher(delLauncher *launcher);
    void AddLogs(denLogger::LogSeverity severity, const std::string &logs);
    bool IsDisconnected();
    void ConnectToHost(const char *name, const char *pathDataDir, const char *address);
    void DisconnectFromHost();
    std::unique_ptr<std::string> GetSystemProperty(const std::string &property) override;
    void SendSystemProperty(const std::string &property, const std::string &value);
    void pFrameUpdate();
    void StartApplication(const derlRunParameters &params) override;
    void StopApplication() override;
    void KillApplication() override;
    void SetRunStatus(RunStatus status);
    void SendLog(denLogger::LogSeverity severity, const std::string &source, const std::string &log);
    void OnConnectionEstablished() override;
    void OnConnectionFailed(denConnection::ConnectionFailedReason reason) override;
    void OnConnectionClosed() override;

private:
    void pNotifyUpdateUIState();
};

#endif //DELAUNCHER_REMOTELAUNCHERCLIENT_H
