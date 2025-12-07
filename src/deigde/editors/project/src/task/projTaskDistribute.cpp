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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "projTaskDistribute.h"
#include "../project.h"
#include "../gui/projWindowMain.h"
#include "../project/projProject.h"
#include "../project/profile/projProfile.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gameproject/igdeGameProject.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/common/file/decMemoryFileWriter.h>
#include <dragengine/common/utils/decDateTime.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>
#include <dragengine/deEngine.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deLoadableModule.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/filesystem/deVFSContainer.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deCollectDirectorySearchVisitor.h>
#include <dragengine/filesystem/deCollectFileSearchVisitor.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>



// Callbacks
//////////////




// Callbacks
//////////////

static voidpf ZCALLBACK fZipOpenFileFunc(voidpf opaque, const char */*filename*/, int /*mode*/){
	return opaque;
}

static uLong ZCALLBACK fZipReadFileFunc(voidpf /*opaque*/, voidpf /*stream*/, void *buf, uLong size){
	printf("fZipReadFileFunc %p %ld\n", buf, size);
	return -1; // not supported
}

static uLong ZCALLBACK fZipWriteFileFunc(voidpf opaque, voidpf /*stream*/, const void *buf, uLong size){
	projTaskDistribute &task = *((projTaskDistribute*)opaque);
	
	try{
		task.WriteToDelga(buf, (long)size);
		return size;
		
	}catch(const deException &){
		return -1;
	}
}

static long ZCALLBACK fZipTellFileFunc(voidpf opaque, voidpf /*stream*/){
	try{
		return ((projTaskDistribute*)opaque)->GetDelgaPosition();
		
	}catch(const deException &){
		return -1;
	}
}

static long ZCALLBACK fZipSeekFileFunc(voidpf opaque, voidpf /*stream*/, uLong offset, int origin){
	try{
		((projTaskDistribute*)opaque)->SeekDelgaFile((long)offset, origin);
		
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



// Class projTaskDistribute
/////////////////////////////

// Constructor, destructor
////////////////////////////

projTaskDistribute::projTaskDistribute(projWindowMain &windowMain,
const projProject &project, const projProfile &profile) :
pWindowMain(windowMain),
pProject(project),
pProfile(profile),

pState(esInitial),

pZipFile(NULL),

pDelgaSize(0),
pDelgaPosition(0),
pDelgaDirectoryCount(0),
pDelgaFileCount(0),

pReadBuffer(NULL),
pReadBufferSize(1024 * 8) // 8k
{
	pReadBuffer = new char[pReadBufferSize];
}

projTaskDistribute::~projTaskDistribute(){
	pCloseDelgaWriter();
	if(pReadBuffer){
		delete [] pReadBuffer;
	}
}



// Management
///////////////

bool projTaskDistribute::Step(){
	try{
		switch(pState){
		case esInitial:
			pBuildExcludeBaseGameDefPath();
			pExcludePatterns = pProfile.GetExcludePatterns();
			pVFS = pWindowMain.GetEnvironment().GetFileSystemGame();
			pCreateDelgaWriter();
			pScanDirectory(decPath::CreatePathUnix("/"));
			
			pState = esProcessFiles;
			return true;
			
		case esProcessFiles:
			pProcessFiles();
			if(pStackDirectories.GetCount() > 0){
				return true;
			}
			
			// all files processed
			pUsedFileExtensions += pProfile.GetRequiredExtensions();
			
			pWriteGameXml();
			pCloseDelgaWriter();
			pState = esFinished;
			SetMessage("Finished");
			
			return true;
			
		case esFinished:
		default:
			return false;
		}
		
	}catch(const deException &){
		pState = esFinished;
		throw;
	}
}



void projTaskDistribute::WriteToDelga(const void *buffer, long length){
	pDelgaWriter->Write(buffer, length);
	
	pDelgaPosition += length;
	if(pDelgaPosition > pDelgaSize){
		pDelgaSize = pDelgaPosition;
	}
}

void projTaskDistribute::SeekDelgaFile(long offset, int zlibOrigin){
	switch(zlibOrigin){
	case ZLIB_FILEFUNC_SEEK_CUR:
		pDelgaWriter->MovePosition(offset);
		break;
		
	case ZLIB_FILEFUNC_SEEK_END:
		pDelgaWriter->SetPositionEnd(offset);
		break;
		
	case ZLIB_FILEFUNC_SEEK_SET:
		pDelgaWriter->SetPosition(offset);
		break;
		
	default:
		DETHROW(deeInvalidParam);
	}
	
	pDelgaPosition = pDelgaWriter->GetPosition();
}



// Private Functions
//////////////////////

void projTaskDistribute::pBuildExcludeBaseGameDefPath(){
	const igdeGameDefinitionList &list = pWindowMain.GetGameProject()->GetBaseGameDefinitionList();
	const int count = list.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const igdeGameDefinition &gameDef = *list.GetAt(i);
		if(gameDef.GetVFSPath().IsEmpty()){
			continue;
		}
		pExcludeBaseGameDefPath.AddIfAbsent(decPath::CreatePathUnix(gameDef.GetVFSPath()));
	}
}

bool projTaskDistribute::pExcludedByBaseGameDefPath(const decPath &path){
	const int pathCount = pExcludeBaseGameDefPath.GetCount();
	const int componentCount = path.GetComponentCount();
	int i, j;
	
	for(i=0; i<pathCount; i++){
		const decPath &bgdPath = pExcludeBaseGameDefPath.GetAt(i);
		
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

bool projTaskDistribute::pExcludedByPattern(const decPath &path){
	const decString fullPath(path.GetPathUnix());
	const int count = pExcludePatterns.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(fullPath.MatchesPattern(pExcludePatterns.GetAt(i))){
			return true;
		}
	}
	
	return false;
}

void projTaskDistribute::pCreateDelgaWriter(){
	// determine delga path
	decPath path;
	path.SetFromNative(pWindowMain.GetEnvironment().GetGameProject()->GetDirectoryPath());
	path.AddUnixPath(pProfile.GetDelgaPath());
	pDelgaPath = path.GetPathNative();
	
	// create delga file. uses virtual file system disk to automatically create the
	// parent directory if not existing
	decPath localPath;
	localPath.SetFromUnix("/");
	localPath.AddComponent(path.GetLastComponent());
	
	path.RemoveLastComponent(); // parent directory
	
	pDelgaWriter.TakeOver(deVFSDiskDirectory::Ref::NewWith(path)->OpenFileForWriting(localPath));
	
	// create zip file operating on the delga writer
	zlib_filefunc_def ffunc;
	ffunc.zopen_file = fZipOpenFileFunc;
	ffunc.zread_file = fZipReadFileFunc;
	ffunc.zwrite_file = fZipWriteFileFunc;
	ffunc.ztell_file = fZipTellFileFunc;
	ffunc.zseek_file = fZipSeekFileFunc;
	ffunc.zclose_file = fZipCloseFileFunc;
	ffunc.zerror_file = fZipErrorFileFunc;
	ffunc.opaque = this;
	
	pZipFile = zipOpen2(pDelgaPath, APPEND_STATUS_CREATE, NULL, &ffunc);
	if(!pZipFile){
		DETHROW_INFO(deeReadFile, pDelgaPath);
	}
}

void projTaskDistribute::pCloseDelgaWriter(){
	if(pZipFile){
		zipClose(pZipFile, "");
		pZipFile = NULL;
	}
	pDelgaWriter = NULL;
}

void projTaskDistribute::pScanDirectory(const decPath &path){
	cProcessDirectory *directory = NULL;
	bool ignore = false;
	
	try{
		directory = new cProcessDirectory;
		directory->path = path.GetPathUnix();
		directory->nextDirectory = 0;
		directory->nextFile = 0;
		directory->hasCountedDir = false;
		
		if(path.GetComponentCount() > 0){
			// ignore igde path added by the IGDE itself
			if(path.GetComponentAt(0) == "igde"){
				ignore = true;
				
			// ignore path added by base game definitions
			}else if(pExcludedByBaseGameDefPath(path)){
				ignore = true;
				
			// ignore by user defined exlude pattern
			}else if(pExcludedByPattern(path)){
				ignore = true;
			}
		}
		
		if(!ignore){
			deCollectDirectorySearchVisitor collectDirectories;
			pVFS->SearchFiles(path, collectDirectories);
			directory->directories = collectDirectories.GetDirectories();
			
			deCollectFileSearchVisitor collectFiles;
			pVFS->SearchFiles(path, collectFiles);
			directory->files = collectFiles.GetFiles();
		}
		
		pStackDirectories.Add(directory);
		directory->FreeReference();
		
	}catch(const deException &){
		if(directory){
			directory->FreeReference();
		}
		throw;
	}
}

void projTaskDistribute::pProcessFiles(){
	int processLimit = 1; // process at most 1 elements per update 1 directory per update
	
	while(processLimit > 0){
		cProcessDirectory * const directory = GetProcessDirectory();
		if(!directory){
			break;
		}
		
		if(directory->nextFile < directory->files.GetCount()){
			const decPath &path = directory->files.GetAt(directory->nextFile);
			
			try{
				pProcessFile(path);
				directory->nextFile++;
				processLimit--;
				
			}catch(const deException &){
				decString message;
				message.Format("Failed accessing VFS path %s", path.GetPathUnix().GetString());
				SetMessage(message);
				throw;
			}
			
		}else if(directory->nextDirectory < directory->directories.GetCount()){
			const decPath &path = directory->directories.GetAt(directory->nextDirectory);
			
			try{
				pScanDirectory(path);
				
			}catch(const deException &){
				decString message;
				message.Format("Failed accessing VFS path %s", path.GetPathUnix().GetString());
				SetMessage(message);
				throw;
			}
			
		}else{
			pCloseDirectory();
			cProcessDirectory * const nextDirectory = GetProcessDirectory();
			if(nextDirectory){
				nextDirectory->nextDirectory++;
			}
			continue;
		}
		
	}
}

void projTaskDistribute::pProcessFile(const decPath &path){
	if(pExcludedByPattern(path)){
		return;
	}
	
	decString message;
	message.Format("Processing %s", path.GetPathUnix().GetString());
	SetMessage(message);
	
	// count directory if not counted yet
	cProcessDirectory * const directory = GetProcessDirectory();
	if(!directory){
		DETHROW(deeInvalidAction);
	}
	
	if(!directory->hasCountedDir){
		pDelgaDirectoryCount++;
		directory->hasCountedDir = true;
	}
	
	// count file
	pDelgaFileCount++;
	
	// for the time being simply copy the file
	const decString extension(pGetFileExtension(path));
	bool compress = true;
	
	if(!extension.IsEmpty()){
		pUsedFileExtensions.Add(extension);
		
		const deLoadableModule * const module = pGetMatchingModule(extension);
		if(module && module->GetNoCompress()){
			compress = false;
		}
	}
	
	pZipBeginFile(path, compress);
	pCopyFile(path);
	pZipCloseFile();
}

decString projTaskDistribute::pGetFileExtension(const decPath &path) const{
	const decString &title = path.GetLastComponent();
	const int delimiter = title.FindReverse('.');
	if(delimiter != -1){
		return title.GetMiddle(delimiter); // keep '.'
	}
	return decString();
}

deLoadableModule *projTaskDistribute::pGetMatchingModule(const decString &extension) const{
	const deModuleSystem &moduleSystem = *pWindowMain.GetEnvironment().GetEngineController()->GetEngine()->GetModuleSystem();
	
	static const int checkTypeCount = 11;
	static const deModuleSystem::eModuleTypes checkTypes[checkTypeCount] = {
		deModuleSystem::emtAnimation,
		deModuleSystem::emtArchive,
		deModuleSystem::emtFont,
		deModuleSystem::emtImage,
		deModuleSystem::emtLanguagePack,
		deModuleSystem::emtModel,
		deModuleSystem::emtOcclusionMesh,
		deModuleSystem::emtRig,
		deModuleSystem::emtSkin,
		deModuleSystem::emtSound,
		deModuleSystem::emtVideo
	};
	
	int i;
	for(i=0; i<checkTypeCount; i++){
		deLoadableModule * const module = moduleSystem.FindMatching(checkTypes[i], extension);
		if(module){
			return module;
		}
	}
	
	return nullptr;
}

const char *projTaskDistribute::pGetModuleTypeName(deModuleSystem::eModuleTypes type) const{
	switch(type){
	case deModuleSystem::emtAnimation:
		return "Animation";
		
	case deModuleSystem::emtArchive:
		return "Archive";
		
	case deModuleSystem::emtFont:
		return "Font";
		
	case deModuleSystem::emtImage:
		return "Image";
		
	case deModuleSystem::emtLanguagePack:
		return "LanguagePack";
		
	case deModuleSystem::emtModel:
		return "Model";
		
	case deModuleSystem::emtOcclusionMesh:
		return "OcclusionMesh";
		
	case deModuleSystem::emtRig:
		return "Rig";
		
	case deModuleSystem::emtSkin:
		return "Skin";
		
	case deModuleSystem::emtSound:
		return "Sound";
		
	case deModuleSystem::emtVideo:
		return "Video";
		
	default:
		DETHROW(deeInvalidParam);
	};
}

void projTaskDistribute::pCopyFile(const decPath &path){
	const decBaseFileReader::Ref reader(decBaseFileReader::Ref::New(pVFS->OpenFileForReading(path)));
	
	const long size = reader->GetLength();
	if(size == 0){
		return;
	}
	
	int position;
	for(position=0; position<size; position+=pReadBufferSize){
		const int copySize = decMath::min(size - position, pReadBufferSize);
		reader->Read(pReadBuffer, copySize);
		pZipWriteFile(pReadBuffer, copySize);
	}
}

void projTaskDistribute::pZipBeginFile(const decPath &path, bool compress){
	const decDateTime modtime(pVFS->GetFileModificationTime(path));
	
	zip_fileinfo info;
	memset(&info, 0, sizeof(info));
	
	info.tmz_date.tm_year = modtime.GetYear();
	info.tmz_date.tm_mon = modtime.GetMonth();
	info.tmz_date.tm_mday = modtime.GetDay() + 1; // zip uses 1 as first day of month
	info.tmz_date.tm_hour = modtime.GetHour();
	info.tmz_date.tm_min = modtime.GetMinute();
	info.tmz_date.tm_sec = modtime.GetSecond();
	info.dosDate = 0; // use tmz_date
	info.internal_fa = 0; // no idea what this is
	info.external_fa = 0; // no idea what this is
	
	// NOTE: path contains '/' as prefix. delga files require path without prefix
	int method = Z_DEFLATED;
	int level = compress ? Z_DEFAULT_COMPRESSION : Z_NO_COMPRESSION;
	
	if(zipOpenNewFileInZip(pZipFile, path.GetPathUnix().GetMiddle(1), &info,
	nullptr, 0, nullptr, 0, nullptr, method, level) != ZIP_OK){
		DETHROW(deeInvalidParam);
	}
}

void projTaskDistribute::pZipWriteFile(const void *buffer, long size){
	if(!buffer || size < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(size == 0){
		return;
	}
	
	if(zipWriteInFileInZip(pZipFile, buffer, size) != ZIP_OK){
		DETHROW(deeInvalidParam);
	}
}

void projTaskDistribute::pZipCloseFile(){
	if(zipCloseFileInZip(pZipFile) != ZIP_OK){
		DETHROW(deeInvalidParam);
	}
}

void projTaskDistribute::pZipWriteMemoryFile(const decMemoryFile &memoryFile){
	const decDateTime modtime(memoryFile.GetModificationTime());
	
	zip_fileinfo info;
	memset(&info, 0, sizeof(info));
	
	info.tmz_date.tm_year = modtime.GetYear();
	info.tmz_date.tm_mon = modtime.GetMonth();
	info.tmz_date.tm_mday = modtime.GetDay() + 1; // zip uses 1 as first day of month
	info.tmz_date.tm_hour = modtime.GetHour();
	info.tmz_date.tm_min = modtime.GetMinute();
	info.tmz_date.tm_sec = modtime.GetSecond();
	info.dosDate = 0; // use tmz_date
	info.internal_fa = 0; // no idea what this is
	info.external_fa = 0; // no idea what this is
	
	// NOTE: path contains '/' as prefix. delga files require path without prefix
	if(zipOpenNewFileInZip(pZipFile, memoryFile.GetFilename().GetMiddle(1), &info,
	NULL, 0, NULL, 0, NULL, Z_DEFLATED, Z_DEFAULT_COMPRESSION) != ZIP_OK){
		DETHROW(deeInvalidParam);
	}
	
	const int length = memoryFile.GetLength();
	if(length > 0){
		if(zipWriteInFileInZip(pZipFile, memoryFile.GetPointer(), length) != ZIP_OK){
			DETHROW(deeInvalidParam);
		}
	}
	
	if(zipCloseFileInZip(pZipFile) != ZIP_OK){
		DETHROW(deeInvalidParam);
	}
}

void projTaskDistribute::pCloseDirectory(){
	if(pStackDirectories.GetCount() == 0){
		DETHROW(deeInvalidAction);
	}
	
	pStackDirectories.RemoveFrom(pStackDirectories.GetCount() - 1);
}

void projTaskDistribute::pWriteGameXml(){
	// create memory file to use with the xml writer
	decPath pathGameXml;
	pathGameXml.SetFromUnix("/");
	
	if(!pProfile.GetAliasIdentifier().IsEmpty()){
		pathGameXml.AddComponent(pProfile.GetAliasIdentifier() + ".degame");
		
	}else{
		pathGameXml.AddComponent(pProfile.GetIdentifier().ToHexString(false) + ".degame");
	}
	
	decMemoryFile::Ref memoryFile(decMemoryFile::Ref::NewWith(pathGameXml.GetPathUnix()));
	{
	decXmlWriter xmlWriter(decMemoryFileWriter::Ref::NewWith(memoryFile, false));
	pWriteGameXml(xmlWriter);
	}
	pZipWriteMemoryFile(memoryFile);
}

void projTaskDistribute::pWriteGameXml(decXmlWriter &writer){
	writer.WriteXMLDeclaration();
	
	writer.WriteOpeningTag("degame");
	
	writer.WriteDataTagString("identifier", pProfile.GetIdentifier().ToHexString(false));
	if(!pProfile.GetAliasIdentifier().IsEmpty()){
		writer.WriteDataTagString("aliasIdentifier", pProfile.GetAliasIdentifier());
	}
	writer.WriteDataTagString("title", pProfile.GetTitle());
	writer.WriteDataTagString("description", pProfile.GetGameDescription());
	writer.WriteDataTagString("creator", pProfile.GetCreator());
	writer.WriteDataTagString("homepage", pProfile.GetWebsite());
	
	igdeEnvironment &env = pWindowMain.GetEnvironment();
	const decStringSet &iconPathList = pProfile.GetIcons();
	const int iconPathCount = iconPathList.GetCount();
	if(iconPathCount > 0){
		deImageManager &imageManager = *env.GetEngineController()->GetEngine()->GetImageManager();
		deImage::Ref icon;
		int i;
		for(i=0; i<iconPathCount; i++){
			icon.TakeOver(imageManager.LoadImage(iconPathList.GetAt(i), "/"));
			writer.WriteOpeningTagStart("icon");
			writer.WriteAttributeInt("size", icon->GetWidth());
			writer.WriteOpeningTagEnd(false, false);
			writer.WriteTextString(iconPathList.GetAt(i));
			writer.WriteClosingTag("icon", false);
		}
	}
	
	writer.WriteDataTagString("scriptDirectory", pProfile.GetScriptDirectory());
	writer.WriteDataTagString("gameObject", pProfile.GetGameObject());
	writer.WriteDataTagString("pathConfig", pProfile.GetPathConfig());
	writer.WriteDataTagString("pathCapture", pProfile.GetPathCapture());
	
	writer.WriteOpeningTagStart("scriptModule");
	if(!env.GetGameProject()->GetScriptModuleVersion().IsEmpty()){
		writer.WriteAttributeString("version", env.GetGameProject()->GetScriptModuleVersion());
	}
	writer.WriteOpeningTagEnd(false, false);
	writer.WriteTextString(env.GetGameProject()->GetScriptModule());
	writer.WriteClosingTag("scriptModule", false);
	
	const decPoint &windowSize = pProfile.GetWindowSize();
	if(windowSize != decPoint()){
		writer.WriteOpeningTagStart("windowSize");
		writer.WriteAttributeInt("x", windowSize.x);
		writer.WriteAttributeInt("y", windowSize.y);
		writer.WriteOpeningTagEnd(true);
	}
	
	pWriteGameXmlRequiredFormats(writer);
	
	writer.WriteClosingTag("degame");
}

void projTaskDistribute::pWriteGameXmlRequiredFormats(decXmlWriter &writer){
	const int usedExtCount = pUsedFileExtensions.GetCount();
	int i;
	
	for(i=0; i<usedExtCount; i++){
		const decString &extension = pUsedFileExtensions.GetAt(i);
		const deLoadableModule * const foundType = pGetMatchingModule(extension);
		if(!foundType){
			continue;
		}
		
		writer.WriteOpeningTagStart("requireFormat");
		writer.WriteAttributeString("type", pGetModuleTypeName(foundType->GetType()));
		writer.WriteOpeningTagEnd(false, false);
		writer.WriteTextString(extension);
		writer.WriteClosingTag("requireFormat", false);
	}
}

projTaskDistribute::cProcessDirectory *projTaskDistribute::GetProcessDirectory(){
	if(pStackDirectories.GetCount() == 0){
		return NULL;
	}
	return (cProcessDirectory*)pStackDirectories.GetAt(pStackDirectories.GetCount() - 1);
}
