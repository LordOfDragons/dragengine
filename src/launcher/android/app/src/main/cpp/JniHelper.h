#ifndef DELAUNCHER_JNIHELPER_H
#define DELAUNCHER_JNIHELPER_H

#include <jni.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>

class JniObjectClass;
class JniClass;
class JniField;
class JniFieldString;
class JniFieldInt;
class JniFieldFloat;
class JniFieldBool;
class JniFieldObject;
class JniFieldObjectArray;
class JniFieldByteArray;
class JniObject;

#define JPATH_BASE "ch/dragondreams/delauncher/launcher/internal/"

/**
 * Global class.
 */
class JniClass{
protected:
    JNIEnv *pEnv;
    decString pName;
    jclass pClass;
    jmethodID pMethodNew;

protected:
    JniClass(JNIEnv *env, jclass clazz, const char *name);

public:
    JniClass(JNIEnv *env, const char *name);

    inline operator jclass() const{ return pClass; }
    inline JNIEnv *GetEnv() const{ return pEnv; }
    inline const decString &GetName() const{ return pName; }

    JniFieldString GetFieldString(const char *name) const;
    JniFieldInt GetFieldInt(const char *name) const;
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
};

/**
 * Actual object class. Released in destructor.
 */
class JniObjectClass : public JniClass{
private:
    JNIEnv *pEnv;
    jclass pClass;

public:
    explicit JniObjectClass(JNIEnv *env, jobject object);
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
    void Set(jobject object, const decUnicodeString &value) const;
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
};

/**
 * Held array object. Released in destructor.
 */
class JniObjectArray : public JniObject{
public:
    JniObjectArray(JNIEnv *env, jclass itemClass, int itemCount);
    JniObjectArray(JNIEnv *env, jobject object);
    explicit JniObjectArray(const JniObjectArray &object);

    inline operator jobjectArray() const{ return reinterpret_cast<jobjectArray>(pObject); }

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
    jstring convertString(const decString &in);

    /**
     * Convert unicode string.
     */
    decUnicodeString convertUnicodeString(jstring in);
    jstring convertUnicodeString(const decUnicodeString &in);
};

#endif //DELAUNCHER_JNIHELPER_H
