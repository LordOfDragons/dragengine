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
#include <sys/types.h>
#include <sys/stat.h>

#include "deLibraryModule.h"
#include "deLoadableModuleVisitor.h"
#include "deBaseModule.h"
#include "../deModuleSystem.h"
#include "../../deEngine.h"
#include "../../logger/deLogger.h"
#include "../../common/xmlparser/decXmlParser.h"
#include "../../common/xmlparser/decXmlDocument.h"
#include "../../common/xmlparser/decXmlElementTag.h"
#include "../../common/xmlparser/decXmlCharacterData.h"
#include "../../common/xmlparser/decXmlAttValue.h"
#include "../../common/xmlparser/decXmlVisitor.h"
#include "../../common/file/decDiskFileReader.h"
#include "../../common/file/decPath.h"
#include "../../common/exceptions.h"

#ifdef OS_W32
#include "../../app/deOSWindows.h"
#endif



// Definitions
////////////////

#define LOGSOURCE "Dragengine"

typedef deBaseModule* (*FUNC_CREATEMODULE)(deLoadableModule*);



// class deLibraryModule
///////////////////////////

// Constructor, destructor
////////////////////////////

deLibraryModule::deLibraryModule(deModuleSystem *system, const char *xmlDefFilename) : deLoadableModule(system){
	if(! xmlDefFilename){
		DETHROW(deeInvalidParam);
	}
	
	pLibFileSize = 0;
	#ifdef OS_BEOS
	pLibHandle = 0;
	#else
	pLibHandle = NULL;
	#endif
	
	try{
		pLoadXML(xmlDefFilename);
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deLibraryModule::~deLibraryModule(){
	pCleanUp();
}



// Module management
//////////////////////

void deLibraryModule::LoadModule(){
	if(IsLoaded()){
		DETHROW(deeInvalidAction);
	}
	
	if(! pVerifyLibrary(pLibFileName)){
		return;
	}
	if(! pLoadLibrary(pLibFileName)){
		return;
	}
	
	SetDefaultLoggingName();
}

void deLibraryModule::UnloadModule(){
	if(! IsLoaded() || IsLocked()){
		DETHROW(deeInvalidAction);
	}
	
	// destroy module and clear error code
	SetModule(NULL);
	SetErrorCode(eecSuccess);
	
	// close library if a handler exist
	if(pLibHandle){
		#ifdef OS_BEOS
		unload_add_on(pLibHandle);
		#endif
		
		#ifdef HAS_LIB_DL
		dlclose(pLibHandle);
		pLibHandle = NULL;
		#endif
		
		#ifdef OS_W32
		FreeLibrary(pLibHandle);
		pLibHandle = NULL;
		#endif
	}
}



// Type Testing and Safe Casting
//////////////////////////////////

bool deLibraryModule::IsLibraryModule() const{
	return true;
}

deLibraryModule *deLibraryModule::CastToLibraryModule(){
	return this;
}



// Visiting
/////////////

void deLibraryModule::Visit(deLoadableModuleVisitor &visitor){
	visitor.VisitLibraryModule(*this);
}



// Private functions
//////////////////////

void deLibraryModule::pCleanUp(){
	if(pLibHandle){
		SetModule(NULL);
		
		#ifdef OS_BEOS
		unload_add_on(pLibHandle);
		#endif
		
		#ifdef HAS_LIB_DL
		dlclose(pLibHandle);
		#endif
		
		#ifdef OS_W32
		FreeLibrary(pLibHandle);
		#endif
	}
	
	pUnloadPreloadedLibraries();
}

bool deLibraryModule::pLoadLibrary(const char *filename){
	deLogger &logger = *GetSystem()->GetEngine()->GetLogger();
	FUNC_CREATEMODULE funcCreateModule;
	
	// preload libraries if required
	pPreloadLibraries();
	if(GetErrorCode() != eecSuccess){
		return false;
	}
	
	// try loading module located at pLibFileName
	#ifdef OS_BEOS
	decPath pathLib;
	pathLib.SetFromNative(filename);
	pathLib.RemoveLastComponent(); // module.xml
	decString newEnvVarLibPath(pathLib.GetPathNative());
	
	const char * const checkEnvVarLibPath = getenv("LIBRARY_PATH");
	decString oldEnvVarLibPath;
	if(checkEnvVarLibPath){
		oldEnvVarLibPath = checkEnvVarLibPath;
		newEnvVarLibPath = newEnvVarLibPath + ":" + checkEnvVarLibPath;
	}
	setenv("LIBRARY_PATH", newEnvVarLibPath, 1);
	
	pLibHandle = load_add_on(filename);
	
	if(checkEnvVarLibPath){
		setenv("LIBRARY_PATH", oldEnvVarLibPath, 1);
		
	}else{
		unsetenv("LIBRARY_PATH");
	}
	
	if(pLibHandle < 0){
		logger.LogErrorFormat(LOGSOURCE, "load_add_on failed: %s", strerror(pLibHandle));
		SetErrorCode(eecLibFileOpenFailed);
		return false;
	}
	#endif
	
	#ifdef HAS_LIB_DL
	pLibHandle = dlopen(filename, RTLD_NOW);
	if(! pLibHandle){
		logger.LogErrorFormat(LOGSOURCE, "dlerror: %s.", dlerror());
		SetErrorCode(eecLibFileOpenFailed);
		return false;
	}
	#endif
	
	#ifdef OS_W32
	//decString oldPath;
	//int requiredLength = GetCurrentDirectory( 0, NULL );
	//oldPath.Set( ' ', requiredLength );
	//GetCurrentDirectory( requiredLength + 1, ( LPTSTR )oldPath.GetString() );
	decPath pathLib;
	pathLib.SetFromNative(filename);
	pathLib.RemoveLastComponent();
	//SetCurrentDirectory( pathLib.GetPathNative() );
	wchar_t widePath[MAX_PATH];
	deOSWindows::Utf8ToWide(pathLib.GetPathNative(), widePath, MAX_PATH);
	SetDllDirectory(widePath);
	deOSWindows::Utf8ToWide(filename, widePath, MAX_PATH);
	pLibHandle = LoadLibrary(widePath);
	SetDllDirectory(NULL);
	//SetCurrentDirectory( oldPath.GetString() );
	if(! pLibHandle){
		int err = GetLastError();
		wchar_t messageBuffer[251];
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			messageBuffer, 250, NULL);
		
		logger.LogErrorFormat(LOGSOURCE, "LoadLibrary(err=%i): %s.",
			err, deOSWindows::WideToUtf8(messageBuffer).GetString());
	}
	#endif
	
	if(! pLibHandle){
		SetErrorCode(eecLibFileOpenFailed);
		return false;
	}
	
	// look for the entry point function
	#ifdef OS_BEOS
	if(get_image_symbol(pLibHandle, pLibFileEntryPoint,
	B_SYMBOL_TYPE_TEXT, (void**)&funcCreateModule) != B_OK){
		funcCreateModule = NULL;
	}
	#endif
	
	#ifdef HAS_LIB_DL
	funcCreateModule = (FUNC_CREATEMODULE)dlsym(pLibHandle, pLibFileEntryPoint);
	#endif
	
	#ifdef OS_W32
	funcCreateModule = (FUNC_CREATEMODULE)GetProcAddress(pLibHandle, pLibFileEntryPoint);
	#endif
	
	if(! funcCreateModule){
		SetErrorCode(eecLibFileEntryPointNotFound);
		return false;
	}
	
	// create package
	SetModule(funcCreateModule(this));
	if(! GetModule()){
		SetErrorCode(eecLibFileCreateModuleFailed);
		return false;
	}
	
	// success
	return true;
}

void deLibraryModule::pLoadXML(const char* filename){
	decDiskFileReader *reader = NULL;
	
	try{
		reader = new decDiskFileReader(filename);
		pParseXML(filename, *reader);
		reader->FreeReference();
		
	}catch(const deException &){
		if(reader){
			reader->FreeReference();
		}
		throw;
	}
	
	pVerifyModule();
}

void deLibraryModule::pParseXML(const char *filename, decBaseFileReader &reader){
	decXmlParser parser(GetSystem()->GetEngine()->GetLogger());
	decStringList &patternList = GetPatternList();
	decXmlElementTag *root, *tag, *tag2;
	decXmlElement *element;
	const char *filepath;
	int i, j;
	
	decPath basePath;
	basePath.SetFromNative(filename);
	basePath.RemoveLastComponent(); // module.xml
	basePath.RemoveLastComponent(); // version
	SetDirectoryName(basePath.GetLastComponent());
	
	// reset parameters so we can test later on
	SetName("");
	SetDescription("");
	SetAuthor("");
	SetVersion("");
	SetType(deModuleSystem::emtUnknown);
	patternList.RemoveAll();
	SetDefaultExtension("");
	
	// parse xml
	decXmlDocument::Ref xmlDoc(decXmlDocument::Ref::NewWith());
	
	parser.ParseXml(&reader, xmlDoc);
	
	// some cleanup visiting
	xmlDoc->StripComments();
	xmlDoc->CleanCharData();
	
	// interpretate xml
	root = xmlDoc->GetRoot();
	if(! root || strcmp(root->GetName(), "module") != 0){
		DETHROW(deeInvalidParam);
	}
	
	for(i=0; i<root->GetElementCount(); i++){
		element = root->GetElementAt(i);
		if(! element->CanCastToElementTag()) continue;
		
		tag = element->CastToElementTag();
		
		if(tag->GetName() == "name"){
			if(tag->GetFirstData()){
				SetName(tag->GetFirstData()->GetData());
				
			}else{
				SetName("");
			}
			
		}else if(tag->GetName() == "description"){
			if(tag->GetFirstData()){
				SetDescription(tag->GetFirstData()->GetData());
				
			}else{
				SetDescription("");
			}
			
		}else if(tag->GetName() == "author"){
			if(tag->GetFirstData()){
				SetAuthor(tag->GetFirstData()->GetData());
				
			}else{
				SetAuthor("");
			}
			
		}else if(tag->GetName() == "version"){
			if(tag->GetFirstData()){
				SetVersion(tag->GetFirstData()->GetData());
				
			}else{
				SetVersion("");
			}
			
		}else if(tag->GetName() == "type"){
			if(tag->GetFirstData()){
				SetType(deModuleSystem::GetTypeFromString(tag->GetFirstData()->GetData()));
				
			}else{
				SetType(deModuleSystem::emtUnknown);
			}
			
		}else if(tag->GetName() == "pattern"){
			if(tag->GetFirstData()){
				patternList.Add(tag->GetFirstData()->GetData());
			}
			
		}else if(tag->GetName() == "defaultExtension"){
			if(tag->GetFirstData()){
				SetDefaultExtension(tag->GetFirstData()->GetData());
				
			}else{
				SetDefaultExtension("");
			}
			
		}else if(tag->GetName() == "library"){
			for(j=0; j<tag->GetElementCount(); j++){
				element = tag->GetElementAt(j);
				if(! element->CanCastToElementTag()){
					continue;
				}
				
				tag2 = element->CastToElementTag();
				
				if(strcmp(tag2->GetName(), "file") == 0){
					if(tag2->GetFirstData()){
						filepath = tag2->GetFirstData()->GetData();
						
					}else{
						filepath = "";
					}
					
					if(decPath::IsNativePathAbsolute(filepath)){
						pLibFileName = filepath;
						
					}else{
						decPath libPath;
						libPath.SetFromNative(filename);
						libPath.RemoveLastComponent(); // module.xml
						libPath.AddNativePath(filepath);
						pLibFileName = libPath.GetPathNative();
					}
					
				}else if(strcmp(tag2->GetName(), "size") == 0){
					pLibFileSize = (int)strtol(tag2->GetFirstData()->GetData(), NULL, 10);
					
				}else if(strcmp(tag2->GetName(), "sha1") == 0){
					if(tag2->GetFirstData()){
						pLibFileHash = tag2->GetFirstData()->GetData();
						
					}else{
						pLibFileHash = "";
					}
					
				}else if(strcmp(tag2->GetName(), "entrypoint") == 0){
					if(tag2->GetFirstData()){
						pLibFileEntryPoint = tag2->GetFirstData()->GetData();
						
					}else{
						pLibFileEntryPoint = "";
					}
					
				}else if(strcmp(tag2->GetName(), "preloadLibrary") == 0){
					if(tag2->GetFirstData()){
						decPath libPath;
						libPath.SetFromNative(filename);
						libPath.RemoveLastComponent(); // module.xml
						libPath.AddNativePath(tag2->GetFirstData()->GetData());
						pPreloadLibraryPath.Add(libPath.GetPathNative());
					}
				}
			}
			
		}else if(tag->GetName() == "data"){
			/*
			for(j=0; j<tag->GetElementCount(); j++){
				element = tag->GetElementAt(j);
				if(! element->CanCastToElementTag()) continue;
				tag2 = element->CastToElementTag();
				if(strcmp(tag2->GetName(), "directory") == 0){
					// todo
				}
			}
			*/
			
		}else if(tag->GetName() == "fallback"){
			SetIsFallback(true);
			
		}else if(tag->GetName() == "noSaving"){
			SetNoSaving(true);
			
		}else if(tag->GetName() == "noCompress"){
			SetNoCompress(true);
			
		}else if(tag->GetName() == "priority"){
			if(tag->GetFirstData()){
				SetPriority(tag->GetFirstData()->GetData().ToInt());
			}
		}
	}
	
	if(GetDefaultExtension().IsEmpty() && patternList.GetCount() > 0){
		SetDefaultExtension(patternList.GetAt(0));
	}
}

bool deLibraryModule::pVerifyLibrary(const char* filename){
#ifdef OS_W32
	wchar_t widePath[MAX_PATH];
	deOSWindows::Utf8ToWide(filename, widePath, MAX_PATH);
	
	WIN32_FILE_ATTRIBUTE_DATA fa;
	if(! GetFileAttributesExW(widePath, GetFileExInfoStandard, &fa)){
		SetErrorCode(eecLibFileNotFound);
		return false;
	}
	
	if((fa.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY){
		SetErrorCode(eecLibFileNotRegularFile);
		return false;
	}
	
	#ifndef NO_ENGINE_MODULE_CHECKS
	if(((uint64_t)fa.nFileSizeHigh << 32) + (uint64_t)fa.nFileSizeLow != (uint64_t)pLibFileSize){
		SetErrorCode(eecLibFileSizeMismatch);
		return false;
	}
	#endif
	
#else
	struct stat fs;
	
	// get file attributes
	if(stat(filename, &fs) != 0){ // not found
		SetErrorCode(eecLibFileNotFound);
		return false;
	}
	
	// check that this is really a file
	if(! S_ISREG(fs.st_mode)){
		SetErrorCode(eecLibFileNotRegularFile);
		return false;
	}
	
	// check that the file size matches the one specified
	#ifndef NO_ENGINE_MODULE_CHECKS
	if(fs.st_size != pLibFileSize){
		SetErrorCode(eecLibFileSizeMismatch);
		return false;
	}
	#endif
#endif
	
	// check that the sha1 sum is the same
	
	// checks passed
	return true;
}

void deLibraryModule::pVerifyModule(){
	if(GetName().IsEmpty()){
		DETHROW(deeInvalidParam);
	}
	if(GetType() == deModuleSystem::emtUnknown){
		DETHROW(deeInvalidParam);
	}
	if(GetAuthor().IsEmpty()){
		DETHROW(deeInvalidParam);
	}
	if(GetVersion().IsEmpty()){
		DETHROW(deeInvalidParam);
	}
	if(pLibFileName.IsEmpty()){
		DETHROW(deeInvalidParam);
	}
	if(pLibFileEntryPoint.IsEmpty()){
		DETHROW(deeInvalidParam);
	}
	/*
	if(pLibFileHash.IsEmpty()){
		DETHROW(deeInvalidParam);
	}
	*/
}

void deLibraryModule::pPreloadLibraries(){
	// haiku
	#ifdef OS_BEOS
	#if 0
	deLogger &logger = *GetSystem()->GetEngine()->GetLogger();
	const int count = pPreloadLibraryPath.GetCount();
	image_id handleLibrary = 0;
	decPath path;
	int i;
	
	for(i=0; i<count; i++){
		const decString &filename = pPreloadLibraryPath.GetAt(i);
		path.SetFromNative(filename);
		
		handleLibrary = load_add_on(filename);
		if(! handleLibrary){
			logger.LogErrorFormat(LOGSOURCE, "Preloaded %s: load_add_on failed: %s",
				path.GetLastComponent().GetString(), strerror(pLibHandle));
			SetErrorCode(eecLibFileOpenFailed);
			return;
		}
		
		pPreloadedLibraries.Add((void*)(intptr_t)handleLibrary);
		handleLibrary = 0;
		logger.LogInfoFormat(LOGSOURCE, "  - Preloaded %s", path.GetLastComponent().GetString());
	}
	#endif
	#endif
	
	// unix, android
	//#ifdef OS_ANDROID
	#if defined OS_UNIX && ! defined OS_BEOS
	deLogger &logger = *GetSystem()->GetEngine()->GetLogger();
	const int count = pPreloadLibraryPath.GetCount();
	void *handleLibrary = NULL;
	decPath path;
	int i;
	
	for(i=0; i<count; i++){
		const decString &filename = pPreloadLibraryPath.GetAt(i);
		path.SetFromNative(filename);
		
		handleLibrary = dlopen(filename, RTLD_NOW);
		
		if(! handleLibrary){
			logger.LogErrorFormat(LOGSOURCE, "%s dlerror: %s.", path.GetLastComponent().GetString(), dlerror());
			SetErrorCode(eecLibFileOpenFailed);
			return;
		}
		
		pPreloadedLibraries.Add(handleLibrary);
		handleLibrary = NULL;
		logger.LogInfoFormat(LOGSOURCE, "  - Preloaded %s", path.GetLastComponent().GetString());
	}
	#endif
}

void deLibraryModule::pUnloadPreloadedLibraries(){
	// beos
	#ifdef OS_BEOS
	int index = pPreloadedLibraries.GetCount();
	while(index > 0){
		index--;
		unload_add_on((image_id)(intptr_t)pPreloadedLibraries.GetAt(index));
		pPreloadedLibraries.RemoveFrom(index);
	}
	#endif
	
	// unix, android
	//#ifdef OS_ANDROID
	#if defined OS_UNIX && ! defined OS_BEOS
	int index = pPreloadedLibraries.GetCount();
	
	while(index > 0){
		index--;
		dlclose(pPreloadedLibraries.GetAt(index));
		pPreloadedLibraries.RemoveFrom(index);
	}
	#endif
}
