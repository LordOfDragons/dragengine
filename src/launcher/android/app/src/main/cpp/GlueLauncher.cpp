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
Java_ch_dragondreams_delauncher_launcher_DragengineLauncher_00024GlueLauncher_createLauncher(
        JNIEnv *env, jobject thiz, jobject config) {
    //__android_log_print(ANDROID_LOG_ERROR, "GlueLauncher", "Checkpoint %d", __LINE__);
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
Java_ch_dragondreams_delauncher_launcher_DragengineLauncher_00024GlueLauncher_destroyLauncher(
        JNIEnv *env, jobject thiz, jlong launcher) {
    delete (delLauncher*)(intptr_t)launcher;
}

#define JPATH_BASE_GLUE "ch/dragondreams/delauncher/launcher/DragengineLauncher$GlueLauncher$"

extern "C"
JNIEXPORT jobjectArray JNICALL
Java_ch_dragondreams_delauncher_launcher_DragengineLauncher_00024GlueLauncher_getEngineModules(
        JNIEnv *env, jobject thiz, jlong plauncher) {
    const delLauncher &launcher = *((delLauncher*)(intptr_t)plauncher);

    JniClass clsEngModParamInfoSelEntry(env, JPATH_BASE_GLUE "GlueEngineModuleParameterInfoSelectionEntry");
    const JniFieldString fldEngModParamInfoSelEntryValue(clsEngModParamInfoSelEntry.GetFieldString("value"));
    const JniFieldString fldEngModParamInfoSelEntryDisplayName(clsEngModParamInfoSelEntry.GetFieldString("displayName"));
    const JniFieldString fldEngModParamInfoSelEntryDescription(clsEngModParamInfoSelEntry.GetFieldString("description"));

    JniClass clsEngModParamInfo(env, JPATH_BASE_GLUE "GlueEngineModuleParameterInfo");
    const JniFieldInt fldEngModParamInfoType(clsEngModParamInfo.GetFieldInt("type"));
    const JniFieldString fldEngModParamInfoName(clsEngModParamInfo.GetFieldString("name"));
    const JniFieldString fldEngModParamInfoDescription(clsEngModParamInfo.GetFieldString("description"));
    const JniFieldFloat fldEngModParamInfoMinValue(clsEngModParamInfo.GetFieldFloat("minValue"));
    const JniFieldFloat fldEngModParamInfoMaxValue(clsEngModParamInfo.GetFieldFloat("maxValue"));
    const JniFieldFloat fldEngModParamInfoValueStepSize(clsEngModParamInfo.GetFieldFloat("valueStepSize"));
    const JniFieldObjectArray fldEngModParamInfoSelectionEntries(clsEngModParamInfo.GetFieldObjectArray(
            "selectionEntries", clsEngModParamInfoSelEntry));
    const JniFieldInt fldEngModParamInfoCategory(clsEngModParamInfo.GetFieldInt("category"));
    const JniFieldString fldEngModParamInfoDisplayName(clsEngModParamInfo.GetFieldString("displayName"));
    const JniFieldString fldEngModParamInfoDefaultValue(clsEngModParamInfo.GetFieldString("defaultValue"));

    JniClass clsEngModParam(env, JPATH_BASE_GLUE "GlueEngineModuleParameter");
    const JniFieldInt fldEngModParamIndex(clsEngModParam.GetFieldInt("index"));
    const JniFieldObject fldEngModParamInfo(clsEngModParam.GetFieldObject("info", clsEngModParamInfo));
    const JniFieldString fldEngModParamValue(clsEngModParam.GetFieldString("value"));

    JniClass clsEngMod(env, JPATH_BASE_GLUE "GlueEngineModule");
    const JniFieldInt fldEngModType(clsEngMod.GetFieldInt("type"));
    const JniFieldString fldEngModName(clsEngMod.GetFieldString("name"));
    const JniFieldString fldEngModDescription(clsEngMod.GetFieldString("description"));
    const JniFieldString fldEngModAuthor(clsEngMod.GetFieldString("author"));
    const JniFieldString fldEngModVersion(clsEngMod.GetFieldString("version"));
    const JniFieldString fldEngModDirName(clsEngMod.GetFieldString("dirName"));
    const JniFieldString fldEngModPattern(clsEngMod.GetFieldString("pattern"));
    const JniFieldInt fldEngModPriority(clsEngMod.GetFieldInt("priority"));
    const JniFieldBool fldEngModIsFallback(clsEngMod.GetFieldBool("isFallback"));
    const JniFieldInt fldEngModStatus(clsEngMod.GetFieldInt("status"));
    const JniFieldInt fldEngModErrorCode(clsEngMod.GetFieldInt("errorCode"));
    const JniFieldString fldEngModLibFileName(clsEngMod.GetFieldString("libFileName"));
    const JniFieldInt fldEngModLibFileSizeShould(clsEngMod.GetFieldInt("libFileSizeShould"));
    const JniFieldInt fldEngModLibFileSizeIs(clsEngMod.GetFieldInt("libFileSizeIs"));
    const JniFieldString fldEngModLibFileHashShould(clsEngMod.GetFieldString("libFileHashShould"));
    const JniFieldString fldEngModLibFileHashIs(clsEngMod.GetFieldString("libFileHashIs"));
    const JniFieldString fldEngModLibFileEntryPoint(clsEngMod.GetFieldString("libFileEntryPoint"));
    const JniFieldObjectArray fldEngModParameters(clsEngMod.GetFieldObjectArray("parameters", clsEngModParam));

    const delEngineModuleList &modules = launcher.GetEngine().GetModules();
    const int moduleCount = modules.GetCount();
    int i;

    JniObjectArray objEngMods(env, clsEngMod, moduleCount);

    for(i=0; i<moduleCount; i++){
        const delEngineModule &module = *modules.GetAt(i);
        const JniObject objEngMod(clsEngMod.New());

        fldEngModType.Set(objEngMod, (int)module.GetType());
        fldEngModName.Set(objEngMod, module.GetName());
        fldEngModDescription.Set(objEngMod, module.GetDescription());
        fldEngModAuthor.Set(objEngMod, module.GetAuthor());
        fldEngModVersion.Set(objEngMod, module.GetVersion());
        fldEngModDirName.Set(objEngMod, module.GetDirectoryName());
        fldEngModPattern.Set(objEngMod, module.GetPattern());
        fldEngModPriority.Set(objEngMod, module.GetPriority());
        fldEngModIsFallback.Set(objEngMod, module.GetIsFallback());
        fldEngModStatus.Set(objEngMod, (int)module.GetStatus());
        fldEngModErrorCode.Set(objEngMod, module.GetErrorCode());
        fldEngModLibFileName.Set(objEngMod, module.GetLibFileName());
        fldEngModLibFileSizeShould.Set(objEngMod, module.GetLibFileSizeShould());
        fldEngModLibFileSizeIs.Set(objEngMod, module.GetLibFileSizeIs());
        fldEngModLibFileHashShould.Set(objEngMod, module.GetLibFileHashShould());
        fldEngModLibFileHashIs.Set(objEngMod, module.GetLibFileHashIs());
        fldEngModLibFileEntryPoint.Set(objEngMod, module.GetLibFileEntryPoint());

        const delEMParameterList &params = module.GetParameters();
        const int paramCount = params.GetCount();
        int j;

        JniObjectArray objEngModParams(env, clsEngModParam, paramCount);
        for(j=0; j<paramCount; j++){
            const delEMParameter &param = *params.GetAt(j);
            const deModuleParameter &info = param.GetInfo();

            const int entryCount = info.GetSelectionEntryCount();
            int k;
            const JniObjectArray objEntries(env, clsEngModParamInfoSelEntry, entryCount);
            for(k=0; k<entryCount; k++){
                const deModuleParameter::SelectionEntry &entry = info.GetSelectionEntryAt(k);
                const JniObject objEntry(clsEngModParamInfoSelEntry.New());
                fldEngModParamInfoSelEntryValue.Set(objEntry, entry.value);
                fldEngModParamInfoSelEntryDisplayName.Set(objEntry, entry.displayName);
                fldEngModParamInfoSelEntryDescription.Set(objEntry, entry.description);
                objEntries.SetAt(k, objEntry);
            }

            const JniObject objInfo(clsEngModParamInfo.New());
            fldEngModParamInfoType.Set(objInfo, (int)info.GetType());
            fldEngModParamInfoName.Set(objInfo, info.GetName());
            fldEngModParamInfoDescription.Set(objInfo, info.GetDescription());
            fldEngModParamInfoMinValue.Set(objInfo, info.GetMinimumValue());
            fldEngModParamInfoMaxValue.Set(objInfo, info.GetMaximumValue());
            fldEngModParamInfoValueStepSize.Set(objInfo, info.GetValueStepSize());
            fldEngModParamInfoSelectionEntries.Set(objInfo, objEntries);
            fldEngModParamInfoCategory.Set(objInfo, (int)info.GetCategory());
            fldEngModParamInfoDisplayName.Set(objInfo, info.GetDisplayName());
            fldEngModParamInfoDefaultValue.Set(objInfo, info.GetDefaultValue());

            const JniObject objParam(clsEngModParam.New());
            fldEngModParamIndex.Set(objParam, param.GetIndex());
            fldEngModParamInfo.Set(objParam, objInfo);
            fldEngModParamValue.Set(objParam, param.GetValue());

            objEngModParams.SetAt(j, objParam);
        }
        fldEngModParameters.Set(objEngMod, objEngModParams);

        objEngMods.SetAt(i, objEngMod);
    }

    return objEngMods.ReturnArray();
}