#include <memory>

#include <jni.h>
#include <android/native_window_jni.h>
#include <android/native_window_jni.h>
#include <android/log.h>

#include <delauncher/delLauncher.h>
#include <delauncher/engine/modules/delEngineModule.h>
#include <delauncher/engine/modules/parameter/delEMParameter.h>

#include "JniHelper.h"

static JavaVM *vJavaVM = nullptr;

jint JNI_OnLoad(JavaVM *vm, void *reserved){
    vJavaVM = vm;
    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_Launcher_createLauncher(
        JNIEnv *env, jobject thiz, jobject config) {
    //__android_log_print(ANDROID_LOG_ERROR, "Launcher", "Checkpoint %d", __LINE__);
    JniObjectClass clsConfig(env, config);

    delLauncher::sConfig delConfig{};

    delConfig.loggerSource = clsConfig.GetFieldString("loggerSource").Get(config);
    delConfig.engineLogFileTitle = clsConfig.GetFieldString("engineLogFileTitle").Get(config);
    delConfig.pathLauncher = clsConfig.GetFieldString("pathLauncher").Get(config);
    delConfig.pathGames = clsConfig.GetFieldString("pathLauncherGames").Get(config);
    delConfig.pathConfig = clsConfig.GetFieldString("pathLauncherConfig").Get(config);

    delConfig.osConfig.pathEngine = clsConfig.GetFieldString("pathEngine").Get(config);
    delConfig.osConfig.pathConfig = clsConfig.GetFieldString("pathEngineConfig").Get(config);
    delConfig.osConfig.pathCache = clsConfig.GetFieldString("pathEngineCache").Get(config);

    delConfig.osConfig.javavm = vJavaVM;

    {
        const JniFieldObject fid(env, clsConfig, "surface", "Landroid/view/SurfaceView;");
        auto js = fid.Get(config);
        if(js) {
            delConfig.osConfig.nativeWindow = ANativeWindow_fromSurface(env, js);
        }
        //ANativeWindow_release();
    }

    delLauncher *launcher = nullptr;
    try {
        launcher = new delLauncher(delConfig);
        launcher->AddFileLogger("delauncher");
        launcher->Prepare();
        return (jlong)(intptr_t)launcher;

    } catch(...) {
        delete launcher;
        throw;
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_Launcher_destroyLauncher(
        JNIEnv *env, jobject thiz, jlong launcher) {
    delete (delLauncher*)(intptr_t)launcher;
}

#define JPATH_BASE "ch/dragondreams/delauncher/launcher/internal/"

extern "C"
JNIEXPORT jobjectArray JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_Launcher_getEngineModules(
        JNIEnv *env, jobject thiz, jlong plauncher) {
    const delLauncher &launcher = *((delLauncher*)(intptr_t)plauncher);

    JniClass clsEntry(env, JPATH_BASE "EngineModuleParameterInfoSelectionEntry");
    const JniFieldString fldEntryValue(clsEntry.GetFieldString("value"));
    const JniFieldString fldEntryDisplayName(clsEntry.GetFieldString("displayName"));
    const JniFieldString fldEntryDescription(clsEntry.GetFieldString("description"));

    JniClass clsInfo(env, JPATH_BASE "EngineModuleParameterInfo");
    const JniFieldInt fldInfoType(clsInfo.GetFieldInt("type"));
    const JniFieldString fldInfoName(clsInfo.GetFieldString("name"));
    const JniFieldString fldInfoDescription(clsInfo.GetFieldString("description"));
    const JniFieldFloat fldInfoMinValue(clsInfo.GetFieldFloat("minValue"));
    const JniFieldFloat fldInfoMaxValue(clsInfo.GetFieldFloat("maxValue"));
    const JniFieldFloat fldInfoValueStepSize(clsInfo.GetFieldFloat("valueStepSize"));
    const JniFieldObjectArray fldInfoSelectionEntries(clsInfo.GetFieldObjectArray(
            "selectionEntries", clsEntry));
    const JniFieldInt fldInfoCategory(clsInfo.GetFieldInt("category"));
    const JniFieldString fldInfoDisplayName(clsInfo.GetFieldString("displayName"));
    const JniFieldString fldInfoDefaultValue(clsInfo.GetFieldString("defaultValue"));

    JniClass clsParam(env, JPATH_BASE "EngineModuleParameter");
    const JniFieldInt fldParamIndex(clsParam.GetFieldInt("index"));
    const JniFieldObject fldParamInfo(clsParam.GetFieldObject("info", clsInfo));
    const JniFieldString fldParamValue(clsParam.GetFieldString("value"));

    JniClass clsMod(env, JPATH_BASE "EngineModule");
    const JniFieldInt fldModType(clsMod.GetFieldInt("type"));
    const JniFieldString fldModName(clsMod.GetFieldString("name"));
    const JniFieldString fldModDescription(clsMod.GetFieldString("description"));
    const JniFieldString fldModAuthor(clsMod.GetFieldString("author"));
    const JniFieldString fldModVersion(clsMod.GetFieldString("version"));
    const JniFieldString fldModDirName(clsMod.GetFieldString("dirName"));
    const JniFieldString fldModPattern(clsMod.GetFieldString("pattern"));
    const JniFieldInt fldModPriority(clsMod.GetFieldInt("priority"));
    const JniFieldBool fldModIsFallback(clsMod.GetFieldBool("isFallback"));
    const JniFieldInt fldModStatus(clsMod.GetFieldInt("status"));
    const JniFieldInt fldModErrorCode(clsMod.GetFieldInt("errorCode"));
    const JniFieldString fldModLibFileName(clsMod.GetFieldString("libFileName"));
    const JniFieldInt fldModLibFileSizeShould(clsMod.GetFieldInt("libFileSizeShould"));
    const JniFieldInt fldModLibFileSizeIs(clsMod.GetFieldInt("libFileSizeIs"));
    const JniFieldString fldModLibFileHashShould(clsMod.GetFieldString("libFileHashShould"));
    const JniFieldString fldModLibFileHashIs(clsMod.GetFieldString("libFileHashIs"));
    const JniFieldString fldModLibFileEntryPoint(clsMod.GetFieldString("libFileEntryPoint"));
    const JniFieldObjectArray fldModParameters(clsMod.GetFieldObjectArray("parameters", clsParam));

    const delEngineModuleList &modules = launcher.GetEngine().GetModules();
    const int moduleCount = modules.GetCount();
    int i;

    JniObjectArray objMods(env, clsMod, moduleCount);

    for(i=0; i<moduleCount; i++){
        const delEngineModule &module = *modules.GetAt(i);
        const JniObject objMod(clsMod.New());

        fldModType.Set(objMod, (int)module.GetType());
        fldModName.Set(objMod, module.GetName());
        fldModDescription.Set(objMod, module.GetDescription());
        fldModAuthor.Set(objMod, module.GetAuthor());
        fldModVersion.Set(objMod, module.GetVersion());
        fldModDirName.Set(objMod, module.GetDirectoryName());
        fldModPattern.Set(objMod, module.GetPattern());
        fldModPriority.Set(objMod, module.GetPriority());
        fldModIsFallback.Set(objMod, module.GetIsFallback());
        fldModStatus.Set(objMod, (int)module.GetStatus());
        fldModErrorCode.Set(objMod, module.GetErrorCode());
        fldModLibFileName.Set(objMod, module.GetLibFileName());
        fldModLibFileSizeShould.Set(objMod, module.GetLibFileSizeShould());
        fldModLibFileSizeIs.Set(objMod, module.GetLibFileSizeIs());
        fldModLibFileHashShould.Set(objMod, module.GetLibFileHashShould());
        fldModLibFileHashIs.Set(objMod, module.GetLibFileHashIs());
        fldModLibFileEntryPoint.Set(objMod, module.GetLibFileEntryPoint());

        const delEMParameterList &params = module.GetParameters();
        const int paramCount = params.GetCount();
        int j;

        JniObjectArray objParams(env, clsParam, paramCount);
        for(j=0; j<paramCount; j++){
            const delEMParameter &param = *params.GetAt(j);
            const deModuleParameter &info = param.GetInfo();

            const int entryCount = info.GetSelectionEntryCount();
            int k;
            const JniObjectArray objEntries(env, clsEntry, entryCount);
            for(k=0; k<entryCount; k++){
                const deModuleParameter::SelectionEntry &entry = info.GetSelectionEntryAt(k);
                const JniObject objEntry(clsEntry.New());
                fldEntryValue.Set(objEntry, entry.value);
                fldEntryDisplayName.Set(objEntry, entry.displayName);
                fldEntryDescription.Set(objEntry, entry.description);
                objEntries.SetAt(k, objEntry);
            }

            const JniObject objInfo(clsInfo.New());
            fldInfoType.Set(objInfo, (int)info.GetType());
            fldInfoName.Set(objInfo, info.GetName());
            fldInfoDescription.Set(objInfo, info.GetDescription());
            fldInfoMinValue.Set(objInfo, info.GetMinimumValue());
            fldInfoMaxValue.Set(objInfo, info.GetMaximumValue());
            fldInfoValueStepSize.Set(objInfo, info.GetValueStepSize());
            fldInfoSelectionEntries.Set(objInfo, objEntries);
            fldInfoCategory.Set(objInfo, (int)info.GetCategory());
            fldInfoDisplayName.Set(objInfo, info.GetDisplayName());
            fldInfoDefaultValue.Set(objInfo, info.GetDefaultValue());

            const JniObject objParam(clsParam.New());
            fldParamIndex.Set(objParam, param.GetIndex());
            fldParamInfo.Set(objParam, objInfo);
            fldParamValue.Set(objParam, param.GetValue());

            objParams.SetAt(j, objParam);
        }
        fldModParameters.Set(objMod, objParams);

        objMods.SetAt(i, objMod);
    }

    return objMods.ReturnArray();
}