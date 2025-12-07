
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deArchiveDelga.h"
#include "deadArchiveFile.h"
#include "deadArchiveFileReader.h"
#include "deadContextUnpack.h"
#include "deadContainer.h"

#include <dragengine/common/exceptions.h>



// Class deadArchiveFileReader
////////////////////////////////

// Constructor, destructor
////////////////////////////

deadArchiveFileReader::deadArchiveFileReader(deadContextUnpack *context, const deadArchiveFile &file) :
pContext(context),
pArchivePosition(file.GetArchivePosition()),
pFilename(file.GetFilename()),
pFilesize(file.GetFileSize()),
pModificationTime(file.GetModificationTime()),
pPosition(0)
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
	
	if(position > pPosition){
		pContext->SeekMoveZipFile(position - pPosition);
		pPosition = position;
		
	}else if(position < pPosition){
		pContext->SeekSetZipFile(position);
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
	pContext->ReadZipFileData(buffer, size);
	pPosition += size;
}

decBaseFileReader::Ref deadArchiveFileReader::Duplicate(){
	DETHROW_INFO(deeInvalidAction, "not supported");
}
