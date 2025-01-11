#ifndef DELAUNCHER_FDFILEREADER_H
#define DELAUNCHER_FDFILEREADER_H

#include <dragengine/common/file/decBaseFileReader.h>
#include "FDProducer.h"

class FDFileReader : public decBaseFileReader {
private:
    decString pFilename;
    int pFileDescriptor;
    long pOffset;
    long pLength;
    TIME_SYSTEM pModificationTime;
    FDProducer::Ref pProducer;

public:
    FDFileReader(const char *filename, int fileDescriptor, long offset, long length,
                 const FDProducer::Ref &producer);

protected:
    ~FDFileReader() override;
    /*@}*/

public:
    const char *GetFilename() override;
    int GetLength() override;
    TIME_SYSTEM GetModificationTime() override;
    int GetPosition() override;
    void SetPosition(int position) override;
    void MovePosition(int offset) override;
    void SetPositionEnd(int position) override;
    void Read(void *buffer, int size) override;
    Ref Duplicate() override;
    /*@}*/
};

#endif //DELAUNCHER_FDFILEREADER_H
