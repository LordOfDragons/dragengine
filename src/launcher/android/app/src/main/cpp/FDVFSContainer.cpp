#include <unistd.h>
#include <unordered_set>
#include "FDVFSContainer.h"
#include "FDFileReader.h"
#include <dragengine/filesystem/deContainerFileSearch.h>

// class FDVFSContainer::Entry
////////////////////////////////

FDVFSContainer::Entry::Entry(const FDProducer::Ref &producer, int offset, int size) :
pProducer(producer),
pOffset(offset),
pSize(size),
pModidicationTime(decDateTime::GetSystemTime()){
}


// class FDVFSContainer
/////////////////////////

FDVFSContainer::FDVFSContainer() = default;

FDVFSContainer::FDVFSContainer(const decPath &rootPath) : deVFSContainer(rootPath){
}

bool FDVFSContainer::ExistsFile(const decPath &path){
    return pEntries.find(path.GetPathUnix().GetString()) != pEntries.cend();
}

bool FDVFSContainer::CanReadFile(const decPath &path){
    return ExistsFile(path);
}

bool FDVFSContainer::CanWriteFile(const decPath &path) {
    return false;
}

bool FDVFSContainer::CanDeleteFile(const decPath &path) {
    return false;
}

decBaseFileReader::Ref FDVFSContainer::OpenFileForReading(const decPath &path) {
    const MapEntries::const_iterator iter(pEntries.find(path.GetPathUnix().GetString()));
    DEASSERT_TRUE(iter != pEntries.cend())

    const FDProducer::Ref &producer = iter->second->GetProducer();
    return FDFileReader::Ref::New(iter->first.c_str(),
                                      producer->CreateFileDescriptor(),
                                      iter->second->GetOffset(),
                                      iter->second->GetSize(), producer);
}

decBaseFileWriter::Ref FDVFSContainer::OpenFileForWriting(const decPath &path) {
    DETHROW(deeInvalidAction);
}

void FDVFSContainer::DeleteFile(const decPath &path){
    DETHROW(deeInvalidAction);
}

void FDVFSContainer::TouchFile(const decPath &path){
    DETHROW(deeInvalidAction);
}

void FDVFSContainer::SearchFiles(const decPath &directory, deContainerFileSearch &searcher){
    std::unordered_set<std::string> entryDirs;
    const std::string searchDir(directory.GetPathUnix() + "/");
    const size_t searchDirLen = searchDir.length();

    for(const auto &entry : pEntries){
        if(entry.first.compare(0, searchDirLen, searchDir) != 0){
            continue;
        }

        const size_t deli = entry.first.find('/', searchDirLen);
        if(deli == std::string::npos){
            searcher.Add(entry.first.substr(searchDirLen).c_str(), deVFSContainer::eftRegularFile);
        }else{
            entryDirs.insert(entry.first.substr(searchDirLen, deli - searchDirLen));
        }
    }

    for(const auto &entryDir : entryDirs){
        searcher.Add(entryDir.c_str(), deVFSContainer::eftDirectory);
    }
}

deVFSContainer::eFileTypes FDVFSContainer::GetFileType(const decPath &path) {
    DEASSERT_TRUE(ExistsFile(path))
    return deVFSContainer::eftRegularFile;
}

uint64_t FDVFSContainer::GetFileSize(const decPath &path){
    const MapEntries::const_iterator iter(pEntries.find(path.GetPathUnix().GetString()));
    DEASSERT_TRUE(iter != pEntries.cend())
    return iter->second->GetSize();
}

TIME_SYSTEM FDVFSContainer::GetFileModificationTime(const decPath &path){
    const MapEntries::const_iterator iter(pEntries.find(path.GetPathUnix().GetString()));
    DEASSERT_TRUE(iter != pEntries.cend())
    return iter->second->GetModificationTime();
}

int FDVFSContainer::GetEntryCount() const {
    return (int)pEntries.size();
}

bool FDVFSContainer::HasEntry(const std::string &path) {
    return pEntries.find(path) != pEntries.cend();
}

void FDVFSContainer::AddEntry(const std::string &path, const FDProducer::Ref &producer, int offset, int size){
    DEASSERT_NOTNULL(producer)
    DEASSERT_TRUE(offset >= 0)
    DEASSERT_TRUE(size >= 0)
    DEASSERT_FALSE(path.empty())
    DEASSERT_TRUE(path[0] == '/')
    DEASSERT_FALSE(path == "/")
    DEASSERT_FALSE(HasEntry(path))
    pEntries[path] = std::make_shared<Entry>(producer, offset, size);
}

void FDVFSContainer::RemoveEntry(const std::string &path){
    const MapEntries::const_iterator iter(pEntries.find(path));
    DEASSERT_TRUE(iter != pEntries.cend())
    pEntries.erase(iter);
}
