#ifndef DELAUNCHER_GAMEACTIVITYHANDLER_H
#define DELAUNCHER_GAMEACTIVITYHANDLER_H

#include <game-activity/native_app_glue/android_native_app_glue.h>

#include "CustomEvent.h"

class BaseGameActivityAdapter{
protected:
    BaseGameActivityAdapter() = default;
    virtual ~BaseGameActivityAdapter() = default;

public:
    virtual ANativeWindow *GetNativeWindow() = 0;
    virtual const ARect &GetContentRect() = 0;
    virtual void FinishActivity() = 0;
    virtual void EnableFullScreenMode(bool enable) = 0;
    virtual void GetSavedState(size_t &size, void *&data) = 0;
};

class GameActivityHandler{
public:
    GameActivityHandler() = default;
    virtual ~GameActivityHandler() = default;

    virtual void Command(BaseGameActivityAdapter &adapter, int32_t cmd) = 0;
    virtual void FrameUpdate(BaseGameActivityAdapter &adapter) = 0;
    virtual void InputEvent(BaseGameActivityAdapter &adapter, const android_input_buffer &inputBuffer) = 0;
    virtual void ProcessCustomEvent(BaseGameActivityAdapter &adapter, const CustomEvent &event) = 0;
};

#endif //DELAUNCHER_GAMEACTIVITYHANDLER_H
