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
#include <deigde/gui/filedialog/igdeFilePattern.h>
#include <deigde/gui/filedialog/igdeFilePatternList.h>
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
	
	pLSFonts = NULL;
	pLSFontCount = 0;
	pLSFontSize = 0;
}

feLoadSaveSystem::~feLoadSaveSystem(){
	pCleanUp();
}
	


// Management
///////////////

feLoadSaveFont *feLoadSaveSystem::GetLSFontAt(int index) const{
	if(index < 0 || index >= pLSFontCount) DETHROW(deeInvalidParam);
	
	return pLSFonts[index];
}

int feLoadSaveSystem::IndexOfLSFont(feLoadSaveFont *lsFont) const{
	if(!lsFont) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pLSFontCount; i++){
		if(lsFont == pLSFonts[i]){
			return i;
		}
	}
	
	return -1;
}

bool feLoadSaveSystem::HasLSFont(feLoadSaveFont *lsFont) const{
	if(!lsFont) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pLSFontCount; i++){
		if(lsFont == pLSFonts[i]){
			return true;
		}
	}
	
	return false;
}

int feLoadSaveSystem::IndexOfLSFontMatching(const char *filename){
	const decString testFilename(filename);
	int i;
	
	for(i=0; i<pLSFontCount; i++){
		if(testFilename.MatchesPattern(pLSFonts[i]->GetPattern())){
			return i;
		}
	}
	
	return -1;
}

void feLoadSaveSystem::AddLSFont(feLoadSaveFont *lsFont){
	if(HasLSFont(lsFont)) DETHROW(deeInvalidParam);
	
	if(pLSFontCount == pLSFontSize){
		int newSize = pLSFontSize * 3 / 2 + 1;
		feLoadSaveFont **newArray = new feLoadSaveFont*[newSize];
		if(!newArray) DETHROW(deeOutOfMemory);
		if(pLSFonts){
			memcpy(newArray, pLSFonts, sizeof(feLoadSaveFont*) * pLSFontSize);
			delete [] pLSFonts;
		}
		pLSFonts = newArray;
		pLSFontSize = newSize;
	}
	
	pLSFonts[pLSFontCount] = lsFont;
	pLSFontCount++;
}

void feLoadSaveSystem::RemoveLSFont(feLoadSaveFont *lsFont){
	int i, index = IndexOfLSFont(lsFont);
	if(index == -1) DETHROW(deeInvalidParam);
	
	for(i=index+1; i<pLSFontCount; i++){
		pLSFonts[i - 1] = pLSFonts[i];
	}
	pLSFontCount--;
	
	delete lsFont;
}

void feLoadSaveSystem::RemoveAllLSFonts(){
	while(pLSFontCount > 0){
		pLSFontCount--;
		delete pLSFonts[pLSFontCount];
	}
}

void feLoadSaveSystem::UpdateLSFonts(){
	deEngine *engine = pWndMain->GetEngine();
	deModuleSystem *modSys = engine->GetModuleSystem();
	int m, moduleCount = modSys->GetModuleCount();
	deLoadableModule *loadableModule;
	feLoadSaveFont *lsFont = NULL;
	
	// remove all load save fonts
	RemoveAllLSFonts();
	
	try{
		// add a new load save font for each font module found in the engine that is also
		// running and usable therefore
		for(m=0; m<moduleCount; m++){
			loadableModule = modSys->GetModuleAt(m);
			
			if(loadableModule->GetType() != deModuleSystem::emtFont) continue;
			if(!loadableModule->IsLoaded()) continue;
			
			lsFont = new feLoadSaveFont((deBaseFontModule*)loadableModule->GetModule());
			if(!lsFont) DETHROW(deeOutOfMemory);
			
			AddLSFont(lsFont);
			lsFont = NULL;
		}
		
	}catch(const deException &){
		if(lsFont) delete lsFont;
		throw;
	}
}



feFont::Ref feLoadSaveSystem::LoadFont(const char *filename, igdeGameDefinition *gameDefinition){
	if(!filename || !gameDefinition) DETHROW(deeInvalidParam);
	//decDiskFileReader file( filename );
	deEngine *engine = pWndMain->GetEngine();
	decBaseFileReader::Ref fileReader = NULL;
	feFont::Ref font = NULL;
	decPath path;
	int lsIndex;
	
	lsIndex = IndexOfLSFontMatching(filename);
	if(lsIndex == -1) DETHROW(deeInvalidParam);
	
	path.SetFromUnix(filename);
	
	try{
		fileReader = engine->GetVirtualFileSystem()->OpenFileForReading(path);
		
		font.TakeOver(new feFont(&pWndMain->GetEnvironment()));
		if(!font) DETHROW(deeOutOfMemory);
		
		pLSFonts[lsIndex]->LoadFont(filename, font, fileReader);
	}catch(const deException &){
		if(font) font->FreeReference();
		throw;
	}
	
	return font;
}

void feLoadSaveSystem::SaveFont(feFont *font, const char *filename){
	if(!font || !filename) DETHROW(deeInvalidParam);
	deEngine *engine = pWndMain->GetEngine();
	decBaseFileWriter::Ref fileWriter = NULL;
	decPath path;
	int lsIndex;
	
	lsIndex = IndexOfLSFontMatching(filename);
	if(lsIndex == -1) DETHROW(deeInvalidParam);
	
//	decDiskFileWriter file( filename, false );
	
	path.SetFromUnix(filename);
	
	try{
		fileWriter = engine->GetVirtualFileSystem()->OpenFileForWriting(path);
		
		pLSFonts[lsIndex]->SaveFont(filename, font, fileWriter);
	}catch(const deException &){
		throw;
	}
}




// Private Functions
//////////////////////	

void feLoadSaveSystem::pCleanUp(){
	RemoveAllLSFonts();
	if(pLSFonts) delete [] pLSFonts;
}
