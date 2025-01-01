#include "GameProfile.h"

#include <delauncher/game/profile/delGameProfile.h>
#include <delauncher/game/profile/delGPDisableModuleVersion.h>
#include <delauncher/game/profile/delGPMParameter.h>
#include <delauncher/game/profile/delGPModule.h>

// GameProfileInfo
////////////////////

GameProfileInfo::GameProfileInfo(JNIEnv *env) :
pEnv(env),

pClsParameter(env, JPATH_BASE "GameProfileModuleParameter"),
pFldParamName(pClsParameter.GetFieldString("name")),
pFldParamValue(pClsParameter.GetFieldString("value")),

pClsModule(env, JPATH_BASE "GameProfileModule"),
pFldModuleName(pClsModule.GetFieldString("name")),
pFldModuleParameters(pClsModule.GetFieldObjectArray("parameters", pClsParameter)),

pClsModuleVersion(env, JPATH_BASE "GameProfileModuleVersion"),
pFldModVerName(pClsModuleVersion.GetFieldString("name")),
pFldModVerVersion(pClsModuleVersion.GetFieldString("version")),

pClsProfile(env, JPATH_BASE "GameProfileInfo"),
pFldProfileName(pClsProfile.GetFieldString("name")),

pFldProfileModuleGraphic(pClsProfile.GetFieldString("moduleGraphic")),
pFldProfileModuleInput(pClsProfile.GetFieldString("moduleInput")),
pFldProfileModulePhysics(pClsProfile.GetFieldString("modulePhysics")),
pFldProfileModuleAnimator(pClsProfile.GetFieldString("moduleAnimator")),
pFldProfileModuleAI(pClsProfile.GetFieldString("moduleAI")),
pFldProfileModuleCrashRecovery(pClsProfile.GetFieldString("moduleCrashRecovery")),
pFldProfileModuleAudio(pClsProfile.GetFieldString("moduleAudio")),
pFldProfileModuleSynthesizer(pClsProfile.GetFieldString("moduleSynthesizer")),
pFldProfileModuleNetwork(pClsProfile.GetFieldString("moduleNetwork")),
pFldProfileModuleVR(pClsProfile.GetFieldString("moduleVR")),

pFldProfileModuleGraphicVersion(pClsProfile.GetFieldString("moduleGraphicVersion")),
pFldProfileModuleInputVersion(pClsProfile.GetFieldString("moduleInputVersion")),
pFldProfileModulePhysicsVersion(pClsProfile.GetFieldString("modulePhysicsVersion")),
pFldProfileModuleAnimatorVersion(pClsProfile.GetFieldString("moduleAnimatorVersion")),
pFldProfileModuleAIVersion(pClsProfile.GetFieldString("moduleAIVersion")),
pFldProfileModuleCrashRecoveryVersion(pClsProfile.GetFieldString("moduleCrashRecoveryVersion")),
pFldProfileModuleAudioVersion(pClsProfile.GetFieldString("moduleAudioVersion")),
pFldProfileModuleSynthesizerVersion(pClsProfile.GetFieldString("moduleSynthesizerVersion")),
pFldProfileModuleNetworkVersion(pClsProfile.GetFieldString("moduleNetworkVersion")),
pFldProfileModuleVRVersion(pClsProfile.GetFieldString("moduleVRVersion")),

pFldProfileDisableModuleVersions(pClsProfile.GetFieldObjectArray("disableModuleVersions", pClsModuleVersion)),

pFldProfileModules(pClsProfile.GetFieldObjectArray("modules", pClsModule)),

pFldProfileRunArgs(pClsProfile.GetFieldString("runArgs")),
pFldProfileReplaceRunArgs(pClsProfile.GetFieldBool("replaceRunArgs")){
}

jobject GameProfileInfo::Convert(const delGameProfile &profile) {
    const delGPModuleList &modules = profile.GetModules();
    const int moduleCount = modules.GetCount();
    const JniObjectArray objModules(pEnv, pClsModule, moduleCount);
    int i;
    for(i=0; i<moduleCount; i++){
        const delGPModule &module = *modules.GetAt(i);
        const JniObject objModule(pClsModule.New());
        pFldModuleName.Set(objModule, module.GetName());

        const delGPMParameterList &params = module.GetParameters();
        const int paramCount = params.GetCount();
        const JniObjectArray objParams(pEnv, pClsParameter, paramCount);
        int j;
        for(j=0; j<paramCount; j++){
            const delGPMParameter &param = *params.GetAt(j);
            const JniObject objParam(pClsParameter.New());
            pFldParamName.Set(objParam, param.GetName());
            pFldParamValue.Set(objParam, param.GetValue());
            objParams.SetAt(j, objParam);
        }
        pFldModuleParameters.Set(objModule, objParams);

        objModules.SetAt(i, objModule);
    }

    const delGPDisableModuleVersionList &disModVers = profile.GetDisableModuleVersions();
    const int disModVerCount = disModVers.GetCount();
    const JniObjectArray objDisModVers(pEnv, pClsModuleVersion, disModVerCount);
    for(i=0; i<disModVerCount; i++){
        const delGPDisableModuleVersion &disModVer = *disModVers.GetAt(i);
        const JniObject objVer(pClsModuleVersion.New());
        pFldModVerName.Set(objVer, disModVer.GetName());
        pFldModVerVersion.Set(objVer, disModVer.GetVersion());
        objDisModVers.SetAt(i, objVer);
    }

    JniObject objProfile(pClsProfile.New());
    pFldProfileName.Set(objProfile, profile.GetName());
    pFldProfileModuleGraphic.Set(objProfile, profile.GetModuleGraphic());
    pFldProfileModuleInput.Set(objProfile, profile.GetModuleInput());
    pFldProfileModulePhysics.Set(objProfile, profile.GetModulePhysics());
    pFldProfileModuleAnimator.Set(objProfile, profile.GetModuleAnimator());
    pFldProfileModuleAI.Set(objProfile, profile.GetModuleAI());
    pFldProfileModuleCrashRecovery.Set(objProfile, profile.GetModuleCrashRecovery());
    pFldProfileModuleAudio.Set(objProfile, profile.GetModuleAudio());
    pFldProfileModuleSynthesizer.Set(objProfile, profile.GetModuleSynthesizer());
    pFldProfileModuleNetwork.Set(objProfile, profile.GetModuleNetwork());
    pFldProfileModuleVR.Set(objProfile, profile.GetModuleVR());

    pFldProfileModuleGraphicVersion.Set(objProfile, profile.GetModuleGraphicVersion());
    pFldProfileModuleInputVersion.Set(objProfile, profile.GetModuleInputVersion());
    pFldProfileModulePhysicsVersion.Set(objProfile, profile.GetModulePhysicsVersion());
    pFldProfileModuleAnimatorVersion.Set(objProfile, profile.GetModuleAnimatorVersion());
    pFldProfileModuleAIVersion.Set(objProfile, profile.GetModuleAIVersion());
    pFldProfileModuleCrashRecoveryVersion.Set(objProfile, profile.GetModuleCrashRecoveryVersion());
    pFldProfileModuleAudioVersion.Set(objProfile, profile.GetModuleAudioVersion());
    pFldProfileModuleSynthesizerVersion.Set(objProfile, profile.GetModuleSynthesizerVersion());
    pFldProfileModuleNetworkVersion.Set(objProfile, profile.GetModuleNetworkVersion());
    pFldProfileModuleVRVersion.Set(objProfile, profile.GetModuleVRVersion());

    pFldProfileDisableModuleVersions.Set(objProfile, objDisModVers);
    pFldProfileModules.Set(objProfile, objModules);

    pFldProfileRunArgs.Set(objProfile, profile.GetRunArguments());
    pFldProfileReplaceRunArgs.Set(objProfile, profile.GetReplaceRunArguments());

    return objProfile.ReturnValue();
}


// GameProfileStatus
//////////////////////

GameProfileStatus::GameProfileStatus(JNIEnv *env) :
pEnv(env),

pClsStatus(env, JPATH_BASE "GameProfileStatus"),
pFldValid(pClsStatus.GetFieldBool("valid")){
}

jobject GameProfileStatus::Convert(const delGameProfile &profile) {
    JniObject objStatus(pClsStatus.New());
    pFldValid.Set(objStatus, profile.GetValid());
    return objStatus.ReturnValue();
}


extern "C"
JNIEXPORT void JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_GameProfile_gameProfileRelease(
JNIEnv *env, jobject thiz, jlong pprofile){
    ((delGameProfile*)pprofile)->FreeReference();
}

extern "C"
JNIEXPORT jobject JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_GameProfile_gameProfileGetInfo(
JNIEnv *env, jobject thiz, jlong pprofile){
    JniHelpers h(env);
    try {
        const delGameProfile &profile = *((delGameProfile*)pprofile);
        return GameProfileInfo(env).Convert(profile);
    }catch(const deException &e){
        h.throwException(e);
        return nullptr; // keep compiler happy. code never gets here
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_GameProfile_gameProfileGetStatus(
JNIEnv *env, jobject thiz, jlong pprofile){
    JniHelpers h(env);
    try {
        const delGameProfile &profile = *((delGameProfile*)pprofile);
        return GameProfileStatus(env).Convert(profile);
    }catch(const deException &e){
        h.throwException(e);
        return nullptr; // keep compiler happy. code never gets here
    }
}

extern "C"
JNIEXPORT jlong JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_GameProfile_00024Companion_gameProfileCreate(
JNIEnv *env, jobject thiz){
    JniHelpers h(env);
    try {
        return (jlong)(intptr_t)new delGameProfile;
    }catch(const deException &e){
        h.throwException(e);
        return 0; // keep compiler happy. code never gets here
    }
}
extern "C"
JNIEXPORT jlong JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_GameProfile_00024Companion_gameProfileCopy(
JNIEnv *env, jobject thiz, jlong pprofile){
    JniHelpers h(env);
    try {
        const delGameProfile &profile = *((delGameProfile*)pprofile);
        return (jlong)(intptr_t)new delGameProfile(profile);
    }catch(const deException &e){
        h.throwException(e);
        return 0; // keep compiler happy. code never gets here
    }
}
