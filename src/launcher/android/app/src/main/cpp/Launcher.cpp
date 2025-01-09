#include <memory>

#include <jni.h>
#include <android/native_window_jni.h>
#include <android/native_window_jni.h>
#include <android/log.h>

#include <delauncher/engine/modules/delEngineModule.h>
#include <delauncher/engine/modules/parameter/delEMParameter.h>
#include <delauncher/game/delGame.h>
#include <delauncher/game/fileformat/delFileFormat.h>
#include <delauncher/game/icon/delGameIcon.h>

#include "JniHelper.h"
#include "EngineModule.h"
#include "Game.h"
#include "Launcher.h"

static JavaVM *vJavaVM = nullptr;

jint JNI_OnLoad(JavaVM *vm, void *reserved){
    vJavaVM = vm;
    return JNI_VERSION_1_6;
}

Launcher::Launcher(const sConfig &config) : delLauncher(config),
pFDContainer(FDVFSContainer::Ref::New(new FDVFSContainer(
    decPath::CreatePathUnix("/fds")))){
}

delGameList Launcher::ReadDelgaGames(const decString &path){
    delGameList games;
    const delEngineInstance::Ref instance(delEngineInstance::Ref::New(
            GetEngineInstanceFactory().CreateEngineInstance(
                    *this, GetEngine().GetLogFile())));
    instance->StartEngine();
    instance->LoadModules();

    GetEngine().ReadDelgaGameDefsVfs(instance,
        deVFSContainer::Ref(pFDContainer), path, games);
    return games;
}

Launcher::~Launcher(){
    if(GetConfig().osConfig.nativeWindow){
        ANativeWindow_release(GetConfig().osConfig.nativeWindow);
    }
}

extern "C"
JNIEXPORT jlong JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_Launcher_createLauncher(
JNIEnv *env, jobject thiz, jobject config) {
    JniHelpers h(env);
    try {
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
            jobject objView = clsConfig.GetFieldObject("surface",
                "Landroid/view/Surface;").Get(config);
            if(objView) {
                delConfig.osConfig.nativeWindow = ANativeWindow_fromSurface(env, objView);
                DEASSERT_NOTNULL(delConfig.osConfig.nativeWindow)
            }
        }

        return (jlong)(intptr_t)(new Launcher(delConfig));
    }catch(const deException &e){
        h.throwException(e);
        return 0; // keep compiler happy. code never gets here
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_Launcher_destroyLauncher(
        JNIEnv *env, jobject thiz, jlong launcher) {
    JniHelpers h(env);
    try {
        delete (Launcher*)(intptr_t)launcher;
    }catch(const deException &e){
        h.throwException(e);
    }
}

extern "C"
JNIEXPORT jobjectArray JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_Launcher_getEngineModules(JNIEnv *env, jobject thiz, jlong plauncher) {
    JniHelpers h(env);
    try {
        const Launcher &launcher = *((Launcher*)(intptr_t)plauncher);
        return EngineModule(env).Convert(launcher.GetEngine().GetModules());
    }catch(const deException &e){
        h.throwException(e);
        return nullptr; // keep compiler happy. code never gets here
    }
}

extern "C"
JNIEXPORT jlongArray JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_Launcher_getGames(
JNIEnv *env, jobject thiz, jlong plauncher) {
    JniHelpers h(env);
    try {
        const Launcher &launcher = *((Launcher *) (intptr_t) plauncher);
        const delGameList &games = launcher.GetGameManager().GetGames();
        const int gameCount = games.GetCount();
        int i;

        JniLongArray objGames(env, gameCount);
        for (i=0; i<gameCount; i++) {
            delGame * const game = games.GetAt(i);
            game->AddReference();
            objGames.SetAt(i, (jlong)(intptr_t)game);
        }
        return objGames.ReturnArray();
    }catch(const deException &e){
        h.throwException(e);
        return nullptr; // keep compiler happy. code never gets here
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_Launcher_vfsContainerAddFd(
JNIEnv *env, jobject thiz, jlong plauncher, jstring path, jint fd, jint offset, jint length){
    JniHelpers h(env);
    try {
        const Launcher &launcher = *((Launcher *) (intptr_t) plauncher);
        std::string spath(h.convertString(path));
        launcher.GetFDContainer()->AddEntry(spath, fd, offset, length);
    }catch(const deException &e){
        h.throwException(e);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_Launcher_vfsContainerRemoveFd(
JNIEnv *env, jobject thiz, jlong plauncher, jstring path){
    JniHelpers h(env);
    try {
        const Launcher &launcher = *((Launcher *) (intptr_t) plauncher);
        const std::string spath(h.convertString(path));
        launcher.GetFDContainer()->RemoveEntry(spath);
    }catch(const deException &e){
        h.throwException(e);
    }
}

extern "C"
JNIEXPORT jlongArray JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_Launcher_readDelgaGames(
JNIEnv *env, jobject thiz, jlong plauncher, jstring path){
    JniHelpers h(env);
    try {
        Launcher &launcher = *((Launcher *) (intptr_t) plauncher);

        const delGameList games(launcher.ReadDelgaGames(h.convertString(path)));
        const int gameCount = games.GetCount();
        int i;

        JniLongArray objGames(env, gameCount);
        for (i=0; i<gameCount; i++) {
            delGame * const game = games.GetAt(i);
            game->AddReference();
            objGames.SetAt(i, (jlong)(intptr_t)game);
        }
        return objGames.ReturnArray();
    }catch(const deException &e){
        h.throwException(e);
        return nullptr; // keep compiler happy. code never gets here
    }
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_Launcher_isModuleSingleType(
JNIEnv *env, jobject thiz, jint type){
    return deModuleSystem::IsSingleType((deModuleSystem::eModuleTypes)type) ? JNI_TRUE : JNI_FALSE;
}
extern "C"
JNIEXPORT jlongArray JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_Launcher_getGameProfiles(
JNIEnv *env, jobject thiz, jlong plauncher){
    JniHelpers h(env);
    try {
        const Launcher &launcher = *((Launcher*)(intptr_t)plauncher);
        const delGameProfileList &profiles = launcher.GetGameManager().GetProfiles();
        const int profileCount = profiles.GetCount();
        int i;

        JniLongArray objProfiles(env, profileCount);
        for (i=0; i<profileCount; i++) {
            delGameProfile * const profile = profiles.GetAt(i);
            profile->AddReference();
            objProfiles.SetAt(i, (jlong)(intptr_t)profile);
        }
        return objProfiles.ReturnArray();
    }catch(const deException &e){
        h.throwException(e);
        return nullptr; // keep compiler happy. code never gets here
    }
}

extern "C"
JNIEXPORT jlong JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_Launcher_getDefaultProfile(
JNIEnv *env, jobject thiz, jlong plauncher){
    JniHelpers h(env);
    try {
        const Launcher &launcher = *((Launcher*)(intptr_t)plauncher);
        delGameProfile * const profile = launcher.GetGameManager().GetDefaultProfile();
        if(profile){
            profile->AddReference();
        }
        return (jlong)(intptr_t)profile;
    }catch(const deException &e){
        h.throwException(e);
        return 0; // keep compiler happy. code never gets here
    }
}

extern "C"
JNIEXPORT jlong JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_Launcher_getActiveProfile(
JNIEnv *env, jobject thiz, jlong plauncher){
    JniHelpers h(env);
    try {
        const Launcher &launcher = *((Launcher*)(intptr_t)plauncher);
        delGameProfile * const profile = launcher.GetGameManager().GetActiveProfile();
        if(profile){
            profile->AddReference();
        }
        return (jlong)(intptr_t)profile;
    }catch(const deException &e){
        h.throwException(e);
        return 0; // keep compiler happy. code never gets here
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_Launcher_setActiveProfile(
JNIEnv *env, jobject thiz, jlong plauncher, jlong pprofile){
    JniHelpers h(env);
    try {
        Launcher &launcher = *((Launcher*)(intptr_t)plauncher);
        delGameProfile * const profile = (delGameProfile*)(intptr_t)pprofile;
        launcher.GetGameManager().SetActiveProfile(profile);
    }catch(const deException &e){
        h.throwException(e);
    }
}


extern "C"
JNIEXPORT void JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_Launcher_logInfo(
JNIEnv *env, jobject thiz, jlong plauncher, jstring message){
    JniHelpers h(env);
    try {
        Launcher &launcher = *((Launcher*)(intptr_t)plauncher);
        launcher.GetLogger()->LogInfo("DELauncherAndroid", h.convertString(message));
    }catch(const deException &e){
        h.throwException(e);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_Launcher_logWarn(
JNIEnv *env, jobject thiz, jlong plauncher, jstring message){
    JniHelpers h(env);
    try {
        Launcher &launcher = *((Launcher*)(intptr_t)plauncher);
        launcher.GetLogger()->LogWarn("DELauncherAndroid", h.convertString(message));
    }catch(const deException &e){
        h.throwException(e);
    }
}


extern "C"
JNIEXPORT void JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_Launcher_logError(
JNIEnv *env, jobject thiz, jlong plauncher, jstring message){
    JniHelpers h(env);
    try {
        Launcher &launcher = *((Launcher*)(intptr_t)plauncher);
        launcher.GetLogger()->LogError("DELauncherAndroid", h.convertString(message));
    }catch(const deException &e){
        h.throwException(e);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_Launcher_addFileLogger(
JNIEnv *env, jobject thiz, jlong plauncher, jstring ppath){
    JniHelpers h(env);
    try {
        ((Launcher*)(intptr_t)plauncher)->AddFileLogger(h.convertString(ppath));
    }catch(const deException &e){
        h.throwException(e);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_Launcher_prepare(
JNIEnv *env, jobject thiz, jlong plauncher){
    JniHelpers h(env);
    try {
        ((Launcher*)(intptr_t)plauncher)->Prepare();
    }catch(const deException &e){
        h.throwException(e);
    }
}


extern "C"
JNIEXPORT jlong JNICALL
Java_ch_dragondreams_delauncher_launcher_internal_Launcher_createGame(
JNIEnv *env, jobject thiz, jlong plauncher){
    JniHelpers h(env);
    try {
        delLauncher &launcher = *((Launcher*)(intptr_t)plauncher);
        return (jlong)(intptr_t)launcher.CreateGame();
    }catch(const deException &e){
        h.throwException(e);
        return 0; // keep compiler happy. code never gets here
    }
}