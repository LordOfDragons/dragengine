#ifndef DELAUNCHER_GAMERUNPARAMS_H
#define DELAUNCHER_GAMERUNPARAMS_H

#include "JniHelper.h"

class delGameRunParams;

class GameRunParams {
    JNIEnv *pEnv;

    JniClass pClsParam;
    JniClass pClsGameProfile;
    const JniFieldObject pFldGameProfile;
    const JniFieldString pFldRunArguments;
    //const JniFieldObjectArray pFldPatches;

public:
    explicit GameRunParams(JNIEnv *env);

    delGameRunParams FromNative(jobject objParams);
};

#endif //DELAUNCHER_GAMERUNPARAMS_H
