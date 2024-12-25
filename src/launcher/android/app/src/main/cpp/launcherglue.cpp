#include <jni.h>
#include <android/native_window_jni.h>
#include <android/native_window_jni.h>

#include <delauncher/delLauncher.h>

struct GlueLauncherConfig {
    jstring loggerSource;
    jstring engineLogFileTitle;
    jstring pathLauncher;
    jstring pathLauncherGames;
    jstring pathLauncherConfig;
    jstring pathEngine;
    jstring pathEngineConfig;
    jstring pathEngineCache;
    jobject surface;
};

static void jstringToDEString(JNIEnv *env, const jstring in, decString &out){
    const char * const ns = env->GetStringUTFChars(in, 0);
    out = ns;
    env->ReleaseStringUTFChars(in, ns);
}

static void jstringToDEString(JNIEnv *env, jclass clazz, jobject object, const char *fieldName, decString &out){
    jfieldID fid = env->GetFieldID(clazz, fieldName,"Ljava/lang/String;");
    auto js = reinterpret_cast<jstring>(env->GetObjectField(object, fid));
    if(js) {
        jstringToDEString(env, js, out);
    }
}

static JavaVM *vJavaVM = nullptr;

jint JNI_OnLoad(JavaVM *vm, void *reserved){
    vJavaVM = vm;
    return JNI_VERSION_1_4;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_ch_dragondreams_delauncher_launcher_DragengineLauncher_00024GlueLauncher_createLauncher(
        JNIEnv *env, jobject thiz, jobject config) {
    jclass clsConfig = env->GetObjectClass(config);

    delLauncher::sConfig delConfig{};

    jstringToDEString(env, clsConfig, config, "loggerSource", delConfig.loggerSource);
    jstringToDEString(env, clsConfig, config, "engineLogFileTitle", delConfig.engineLogFileTitle);
    jstringToDEString(env, clsConfig, config, "pathLauncher", delConfig.pathLauncher);
    jstringToDEString(env, clsConfig, config, "pathLauncherGames", delConfig.pathGames);
    jstringToDEString(env, clsConfig, config, "pathLauncherConfig", delConfig.pathConfig);

    jstringToDEString(env, clsConfig, config, "pathEngine", delConfig.osConfig.pathEngine);
    jstringToDEString(env, clsConfig, config, "pathEngineConfig", delConfig.osConfig.pathConfig);
    jstringToDEString(env, clsConfig, config, "pathEngineCache", delConfig.osConfig.pathCache);

    delConfig.osConfig.javavm = vJavaVM;

    {
        jfieldID fid = env->GetFieldID(clsConfig, "surface", "Landroid/view/SurfaceView;");
        auto js = reinterpret_cast<jobject>(env->GetObjectField(config, fid));
        delConfig.osConfig.nativeWindow = ANativeWindow_fromSurface(env, js);
        //ANativeWindow_release();
    }

    return (jlong)(intptr_t)new delLauncher(delConfig);
}

extern "C"
JNIEXPORT void JNICALL
Java_ch_dragondreams_delauncher_launcher_DragengineLauncher_00024GlueLauncher_destroyLauncher(
        JNIEnv *env, jobject thiz, jlong launcher) {
    delete (delLauncher*)(intptr_t)launcher;
}
