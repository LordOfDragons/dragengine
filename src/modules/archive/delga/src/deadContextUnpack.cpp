
/* 
 * Drag[en]gine DELGA Archive Module
 *
 * Copyright (C) 2017, Plüss Roland ( roland@rptd.ch )
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deArchiveDelga.h"
#include "deadArchiveFile.h"
#include "deadArchiveDirectory.h"
#include "deadContainer.h"
#include "deadContextUnpack.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decWeakFileReader.h>
#include <dragengine/common/file/decWeakFileWriter.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/common/file/decMemoryFileReader.h>
#include <dragengine/common/file/decBaseFileReader.h>



// Callbacks
//////////////

static voidpf ZCALLBACK fZipOpenFileFunc(voidpf opaque, const char */*filename*/, int /*mode*/){
	return opaque;
}

static uLong ZCALLBACK fZipReadFileFunc(voidpf opaque, voidpf /*stream*/, void *buf, uLong size){
	deadContextUnpack &context = *((deadContextUnpack*)opaque);
	
	try{
		context.ReadData(buf, (long)size);
		return size;
		
	}catch(const deException &){
		return -1;
	}
}

static uLong ZCALLBACK fZipWriteFileFunc(voidpf /*opaque*/, voidpf /*stream*/, const void */*buf*/, uLong /*size*/){
	return -1; // not supported
}

static long ZCALLBACK fZipTellFileFunc(voidpf opaque, voidpf /*stream*/){
	deadContextUnpack &context = *((deadContextUnpack*)opaque);
	
	try{
		return context.GetFilePosition();
		
	}catch(const deException &){
		return -1;
	}
}

static long ZCALLBACK fZipSeekFileFunc(voidpf opaque, voidpf /*stream*/, uLong offset, int origin){
	deadContextUnpack &context = *((deadContextUnpack*)opaque);
	
	try{
		context.SeekFile(origin, offset);
		
	}catch(const deException &){
		return -1;
	}
	
	return 0;
}

static int ZCALLBACK fZipCloseFileFunc(voidpf /*opaque*/, voidpf /*stream*/){
	return 0;
}

static int ZCALLBACK fZipErrorFileFunc(voidpf /*opaque*/, voidpf /*stream*/){
	return 0; // not implemented
}



// Class deadContextUnpack
////////////////////////////

// Constructor, destructor
////////////////////////////

deadContextUnpack::deadContextUnpack(deArchiveDelga &module, deadContainer *container) :
pModule(module),
pContainer(container),
pReader(container->GetReader()->Duplicate()),
pZipFile(nullptr),
pZipFileOpen(false)
{
	zlib_filefunc_def ffunc;
	ffunc.zopen_file = fZipOpenFileFunc;
	ffunc.zread_file = fZipReadFileFunc;
	ffunc.zwrite_file = fZipWriteFileFunc;
	ffunc.ztell_file = fZipTellFileFunc;
	ffunc.zseek_file = fZipSeekFileFunc;
	ffunc.zclose_file = fZipCloseFileFunc;
	ffunc.zerror_file = fZipErrorFileFunc;
	ffunc.opaque = this;
	
	pZipFile = unzOpen2(container->GetFilename(), &ffunc);
	if(!pZipFile){
		DETHROW_INFO(deeReadFile, container->GetFilename());
	}
	
	unz_global_info gi;
	if(unzGetGlobalInfo(pZipFile, &gi) != UNZ_OK){
		unzClose(pZipFile);
		DETHROW_INFO(deeReadFile, container->GetFilename());
	}
}

deadContextUnpack::~deadContextUnpack(){
	DropContainer();
	
	if(pZipFile){
		unzClose(pZipFile);
	}
}



// Management
///////////////

void deadContextUnpack::DropContainer(){
	CloseFile();
	pReader = nullptr;
	pContainer = nullptr;
}



deadArchiveFileReader::Ref deadContextUnpack::OpenFileForReading(const deadArchiveFile &file){
	DEASSERT_FALSE(pZipFileOpen)
	DEASSERT_NOTNULL(pContainer)
	DEASSERT_NOTNULL(pReader)
	
	try{
		unz_file_pos archivePosition(file.GetArchivePosition());
		if(unzGoToFilePos(pZipFile, &archivePosition) != UNZ_OK){
			DETHROW_INFO(deeReadFile, pContainer->GetFilename());
		}
		
		if(unzOpenCurrentFile(pZipFile) != UNZ_OK){
			DETHROW_INFO(deeReadFile, pContainer->GetFilename());
		}
		pZipFileOpen = true;
		pArchiveFilePosition = file.GetArchivePosition();
		
		return deadArchiveFileReader::Ref::NewWith(this, file);
		
	}catch(const deException &){
		CloseFile();
		throw;
	}
}

decWeakFileWriter *deadContextUnpack::OpenFileForWriting(const deadArchiveFile &){
	// not supported for the time being
	DETHROW(deeInvalidParam);
}

void deadContextUnpack::CloseFile(){
	if(pZipFileOpen){
		unzCloseCurrentFile(pZipFile);
		pZipFileOpen = false;
	}
}



void deadContextUnpack::ReadZipFileData(void *buffer, long size){
	DEASSERT_TRUE(pZipFileOpen)
	
	const int readBytes = unzReadCurrentFile(pZipFile, buffer, size);
	if(readBytes != size){
		DETHROW_INFO(deeReadFile, pContainer->GetFilename());
	}
}

void deadContextUnpack::SeekMoveZipFile(int position){
	DEASSERT_TRUE(pZipFileOpen)
	DEASSERT_TRUE(position >= 0)
	
	char buffer[1024];
	while(position > 0){
		const int len = decMath::min(position, 1024);
		const int readBytes = unzReadCurrentFile(pZipFile, buffer, len);
		if(readBytes != len){
			DETHROW_INFO(deeReadFile, pContainer->GetFilename());
		}
		position -= len;
	}
}

void deadContextUnpack::SeekSetZipFile(int position){
	DEASSERT_TRUE(pZipFileOpen)
	
	unz_file_pos archivePosition = pArchiveFilePosition;
	
	CloseFile();
	
	if(unzGoToFilePos(pZipFile, &archivePosition) != UNZ_OK){
		DETHROW_INFO(deeReadFile, pContainer->GetFilename());
	}
	
	if(unzOpenCurrentFile(pZipFile) != UNZ_OK){
		DETHROW_INFO(deeReadFile, pContainer->GetFilename());
	}
	pZipFileOpen = true;
	
	SeekMoveZipFile(position);
}



void deadContextUnpack::ReadData(void *buffer, long size){
	pReader->Read(buffer, size);
}

long deadContextUnpack::GetFilePosition() const{
	return pReader->GetPosition();
}

void deadContextUnpack::SeekFile(int origin, long offset){
	if(origin == ZLIB_FILEFUNC_SEEK_CUR){
		pReader->MovePosition(offset);
		
	}else if(origin == ZLIB_FILEFUNC_SEEK_END){
		pReader->SetPositionEnd(offset);
		
	}else if(origin == ZLIB_FILEFUNC_SEEK_SET){
		pReader->SetPosition(offset);
		
	}else{
		DETHROW(deeInvalidParam);
	}
}

deadArchiveDirectory::Ref deadContextUnpack::ReadFileTable(){
	DEASSERT_FALSE(pZipFileOpen)
	DEASSERT_NOTNULL(pReader)
	
	unz_file_info info;
	decString filename;
	int error;
	
	error = unzGoToFirstFile(pZipFile);
	if(error != UNZ_OK){
		DETHROW_INFO(deeReadFile, pContainer->GetFilename());
	}
	
	const deadArchiveDirectory::Ref archiveDirectory(deadArchiveDirectory::Ref::NewWith(pModule, ""));
	
	while(error == UNZ_OK){ // exit if error == UNZ_END_OF_LIST_OF_FILE
		if(unzGetCurrentFileInfo(pZipFile, &info, NULL, 0, NULL, 0, NULL, 0) != UNZ_OK){
			DETHROW_INFO(deeReadFile, pContainer->GetFilename());
		}
		
		filename.Set(' ', info.size_filename);
		if(unzGetCurrentFileInfo(pZipFile, &info, (char*)filename.GetString(),
		info.size_filename, NULL, 0, NULL, 0) != UNZ_OK){
			DETHROW_INFO(deeReadFile, filename);
		}

		filename.Replace('\\', '/'); // windows idiocy protection
		
		if(!filename.IsEmpty() && filename.GetAt(filename.GetLength() - 1) != '/'){
			const decPath archivePath(decPath::CreatePathUnix(filename));
			
			deadArchiveDirectory *directory = archiveDirectory;
			const int count = archivePath.GetComponentCount();
			int i;
			for(i=0; i<count-1; i++){
				directory = directory->GetOrAddDirectoryNamed(archivePath.GetComponentAt(i));
			}
			
			unz_file_pos archivePosition;
			if(unzGetFilePos(pZipFile, &archivePosition) != UNZ_OK){
				DETHROW_INFO(deeReadFile, filename);
			}
			
			directory->AddFile(deadArchiveFile::Ref::NewWith(pModule, archivePath.GetLastComponent(), info, archivePosition));
		}
		
		error = unzGoToNextFile(pZipFile);
		if(error != UNZ_OK && error != UNZ_END_OF_LIST_OF_FILE){
			DETHROW_INFO(deeReadFile, filename);
		}
	}
	
	return archiveDirectory;
}
