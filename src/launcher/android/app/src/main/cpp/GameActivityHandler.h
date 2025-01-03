#ifndef DELAUNCHER_GAMEACTIVITYHANDLER_H
#define DELAUNCHER_GAMEACTIVITYHANDLER_H

#include <game-activity/native_app_glue/android_native_app_glue.h>

class GameActivityHandler{
public:
    GameActivityHandler() = default;
    virtual ~GameActivityHandler() = default;

    virtual void Activated(){}
    virtual void Command(android_app *app, int32_t cmd) = 0;
};

#endif //DELAUNCHER_GAMEACTIVITYHANDLER_H
