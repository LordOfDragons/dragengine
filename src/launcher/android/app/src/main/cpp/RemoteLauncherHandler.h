#ifndef DELAUNCHER_REMOTELAUNCHERHANDLER_H
#define DELAUNCHER_REMOTELAUNCHERHANDLER_H

#include "RunGameHandler.h"

class RemoteLauncherClient;

class RemoteLauncherHandler : public RunGameHandler{
private:
    RemoteLauncherClient *pClient;

public:
    RemoteLauncherHandler(RemoteLauncherClient *client, Launcher *launcher,
        delGame *game, const delGameRunParams &params);

    void GameExited(BaseGameActivityAdapter &adapter) override;

protected:
    void pStateChanged() override;
    void pInitEngineInstanceFactory(delEngineInstanceDirect::Factory &factory) override;
    void pInitGameForRun() override;
};

#endif //DELAUNCHER_REMOTELAUNCHERHANDLER_H
