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

#include "gdeLoadSaveSystem.h"
#include "gdeLoadSaveGameDefinition.h"
#include "gdeLoadSaveXmlEClass.h"
#include "../gdEditor.h"
#include "../gamedef/gdeGameDefinition.h"
#include "../gui/gdeWindowMain.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/filedialog/igdeFilePattern.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/filesystem/dePatternList.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deLoadableModule.h>



// Class gdeLoadSaveSystem
////////////////////////////

// Constructor, destructor
////////////////////////////

gdeLoadSaveSystem::gdeLoadSaveSystem(gdeWindowMain &windowMain) :
pWindowMain(windowMain),
pLSGameDef(*this, windowMain.GetEnvironment().GetLogger(), LOGSOURCE),
pLSXmlEClass(*this, windowMain.GetEnvironment().GetLogger(), LOGSOURCE)
{
	pBuildFilePattern();
}

gdeLoadSaveSystem::~gdeLoadSaveSystem(){
}



// Management
///////////////

gdeGameDefinition::Ref gdeLoadSaveSystem::LoadGameDefinition(const char *filename){
	if(!filename){
		DETHROW(deeInvalidParam);
	}
	
	const gdeGameDefinition::Ref gameDefinition(
		gdeGameDefinition::Ref::New(&pWindowMain.GetEnvironment()));
	
	pLSGameDef.LoadGameDefinition(gameDefinition, decDiskFileReader::Ref::New(filename));
	
	return gameDefinition;
}

void gdeLoadSaveSystem::SaveGameDefinition(const gdeGameDefinition &gameDefinition, const char *filename){
	if(!filename){
		DETHROW(deeInvalidParam);
	}
	
	pLSGameDef.SaveGameDefinition(gameDefinition, decDiskFileWriter::Ref::New(filename, false));
}



gdeObjectClass::Ref gdeLoadSaveSystem::LoadXmlEClass(const char *filename){
	if(!filename){
		DETHROW(deeInvalidParam);
	}
	
	return pLSXmlEClass.LoadXmlEClass(decDiskFileReader::Ref::New(filename));
}

void gdeLoadSaveSystem::SaveXmlEClass(const gdeGameDefinition &gameDefinition,
const gdeObjectClass &objectClass, const char *filename){
	pLSXmlEClass.SaveXmlEClass(gameDefinition, objectClass, decDiskFileWriter::Ref::New(filename, false));
}



// Private Functions
//////////////////////

void gdeLoadSaveSystem::pBuildFilePattern(){
	decString pattern;
	
	// load/save game definition
	pattern.Format("*%s", pLSGameDef.GetPattern().GetString());
	pFPGameDef.Add(igdeFilePattern::Ref::New(pLSGameDef.GetName(), pattern, pLSGameDef.GetPattern()));
	
	// load/save xml element class
	pattern.Format("*%s", pLSXmlEClass.GetPattern().GetString());
	pFPXmlEClass.Add(igdeFilePattern::Ref::New(pLSXmlEClass.GetName(), pattern, pLSXmlEClass.GetPattern()));
	
	// convert file pattern lists to fox ones
	pConvertToFOX(pFPGameDef, pFoxFPLGameDef);
	pConvertToFOX(pFPXmlEClass, pFoxFPLXmlEClass);
}

void gdeLoadSaveSystem::pConvertToFOX(const igdeFilePattern::List &fpl, decString &foxfpl){
	foxfpl.Empty();
	fpl.Visit([&](const igdeFilePattern &p){
		if(!foxfpl.IsEmpty()){
			foxfpl.AppendCharacter('\n');
		}
		foxfpl.AppendFormat("%s (%s)", p.GetName().GetString(), p.GetPattern().GetString());
	});
	
	//foxfpl.Append( "\nAll Files (*)" );
}
