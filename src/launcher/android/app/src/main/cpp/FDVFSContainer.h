#ifndef DELAUNCHER_FDVFSCONTAINER_H
#define DELAUNCHER_FDVFSCONTAINER_H

#include <unordered_map>
#include <string>
#include <memory>
#include <dragengine/filesystem/deVFSContainer.h>

class FDVFSContainer : public deVFSContainer{
public:
    typedef deTObjectReference<FDVFSContainer> Ref;

private:
    class Entry{
    public:
        typedef std::shared_ptr<Entry> Ref;

    private:
        int pFileDescriptor;
        int pOffset;
        int pSize;
        TIME_SYSTEM pModidicationTime;

    public:
        Entry(int fd, int offset, int size);
        ~Entry();

        inline int GetFileDescriptor() const{ return pFileDescriptor; }
        inline int GetOffset() const{ return pOffset; }
        inline int GetSize() const{ return pSize; }
        inline TIME_SYSTEM GetModificationTime() const{ return pModidicationTime; }
    };

    typedef std::unordered_map<std::string,Entry::Ref> MapEntries;

    MapEntries pEntries;

public:
    FDVFSContainer();
    explicit FDVFSContainer(const decPath &rootPath);

    bool ExistsFile(const decPath &path) override;
    bool CanReadFile(const decPath &path) override;
    bool CanWriteFile(const decPath &path) override;
    bool CanDeleteFile(const decPath &path) override;
    decBaseFileReader *OpenFileForReading(const decPath &path) override;
    decBaseFileWriter *OpenFileForWriting(const decPath &path) override;
    void DeleteFile(const decPath &path) override;
    void TouchFile(const decPath &path) override;
    void SearchFiles(const decPath &directory, deContainerFileSearch &searcher) override;
    eFileTypes GetFileType(const decPath &path) override;
    uint64_t GetFileSize(const decPath &path) override;
    TIME_SYSTEM GetFileModificationTime(const decPath &path) override;

    int GetEntryCount() const;
    bool HasEntry(const std::string &path);
    void AddEntry(const std::string &path, int fileDescriptor, int offset, int size);
    void RemoveEntry(const std::string &path);
};

#endif //DELAUNCHER_FDVFSCONTAINER_H
