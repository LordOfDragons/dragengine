#ifndef DELAUNCHER_REMOTELAUNCHERHANDLER_H
#define DELAUNCHER_REMOTELAUNCHERHANDLER_H

#include "RunGameHandler.h"

class RemoteLauncherHandler : public RunGameHandler{
public:
    RemoteLauncherHandler(Launcher *launcher, delGame *game, const delGameRunParams &params);

    void StartGame(BaseGameActivityAdapter &adapter) override;
    void GameExited(BaseGameActivityAdapter &adapter) override;
};

#endif //DELAUNCHER_REMOTELAUNCHERHANDLER_H
