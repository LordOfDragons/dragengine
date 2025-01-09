#ifndef DELAUNCHER_RUNGAMEHANDLER_H
#define DELAUNCHER_RUNGAMEHANDLER_H

#include "GameActivityHandler.h"
#include <delauncher/game/delGame.h>
#include <delauncher/game/delGameRunParams.h>

class Launcher;

class RunGameHandler : public GameActivityHandler{
public:
    enum class State{
        startGame,
        gameRunning,
        gameStopped
    };

protected:
    Launcher *pLauncher;
    const delGame::Ref pGame;
    delGameRunParams pRunParams;

private:
    State pState;

public:
    RunGameHandler(Launcher *launcher, delGame *game, const delGameRunParams &params);

    inline State GetState() const{ return pState; }
    void SetState(State state);

    void Command(BaseGameActivityAdapter &adapter, int32_t cmd) override;
    void FrameUpdate(BaseGameActivityAdapter &adapter) override;

    virtual void StartGame(BaseGameActivityAdapter &adapter);
    virtual void ProcessRunning(BaseGameActivityAdapter &adapter);
    virtual void GameExited(BaseGameActivityAdapter &adapter);

protected:
    virtual void pStateChanged();
    virtual void pInitEngineInstanceFactory(delEngineInstanceDirect::Factory &factory);
    virtual void pInitGameForRun();
};

#endif //DELAUNCHER_RUNGAMEHANDLER_H
