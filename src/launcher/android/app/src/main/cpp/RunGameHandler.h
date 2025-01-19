#ifndef DELAUNCHER_RUNGAMEHANDLER_H
#define DELAUNCHER_RUNGAMEHANDLER_H

#include <mutex>
#include "GameActivityHandler.h"
#include <delauncher/game/delGame.h>
#include <delauncher/game/delGameRunParams.h>
#include <dragengine/common/file/decMemoryFile.h>

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
    deLoggerChain::Ref pEngineLogger;

    decMemoryFile::Ref pSavedState;

    void pSetState(State state);

    JniObject pObjListener;
    JniObjectClass pClsListener;
    jmethodID pMetListenerStateChanged;

private:
    std::atomic<State> pState;
    std::atomic<bool> pRequestStopGame;

public:
    RunGameHandler(JNIEnv *env, Launcher *launcher, delGame *game,
       const delGameRunParams &params, jobject objListener);
    ~RunGameHandler() override;

    inline State GetState() const{ return pState; }

    void RequestStopGame();

    void Command(BaseGameActivityAdapter &adapter, int32_t cmd) override;
    void FrameUpdate(BaseGameActivityAdapter &adapter) override;
    void InputEvent(BaseGameActivityAdapter &adapter, const android_input_buffer &inputBuffer) override;
    void ProcessCustomEvent(BaseGameActivityAdapter &adapter, const CustomEvent &event) override;

    virtual void StartGame(BaseGameActivityAdapter &adapter);
    virtual void ProcessRunning(BaseGameActivityAdapter &adapter);
    virtual void StopGame(BaseGameActivityAdapter &adapter);
    virtual void GameExited(BaseGameActivityAdapter &adapter);

protected:
    virtual void pStateChanged();
    virtual void pInitEngineInstanceFactory(delEngineInstanceDirect::Factory &factory);
    virtual void pInitGameForRun();
    virtual void pCreateEngineLogger();
};

#endif //DELAUNCHER_RUNGAMEHANDLER_H
