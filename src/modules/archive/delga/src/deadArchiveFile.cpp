
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

#include <dragengine/common/exceptions.h>



// Class deadArchiveFile
//////////////////////////

// Constructor, destructor
////////////////////////////

deadArchiveFile::deadArchiveFile(deArchiveDelga &module, const char *filename,
const unz_file_info &info, const unz_file_pos &archivePosition) :
pModule(module),
pFilename(filename),
pArchivePosition(archivePosition),
pFileSize((int)info.uncompressed_size),
pCompressedSize((int)info.compressed_size),
pReadBlockSize((int)pCompressedSize)
{
	(void)pModule;
	
	decDateTime time;
	time.SetYear(info.tmu_date.tm_year);
	time.SetMonth(info.tmu_date.tm_mon);
	time.SetDay(info.tmu_date.tm_mday - 1);
	time.SetHour(info.tmu_date.tm_hour);
	time.SetMinute(info.tmu_date.tm_min);
	time.SetSecond(info.tmu_date.tm_sec);
	pModificationTime = time.ToSystemTime();
}

deadArchiveFile::~deadArchiveFile(){
}
