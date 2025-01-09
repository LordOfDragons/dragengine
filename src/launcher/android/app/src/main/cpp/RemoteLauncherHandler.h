#ifndef DELAUNCHER_REMOTELAUNCHERHANDLER_H
#define DELAUNCHER_REMOTELAUNCHERHANDLER_H

#include "RunGameHandler.h"

class RemoteLauncherClient;

class RemoteLauncherHandler : public RunGameHandler{
private:
    RemoteLauncherClient *pClient;
    deLoggerChain::Ref pEngineLogger;

    JniObject pObjListener;
    JniObjectClass pClsListener;
    jmethodID pMetListenerStateChanged;

public:
    RemoteLauncherHandler(JNIEnv *env, RemoteLauncherClient *client, Launcher *launcher,
        delGame *game, const delGameRunParams &params, jobject objListener);
    ~RemoteLauncherHandler() override;

    void GameExited(BaseGameActivityAdapter &adapter) override;

protected:
    void pInitEngineInstanceFactory(delEngineInstanceDirect::Factory &factory) override;
    void pInitGameForRun() override;
    void pCreateEngineLogger();
    void pStateChanged() override;
};

#endif //DELAUNCHER_REMOTELAUNCHERHANDLER_H
