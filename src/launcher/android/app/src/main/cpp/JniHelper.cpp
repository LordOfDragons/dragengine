#include "JniHelper.h"
#include <dragengine/common/exceptions.h>
#include <android/log.h>

// JniClass
//////////////

JniClass::JniClass(JNIEnv *env, const char *name) :
pEnv(env),
pName(name),
pClass(env->FindClass(name)),
pMethodNew(0)
{
    DEASSERT_NOTNULL(pClass)
}

JniClass::JniClass(JNIEnv *env, jclass clazz, const char *name) :
pEnv(env),
pName(name),
pClass(clazz),
pMethodNew(0)
{
    DEASSERT_NOTNULL(clazz)
}

JniFieldString JniClass::GetFieldString(const char *name) const {
    return {pEnv, pClass, name};
}

JniFieldInt JniClass::GetFieldInt(const char *name) const {
    return {pEnv, pClass, name};
}

JniFieldPointer JniClass::GetFieldPointer(const char *name) const {
    return {pEnv, pClass, name};
}

JniFieldFloat JniClass::GetFieldFloat(const char *name) const {
    return {pEnv, pClass, name};
}

JniFieldBool JniClass::GetFieldBool(const char *name) const {
    return {pEnv, pClass, name};
}

JniFieldObject JniClass::GetFieldObject(const char *name, const char *itemSig) const {
    return {pEnv, pClass, name, itemSig};
}

JniFieldObject JniClass::GetFieldObject(const char *name, const JniClass &itemClass) const {
    decString itemSig;
    itemSig.Format("L%s;", itemClass.GetName().GetString());
    return {pEnv, pClass, name, itemSig};
}

JniFieldObjectArray JniClass::GetFieldObjectArray(const char *name, const char *itemSig) const {
    return {pEnv, pClass, name, itemSig};
}

JniFieldObjectArray JniClass::GetFieldObjectArray(const char *name, const JniClass &itemClass) const {
    decString itemSig;
    itemSig.Format("[L%s;", itemClass.GetName().GetString());
    return {pEnv, pClass, name, itemSig};
}

JniFieldByteArray JniClass::GetFieldByteArray(const char *name) const {
    return {pEnv, pClass, name};
}


JniObject JniClass::New(){
    if(!pMethodNew) {
        pMethodNew = pEnv->GetMethodID(pClass, "<init>", "()V");
    }
    return {pEnv, pEnv->NewObject(pClass, pMethodNew)};
}

// JniObjectClass
///////////////////

JniObjectClass::JniObjectClass(JNIEnv *env, jobject object) :
JniClass(env, env->GetObjectClass(object), "java/lang/Object")
{
    // segfaults all the time. stupid JVM U_U
    /*
    __android_log_print(ANDROID_LOG_ERROR, "Dummy", "Checkpoint %d %p", __LINE__, pClass);
    jmethodID metGetClass = env->GetMethodID(pClass, "getClass", "()Ljava/lang/Class;");
    __android_log_print(ANDROID_LOG_ERROR, "Dummy", "Checkpoint %d", __LINE__);
    jobject objClass = env->CallObjectMethod(object, metGetClass);
    __android_log_print(ANDROID_LOG_ERROR, "Dummy", "Checkpoint %d", __LINE__);

    jclass clsClass = env->GetObjectClass(objClass);
    __android_log_print(ANDROID_LOG_ERROR, "Dummy", "Checkpoint %d", __LINE__);
    jmethodID metGetName = env->GetMethodID(clsClass, "getName", "()Ljava/lang/String;");
    __android_log_print(ANDROID_LOG_ERROR, "Dummy", "Checkpoint %d", __LINE__);

    pName = JniHelpers(env).convertString(reinterpret_cast<jstring>(
            env->CallObjectMethod(objClass, metGetName)));
    */
}

JniObjectClass::~JniObjectClass() {
    if(pClass){
        //pEnv->DeleteLocalRef(pClass); // segfaults... why?!
    }
}


// JniField
///////////////

JniField::JniField(JNIEnv *env, jclass clazz, const char *name, const char *signature) :
pEnv(env),
pClass(clazz),
pId(env->GetFieldID(clazz, name, signature))
{
    DEASSERT_NOTNULL(pId)
}


// JniString
//////////////

JniFieldString::JniFieldString(JNIEnv *env, jclass clazz, const char *name) :
JniField(env, clazz, name, "Ljava/lang/String;"){
}

decString JniFieldString::Get(jobject object) const {
    return JniHelpers(pEnv).convertString(reinterpret_cast<jstring>(
        pEnv->GetObjectField(object, pId)));
}

void JniFieldString::Set(jobject object, const decString &value) const {
    pEnv->SetObjectField(object, pId, JniHelpers(pEnv).convertString(value));
}

decUnicodeString JniFieldString::GetUnicode(jobject object) const {
    return JniHelpers(pEnv).convertUnicodeString(reinterpret_cast<jstring>(
         pEnv->GetObjectField(object, pId)));
}

void JniFieldString::Set(jobject object, const decUnicodeString &value) const {
    pEnv->SetObjectField(object, pId, JniHelpers(pEnv).convertUnicodeString(value));
}


// JniFieldInt
////////////////

JniFieldInt::JniFieldInt(JNIEnv *env, jclass clazz, const char *name) :
JniField(env, clazz, name, "I"){
}

int JniFieldInt::Get(jobject object) const {
    return pEnv->GetIntField(object, pId);
}

void JniFieldInt::Set(jobject object, int value) const {
    pEnv->SetIntField(object, pId, value);
}


// JniFieldPointer
////////////////////

JniFieldPointer::JniFieldPointer(JNIEnv *env, jclass clazz, const char *name) :
JniField(env, clazz, name, "J"){
}

void *JniFieldPointer::Get(jobject object) const {
    return (void*)(intptr_t)pEnv->GetLongField(object, pId);
}

void JniFieldPointer::Set(jobject object, void *value) const {
    pEnv->SetLongField(object, pId, (jlong)(intptr_t)value);
}


// JniFieldFloat
//////////////////

JniFieldFloat::JniFieldFloat(JNIEnv *env, jclass clazz, const char *name) :
JniField(env, clazz, name, "F"){
}

float JniFieldFloat::Get(jobject object) const {
    return pEnv->GetFloatField(object, pId);
}

void JniFieldFloat::Set(jobject object, float value) const {
    pEnv->SetFloatField(object, pId, value);
}


// JniFieldBool
/////////////////

JniFieldBool::JniFieldBool(JNIEnv *env, jclass clazz, const char *name) :
JniField(env, clazz, name, "Z"){
}

bool JniFieldBool::Get(jobject object) const {
    return pEnv->GetBooleanField(object, pId) == JNI_TRUE;
}

void JniFieldBool::Set(jobject object, bool value) const {
    pEnv->SetBooleanField(object, pId, value ? JNI_TRUE : JNI_FALSE);
}


// JniFieldObject
///////////////////

JniFieldObject::JniFieldObject(JNIEnv *env, jclass clazz, const char *name, const char *itemSig) :
JniField(env, clazz, name, itemSig){
}

jobject JniFieldObject::Get(jobject object) const {
    return pEnv->GetObjectField(object, pId);
}

void JniFieldObject::Set(jobject object, jobject value) const {
    pEnv->SetObjectField(object, pId, value);
}


// JniFieldObjectArray
////////////////////////

JniFieldObjectArray::JniFieldObjectArray(JNIEnv *env, jclass clazz, const char *name, const char *itemSig) :
JniField(env, clazz, name, itemSig){
}

jobjectArray JniFieldObjectArray::Get(jobject object) const {
    return reinterpret_cast<jobjectArray>(pEnv->GetObjectField(object, pId));
}

void JniFieldObjectArray::Set(jobject object, jobjectArray value) const {
    pEnv->SetObjectField(object, pId, value);
}


// JniFieldByteArray
//////////////////////

JniFieldByteArray::JniFieldByteArray(JNIEnv *env, jclass clazz, const char *name) :
JniField(env, clazz, name, "[B"){
}

jbyteArray JniFieldByteArray::Get(jobject object) const {
    return reinterpret_cast<jbyteArray>(pEnv->GetObjectField(object, pId));
}

void JniFieldByteArray::Set(jobject object, jbyteArray value) const {
    pEnv->SetObjectField(object, pId, value);
}


// JniObject
//////////////

JniObject::JniObject(JNIEnv *env, jobject object) :
pEnv(env),
pObject(object)
{
    DEASSERT_NOTNULL(object)
}

JniObject::~JniObject(){
    if(pObject){
        pEnv->DeleteLocalRef(pObject);
    }
}

JniObject::JniObject(const JniObject &object) :
pEnv(object.pEnv),
pObject(object.pObject)
{
    pEnv->NewLocalRef(pObject);
}

jobject JniObject::ReturnValue() {
    jobject object = pObject;
    pObject = nullptr;
    return object;
}


// JniObjectArray
///////////////////

JniObjectArray::JniObjectArray(JNIEnv *env, jobjectArray object) :
JniObject(env, object),
pCount(env->GetArrayLength(object)){
}

JniObjectArray::JniObjectArray(JNIEnv *env, jclass itemClass, int itemCount) :
JniObject(env, env->NewObjectArray(itemCount, itemClass, nullptr)),
pCount(itemCount){
}

JniObjectArray::JniObjectArray(const JniObjectArray &object) = default;

jobject JniObjectArray::GetAt(int index) const{
    DEASSERT_TRUE(index >= 0)
    DEASSERT_TRUE(index < pCount)
    return pEnv->GetObjectArrayElement(reinterpret_cast<jobjectArray>(pObject), (jsize)index);
}

void JniObjectArray::SetAt(int index, jobject object) const{
    pEnv->SetObjectArrayElement(reinterpret_cast<jobjectArray>(pObject), index, object);
}

jobjectArray JniObjectArray::ReturnArray() {
    return reinterpret_cast<jobjectArray>(ReturnValue());
}


// JniByteArray
/////////////////

JniByteArray::JniByteArray(JNIEnv *env, int size) :
JniObject(env, env->NewByteArray(size)),
pSize(size){
}

JniByteArray::JniByteArray(const JniByteArray &object) = default;

void JniByteArray::Set(const void *data) const{
    pEnv->SetByteArrayRegion(reinterpret_cast<jbyteArray>(pObject), 0, pSize, (const jbyte*)data);
}

jbyteArray JniByteArray::ReturnArray() {
    return reinterpret_cast<jbyteArray>(ReturnValue());
}


// JniLongArray
/////////////////

JniLongArray::JniLongArray(JNIEnv *env, int size) :
JniObject(env, env->NewLongArray(size)),
pSize(size){
}

JniLongArray::JniLongArray(const JniLongArray &object) = default;

void JniLongArray::SetAt(int index, jlong value) const{
    pEnv->SetLongArrayRegion(reinterpret_cast<jlongArray>(pObject), index, 1, &value);
}

jlongArray JniLongArray::ReturnArray() {
    return reinterpret_cast<jlongArray>(ReturnValue());
}


// JniHelpers
///////////////

JniHelpers::JniHelpers(JNIEnv *env) :
pEnv(env){
}

decString JniHelpers::convertString(jstring in) {
    const char * const ns = pEnv->GetStringUTFChars(in, nullptr);
    const decString out(ns);
    pEnv->ReleaseStringUTFChars(in, ns);
    return out;
}

jstring JniHelpers::convertString(const decString &in) {
    return pEnv->NewStringUTF(in);
}

decUnicodeString JniHelpers::convertUnicodeString(jstring in) {
    return decUnicodeString::NewFromUTF8(convertString(in));
}

jstring JniHelpers::convertUnicodeString(const decUnicodeString &in) {
    return convertString(in.ToUTF8());
}

void JniHelpers::throwException(const deException &exception){
    jclass exClass;
    const char * const className = "java/lang/RuntimeException";

    exClass = pEnv->FindClass(className);
    DEASSERT_NOTNULL(exClass)

    const decStringList lines(exception.FormatOutput());
    const int count = lines.GetCount();
    int i;
    for(i=0; i<count; i++){
        __android_log_print(ANDROID_LOG_ERROR, "Launcher", "%s", lines.GetAt(i).GetString());
    }

    pEnv->ThrowNew(exClass, exception.GetDescription().GetString());
}
