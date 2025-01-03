#ifndef DELAUNCHER_GAMERUNPARAMS_H
#define DELAUNCHER_GAMERUNPARAMS_H

#include "JniHelper.h"

class delGameRunParams;

class GameRunParams {
    JNIEnv *pEnv;

    JniClass pClsParam;
    const JniFieldPointer pFldGameProfile;
    const JniFieldString pFldRunArguments;
    const JniFieldInt pFldWidth;
    const JniFieldInt pFldHeight;
    //const JniFieldObjectArray pFldPatches;

public:
    explicit GameRunParams(JNIEnv *env);

    delGameRunParams FromNative(jobject objParams);
};

#endif //DELAUNCHER_GAMERUNPARAMS_H
