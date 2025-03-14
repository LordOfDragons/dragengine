#ifndef DELAUNCHER_ENGINEMODULE_H
#define DELAUNCHER_ENGINEMODULE_H

#include "JniHelper.h"

class delEngineModule;
class delEngineModuleList;

class EngineModule {
private:
    JNIEnv *pEnv;

    JniClass pClsEntry;
    const JniFieldString pFldEntryValue;
    const JniFieldString pFldEntryDisplayName;
    const JniFieldString pFldEntryDescription;

    JniClass pClsInfo;
    const JniFieldInt pFldInfoType;
    const JniFieldString pFldInfoName;
    const JniFieldString pFldInfoDescription;
    const JniFieldFloat pFldInfoMinValue;
    const JniFieldFloat pFldInfoMaxValue;
    const JniFieldFloat pFldInfoValueStepSize;
    const JniFieldObjectArray pFldInfoSelectionEntries;
    const JniFieldInt pFldInfoCategory;
    const JniFieldString pFldInfoDisplayName;
    const JniFieldString pFldInfoDefaultValue;

    JniClass pClsParam;
    const JniFieldInt pFldParamIndex;
    const JniFieldObject pFldParamInfo;
    const JniFieldString pFldParamValue;

    JniClass pClsMod;
    const JniFieldInt pFldModType;
    const JniFieldString pFldModName;
    const JniFieldString pFldModDescription;
    const JniFieldString pFldModAuthor;
    const JniFieldString pFldModVersion;
    const JniFieldString pFldModDirName;
    const JniFieldString pFldModPattern;
    const JniFieldInt pFldModPriority;
    const JniFieldBool pFldModIsFallback;
    const JniFieldInt pFldModStatus;
    const JniFieldInt pFldModErrorCode;
    const JniFieldString pFldModLibFileName;
    const JniFieldInt pFldModLibFileSizeShould;
    const JniFieldInt pFldModLibFileSizeIs;
    const JniFieldString pFldModLibFileHashShould;
    const JniFieldString pFldModLibFileHashIs;
    const JniFieldString pFldModLibFileEntryPoint;
    const JniFieldObjectArray pFldModParameters;

public:
    explicit EngineModule(JNIEnv *env);

    jobjectArray Convert(const delEngineModuleList &modules);
    jobject Convert(const delEngineModule &module);
};

#endif //DELAUNCHER_ENGINEMODULE_H
