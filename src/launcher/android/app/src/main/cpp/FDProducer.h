#ifndef DELAUNCHER_FDPRODUCER_H
#define DELAUNCHER_FDPRODUCER_H

#include <memory>

/**
 * Interface knowing how to produce file descriptors
 * for use with FDVFSContainer and FDFileReader.
 */
class FDProducer {
public:
    typedef std::shared_ptr<FDProducer> Ref;

    virtual ~FDProducer() = default;

    /**
     * Create new file descriptor. File descriptor has to be unique with its
     * own file position and state.
     */
    virtual int CreateFileDescriptor() = 0;
};

#endif //DELAUNCHER_FDPRODUCER_H
