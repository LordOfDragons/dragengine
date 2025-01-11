#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

#include "FDFileReader.h"
#include <dragengine/common/exceptions.h>
#include <dragengine/common/utils/decDateTime.h>

FDFileReader::FDFileReader(const char *filename, int fileDescriptor, long offset,
                           long length, const FDProducer::Ref &producer) :
pFilename(filename),
pFileDescriptor(fileDescriptor),
pOffset(offset),
pLength(length),
pModificationTime(decDateTime::GetSystemTime()),
pProducer(producer)
{
    DEASSERT_TRUE(fileDescriptor != 0)
    DEASSERT_NOTNULL(producer)

    if(lseek(fileDescriptor, offset, SEEK_SET) == -1){
        DETHROW_INFO(deeReadFile, "file descriptor does not support seeking");
    }
}

FDFileReader::~FDFileReader(){
    close(pFileDescriptor);
}

const char *FDFileReader::GetFilename(){
    return pFilename;
}

int FDFileReader::GetLength(){
    return (int)pLength;
}

TIME_SYSTEM FDFileReader::GetModificationTime(){
    return pModificationTime;
}

int FDFileReader::GetPosition(){
    const long position = lseek(pFileDescriptor, 0, SEEK_CUR);
    DEASSERT_TRUE(position >= pOffset)
    DEASSERT_TRUE(position <= pOffset + pLength)
    return (int)(position - pOffset);
}

void FDFileReader::SetPosition(int position){
    DEASSERT_TRUE(position >= 0)
    DEASSERT_TRUE(position <= (int)pLength)

    if(lseek(pFileDescriptor, pOffset + (long)position, SEEK_SET) == -1){
        DETHROW_INFO(deeReadFile, "file descriptor does not support seeking");
    }
}

void FDFileReader::MovePosition(int offset){
    long position = lseek(pFileDescriptor, 0, SEEK_CUR);
    if(position == -1){
        DETHROW_INFO(deeReadFile, "file descriptor does not support seeking");
    }

    position += (long)offset;
    DEASSERT_TRUE(position >= pOffset)
    DEASSERT_TRUE(position <= pOffset + pLength)

    if(lseek(pFileDescriptor, position, SEEK_SET) == -1){
        DETHROW_INFO(deeReadFile, "file descriptor does not support seeking");
    }
}

void FDFileReader::SetPositionEnd(int position){
    DEASSERT_TRUE(position <= 0)
    DEASSERT_TRUE(position >= -(int)pLength)

    if(lseek(pFileDescriptor, pOffset + pLength + (long)position, SEEK_SET) == -1){
        DETHROW_INFO(deeReadFile, "file descriptor does not support seeking");
    }
}

void FDFileReader::Read(void *buffer, int size){
    const long position = lseek(pFileDescriptor, 0, SEEK_CUR);
    const long eof = pOffset + pLength;
    DEASSERT_TRUE(position >= pOffset)
    DEASSERT_TRUE(position <= eof)

    const int readBytes = (int)read(pFileDescriptor, buffer,
    decMath::min(size, (int)(eof - position)));
    DEASSERT_TRUE(readBytes == size) // error(-1), eof(<size)
}

decBaseFileReader::Ref FDFileReader::Duplicate() {
    return Ref::New(new FDFileReader(GetFilename(),
        pProducer->CreateFileDescriptor(),
        pOffset, pLength, pProducer));
}
