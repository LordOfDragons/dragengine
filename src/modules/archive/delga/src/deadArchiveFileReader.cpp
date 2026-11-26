
/* 
 * Drag[en]gine DELGA Archive Module
 *
 * Copyright (C) 2022, Plüss Roland (roland@rptd.ch)
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

#include "deArchiveDelga.h"
#include "deadArchiveFile.h"
#include "deadArchiveFileReader.h"
#include "deadContextUnpack.h"
#include "deadContainer.h"
#include "deArchiveDelga.h"

#include <dragengine/common/exceptions.h>



// Class deadArchiveFileReader
////////////////////////////////

// Constructor, destructor
////////////////////////////

deadArchiveFileReader::deadArchiveFileReader(deadContextUnpack *context, const deadArchiveFile &file) :
pContext(context),
pArchivePosition(file.GetArchivePosition()),
pFilename(file.GetFilename()),
pFullFilename(file.GetFullFilename()),
pFilesize(file.GetFileSize()),
pModificationTime(file.GetModificationTime()),
pPosition(0),
pCached(false)
{
	DEASSERT_NOTNULL(context)
}

deadArchiveFileReader::~deadArchiveFileReader(){
	pContext->CloseFile();
	if(pContext->GetContainer()){
		pContext->GetContainer()->ReleaseContextUnpack(pContext);
	}
}



// Management
///////////////

const char * deadArchiveFileReader::GetFilename(){
	return pFilename;
}

int deadArchiveFileReader::GetLength(){
	return pFilesize;
}

TIME_SYSTEM deadArchiveFileReader::GetModificationTime(){
	return pModificationTime;
}

int deadArchiveFileReader::GetPosition(){
	return pPosition;
}

void deadArchiveFileReader::SetPosition(int position){
	DEASSERT_TRUE(position >= 0)
	DEASSERT_TRUE(position < pFilesize)
	
	if(pCached){
		pPosition = position;
		return;
	}
	
	if(position > pPosition){
		pContext->SeekMoveZipFile(position - pPosition);
		pPosition = position;
		
	}else if(position < pPosition){
		// backward seeks restart decoding from the beginning. as a quick optimizaion cache
		// the entire file content to avoid future re-inflation. certain file formats process
		// large files and keep the file reader open for a longer time period with potentially
		// random seeking or restarting from the start (looping videos, music). we can keep
		// reopening the entry (which is cheap) and decoding again from the start. this avoids
		// loading the content into memory but costs processing time decoding the same data over
		// and over again. on the other hand caching these large files in memory avoids the
		// decompression work at the cost of extra memory usage and a potentially hickup when the
		// file is fully decompressed into memory. so right now the caching is done only if a
		// backward seek is done to a random position. for seeking back to 0 we do not cache the
		// file. this should keep large files like videos or music from fully loading into memory
		// file still allowing other files doing random seeking to work more efficiently.
		if(position == 0){
			pContext->SeekSetZipFile(0);
			
		}else{
			pContext->GetModule().LogInfoFormat(
				"Seeking backwards (%d -> %d), caching content (%d): %s [%s]",
				pPosition, position, pFilesize, pFullFilename.GetString(),
				pContext->GetContainer()->GetFilename().GetString());
			pCacheContent();
		}
		
		pPosition = position;
	}
}

void deadArchiveFileReader::MovePosition(int offset){
	SetPosition(pPosition + offset);
}

void deadArchiveFileReader::SetPositionEnd(int position){
	SetPosition(pFilesize - 1 - position);
}

void deadArchiveFileReader::Read(void *buffer, int size){
	if(pCached){
		if(pPosition + size > pFilesize){
			DETHROW_INFO(deeReadFile, pFilename);
		}
		memcpy(buffer, pCache.GetArrayPointer() + pPosition, size);
	}else{
		pContext->ReadZipFileData(buffer, size);
	}
	pPosition += size;
}

decBaseFileReader::Ref deadArchiveFileReader::Duplicate(){
	DETHROW_INFO(deeInvalidAction, "not supported");
}


// Private Functions
//////////////////////

void deadArchiveFileReader::pCacheContent(){
	pContext->SeekSetZipFile(0);
	pCache.SetCountDiscard(pFilesize);
	if(pFilesize > 0){
		pContext->ReadZipFileData(pCache.GetArrayPointer(), pFilesize);
	}
	pContext->CloseFile();
	pCached = true;
}
