/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdlib.h>
#include <string.h>

#include "dealEngineInstance.h"
#include "dealVFSZipArchive.h"
#include "unzip.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/common/file/decMemoryFileReader.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/filesystem/dePathList.h>
#include <dragengine/filesystem/dePatternList.h>
#include <dragengine/filesystem/deContainerFileSearch.h>
#include <dragengine/logger/deLogger.h>



// Definitions
////////////////

#define LOGSOURCE "DEAndroidLauncher"

#define ZLIB_CASE_SENSITIVE 1



// Callbacks
//////////////

static voidpf ZCALLBACK fZipOpenFileFunc(voidpf opaque, const char *filename, int mode){
	return opaque;
}

static uLong ZCALLBACK fZipReadFileFunc(voidpf opaque, voidpf stream, void *buf, uLong size){
	dealVFSZipArchive &zipArchive = *((dealVFSZipArchive*)opaque);
	
	try{
		zipArchive.GetFileReader().Read(buf, size);
		return size;
		
	}catch(const deException &){
		return -1;
	}
}

static uLong ZCALLBACK fZipWriteFileFunc(voidpf opaque, voidpf stream, const void *buf, uLong size){
	return -1; // not supported
}

static long ZCALLBACK fZipTellFileFunc(voidpf opaque, voidpf stream){
	dealVFSZipArchive &zipArchive = *((dealVFSZipArchive*)opaque);
	
	try{
		return zipArchive.GetFileReader().GetPosition();
		
	}catch(const deException &){
		return -1;
	}
}

static long ZCALLBACK fZipSeekFileFunc(voidpf opaque, voidpf stream, uLong offset, int origin){
	dealVFSZipArchive &zipArchive = *((dealVFSZipArchive*)opaque);
	
	try{
		if(origin == ZLIB_FILEFUNC_SEEK_CUR){
			zipArchive.GetFileReader().MovePosition(offset);
			
		}else if(origin == ZLIB_FILEFUNC_SEEK_END){
			zipArchive.GetFileReader().SetPositionEnd(offset);
			
		}else if(origin == ZLIB_FILEFUNC_SEEK_SET){
			zipArchive.GetFileReader().SetPosition(offset);
			
		}else{
			DETHROW(deeInvalidParam);
		}
		
	}catch(const deException &){
		return -1;
	}
	
	return 0;
}

static int ZCALLBACK fZipCloseFileFunc(voidpf opaque, voidpf stream){
	return 0;
}

static int ZCALLBACK fZipErrorFileFunc(voidpf opaque, voidpf stream){
	return 0; // not implemented
}



// Class dealVFSZipArchive::cArchiveFile
//////////////////////////////////////////

dealVFSZipArchive::cArchiveFile::cArchiveFile(const char *filename,
	const unz_file_pos &archivePosition, uint64_t fileSize, TIME_SYSTEM modificationTime) :
pFilename(filename),
pArchivePosition(archivePosition),
pFileSize(fileSize),
pModificationTime(modificationTime){
}

dealVFSZipArchive::cArchiveFile::~cArchiveFile(){
}



// Class dealVFSZipArchive::cArchiveDirectory
///////////////////////////////////////////////

dealVFSZipArchive::cArchiveDirectory::cArchiveDirectory(const char *filename) :
pFilename(filename){
}

dealVFSZipArchive::cArchiveDirectory::~cArchiveDirectory(){
}



int dealVFSZipArchive::cArchiveDirectory::GetDirectoryCount() const{
	return pDirectories.GetCount();
}

dealVFSZipArchive::cArchiveDirectory *dealVFSZipArchive::cArchiveDirectory::GetDirectoryAt(int index) const{
	return (cArchiveDirectory*)pDirectories.GetAt(index);
}

bool dealVFSZipArchive::cArchiveDirectory::HasDirectoryNamed(const char *filename) const{
	const int count = pDirectories.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		cArchiveDirectory * const directory = (cArchiveDirectory*)pDirectories.GetAt(i);
		if(directory->GetFilename() == filename){
			return true;
		}
	}
	
	return false;
}

dealVFSZipArchive::cArchiveDirectory *dealVFSZipArchive::cArchiveDirectory
::GetDirectoryNamed(const char *filename) const{
	const int count = pDirectories.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		cArchiveDirectory * const directory = (cArchiveDirectory*)pDirectories.GetAt(i);
		if(directory->GetFilename() == filename){
			return directory;
		}
	}
	
	return NULL;
}

dealVFSZipArchive::cArchiveDirectory *dealVFSZipArchive::cArchiveDirectory
::GetOrAddDirectoryNamed(const char *filename){
	cArchiveDirectory * directory = GetDirectoryNamed(filename);
	if(directory){
		return directory;
	}
	
	try{
		directory = new cArchiveDirectory(filename);
		pDirectories.Add(directory);
		directory->FreeReference();
		
	}catch(const deException &){
		if(directory){
			directory->FreeReference();
		}
		throw;
	}
	return directory;
}

dealVFSZipArchive::cArchiveDirectory *dealVFSZipArchive::cArchiveDirectory
::GetDirectoryByPath(const decPath &path){
	const int count = path.GetComponentCount();
	if(count == 0){
		return NULL;
		
	}else if(count == 1){
		return GetDirectoryNamed(path.GetComponentAt(0));
	}
	
	cArchiveDirectory *directory = this;
	int i;
	
	for(i=0; i<count; i++){
		directory = directory->GetDirectoryNamed(path.GetComponentAt(i));
		if(!directory){
			return NULL;
		}
	}
	
	return directory;
}

void dealVFSZipArchive::cArchiveDirectory::AddDirectory(cArchiveDirectory *directory){
	if(!directory || HasDirectoryNamed(directory->GetFilename()) || HasFileNamed(directory->GetFilename())){
		DETHROW(deeInvalidParam);
	}
	pDirectories.Add(directory);
}



int dealVFSZipArchive::cArchiveDirectory::GetFileCount() const{
	return pFiles.GetCount();
}

dealVFSZipArchive::cArchiveFile *dealVFSZipArchive::cArchiveDirectory::GetFileAt(int index) const{
	return (cArchiveFile*)pFiles.GetAt(index);
}

bool dealVFSZipArchive::cArchiveDirectory::HasFileNamed(const char *filename) const{
	const int count = pFiles.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		cArchiveFile * const file = (cArchiveFile*)pFiles.GetAt(i);
		if(file->GetFilename() == filename){
			return true;
		}
	}
	
	return false;
}

dealVFSZipArchive::cArchiveFile *dealVFSZipArchive::cArchiveDirectory
::GetFileNamed(const char *filename) const{
	const int count = pFiles.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		cArchiveFile * const file = (cArchiveFile*)pFiles.GetAt(i);
		if(file->GetFilename() == filename){
			return file;
		}
	}
	
	return NULL;
}

dealVFSZipArchive::cArchiveFile *dealVFSZipArchive::cArchiveDirectory
::GetFileByPath(const decPath &path) const{
	const int count = path.GetComponentCount();
	if(count == 0){
		return NULL;
		
	}else if(count == 1){
		return GetFileNamed(path.GetComponentAt(0));
	}
	
	const cArchiveDirectory *directory = this;
	int i;
	
	for(i=0; i<count-1; i++){
		directory = directory->GetDirectoryNamed(path.GetComponentAt(i));
		if(!directory){
			return NULL;
		}
	}
	
	return directory->GetFileNamed(path.GetComponentAt(count - 1));
}

void dealVFSZipArchive::cArchiveDirectory::AddFile(cArchiveFile *file){
	if(!file || HasDirectoryNamed(file->GetFilename()) || HasFileNamed(file->GetFilename())){
		DETHROW(deeInvalidParam);
	}
	pFiles.Add(file);
}



// Class dealVFSZipArchive
////////////////////////////

// Constructor, destructor
////////////////////////////

dealVFSZipArchive::dealVFSZipArchive(dealEngineInstance &engineInstance,
decBaseFileReader *fileReader, const decPath &pathRoot) :
deVFSContainer(pathRoot),
pEngineInstance(engineInstance),
pFileReader(NULL),
pZipFile(NULL),
pArchiveDirectory(NULL)
{
	if(!fileReader){
		DETHROW(deeInvalidParam);
	}
	
	try{
		pFileReader = fileReader;
		fileReader->AddReference();
		
		pOpenZipFile();
		pBuildFileTable();
		//LogArchiveContent();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

dealVFSZipArchive::~dealVFSZipArchive(){
	pCleanUp();
}



// Management
///////////////

bool dealVFSZipArchive::ExistsFile(const decPath &path){
	if(!pArchiveDirectory){
		return false;
	}
	
	const cArchiveFile * const file = pArchiveDirectory->GetFileByPath(path);
	const cArchiveDirectory * const directory = pArchiveDirectory->GetDirectoryByPath(path);
	/*if( pEngineInstance.GetLogger() ){
		pEngineInstance.GetLogger()->LogInfoFormat(LOGSOURCE, "ExistsFile: %s %s %s",
			path.GetPathUnix().GetString(), file!=NULL?"yes":"no", directory!=NULL?"yes":"no");
	}*/
	return file != NULL || directory != NULL;
}

bool dealVFSZipArchive::CanReadFile(const decPath &path){
	if(!pArchiveDirectory){
		return false;
	}
	
	const cArchiveFile * const file = pArchiveDirectory->GetFileByPath(path);
	/*if( pEngineInstance.GetLogger() ){
		pEngineInstance.GetLogger()->LogInfoFormat(LOGSOURCE, "CanReadFile: %s %s",
			path.GetPathUnix().GetString(), file!=NULL?"yes":"no");
	}*/
	return file != NULL;
}

bool dealVFSZipArchive::CanWriteFile(const decPath &path){
	return false;
}

bool dealVFSZipArchive::CanDeleteFile(const decPath &path){
	return false;
}

decBaseFileReader *dealVFSZipArchive::OpenFileForReading(const decPath &path){
// 	if( pEngineInstance.GetLogger() ){
// 		pEngineInstance.GetLogger()->LogInfoFormat( LOGSOURCE, "OpenFileForReading: %s",
// 			path.GetPathUnix().GetString() );
// 	}
	
	// NOTE this is bad here. we can not keep open multiple file readers since we have
	//      only one file reader. for this to work we would need either multiple file
	//      readers or thread safe manage access to the same zip file. multiple file
	//      readers is not an option since we get one file reader and we can not open
	//      a new one nor duplicate it. thus we need to manage access by multiple
	//      shallow file readers managed to access the same zip file. problem here is
	//      that we need to keep switching between current files in the zip file and
	//      the file pointer ssociated with it. all this is tricky to manage and not
	//      done quickly. the dirty solution right now is to just read the entire
	//      file into a memory file and be done with it. works for small files but is
	//      bad for large files. using the complex solution only for larger files
	//      would be a solution for later
	//      
	// NOTE best solution not requiring complicated duplicating or opening new file
	//      file descriptors works like this. each file small enough is loaded into
	//      memory in a single read. this should be a couepl of hundret kB. for large
	//      files a special new file reader is used being aware of this class. a global
	//      mutex is required on this class for this all to work. these special readers
	//      create a new zip file handler and advance to the file they read. thea also
	//      store a window storing the starting file pointer position, the size of the
	//      window and the window data. the window size should be a couple of hundret kB
	//      to access the file less often. the window is read into the buffer. now if
	//      read data would go outside the window the present data is first copied into
	//      the read buffer. then the mutex is acquired. if acquired the file pointer
	//      is set to the next window which starts at window position plus window size.
	//      the window is read into the window buffer. since an own zip file is used
	//      only the file pointer has to be restored to the right location for the
	//      zip file handler to keep function properly. once done reading the window
	//      the mutex is release and the copying into the read buffer continuous. this
	//      system allows multiple files to be opened and decompressed on the same
	//      file handler without interfering.
	if(!pArchiveDirectory){
		DETHROW(deeFileNotFound);
	}
	
	const cArchiveFile * const file = pArchiveDirectory->GetFileByPath(path);
	if(!file){
		DETHROW(deeFileNotFound);
	}
	
	decMemoryFile *memoryFile = NULL;
	decMemoryFileReader *memoryFileReader = NULL;
	bool zipFileOpen = false;
	
	try{
		pMutex.Lock();
		
		if(unzGoToFilePos(pZipFile, (unz_file_pos*)&file->GetArchivePosition()) != UNZ_OK){
			/*if( pEngineInstance.GetLogger() ){
				pEngineInstance.GetLogger()->LogInfoFormat(LOGSOURCE, "OpenFileForReading: %s gotoFailed",
					path.GetPathUnix().GetString());
			}*/
			DETHROW(deeReadFile);
		}
		
		if(unzOpenCurrentFile(pZipFile) != UNZ_OK){
			/*if( pEngineInstance.GetLogger() ){
				pEngineInstance.GetLogger()->LogInfoFormat(LOGSOURCE, "OpenFileForReading: %s openFailed",
					path.GetPathUnix().GetString());
			}*/
			DETHROW(deeReadFile);
		}
		zipFileOpen = true;
		
		memoryFile = new decMemoryFile(path.GetPathUnix());
		memoryFile->Resize(file->GetFileSize());
		const int readBytes = unzReadCurrentFile(pZipFile, memoryFile->GetPointer(), file->GetFileSize());
		if(readBytes != file->GetFileSize()){
			/*if( pEngineInstance.GetLogger() ){
				pEngineInstance.GetLogger()->LogInfoFormat(LOGSOURCE, "OpenFileForReading: %s readFailed (%d %d)",
					path.GetPathUnix().GetString(), readBytes, file->GetFileSize());
			}*/
			DETHROW(deeReadFile);
		}
		if(unzCloseCurrentFile(pZipFile) != UNZ_OK){
			/*if( pEngineInstance.GetLogger() ){
				pEngineInstance.GetLogger()->LogInfoFormat(LOGSOURCE, "OpenFileForReading: %s closeFailed",
					path.GetPathUnix().GetString());
			}*/
			DETHROW(deeReadFile);
		}
		zipFileOpen = false;
		
		memoryFileReader = new decMemoryFileReader(memoryFile);
		memoryFile->FreeReference();
		memoryFile = NULL;
		
		pMutex.Unlock();
		
	}catch(const deException &){
		if(zipFileOpen){
			unzCloseCurrentFile(pZipFile);
		}
		pMutex.Unlock();
		
		if(memoryFileReader){
			memoryFileReader->FreeReference();
		}
		if(memoryFile){
			memoryFile->FreeReference();
		}
		throw;
	}
	
	/*if( pEngineInstance.GetLogger() ){
		pEngineInstance.GetLogger()->LogInfoFormat(LOGSOURCE, "OpenFileForReading: %s read",
			path.GetPathUnix().GetString());
	}*/
	
	return memoryFileReader;
}

decBaseFileWriter *dealVFSZipArchive::OpenFileForWriting(const decPath &path){
	DETHROW(deeInvalidAction);
}

void dealVFSZipArchive::DeleteFile(const decPath &path){
	DETHROW(deeInvalidAction);
}

void dealVFSZipArchive::TouchFile(const decPath &path){
	DETHROW(deeInvalidAction);
}

void dealVFSZipArchive::SearchFiles(const decPath &directory, deContainerFileSearch &searcher){
	if(!pArchiveDirectory){
		return;
	}
	
	/*if( pEngineInstance.GetLogger() ){
		pEngineInstance.GetLogger()->LogInfoFormat(LOGSOURCE,
			"SearchFilesMatching: search '%s'", pattern.GetPathUnix().GetString());
	}*/
	
	cArchiveDirectory *adir = pArchiveDirectory;
	if(directory.GetComponentCount() > 0){
		adir = adir->GetDirectoryByPath(directory);
	}
	if(!adir){
		return;
	}
	
	const int directoryCount = adir->GetDirectoryCount();
	const int fileCount = adir->GetFileCount();
	int i;
	
	for(i=0; i<directoryCount; i++){
		searcher.Add(adir->GetDirectoryAt(i)->GetFilename(), deVFSContainer::eftDirectory);
	}
	for(i=0; i<fileCount; i++){
		searcher.Add(adir->GetFileAt(i)->GetFilename(), deVFSContainer::eftRegularFile);
	}
}

deVFSContainer::eFileTypes dealVFSZipArchive::GetFileType(const decPath &path){
	if(!pArchiveDirectory){
		DETHROW(deeFileNotFound);
	}
	
	if(pArchiveDirectory->GetFileByPath(path)){
		/*if( pEngineInstance.GetLogger() ){
			pEngineInstance.GetLogger()->LogInfoFormat(LOGSOURCE,
				"GetFileType: file '%s'", path.GetPathUnix().GetString());
		}*/
		return eftRegularFile;
	}
	if(pArchiveDirectory->GetDirectoryByPath(path)){
		/*if( pEngineInstance.GetLogger() ){
			pEngineInstance.GetLogger()->LogInfoFormat(LOGSOURCE,
				"GetFileType: directory '%s'", path.GetPathUnix().GetString());
		}*/
		return eftDirectory;
	}
	
	DETHROW(deeFileNotFound);
}

uint64_t dealVFSZipArchive::GetFileSize(const decPath &path){
	if(!pArchiveDirectory){
		DETHROW(deeFileNotFound);
	}
	
	const cArchiveFile * const file = pArchiveDirectory->GetFileByPath(path);
	if(!file){
		DETHROW(deeFileNotFound);
	}
	return file->GetFileSize();
}

TIME_SYSTEM dealVFSZipArchive::GetFileModificationTime(const decPath &path){
	if(!pArchiveDirectory){
		DETHROW(deeFileNotFound);
	}
	
	const cArchiveFile * const file = pArchiveDirectory->GetFileByPath(path);
	if(!file){
		DETHROW(deeFileNotFound);
	}
	return file->GetModificationTime();
}



// Private Functions
//////////////////////

void dealVFSZipArchive::pCleanUp(){
	if(pZipFile){
		unzClose(pZipFile);
	}
}

void dealVFSZipArchive::pBuildFileTable(){
	unz_file_info info;
	decString filename;
	int error;
	
	error = unzGoToFirstFile(pZipFile);
	if(error == UNZ_END_OF_LIST_OF_FILE){
		return;
	}
	if(error != UNZ_OK){
		DETHROW(deeReadFile);
	}
	
	pArchiveDirectory.TakeOverWith("");
	
	while(true){
		if(unzGetCurrentFileInfo(pZipFile, &info, NULL, 0, NULL, 0, NULL, 0) != UNZ_OK){
			DETHROW(deeReadFile);
		}
		
		filename.Set(' ', info.size_filename);
		if(unzGetCurrentFileInfo(pZipFile, &info, (char*)filename.GetString(),
		info.size_filename, NULL, 0, NULL, 0) != UNZ_OK){
			DETHROW(deeReadFile);
		}
		
		if(!filename.IsEmpty() && filename.GetAt(filename.GetLength() - 1) != '/'){
			decPath archivePath;
			archivePath.SetFromUnix(filename);
			
			cArchiveDirectory *directory = pArchiveDirectory;
			const int count = archivePath.GetComponentCount();
			int i;
			for(i=0; i<count-1; i++){
				directory = directory->GetOrAddDirectoryNamed(archivePath.GetComponentAt(i));
			}
			
			cArchiveFile *file = NULL;
			
			decDateTime time;
			time.SetYear(info.tmu_date.tm_year);
			time.SetMonth(info.tmu_date.tm_mon);
			time.SetDay(info.tmu_date.tm_mday - 1);
			time.SetHour(info.tmu_date.tm_hour);
			time.SetMinute(info.tmu_date.tm_min);
			time.SetSecond(info.tmu_date.tm_sec);
			
			unz_file_pos archivePosition;
			if(unzGetFilePos(pZipFile, &archivePosition) != UNZ_OK){
				DETHROW(deeReadFile);
			}
			
			try{
				file = new cArchiveFile(archivePath.GetLastComponent(), archivePosition,
					info.uncompressed_size, time.ToSystemTime());
				directory->AddFile(file);
				file->FreeReference();
				
			}catch(const deException &){
				if(file){
					file->FreeReference();
				}
				throw;
			}
		}
		
		error = unzGoToNextFile(pZipFile);
		if(error == UNZ_END_OF_LIST_OF_FILE){
			break;
		}
		if(error != UNZ_OK){
			DETHROW(deeReadFile);
		}
	}
}

void dealVFSZipArchive::pLogArchiveContent(){
	/*if( pEngineInstance.GetLogger() && pArchiveDirectory ){
		pLogArchiveContent(*pArchiveDirectory, "");
	}*/
}

void dealVFSZipArchive::pLogArchiveContent(const cArchiveDirectory &directory, const char *prefix){
	deLogger &logger = *pEngineInstance.GetLogger();
	
	logger.LogInfoFormat(LOGSOURCE, "%s+ %s", prefix, directory.GetFilename().GetString());
	
	const decString childPrefix(decString(prefix) + "  ");
	const int directoryCount = directory.GetDirectoryCount();
	int i;
	for(i=0; i<directoryCount; i++){
		pLogArchiveContent(*directory.GetDirectoryAt(i), childPrefix);
	}
	
	const int fileCount = directory.GetFileCount();
	for(i=0; i<fileCount; i++){
		const cArchiveFile &file = *directory.GetFileAt(i);
		logger.LogInfoFormat(LOGSOURCE, "%s- %s", childPrefix.GetString(), file.GetFilename().GetString());
	}
}

void dealVFSZipArchive::pOpenZipFile(){
	zlib_filefunc_def ffunc;
	ffunc.zopen_file = fZipOpenFileFunc;
	ffunc.zread_file = fZipReadFileFunc;
	ffunc.zwrite_file = fZipWriteFileFunc;
	ffunc.ztell_file = fZipTellFileFunc;
	ffunc.zseek_file = fZipSeekFileFunc;
	ffunc.zclose_file = fZipCloseFileFunc;
	ffunc.zerror_file = fZipErrorFileFunc;
	ffunc.opaque = this;
	
	pZipFile = unzOpen2(pFileReader->GetFilename(), &ffunc);
	if(!pZipFile){
		DETHROW(deeReadFile);
	}
	
	unz_global_info gi;
	if(unzGetGlobalInfo(pZipFile, &gi) != UNZ_OK){
		DETHROW(deeReadFile);
	}
	
	/*if( pEngineInstance.GetLogger() ){
		pEngineInstance.GetLogger()->LogInfoFormat(LOGSOURCE, "pOpenZipFile: %d Entries", gi.number_entry);
	}*/
}
