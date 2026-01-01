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

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "peeLoadSaveSystem.h"
#include "peeLoadSaveEmitter.h"
#include "../emitter/peeEmitter.h"
#include "../gui/peeWindowMain.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>

#include <dragengine/deEngine.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deLoadableModule.h>
#include <dragengine/filesystem/dePatternList.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>



// Definitions
////////////////

#define LOGSOURCE "Particle Emitter Editor"



// Class peeLoadSaveSystem
///////////////////////////

// Constructor, destructor
////////////////////////////

peeLoadSaveSystem::peeLoadSaveSystem(peeWindowMain &windowMain) :
pWindowMain(windowMain)
{
	pLSEmitter = new peeLoadSaveEmitter(this, windowMain.GetEnvironment().GetLogger(), LOGSOURCE);
	pBuildFilePattern();
}

peeLoadSaveSystem::~peeLoadSaveSystem(){
	if(pLSEmitter) delete pLSEmitter;
}



// Management
///////////////

peeEmitter::Ref peeLoadSaveSystem::LoadEmitter(const char *filename){
	if(!filename) DETHROW(deeInvalidParam);
	
	const peeEmitter::Ref emitter(peeEmitter::Ref::New(&pWindowMain.GetEnvironment(), *this));
	emitter->SetFilePath(filename);
	
	pLSEmitter->LoadEmitter(*this, emitter, pWindowMain.GetEngineController().GetEngine()->
		GetVirtualFileSystem()->OpenFileForReading(decPath::CreatePathUnix(filename)));
	
	return emitter;
}

void peeLoadSaveSystem::SaveEmitter(peeEmitter *emitter, const char *filename){
	if(!emitter || !filename) DETHROW(deeInvalidParam);
	
	pLSEmitter->SaveEmitter(*this, *emitter, pWindowMain.GetEngineController().GetEngine()->
		GetVirtualFileSystem()->OpenFileForWriting(decPath::CreatePathUnix(filename)));
}




// Private Functions
//////////////////////	

void peeLoadSaveSystem::pBuildFilePattern(){
	decString pattern;
	pattern.Format("*%s", pLSEmitter->GetPattern().GetString());
	pFPEmitter.Add(igdeFilePattern::Ref::New(pLSEmitter->GetName(), pattern, pLSEmitter->GetPattern()));
}
