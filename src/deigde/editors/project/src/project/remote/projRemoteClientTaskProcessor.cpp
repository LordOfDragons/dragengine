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

#include "projRemoteClient.h"
#include "projRemoteClientTaskProcessor.h"
#include "../projProject.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deFileSearchVisitor.h>


// Class projRemoteClientTaskProcessor
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

projRemoteClientTaskProcessor::projRemoteClientTaskProcessor(projRemoteClient &client) :
derlTaskProcessorRemoteClient(client){
}

projRemoteClientTaskProcessor::~projRemoteClientTaskProcessor(){
}


// Management
///////////////

class cScanDirectoryVisitor : public deFileSearchVisitor{
private:
	derlBaseTaskProcessor::ListDirEntries &pEntries;
	
public:
	cScanDirectoryVisitor(derlBaseTaskProcessor::ListDirEntries &entries) : pEntries(entries){}
	
	bool VisitFile(const deVirtualFileSystem &vfs, const decPath &path) override{
		pEntries.push_back({path.GetLastComponent().GetString(),
			path.GetPathUnix().GetString(), vfs.GetFileSize(path), false});
		return true;
	}
	
	bool VisitDirectory(const deVirtualFileSystem &vfs, const decPath &path) override{
		pEntries.push_back({path.GetLastComponent().GetString(),
			path.GetPathUnix().GetString(), 0, true});
		return true;
	}
	
	bool VisitSpecial(const deVirtualFileSystem &vfs, const decPath &path) override{
		return true;
	}
};

bool projRemoteClientTaskProcessor::IsPathDirectory(const std::string &pathDir){
	const decPath vfsPath(decPath::CreatePathUnix(pathDir.c_str()));
	return pTaskProfileData->vfs->ExistsFile(vfsPath)
		&& pTaskProfileData->vfs->GetFileType(vfsPath) == deVFSContainer::eftDirectory;
}

void projRemoteClientTaskProcessor::ListDirectoryFiles(
ListDirEntries &entries, const std::string &pathDir){
	const decPath vfsPath(decPath::CreatePathUnix(pathDir.c_str()));
	
	if(vfsPath.GetComponentCount() > 0){
		// ignore igde path added by the IGDE itself
		if(vfsPath.GetComponentAt(0) == "igde"){
			return;
			
		// ignore path added by base game definitions
		}else if(pExcludedByBaseGameDefPath(vfsPath)){
			return;
			
		// ignore by user defined exlude pattern
		}else if(pExcludedByPattern(vfsPath)){
			return;
		}
	}
	
	cScanDirectoryVisitor scanner(entries);
	pTaskProfileData->vfs->SearchFiles(vfsPath, scanner);
}

void projRemoteClientTaskProcessor::TruncateFile(const std::string &path){
	throw std::runtime_error("Not valid on task processor");
}

void projRemoteClientTaskProcessor::OpenFile(const std::string &path, bool write){
	if(write){
		throw std::runtime_error("Not valid on task processor");
	}
	
	CloseFile();
	pVfsFilePath = path;
	
	try{
		pVfsFileReader.TakeOver(pTaskProfileData->vfs->OpenFileForReading(
			decPath::CreatePathUnix(path.c_str())));
		
	}catch(const deException &e){
		Log(denLogger::LogSeverity::error, "OpenFile", e.FormatOutput().Join("\n").GetString());
		throw;
		
	}catch(...){
		Log(denLogger::LogSeverity::error, "OpenFile", path);
		throw;
	}
}

uint64_t projRemoteClientTaskProcessor::GetFileSize(){
	try{
		pVfsFileReader->SetPositionEnd(0);
		return (uint64_t)pVfsFileReader->GetPosition();
		
	}catch(const deException &e){
		Log(denLogger::LogSeverity::error, "GetFileSize", e.FormatOutput().Join("\n").GetString());
		throw;
		
	}catch(...){
		Log(denLogger::LogSeverity::error, "GetFileSize", pVfsFilePath);
		throw;
	}
}

void projRemoteClientTaskProcessor::ReadFile(void *data, uint64_t offset, uint64_t size){
	try{
		pVfsFileReader->SetPosition((int)offset);
		pVfsFileReader->Read(data, (int)size);
		
	}catch(const deException &e){
		Log(denLogger::LogSeverity::error, "ReadFile", e.FormatOutput().Join("\n").GetString());
		throw;
		
	}catch(...){
		Log(denLogger::LogSeverity::error, "ReadFile", pVfsFilePath);
		throw;
	}
}

void projRemoteClientTaskProcessor::CloseFile(){
	pVfsFilePath.clear();
	pVfsFileReader = nullptr;
}


// Protected Functions
////////////////////////

void projRemoteClientTaskProcessor::PrepareRunTask(){
	pTaskProfileData = ((projRemoteServer&)pClient.GetServer()).GetTaskProfileData();
}


// Private Functions
//////////////////////

bool projRemoteClientTaskProcessor::pExcludedByBaseGameDefPath(const decPath &path){
	const int pathCount = pTaskProfileData->excludeBaseGameDefPath.GetCount();
	const int componentCount = path.GetComponentCount();
	int i, j;
	
	for(i=0; i<pathCount; i++){
		const decPath &bgdPath = pTaskProfileData->excludeBaseGameDefPath.GetAt(i);
		
		if(bgdPath.GetComponentCount() < componentCount){
			continue;
		}
		
		for(j=0; j<componentCount; j++){
			if(path.GetComponentAt(j) != bgdPath.GetComponentAt(j)){
				break;
			}
		}
		
		if(j == componentCount){
			return true;
		}
	}
	
	return false;
}

bool projRemoteClientTaskProcessor::pExcludedByPattern(const decPath &path){
	const decString fullPath(path.GetPathUnix());
	const int count = pTaskProfileData->excludePatterns.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(fullPath.MatchesPattern(pTaskProfileData->excludePatterns.GetAt(i))){
			return true;
		}
	}
	
	return false;
}
