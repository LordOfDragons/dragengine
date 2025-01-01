#ifndef DELAUNCHER_GAMEPROFILE_H
#define DELAUNCHER_GAMEPROFILE_H

#include "JniHelper.h"

class delGameProfile;

class GameProfileInfo {
    JNIEnv *pEnv;

    JniClass pClsParameter;
    const JniFieldString pFldParamName;
    const JniFieldString pFldParamValue;

    JniClass pClsModule;
    const JniFieldString pFldModuleName;
    const JniFieldObjectArray pFldModuleParameters;

    JniClass pClsModuleVersion;
    const JniFieldString pFldModVerName;
    const JniFieldString pFldModVerVersion;

    JniClass pClsProfile;
    const JniFieldString pFldProfileName;

    const JniFieldString pFldProfileModuleGraphic;
    const JniFieldString pFldProfileModuleInput;
    const JniFieldString pFldProfileModulePhysics;
    const JniFieldString pFldProfileModuleAnimator;
    const JniFieldString pFldProfileModuleAI;
    const JniFieldString pFldProfileModuleCrashRecovery;
    const JniFieldString pFldProfileModuleAudio;
    const JniFieldString pFldProfileModuleSynthesizer;
    const JniFieldString pFldProfileModuleNetwork;
    const JniFieldString pFldProfileModuleVR;

    const JniFieldString pFldProfileModuleGraphicVersion;
    const JniFieldString pFldProfileModuleInputVersion;
    const JniFieldString pFldProfileModulePhysicsVersion;
    const JniFieldString pFldProfileModuleAnimatorVersion;
    const JniFieldString pFldProfileModuleAIVersion;
    const JniFieldString pFldProfileModuleCrashRecoveryVersion;
    const JniFieldString pFldProfileModuleAudioVersion;
    const JniFieldString pFldProfileModuleSynthesizerVersion;
    const JniFieldString pFldProfileModuleNetworkVersion;
    const JniFieldString pFldProfileModuleVRVersion;

    const JniFieldObjectArray pFldProfileDisableModuleVersions;

    const JniFieldObjectArray pFldProfileModules;

    const JniFieldString pFldProfileRunArgs;
    const JniFieldBool pFldProfileReplaceRunArgs;

public:
    explicit GameProfileInfo(JNIEnv *env);

    jobject Convert(const delGameProfile &profile);
};

class GameProfileStatus{
    JNIEnv *pEnv;

    JniClass pClsStatus;
    const JniFieldBool pFldValid;

public:
    explicit GameProfileStatus(JNIEnv *env);

    jobject Convert(const delGameProfile &profile);
};

#endif //DELAUNCHER_GAMEPROFILE_H
