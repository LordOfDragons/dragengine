#ifndef DELAUNCHER_REMOTELAUNCHERHANDLER_H
#define DELAUNCHER_REMOTELAUNCHERHANDLER_H

#include "RunGameHandler.h"

class RemoteLauncherClient;

class RemoteLauncherHandler : public RunGameHandler{
private:
    RemoteLauncherClient *pClient;

public:
    RemoteLauncherHandler(JNIEnv *env, RemoteLauncherClient *client, Launcher *launcher,
        delGame *game, const delGameRunParams &params, jobject objListener);
    ~RemoteLauncherHandler() override;

    void GameExited(BaseGameActivityAdapter &adapter) override;

protected:
    void pInitGameForRun() override;
    void pCreateEngineLogger() override;
};

#endif //DELAUNCHER_REMOTELAUNCHERHANDLER_H
