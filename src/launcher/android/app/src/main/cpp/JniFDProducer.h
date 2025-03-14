#ifndef DELAUNCHER_JNIFDPRODUCER_H
#define DELAUNCHER_JNIFDPRODUCER_H

#include "JniHelper.h"
#include "FDProducer.h"

class JniFDProducer : public FDProducer{
private:
    JniObject pObjProducer;
    JniObjectClass pClsProducer;
    jmethodID pMetProduceFileDescriptor;

public:
    JniFDProducer(JNIEnv *env, jobject objProducer);
    ~JniFDProducer() override;

    /**
     * Create new file descriptor. File descriptor has to be unique with its
     * own file position and state.
     */
    int CreateFileDescriptor() override;
};

#endif //DELAUNCHER_JNIFDPRODUCER_H
