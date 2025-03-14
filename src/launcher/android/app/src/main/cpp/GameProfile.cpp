#include "GameProfile.h"

#include <delauncher/game/profile/delGameProfile.h>
#include <delauncher/game/profile/delGPDisableModuleVersion.h>
#include <delauncher/game/profile/delGPMParameter.h>
#include <delauncher/game/profile/delGPModule.h>

// GameProfileConfig
////////////////////

GameProfileConfig::GameProfileConfig(JNIEnv *env) :
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

pClsProfile(env, JPATH_BASE "GameProfileConfig"),
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

jobject GameProfileConfig::Convert(const delGameProfile &profile) {
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

void GameProfileConfig::Store(jobject objConfig, delGameProfile &profile) {
    profile.SetName(pFldProfileName.Get(objConfig));
    profile.SetModuleGraphic(pFldProfileModuleGraphic.Get(objConfig));
    profile.SetModuleInput(pFldProfileModuleInput.Get(objConfig));
    profile.SetModulePhysics(pFldProfileModulePhysics.Get(objConfig));
    profile.SetModuleAnimator(pFldProfileModuleAnimator.Get(objConfig));
    profile.SetModuleAI(pFldProfileModuleAI.Get(objConfig));
    profile.SetModuleCrashRecovery(pFldProfileModuleCrashRecovery.Get(objConfig));
    profile.SetModuleAudio(pFldProfileModuleAudio.Get(objConfig));
    profile.SetModuleSynthesizer(pFldProfileModuleSynthesizer.Get(objConfig));
    profile.SetModuleNetwork(pFldProfileModuleNetwork.Get(objConfig));
    profile.SetModuleVR(pFldProfileModuleVR.Get(objConfig));

    profile.SetModuleGraphicVersion(pFldProfileModuleGraphicVersion.Get(objConfig));
    profile.SetModuleInputVersion(pFldProfileModuleInputVersion.Get(objConfig));
    profile.SetModulePhysicsVersion(pFldProfileModulePhysicsVersion.Get(objConfig));
    profile.SetModuleAnimatorVersion(pFldProfileModuleAnimatorVersion.Get(objConfig));
    profile.SetModuleAIVersion(pFldProfileModuleAIVersion.Get(objConfig));
    profile.SetModuleCrashRecoveryVersion(pFldProfileModuleCrashRecoveryVersion.Get(objConfig));
    profile.SetModuleAudioVersion(pFldProfileModuleAudioVersion.Get(objConfig));
    profile.SetModuleSynthesizerVersion(pFldProfileModuleSynthesizerVersion.Get(objConfig));
    profile.SetModuleNetworkVersion(pFldProfileModuleNetworkVersion.Get(objConfig));
    profile.SetModuleVRVersion(pFldProfileModuleVRVersion.Get(objConfig));

    const JniObjectArray objDisModVers(pEnv, pFldProfileDisableModuleVersions.Get(objConfig));
    const int disObjVersCount = objDisModVers.GetCount();
    int i;
    for(i=0; i<disObjVersCount; i++){
        jobject objVer = objDisModVers.GetAt(i);
        auto * const ver = new delGPDisableModuleVersion;
        ver->SetName(pFldModVerName.Get(objVer));
        ver->SetVersion(pFldModVerVersion.Get(objVer));
        profile.GetDisableModuleVersions().Add(ver);
    }

    const JniObjectArray objMods(pEnv, pFldProfileModules.Get(objConfig));
    const int modCount = objMods.GetCount();
    for(i=0; i<modCount; i++) {
        jobject objMod = objMods.GetAt(i);
        auto * const mod = new delGPModule;
        mod->SetName(pFldModuleName.Get(objMod));

        JniObjectArray objParams(pEnv, pFldModuleParameters.Get(objMod));
        const int paramCount = objParams.GetCount();
        int j;
        for(j=0; j<paramCount; j++){
            jobject objParam = objParams.GetAt(j);
            auto * const param = new delGPMParameter;
            param->SetName(pFldParamName.Get(objParam));
            param->SetValue(pFldParamValue.Get(objParam));
            mod->GetParameters().Add(param);
        }

        profile.GetModules().Add(mod);
    }

    profile.SetRunArguments(pFldProfileRunArgs.Get(objConfig));
    profile.SetReplaceRunArguments(pFldProfileReplaceRunArgs.Get(objConfig));
}


// GameProfileStatus
//////////////////////

GameProfileStatus::GameProfileStatus(JNIEnv *env) :
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
Java_ch_dragondreams_delauncher_launcher_internal_GameProfile_gameProfileGetConfig(
JNIEnv *env, jobject thiz, jlong pprofile){
    JniHelpers h(env);
    try {
        const delGameProfile &profile = *((delGameProfile*)pprofile);
        return GameProfileConfig(env).Convert(profile);
    }catch(const deException &e){
        h.throwException(e);
        return nullptr; // keep compiler happy. code never gets here
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_GameProfile_gameProfileSetConfig(
JNIEnv *env, jobject thiz, jlong pprofile, jobject pconfig) {
    JniHelpers h(env);
    try {
        GameProfileConfig(env).Store(pconfig, *((delGameProfile*)pprofile));
    }catch(const deException &e){
        h.throwException(e);
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
