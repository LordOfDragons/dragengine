#include "GameRunParams.h"
#include <delauncher/game/delGameRunParams.h>

// GameRunParams
//////////////////

GameRunParams::GameRunParams(JNIEnv *env) :
pEnv(env),

pClsParam(env, JPATH_BASE "GameRunParams"),
pFldGameProfile(pClsParam.GetFieldPointer("gameProfile")),
pFldRunArguments(pClsParam.GetFieldString("runArguments")),
pFldWidth(pClsParam.GetFieldInt("width")),
pFldHeight(pClsParam.GetFieldInt("height"))
//pFldPatches(pClsParam.GetFieldObjectArray("patches", "]Ljava/lang/String;"))
{
}

delGameRunParams GameRunParams::FromNative(jobject objParams) {
    delGameRunParams params;
    params.SetGameProfile((delGameProfile*)pFldGameProfile.Get(objParams));
    params.SetRunArguments(pFldRunArguments.Get(objParams));
    params.SetWidth(pFldWidth.Get(objParams));
    params.SetHeight(pFldHeight.Get(objParams));
    params.SetFullScreen(true);
    /*
    const JniObjectArray objPatches(env, pFldPatches.Get(objParams));
    const int patchCount = objPatches.GetCount();
    int i;
    for(i=0; i<patchCount; i++){
        params.GetPatches().Add()
    }
    */
    return params;
}
