#ifndef DELAUNCHER_JNIHELPER_H
#define DELAUNCHER_JNIHELPER_H

#include <jni.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>

class JniObjectClass;
class JniClass;
class JniField;
class JniFieldString;
class JniFieldInt;
class JniFieldPointer;
class JniFieldFloat;
class JniFieldBool;
class JniFieldObject;
class JniFieldObjectArray;
class JniFieldByteArray;
class JniObject;

#define JPATH_BASE "ch/dragondreams/delauncher/launcher/internal/"

/**
 * Class.
 */
class JniClass{
protected:
    JNIEnv *pEnv;
    decString pName;
    jclass pClass;
    jmethodID pMethodNew;
    bool pGlobalRef;

protected:
    JniClass(JNIEnv *env, jclass clazz, const char *name, bool globalRef);

public:
    JniClass(JNIEnv *env, const char *name, bool globalRef = false);
    void Dispose(JNIEnv *env);

    inline operator jclass() const{ return pClass; }
    [[nodiscard]] inline JNIEnv *GetEnv() const{ return pEnv; }
    [[nodiscard]] inline const decString &GetName() const{ return pName; }

    JniFieldString GetFieldString(const char *name) const;
    JniFieldInt GetFieldInt(const char *name) const;
    JniFieldPointer GetFieldPointer(const char *name) const;
    JniFieldFloat GetFieldFloat(const char *name) const;
    JniFieldBool GetFieldBool(const char *name) const;
    JniFieldObject GetFieldObject(const char *name, const char *itemSig) const;
    JniFieldObject GetFieldObject(const char *name, const JniClass &itemClass) const;
    JniFieldObjectArray GetFieldObjectArray(const char *name, const char *itemSig) const;
    JniFieldObjectArray GetFieldObjectArray(const char *name, const JniClass &itemClass) const;
    JniFieldByteArray GetFieldByteArray(const char *name) const;

    /**
     * Create instance of class using default constructor.
     */
    JniObject New();
    JniObject New(JNIEnv *env);
};

/**
 * Actual object class. Released in destructor.
 */
class JniObjectClass : public JniClass{
private:
    JNIEnv *pEnv;
    jclass pClass;

public:
    JniObjectClass(JNIEnv *env, jobject object);
    ~JniObjectClass();
};

/**
 * Class field.
 */
class JniField{
protected:
    JNIEnv *pEnv;
    jclass pClass;
    jfieldID pId;

public:
    JniField(JNIEnv *env, jclass clazz, const char *name, const char *signature);

    inline operator jfieldID() const{ return pId; }
};

/**
 * String class field.
 */
class JniFieldString : public JniField{
public:
    JniFieldString(JNIEnv *env, jclass clazz, const char *name);

    decString Get(jobject object) const;
    decUnicodeString GetUnicode(jobject object) const;
    void Set(jobject object, const decString &value) const;
    void Set(JNIEnv *env, jobject object, const decString &value) const;
    void Set(jobject object, const decUnicodeString &value) const;
    void Set(JNIEnv *env, jobject object, const decUnicodeString &value) const;
};

/**
 * Integer class field.
 */
class JniFieldInt : public JniField{
public:
    JniFieldInt(JNIEnv *env, jclass clazz, const char *name);

    int Get(jobject object) const;
    void Set(jobject object, int value) const;
};

/**
 * Pointer class field.
 */
class JniFieldPointer : public JniField{
public:
    JniFieldPointer(JNIEnv *env, jclass clazz, const char *name);

    void *Get(jobject object) const;
    void Set(jobject object, void *value) const;
};

/**
 * Float class field.
 */
class JniFieldFloat : public JniField{
public:
    JniFieldFloat(JNIEnv *env, jclass clazz, const char *name);

    float Get(jobject object) const;
    void Set(jobject object, float value) const;
};

/**
 * Bool class field.
 */
class JniFieldBool : public JniField{
public:
    JniFieldBool(JNIEnv *env, jclass clazz, const char *name);

    bool Get(jobject object) const;
    void Set(jobject object, bool value) const;
};

/**
 * Object class field.
 */
class JniFieldObject : public JniField{
public:
    JniFieldObject(JNIEnv *env, jclass clazz, const char *name, const char *itemSig);

    jobject Get(jobject object) const;
    void Set(jobject object, jobject value) const;
};

/**
 * Object array class field.
 */
class JniFieldObjectArray : public JniField{
public:
    JniFieldObjectArray(JNIEnv *env, jclass clazz, const char *name, const char *itemSig);

    jobjectArray Get(jobject object) const;
    void Set(jobject object, jobjectArray value) const;
};

/**
 * Byte array class field.
 */
class JniFieldByteArray : public JniField{
public:
    JniFieldByteArray(JNIEnv *env, jclass clazz, const char *name);

    jbyteArray Get(jobject object) const;
    void Set(jobject object, jbyteArray value) const;
};

/**
 * Held object. Released in destructor.
 */
class JniObject{
protected:
    JNIEnv *pEnv;
    jobject pObject;

public:
    JniObject(JNIEnv *env, jobject object);
    explicit JniObject(const JniObject &object);
    virtual ~JniObject();

    inline operator jobject() const{ return pObject; }
    jobject ReturnValue();
    inline jobject CallArgument() const{ return pObject; }
};

/**
 * Held array object. Released in destructor.
 */
class JniObjectArray : public JniObject{
private:
    int pCount;

public:
    JniObjectArray(JNIEnv *env, jclass itemClass, int itemCount);
    JniObjectArray(JNIEnv *env, jobjectArray object);
    explicit JniObjectArray(const JniObjectArray &object);

    inline operator jobjectArray() const{ return reinterpret_cast<jobjectArray>(pObject); }

    [[nodiscard]] inline int GetCount() const{ return pCount; }
    jobject GetAt(int index) const;
    void SetAt(int index, jobject object) const;
    jobjectArray ReturnArray();
};

/**
 * Held array object. Released in destructor.
 */
class JniByteArray : public JniObject{
private:
    int pSize;

public:
    JniByteArray(JNIEnv *env, int size);
    explicit JniByteArray(const JniByteArray &object);

    inline operator jbyteArray() const{ return reinterpret_cast<jbyteArray>(pObject); }

    void Set(const void *data) const;
    jbyteArray ReturnArray();
};

/**
 * Held array object. Released in destructor.
 */
class JniLongArray : public JniObject{
private:
    int pSize;

public:
    JniLongArray(JNIEnv *env, int size);
    explicit JniLongArray(const JniLongArray &object);

    inline operator jlongArray() const{ return reinterpret_cast<jlongArray>(pObject); }

    void SetAt(int index, jlong value) const;
    jlongArray ReturnArray();
};

/**
 * Helpers
 */
class JniHelpers{
private:
    JNIEnv *pEnv;

public:
    explicit JniHelpers(JNIEnv *env);

    /**
     * Convert string.
     */
    decString convertString(jstring in);
    jstring convertString(const char *in);

    /**
     * Convert unicode string.
     */
    decUnicodeString convertUnicodeString(jstring in);
    jstring convertUnicodeString(const decUnicodeString &in);

    /**
     * Throw Drag[en]gine exception as java exception.
     */
    void throwException(const deException &exception);

    /**
     * Log Drag[en]gine exception.
     */
    static void logException(const deException &exception);
};

#endif //DELAUNCHER_JNIHELPER_H
