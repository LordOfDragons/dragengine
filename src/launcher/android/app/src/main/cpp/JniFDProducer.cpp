#include "JniFDProducer.h"

extern JavaVM *vJavaVM;

JniFDProducer::JniFDProducer(JNIEnv *env, jobject objProducer) :
pObjProducer(env, objProducer, true),
pClsProducer(pObjProducer),
pMetProduceFileDescriptor(env->GetMethodID(
    pClsProducer,"produceFileDescriptor", "()I")){
}

JniFDProducer::~JniFDProducer() {
    JNIEnv *env = nullptr;
    vJavaVM->AttachCurrentThread(&env, nullptr);
    pObjProducer.Dispose(env);
}

int JniFDProducer::CreateFileDescriptor() {
    JNIEnv *env = nullptr;
    vJavaVM->AttachCurrentThread(&env, nullptr);
    const int fd = (int)env->CallIntMethod(pObjProducer, pMetProduceFileDescriptor);
    DEASSERT_NOTNULL(fd)
    return fd;
}
