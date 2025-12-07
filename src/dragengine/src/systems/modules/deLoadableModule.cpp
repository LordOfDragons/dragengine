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

#include "deLoadableModule.h"
#include "deInternalModule.h"
#include "deLibraryModule.h"
#include "deLoadableModuleVisitor.h"
#include "deBaseModule.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"



// Class deLoadableModule
///////////////////////////

// Constructor, destructor
////////////////////////////

deLoadableModule::deLoadableModule(deModuleSystem *system) :
pSystem(system),
	
pName("Unnamed"),
pType(deModuleSystem::emtUnknown),

pDescription("No description"),
pAuthor("Unknown"),
pVersion("1.0"),

pPriority(0),
pFallback(false),
pNoSaving(false),
pNoCompress(false),

pEnabled(true),

pModule(NULL),
pLockCount(0),

pErrorCode(eecSuccess)
{
	if(!system){
		DETHROW(deeInvalidParam);
	}
	
	try{
		SetDefaultLoggingName();
		
	}catch(const deException &){
		pLMCleanUp();
		throw;
	}
}

deLoadableModule::~deLoadableModule(){
	pLMCleanUp();
}



// Module management
//////////////////////

void deLoadableModule::SetLoggingName(const char *name){
	pLoggingName = name;
}

void deLoadableModule::SetDefaultLoggingName(){
	pLoggingName = pName;
}



void deLoadableModule::SetName(const char *name){
	pName = name;
}

void deLoadableModule::SetType(deModuleSystem::eModuleTypes type){
	pType = type;
}

void deLoadableModule::SetDescription(const char *description){
	pDescription = description;
}

void deLoadableModule::SetAuthor(const char *author){
	pAuthor = author;
}

void deLoadableModule::SetVersion(const char *version){
	pVersion = version;
}

void deLoadableModule::SetDefaultExtension(const char *extension){
	pDefaultExtension = extension;
}

void deLoadableModule::SetDirectoryName(const char *dirName){
	pDirName = dirName;
}

void deLoadableModule::SetPriority(int priority){
	pPriority = priority;
}

void deLoadableModule::SetIsFallback(bool fallback){
	pFallback = fallback;
}

void deLoadableModule::SetNoSaving(bool noSaving){
	pNoSaving = noSaving;
}

void deLoadableModule::SetNoCompress(bool noCompress){
	pNoCompress = noCompress;
}

void deLoadableModule::SetEnabled(bool enabled){
	pEnabled = enabled;
}



// Module Management
//////////////////////

void deLoadableModule::SetModule(deBaseModule *module){
	if(pModule){
		delete pModule;
	}
	pModule = module;
}

bool deLoadableModule::IsLoaded() const{
	return pModule != NULL;
}

void deLoadableModule::SetErrorCode(int code){
	pErrorCode = code;
}

void deLoadableModule::Lock(){
	pLockCount++;
}

void deLoadableModule::Unlock(){
	if(pLockCount == 0){
		DETHROW(deeInvalidParam);
	}
	pLockCount--;
}



// Type Testing and Safe Casting
//////////////////////////////////

bool deLoadableModule::IsInternalModule() const{
	return false;
}

bool deLoadableModule::IsLibraryModule() const{
	return false;
}

deInternalModule *deLoadableModule::CastToInternalModule(){
	DETHROW(deeInvalidParam);
}

deLibraryModule *deLoadableModule::CastToLibraryModule(){
	DETHROW(deeInvalidParam);
}



// visiting
/////////////

void deLoadableModule::Visit(deLoadableModuleVisitor &visitor){
	visitor.VisitLoadableModule(*this);
}



// Private functions
//////////////////////

void deLoadableModule::pLMCleanUp(){
	if(pModule){
		delete pModule;
	}
}
