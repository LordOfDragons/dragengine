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

#include "delEngineInstance.h"

#include <dragengine/common/exceptions.h>



// Class delEngineInstance::Factory
/////////////////////////////////////

delEngineInstance::Factory::Factory(){}
delEngineInstance::Factory::~Factory(){}



// Class delEngineInstance
////////////////////////////

// Constructors and Destructors
/////////////////////////////////

delEngineInstance::delEngineInstance( delLauncher &launcher, const char *logfile ) :
pLauncher( launcher ),
pLogFile( logfile ),
pUseConsole( false ){
}

delEngineInstance::~delEngineInstance(){
}



// Management
///////////////

void delEngineInstance::SetLogFile( const decString &logFile ){
	pLogFile = logFile;
}

void delEngineInstance::SetUseConsole( bool useConsole ){
	pUseConsole = useConsole;
}

#ifdef OS_BEOS
void delEngineInstance::BeosMessageReceived( BMessage *message ){
}
#endif

#ifdef OS_ANDROID
void delEngineInstance::ReadDelgaGameDefsVfs(const deVFSContainer::Ref &container,
const char *delgaFile, decStringList &list){
	DETHROW_INFO(deeInvalidAction, "not supported");
}

void delEngineInstance::ReadDelgaFilesVfs(const deVFSContainer::Ref &container,
const char *delgaFile, const decStringList &filenames, decObjectOrderedSet &filesContent ){
	DETHROW_INFO(deeInvalidAction, "not supported");
}

void delEngineInstance::VFSAddDelgaFileVfs(const deVFSContainer::Ref &container,
const char *delgaFile, const char *archivePath, const decStringSet &hiddenPath){
	DETHROW_INFO(deeInvalidAction, "not supported");
}

void delEngineInstance::RunSingleFrameUpdate(){
	DETHROW_INFO(deeInvalidAction, "not supported");
}

void delEngineInstance::SetAppActive(bool gained){
	DETHROW_INFO(deeInvalidAction, "not supported");
}

void delEngineInstance::SetAppPaused(bool paused){
	DETHROW_INFO(deeInvalidAction, "not supported");
}

void delEngineInstance::UpdateContentRect(const decBoundary &contentRect){
	DETHROW_INFO(deeInvalidAction, "not supported");
}

void delEngineInstance::InputEvent(const android_input_buffer &inputBuffer){
	DETHROW_INFO(deeInvalidAction, "not supported");
}
#endif
