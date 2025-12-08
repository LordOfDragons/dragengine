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
#include <stdio.h>

#include "deCacheHelper.h"
#include "deVirtualFileSystem.h"
#include "deCollectFileSearchVisitor.h"
#include "dePathList.h"
#include "../common/file/decPath.h"
#include "../common/file/decBaseFileReader.h"
#include "../common/file/decBaseFileWriter.h"
#include "../common/file/decZFileWriter.h"
#include "../common/file/decZFileReader.h"
#include "../common/exceptions.h"
#include "../logger/deLogger.h"


/*
possible improvement for large caches:
d00/f0 -> f99
d01/f0 -> f99
store a list of buckets in each a map of let's say 100 files. this would keep the
number of files per directory limited improving searching for free slots.
*/


// Class deCacheHelper
////////////////////////

// Constructor, destructor
////////////////////////////

deCacheHelper::deCacheHelper(deVirtualFileSystem::Ref vfs, const decPath &cachePath) :

pCachePath(cachePath),
pCompressionMethod(ecmZCompression)
{
	if(!vfs){
		DETHROW(deeInvalidParam);
	}
	
	pVFS = vfs;
	BuildMapping();
}

deCacheHelper::~deCacheHelper(){
}




// Management
///////////////

void deCacheHelper::SetCompressionMethod(eCompressionMethods compressionMethod){
	pCompressionMethod = compressionMethod;
}



decBaseFileReader *deCacheHelper::Read(const char *id){
	const int slot = pMapping.IndexOf(id);
	if(slot == -1){
		return NULL;
	}
	
	decPath path(pCachePath);
	decString fileTitle;
	
	fileTitle.Format("f%i", slot);
	
	path.AddComponent(fileTitle);
	
	decBaseFileReader::Ref reader = NULL;
	decZFileReader::Ref zreader = NULL;
	
	if(pVFS->CanReadFile(path)){
		decString testID;
		
		pVFS->TouchFile(path);
		
		try{
			reader = pVFS->OpenFileForReading(path);
			
			reader->ReadString16Into(testID);
			if(testID != id){
				pMapping.SetAt(slot, "");
				return NULL;
			}
			
			const int compression = reader->ReadByte();
			if(compression == 'z'){
				zreader.TakeOver(new decZFileReader(reader));
				reader = zreader;
				zreader = NULL;
			}
			
		}catch(const deException &){
			throw;
		}
		
	}else{
		pMapping.SetAt(slot, "");
	}
	
	return reader;
}

decBaseFileWriter *deCacheHelper::Write(const char *id){
	int slot = pMapping.IndexOf(id);
	
	if(slot == -1){
		const int slotCount = pMapping.GetCount();
		
		for(slot=0; slot<slotCount; slot++){
			if(pMapping.GetAt(slot).IsEmpty()){
				pMapping.SetAt(slot, id);
				break;
			}
		}
		
		if(slot == slotCount){
			pMapping.Add(id);
		}
	}
	
	decPath path(pCachePath);
	decString fileTitle;
	
	fileTitle.Format("f%i", slot);
	
	path.AddComponent(fileTitle);
	
	decBaseFileWriter::Ref writer = NULL;
	decZFileWriter::Ref zwriter = NULL;
	
	try{
		writer = pVFS->OpenFileForWriting(path);
		writer->WriteString16(id);
		
		if(pCompressionMethod == ecmZCompression){
			writer->WriteByte('z'); // z-compressed
			zwriter.TakeOver(new decZFileWriter(writer));
			writer = zwriter;
			zwriter = NULL;
			
		}else{ // no compression
			writer->WriteByte('-'); // no compression
		}
		
	}catch(const deException &){
		throw;
	}
	
	return writer;
}

void deCacheHelper::Delete(const char *id){
	const int slot = pMapping.IndexOf(id);
	if(slot == -1){
		return;
	}
	
	decPath path(pCachePath);
	decString fileTitle;
	
	fileTitle.Format("f%i", slot);
	
	path.AddComponent(fileTitle);
	
	pVFS->DeleteFile(path);
	
	pMapping.SetAt(slot, "");
}

void deCacheHelper::DeleteAll(){
	const int count = pMapping.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(pMapping.GetAt(i).IsEmpty()){
			continue;
		}
		
		decPath path(pCachePath);
		decString fileTitle;
		fileTitle.Format("f%i", i);
		path.AddComponent(fileTitle);
		
		pVFS->DeleteFile(path);
		
		pMapping.SetAt(i, "");
	}
}



void deCacheHelper::BuildMapping(){
	// find all cache files
	deCollectFileSearchVisitor collect("f*");
	pVFS->SearchFiles(pCachePath, collect);
	
	// determine the largest slot number used by all the cache files
	const dePathList &files = collect.GetFiles();
	const int count = files.GetCount();
	int maxSlot = 0;
	int i;
	
	for(i=0; i<count; i++){
		const int slot = decString(files.GetAt(i).GetLastComponent()).GetMiddle(1).ToInt();
		
		if(slot >= maxSlot){
			maxSlot = slot + 1;
		}
	}
	
	// create mapping table with the required number of empty entries
	pMapping.RemoveAll();
	for(i=0; i<maxSlot; i++){
		pMapping.Add("");
	}
	
	// read the IDs from all cache files entering them into the proper slot of the mapping table
	decBaseFileReader::Ref reader = NULL;
	decString id;
	
	try{
		for(i=0; i<count; i++){
			const decPath &file = files.GetAt(i);
			const int slot = decString(file.GetLastComponent()).GetMiddle(1).ToInt();
			
			reader = pVFS->OpenFileForReading(file);
			reader->ReadString16Into(id);
			reader = NULL;
			
			pMapping.SetAt(slot, id);
		}
		
	}catch(const deException &){
		throw;
	}
}

void deCacheHelper::DebugPrint(deLogger &logger, const char *loggingSource){
	const int count = pMapping.GetCount();
	int i;
	
	logger.LogInfoFormat(loggingSource, "Cache Directory '%s'", pCachePath.GetPathUnix().GetString());
	for(i=0; i<count; i++){
		const decString &id = pMapping.GetAt(i);
		
		if(!id.IsEmpty()){
			logger.LogInfoFormat(loggingSource, "- Slot %i: '%s' => f%i", i, id.GetString(), i);
		}
	}
}
