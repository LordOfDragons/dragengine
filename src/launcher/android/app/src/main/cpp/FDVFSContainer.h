#ifndef DELAUNCHER_FDVFSCONTAINER_H
#define DELAUNCHER_FDVFSCONTAINER_H

#include <unordered_map>
#include <string>
#include <memory>
#include <dragengine/filesystem/deVFSContainer.h>
#include "FDProducer.h"

/**
 * File descriptor container.
 *
 * Each entry is added with a producer subclass able to produce new file descriptor
 * for the same resource each time called. File descriptor are unique having their
 * own file position and state.
 */
class FDVFSContainer : public deVFSContainer{
public:
    typedef deTObjectReference<FDVFSContainer> Ref;
	
	
private:
    class Entry{
    public:
        typedef std::shared_ptr<Entry> Ref;

    private:
        FDProducer::Ref pProducer;
        int pOffset;
        int pSize;
        TIME_SYSTEM pModidicationTime;

    public:
        Entry(const FDProducer::Ref &producer, int offset, int size);

        inline const FDProducer::Ref &GetProducer() const{return pProducer;}
        [[nodiscard]] inline int GetOffset() const{return pOffset;}
        [[nodiscard]] inline int GetSize() const{return pSize;}
        [[nodiscard]] inline TIME_SYSTEM GetModificationTime() const{return pModidicationTime;}
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

    [[nodiscard]] int GetEntryCount() const;
    bool HasEntry(const std::string &path);
    void AddEntry(const std::string &path, const FDProducer::Ref &producer, int offset, int size);
    void RemoveEntry(const std::string &path);
};

#endif //DELAUNCHER_FDVFSCONTAINER_H
