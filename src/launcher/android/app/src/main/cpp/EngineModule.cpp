#include "EngineModule.h"
#include <delauncher/engine/modules/delEngineModule.h>
#include <delauncher/engine/modules/delEngineModuleList.h>
#include <delauncher/engine/modules/parameter/delEMParameter.h>

EngineModule::EngineModule(JNIEnv *env) :
pEnv(env),

pClsEntry(env, JPATH_BASE "EngineModuleParameterInfoSelectionEntry"),
pFldEntryValue(pClsEntry.GetFieldString("value")),
pFldEntryDisplayName(pClsEntry.GetFieldString("displayName")),
pFldEntryDescription(pClsEntry.GetFieldString("description")),

pClsInfo(env, JPATH_BASE "EngineModuleParameterInfo"),
pFldInfoType(pClsInfo.GetFieldInt("type")),
pFldInfoName(pClsInfo.GetFieldString("name")),
pFldInfoDescription(pClsInfo.GetFieldString("description")),
pFldInfoMinValue(pClsInfo.GetFieldFloat("minValue")),
pFldInfoMaxValue(pClsInfo.GetFieldFloat("maxValue")),
pFldInfoValueStepSize(pClsInfo.GetFieldFloat("valueStepSize")),
pFldInfoSelectionEntries(pClsInfo.GetFieldObjectArray("selectionEntries", pClsEntry)),
pFldInfoCategory(pClsInfo.GetFieldInt("category")),
pFldInfoDisplayName(pClsInfo.GetFieldString("displayName")),
pFldInfoDefaultValue(pClsInfo.GetFieldString("defaultValue")),

pClsParam(env, JPATH_BASE "EngineModuleParameter"),
pFldParamIndex(pClsParam.GetFieldInt("index")),
pFldParamInfo(pClsParam.GetFieldObject("info", pClsInfo)),
pFldParamValue(pClsParam.GetFieldString("value")),

pClsMod(env, JPATH_BASE "EngineModule"),
pFldModType(pClsMod.GetFieldInt("type")),
pFldModName(pClsMod.GetFieldString("name")),
pFldModDescription(pClsMod.GetFieldString("description")),
pFldModAuthor(pClsMod.GetFieldString("author")),
pFldModVersion(pClsMod.GetFieldString("version")),
pFldModDirName(pClsMod.GetFieldString("dirName")),
pFldModPattern(pClsMod.GetFieldString("pattern")),
pFldModPriority(pClsMod.GetFieldInt("priority")),
pFldModIsFallback(pClsMod.GetFieldBool("isFallback")),
pFldModStatus(pClsMod.GetFieldInt("status")),
pFldModErrorCode(pClsMod.GetFieldInt("errorCode")),
pFldModLibFileName(pClsMod.GetFieldString("libFileName")),
pFldModLibFileSizeShould(pClsMod.GetFieldInt("libFileSizeShould")),
pFldModLibFileSizeIs(pClsMod.GetFieldInt("libFileSizeIs")),
pFldModLibFileHashShould(pClsMod.GetFieldString("libFileHashShould")),
pFldModLibFileHashIs(pClsMod.GetFieldString("libFileHashIs")),
pFldModLibFileEntryPoint(pClsMod.GetFieldString("libFileEntryPoint")),
pFldModParameters(pClsMod.GetFieldObjectArray("parameters", pClsParam)){
}

jobjectArray EngineModule::Convert(const delEngineModuleList &modules) {
    const int moduleCount = modules.GetCount();
    int i;

    JniObjectArray objMods(pEnv, pClsMod, moduleCount);

    for(i=0; i<moduleCount; i++){
        objMods.SetAt(i, Convert(*modules.GetAt(i)));
    }

    return objMods.ReturnArray();
}

jobject EngineModule::Convert(const delEngineModule &module) {
    JniObject objMod(pClsMod.New());

    pFldModType.Set(objMod, (int)module.GetType());
    pFldModName.Set(objMod, module.GetName());
    pFldModDescription.Set(objMod, module.GetDescription());
    pFldModAuthor.Set(objMod, module.GetAuthor());
    pFldModVersion.Set(objMod, module.GetVersion());
    pFldModDirName.Set(objMod, module.GetDirectoryName());
    pFldModPattern.Set(objMod, module.GetPattern());
    pFldModPriority.Set(objMod, module.GetPriority());
    pFldModIsFallback.Set(objMod, module.GetIsFallback());
    pFldModStatus.Set(objMod, (int)module.GetStatus());
    pFldModErrorCode.Set(objMod, module.GetErrorCode());
    pFldModLibFileName.Set(objMod, module.GetLibFileName());
    pFldModLibFileSizeShould.Set(objMod, module.GetLibFileSizeShould());
    pFldModLibFileSizeIs.Set(objMod, module.GetLibFileSizeIs());
    pFldModLibFileHashShould.Set(objMod, module.GetLibFileHashShould());
    pFldModLibFileHashIs.Set(objMod, module.GetLibFileHashIs());
    pFldModLibFileEntryPoint.Set(objMod, module.GetLibFileEntryPoint());

    const delEMParameterList &params = module.GetParameters();
    const int paramCount = params.GetCount();
    int j;

    JniObjectArray objParams(pEnv, pClsParam, paramCount);
    for(j=0; j<paramCount; j++){
        const delEMParameter &param = *params.GetAt(j);
        const deModuleParameter &info = param.GetInfo();

        const int entryCount = info.GetSelectionEntryCount();
        int k;
        const JniObjectArray objEntries(pEnv, pClsEntry, entryCount);
        for(k=0; k<entryCount; k++){
            const deModuleParameter::SelectionEntry &entry = info.GetSelectionEntryAt(k);
            const JniObject objEntry(pClsEntry.New());
            pFldEntryValue.Set(objEntry, entry.value);
            pFldEntryDisplayName.Set(objEntry, entry.displayName);
            pFldEntryDescription.Set(objEntry, entry.description);
            objEntries.SetAt(k, objEntry);
        }

        const JniObject objInfo(pClsInfo.New());
        pFldInfoType.Set(objInfo, (int)info.GetType());
        pFldInfoName.Set(objInfo, info.GetName());
        pFldInfoDescription.Set(objInfo, info.GetDescription());
        pFldInfoMinValue.Set(objInfo, info.GetMinimumValue());
        pFldInfoMaxValue.Set(objInfo, info.GetMaximumValue());
        pFldInfoValueStepSize.Set(objInfo, info.GetValueStepSize());
        pFldInfoSelectionEntries.Set(objInfo, objEntries);
        pFldInfoCategory.Set(objInfo, (int)info.GetCategory());
        pFldInfoDisplayName.Set(objInfo, info.GetDisplayName());
        pFldInfoDefaultValue.Set(objInfo, info.GetDefaultValue());

        const JniObject objParam(pClsParam.New());
        pFldParamIndex.Set(objParam, param.GetIndex());
        pFldParamInfo.Set(objParam, objInfo);
        pFldParamValue.Set(objParam, param.GetValue());

        objParams.SetAt(j, objParam);
    }
    pFldModParameters.Set(objMod, objParams);

    return objMod.ReturnValue();
}
