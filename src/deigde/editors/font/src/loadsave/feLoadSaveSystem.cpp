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

#include "feLoadSaveSystem.h"
#include "feLoadSaveFont.h"
#include "../font/feFont.h"
#include "../gui/feWindowMain.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>

#include <dragengine/deEngine.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deLoadableModule.h>
#include <dragengine/systems/modules/font/deBaseFontModule.h>
#include <dragengine/filesystem/dePatternList.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/exceptions.h>



// Class feLoadSaveSystem
///////////////////////////

// Constructor, destructor
////////////////////////////

feLoadSaveSystem::feLoadSaveSystem(feWindowMain *wndMain){
	if(!wndMain) DETHROW(deeInvalidParam);
	
	pWndMain = wndMain;
}

feLoadSaveSystem::~feLoadSaveSystem(){
}
	


// Management
///////////////

void feLoadSaveSystem::UpdateLSFonts(){
	deEngine *engine = pWndMain->GetEngine();
	deModuleSystem *modSys = engine->GetModuleSystem();
	int m, moduleCount = modSys->GetModuleCount();
	deLoadableModule *loadableModule;
	
	pLSFonts.RemoveAll();
	
	// add a new load save font for each font module found in the engine that is also
	// running and usable therefore
	for(m=0; m<moduleCount; m++){
		loadableModule = modSys->GetModuleAt(m);
		
		if(loadableModule->GetType() != deModuleSystem::emtFont) continue;
		if(!loadableModule->IsLoaded()) continue;
		
		pLSFonts.Add(feLoadSaveFont::Ref::New((deBaseFontModule*)loadableModule->GetModule()));
	}
}



feFont::Ref feLoadSaveSystem::LoadFont(const char *filename, igdeGameDefinition *gameDefinition){
	if(!filename || !gameDefinition) DETHROW(deeInvalidParam);
	deEngine *engine = pWndMain->GetEngine();
	
	feLoadSaveFont &lsfont = pLSFonts.FindOrDefault([&](feLoadSaveFont &lf){
		return decString::StringMatchesPattern(filename, lf.GetPattern());
	});
	
	const feFont::Ref font(feFont::Ref::New(&pWndMain->GetEnvironment()));
	lsfont.LoadFont(filename, font, engine->GetVirtualFileSystem()->OpenFileForReading(
		decPath::CreatePathUnix(filename)));
	
	return font;
}

void feLoadSaveSystem::SaveFont(feFont *font, const char *filename){
	if(!font || !filename) DETHROW(deeInvalidParam);
	deEngine *engine = pWndMain->GetEngine();
	
	feLoadSaveFont &lsfont = pLSFonts.FindOrDefault([&](feLoadSaveFont &lf){
		return decString::StringMatchesPattern(filename, lf.GetPattern());
	});
	
	lsfont.SaveFont(filename, font, engine->GetVirtualFileSystem()->OpenFileForWriting(
		decPath::CreatePathUnix(filename)));
}




// Private Functions
//////////////////////
