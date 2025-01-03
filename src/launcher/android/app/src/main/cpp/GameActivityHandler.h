#ifndef DELAUNCHER_GAMEACTIVITYHANDLER_H
#define DELAUNCHER_GAMEACTIVITYHANDLER_H

#include <game-activity/native_app_glue/android_native_app_glue.h>

class BaseGameActivityAdapter{
protected:
    BaseGameActivityAdapter() = default;
    virtual ~BaseGameActivityAdapter() = default;

public:
    virtual ANativeWindow *GetNativeWindow() = 0;
    virtual void QuitActivity() = 0;
};

class GameActivityHandler{
public:
    GameActivityHandler() = default;
    virtual ~GameActivityHandler() = default;

    virtual void Command(BaseGameActivityAdapter &adapter, int32_t cmd) = 0;
    virtual void FrameUpdate(BaseGameActivityAdapter &adapter) = 0;
};

#endif //DELAUNCHER_GAMEACTIVITYHANDLER_H
