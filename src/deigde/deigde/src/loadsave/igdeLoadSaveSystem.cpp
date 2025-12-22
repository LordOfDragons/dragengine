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

#include "igdeLoadSaveSystem.h"
#include "igdeLSGameProject.h"
#include "../gui/igdeWindowMain.h"
#include "../environment/igdeEnvironmentIGDE.h"

#include <deigde/gui/filedialog/igdeFilePattern.h>
#include <deigde/engine/igdeEngineController.h>
#include <deigde/gameproject/igdeGameProject.h>

#include <dragengine/deEngine.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deLoadableModule.h>
#include <dragengine/filesystem/dePatternList.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/exceptions.h>



// Definitions
////////////////

struct sFPLMapping{
	igdeLoadSaveSystem::eFilePatternLists list;
	deModuleSystem::eModuleTypes moduleType;
};

#define FPL_MAPPING_COUNT 10

static sFPLMapping vFPLMappings[FPL_MAPPING_COUNT] = {
	{igdeLoadSaveSystem::efplModel, deModuleSystem::emtModel},
	{igdeLoadSaveSystem::efplSkin, deModuleSystem::emtSkin},
	{igdeLoadSaveSystem::efplRig, deModuleSystem::emtRig},
	{igdeLoadSaveSystem::efplAnimation, deModuleSystem::emtAnimation},
	{igdeLoadSaveSystem::efplImage, deModuleSystem::emtImage},
	{igdeLoadSaveSystem::efplSound, deModuleSystem::emtSound},
	{igdeLoadSaveSystem::efplOcclusionMesh, deModuleSystem::emtOcclusionMesh},
	{igdeLoadSaveSystem::efplVideo, deModuleSystem::emtVideo},
	{igdeLoadSaveSystem::efplLanguagePack, deModuleSystem::emtLanguagePack},
	{igdeLoadSaveSystem::efplFont, deModuleSystem::emtFont}
};



// Class igdeLoadSaveSystem
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeLoadSaveSystem::igdeLoadSaveSystem(igdeWindowMain *windowMain){
	if(!windowMain){
		DETHROW(deeInvalidParam);
	}
	
	pWindowMain = windowMain;
	pLSGameProject = nullptr;
	
	try{
		pLSGameProject = new igdeLSGameProject(this);
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

igdeLoadSaveSystem::~igdeLoadSaveSystem(){
	pCleanUp();
}



// Management
///////////////

igdeGameProject::Ref igdeLoadSaveSystem::LoadGameProject(const char *filename){
	DEASSERT_NOTNULL(filename)
	
	const igdeGameProject::Ref project(igdeGameProject::Ref::New(pWindowMain->GetEnvironment()));
	project->SetFilePath(filename);
	
	pLSGameProject->Load(filename, project, decDiskFileReader::Ref::New(filename));
	
	return project;
}

void igdeLoadSaveSystem::SaveGameProject(igdeGameProject *project, const char *filename){
	DEASSERT_NOTNULL(project)
	DEASSERT_NOTNULL(filename)
	
	pLSGameProject->Save(project, decDiskFileWriter::Ref::New(filename, false));
}



void igdeLoadSaveSystem::UpdatePatternLists(){
	const deEngine &engine = *pWindowMain->GetEngine();
	const deModuleSystem &modSys = *engine.GetModuleSystem();
	const int moduleCount = modSys.GetModuleCount();
	int i, j, k;
	
	// reset the file pattern lists
	for(i=0; i<FilePatternListCount; i++){
		pFPLOpen[i].RemoveAll();
		pFPLSave[i].RemoveAll();
	}
	
	// add show-all file patterns to the file pattern lists. these have to come first
	// in the load file pattern lists. save lists do not have them
	decString showAllFormat[FPL_MAPPING_COUNT];
	decString showAllDefaultExtension[FPL_MAPPING_COUNT];
	
	for(i=0; i<FPL_MAPPING_COUNT; i++){
		for(j=0; j<moduleCount; j++){
			const deLoadableModule &loadableModule = *modSys.GetModuleAt(j);
			
			if(loadableModule.GetType() == vFPLMappings[i].moduleType){
				const decStringList &patternList = loadableModule.GetPatternList();
				const int patternCount = patternList.GetCount();
				
				for(k=0; k<patternCount; k++){
					if(!showAllFormat[i].IsEmpty()){
						showAllFormat[i].AppendCharacter(',');
					}
					showAllFormat[i].AppendCharacter('*');
					showAllFormat[i].Append(patternList.GetAt(k));
				}
				
				if(showAllDefaultExtension[i].IsEmpty() && !loadableModule.GetNoSaving()){
					showAllDefaultExtension[i] = loadableModule.GetDefaultExtension();
				}
			}
		}
	}
	
	for(i=0; i<FPL_MAPPING_COUNT; i++){
		pAddPattern(pFPLOpen[vFPLMappings[i].list], "All formats",
			showAllFormat[i].GetString(), showAllDefaultExtension[i].GetString());
	}
	
	// fill the file pattern lists. these are added to both load and save lists
	for(i=0; i<FPL_MAPPING_COUNT; i++){
		for(j=0; j<moduleCount; j++){
			const deLoadableModule &loadableModule = *modSys.GetModuleAt(j);
			
			if(loadableModule.GetType() == vFPLMappings[i].moduleType){
				pAddPattern(pFPLOpen[vFPLMappings[i].list], loadableModule);
				
				if(!loadableModule.GetNoSaving()){
					pAddPattern(pFPLSave[vFPLMappings[i].list], loadableModule);
				}
			}
		}
	}
	
	// some lists are special for the time being
	pAddPattern(pFPLOpen[efplAnimator], "All formats", "*.deanimator", ".deanimator");
	pAddPattern(pFPLOpen[efplAnimator], "Animator", "*.deanimator", ".deanimator");
	
	pAddPattern(pFPLSave[efplAnimator], "Animator", "*.deanimator", ".deanimator");
	
	pAddPattern(pFPLOpen[efplNavigationSpace], "All formats", "*.denavspace", ".denavspace");
	pAddPattern(pFPLOpen[efplNavigationSpace], "Navigation Space", "*.denavspace", ".denavspace");
	
	pAddPattern(pFPLSave[efplNavigationSpace], "Navigation Space", "*.denavspace", ".denavspace");
	
	pAddPattern(pFPLOpen[efplParticleEmitter], "All formats", "*.depemit", ".depemit");
	pAddPattern(pFPLOpen[efplParticleEmitter], "Particle Emitter", "*.depemit", ".depemit");
	
	pAddPattern(pFPLSave[efplParticleEmitter], "Particle Emitter", "*.depemit", ".depemit");
	
	pAddPattern(pFPLOpen[efplSky], "All formats", "*.desky", ".desky");
	pAddPattern(pFPLOpen[efplSky], "Sky", "*.desky", ".desky");
	
	pAddPattern(pFPLSave[efplSky], "Sky", "*.desky", ".desky");
	
	pAddPattern(pFPLOpen[efplSynthesizer], "All formats", "*.desynth", ".desynth");
	pAddPattern(pFPLOpen[efplSynthesizer], "Synthesizer", "*.desynth", ".desynth");
	
	pAddPattern(pFPLSave[efplSynthesizer], "Synthesizer", "*.desynth", ".desynth");
	
	pAddPattern(pFPLOpen[efplSpeechAnimation], "All formats", "*.desanim", ".desanim");
	pAddPattern(pFPLOpen[efplSpeechAnimation], "Speech Animation", "*.desanim", ".desanim");
	
	pAddPattern(pFPLSave[efplSpeechAnimation], "Speech Animation", "*.desanim", ".desanim");
	
	pAddPattern(pFPLOpen[efplCamera], "All formats", "*.decamera", ".decamera");
	pAddPattern(pFPLOpen[efplCamera], "Camera", "*.decamera", ".decamera");
	
	pAddPattern(pFPLSave[efplCamera], "Camera", "*.decamera", ".decamera");
	
	pAddPattern(pFPLOpen[efplWorld], "All formats", "*.deworld", ".deworld");
	pAddPattern(pFPLOpen[efplWorld], "World", "*.deworld", ".deworld");
	
	pAddPattern(pFPLSave[efplWorld], "World", "*.deworld", ".deworld");
	
	// game project file list
	pAddPattern(pFPLOpen[efplGameProject], "All formats", "*.degp", "degp");
	pAddPattern(pFPLOpen[efplGameProject], pLSGameProject->GetName().GetString(),
		pLSGameProject->GetPattern().GetString(), pLSGameProject->GetDefaultExtension().GetString());
	
	pAddPattern(pFPLSave[efplGameProject], pLSGameProject->GetName().GetString(),
		pLSGameProject->GetPattern().GetString(), pLSGameProject->GetDefaultExtension().GetString());
	
	// convert file pattern lists to fox ones
	pConvertToFOX(pFPLOpen[efplGameProject], pFoxFPLOpen[effplGameProject]);
	pConvertToFOX(pFPLSave[efplGameProject], pFoxFPLSave[effplGameProject]);
}



const igdeFilePattern::List &igdeLoadSaveSystem::GetOpenFilePatternList(eFilePatternLists list) const{
	return pFPLOpen[list];
}

const igdeFilePattern::List &igdeLoadSaveSystem::GetSaveFilePatternList(eFilePatternLists list) const{
	return pFPLSave[list];
}

const decString &igdeLoadSaveSystem::GetFOXOpenFilePatternList(eFOXFilePatternLists list) const{
	return pFoxFPLOpen[list];
}

const decString &igdeLoadSaveSystem::GetFOXSaveFilePatternList(eFOXFilePatternLists list) const{
	return pFoxFPLSave[list];
}



// Private Functions
//////////////////////

void igdeLoadSaveSystem::pCleanUp(){
	if(pLSGameProject){
		delete pLSGameProject;
	}
}

void igdeLoadSaveSystem::pAddPattern(igdeFilePattern::List &fpl, const deLoadableModule &module){
	decString patterns;
	
	module.GetPatternList().Visit([&](const decString &p){
		if(!patterns.IsEmpty()){
			patterns.AppendCharacter(',');
		}
		patterns.AppendCharacter('*');
		patterns.Append(p);
	});
	
	fpl.Add(igdeFilePattern::Ref::New(module.GetName(), patterns, module.GetDefaultExtension()));
}

void igdeLoadSaveSystem::pAddPattern(igdeFilePattern::List &fpl, const char *name, const char *pattern, const char *defaultExtension){
	fpl.Add(igdeFilePattern::Ref::New(name, pattern, defaultExtension));
}

void igdeLoadSaveSystem::pConvertToFOX(const igdeFilePattern::List &fpl, decString &foxfpl){
	foxfpl.Empty();
	fpl.Visit([&](const igdeFilePattern &p){
		if(!foxfpl.IsEmpty()){
			foxfpl.AppendCharacter('\n');
		}
		foxfpl.AppendFormat("%s (%s)", p.GetName().GetString(), p.GetPattern().GetString());
	});
}
