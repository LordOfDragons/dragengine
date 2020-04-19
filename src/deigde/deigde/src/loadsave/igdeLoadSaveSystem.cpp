/* 
 * Drag[en]gine IGDE
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
#include <dragengine/common/file/decBaseFileReaderReference.h>
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

static sFPLMapping vFPLMappings[ FPL_MAPPING_COUNT ] = {
	{ igdeLoadSaveSystem::efplModel, deModuleSystem::emtModel },
	{ igdeLoadSaveSystem::efplSkin, deModuleSystem::emtSkin },
	{ igdeLoadSaveSystem::efplRig, deModuleSystem::emtRig },
	{ igdeLoadSaveSystem::efplAnimation, deModuleSystem::emtAnimation },
	{ igdeLoadSaveSystem::efplImage, deModuleSystem::emtImage },
	{ igdeLoadSaveSystem::efplSound, deModuleSystem::emtSound },
	{ igdeLoadSaveSystem::efplOcclusionMesh, deModuleSystem::emtOcclusionMesh },
	{ igdeLoadSaveSystem::efplVideo, deModuleSystem::emtVideo },
	{ igdeLoadSaveSystem::efplLanguagePack, deModuleSystem::emtLanguagePack },
	{ igdeLoadSaveSystem::efplFont, deModuleSystem::emtFont }
};



// Class igdeLoadSaveSystem
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeLoadSaveSystem::igdeLoadSaveSystem( igdeWindowMain *windowMain ){
	if( ! windowMain ){
		DETHROW( deeInvalidParam );
	}
	
	pWindowMain = windowMain;
	pLSGameProject = NULL;
	
	try{
		pLSGameProject = new igdeLSGameProject( this );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

igdeLoadSaveSystem::~igdeLoadSaveSystem(){
	pCleanUp();
}



// Management
///////////////

igdeGameProject *igdeLoadSaveSystem::LoadGameProject( const char *filename ){
	if( ! filename ){
		DETHROW( deeInvalidParam );
	}
	
	decBaseFileReaderReference fileReader;
	igdeGameProject *project = NULL;
	
	try{
		project = new igdeGameProject( pWindowMain->GetEnvironment() );
		project->SetFilePath( filename );
		
		fileReader.TakeOver( new decDiskFileReader( filename ) );
		pLSGameProject->Load( filename, project, fileReader );
		
	}catch( const deException & ){
		if( project ){
			project->FreeReference();
		}
		throw;
	}
	
	return project;
}

void igdeLoadSaveSystem::SaveGameProject( igdeGameProject *project, const char *filename ){
	if( ! project || ! filename ){
		DETHROW( deeInvalidParam );
	}
	
	decDiskFileWriter *fileWriter = NULL;
	
	try{
		fileWriter = new decDiskFileWriter( filename, false );
		pLSGameProject->Save( project, fileWriter );
		fileWriter->FreeReference();
		
	}catch( const deException & ){
		if( fileWriter ){
			fileWriter->FreeReference();
		}
		throw;
	}
}



void igdeLoadSaveSystem::UpdatePatternLists(){
	const deEngine &engine = *pWindowMain->GetEngine();
	const deModuleSystem &modSys = *engine.GetModuleSystem();
	const int moduleCount = modSys.GetModuleCount();
	int i, j, k;
	
	// reset the file pattern lists
	for( i=0; i<=efplSpeechAnimation; i++ ){
		pFPLOpen[ i ].RemoveAllFilePatterns();
		pFPLSave[ i ].RemoveAllFilePatterns();
	}
	
	// add show-all file patterns to the file pattern lists. these have to come first
	// in the load file pattern lists. save lists do not have them
	decString showAllFormat[ FPL_MAPPING_COUNT ];
	decString showAllDefaultExtension[ FPL_MAPPING_COUNT ];
	
	for( i=0; i<FPL_MAPPING_COUNT; i++ ){
		for( j=0; j<moduleCount; j++ ){
			const deLoadableModule &loadableModule = *modSys.GetModuleAt( j );
			
			if( loadableModule.GetType() == vFPLMappings[ i ].moduleType ){
				const decStringList &patternList = loadableModule.GetPatternList();
				const int patternCount = patternList.GetCount();
				
				for( k=0; k<patternCount; k++ ){
					if( ! showAllFormat[ i ].IsEmpty() ){
						showAllFormat[ i ].AppendCharacter( ',' );
					}
					showAllFormat[ i ].AppendCharacter( '*' );
					showAllFormat[ i ].Append( patternList.GetAt( k ) );
				}
				
				if( showAllDefaultExtension[ i ].IsEmpty() ){
					showAllDefaultExtension[ i ] = loadableModule.GetDefaultExtension();
				}
			}
		}
	}
	
	for( i=0; i<FPL_MAPPING_COUNT; i++ ){
		pAddPattern( pFPLOpen[ vFPLMappings[ i ].list ], "All formats",
			showAllFormat[ i ].GetString(), showAllDefaultExtension[ i ].GetString() );
	}
	
	// fill the file pattern lists. these are added to both load and save lists
	for( i=0; i<FPL_MAPPING_COUNT; i++ ){
		for( j=0; j<moduleCount; j++ ){
			const deLoadableModule &loadableModule = *modSys.GetModuleAt( j );
			
			if( loadableModule.GetType() == vFPLMappings[ i ].moduleType ){
				pAddPattern( pFPLOpen[ vFPLMappings[ i ].list ], loadableModule );
				pAddPattern( pFPLSave[ vFPLMappings[ i ].list ], loadableModule );
			}
		}
	}
	
	// some lists are special for the time being
	pAddPattern( pFPLOpen[ efplAnimator ], "All formats", "*.deanimator", ".deanimator" );
	pAddPattern( pFPLOpen[ efplAnimator ], "Animator", "*.deanimator", ".deanimator" );
	
	pAddPattern( pFPLSave[ efplAnimator ], "Animator", "*.deanimator", ".deanimator" );
	
	pAddPattern( pFPLOpen[ efplNavigationSpace ], "All formats", "*.denavspace", ".denavspace" );
	pAddPattern( pFPLOpen[ efplNavigationSpace ], "Navigation Space", "*.denavspace", ".denavspace" );
	
	pAddPattern( pFPLSave[ efplNavigationSpace ], "Navigation Space", "*.denavspace", ".denavspace" );
	
	pAddPattern( pFPLOpen[ efplParticleEmitter ], "All formats", "*.depemit", ".depemit" );
	pAddPattern( pFPLOpen[ efplParticleEmitter ], "Particle Emitter", "*.depemit", ".depemit" );
	
	pAddPattern( pFPLSave[ efplParticleEmitter ], "Particle Emitter", "*.depemit", ".depemit" );
	
	pAddPattern( pFPLOpen[ efplSky ], "All formats", "*.desky", ".desky" );
	pAddPattern( pFPLOpen[ efplSky ], "Sky", "*.desky", ".desky" );
	
	pAddPattern( pFPLSave[ efplSky ], "Sky", "*.desky", ".desky" );
	
	pAddPattern( pFPLOpen[ efplSynthesizer ], "All formats", "*.desynth", ".desynth" );
	pAddPattern( pFPLOpen[ efplSynthesizer ], "Synthesizer", "*.desynth", ".desynth" );
	
	pAddPattern( pFPLSave[ efplSynthesizer ], "Synthesizer", "*.desynth", ".desynth" );
	
	pAddPattern( pFPLOpen[ efplSpeechAnimation ], "All formats", "*.desanim", ".desanim" );
	pAddPattern( pFPLOpen[ efplSpeechAnimation ], "Speech Animation", "*.desanim", ".desanim" );
	
	pAddPattern( pFPLSave[ efplSpeechAnimation ], "Speech Animation", "*.desanim", ".desanim" );
	
	// game project file list
	pAddPattern( pFPLOpen[ efplGameProject ], "All formats", "*.degp", "degp" );
	pAddPattern( pFPLOpen[ efplGameProject ], pLSGameProject->GetName().GetString(),
		pLSGameProject->GetPattern().GetString(), pLSGameProject->GetDefaultExtension().GetString() );
	
	pAddPattern( pFPLSave[ efplGameProject ], pLSGameProject->GetName().GetString(),
		pLSGameProject->GetPattern().GetString(), pLSGameProject->GetDefaultExtension().GetString() );
	
	// convert file pattern lists to fox ones
	pConvertToFOX( pFPLOpen[ efplGameProject ], pFoxFPLOpen[ effplGameProject ] );
	pConvertToFOX( pFPLSave[ efplGameProject ], pFoxFPLSave[ effplGameProject ] );
}



const igdeFilePatternList &igdeLoadSaveSystem::GetOpenFilePatternList( eFilePatternLists list ) const{
	return pFPLOpen[ list ];
}

const igdeFilePatternList &igdeLoadSaveSystem::GetSaveFilePatternList( eFilePatternLists list ) const{
	return pFPLSave[ list ];
}

const decString &igdeLoadSaveSystem::GetFOXOpenFilePatternList( eFOXFilePatternLists list ) const{
	return pFoxFPLOpen[ list ];
}

const decString &igdeLoadSaveSystem::GetFOXSaveFilePatternList( eFOXFilePatternLists list ) const{
	return pFoxFPLSave[ list ];
}



// Private Functions
//////////////////////

void igdeLoadSaveSystem::pCleanUp(){
	if( pLSGameProject ){
		delete pLSGameProject;
	}
}

void igdeLoadSaveSystem::pAddPattern( igdeFilePatternList &fpl, const deLoadableModule &module ){
	const decStringList &patternList = module.GetPatternList();
	const int patternCount = patternList.GetCount();
	igdeFilePattern *filePattern = NULL;
	decString patterns;
	int i;
	
	for( i=0; i<patternCount; i++ ){
		if( i > 0 ){
			patterns.AppendCharacter( ',' );
		}
		patterns.AppendCharacter( '*' );
		patterns.Append( patternList.GetAt( i ) );
	}
	
	try{
		filePattern = new igdeFilePattern( module.GetName(), patterns.GetString(), module.GetDefaultExtension().GetString() );
		fpl.AddFilePattern( filePattern );
		
	}catch( const deException & ){
		if( filePattern ){
			delete filePattern;
		}
		throw;
	}
}

void igdeLoadSaveSystem::pAddPattern( igdeFilePatternList &fpl, const char *name, const char *pattern, const char *defaultExtension ){
	igdeFilePattern *filePattern = NULL;
	
	try{
		filePattern = new igdeFilePattern( name, pattern, defaultExtension );
		fpl.AddFilePattern( filePattern );
		
	}catch( const deException & ){
		if( filePattern ){
			delete filePattern;
		}
		throw;
	}
}

void igdeLoadSaveSystem::pConvertToFOX( const igdeFilePatternList &fpl, decString &foxfpl ){
	const int count = fpl.GetFilePatternCount();
	int i;
	
	foxfpl.Empty();
	
	for( i=0; i<count; i++ ){
		const igdeFilePattern &pattern = *fpl.GetFilePatternAt( i );
		
		if( i > 0 ){
			foxfpl.AppendCharacter( '\n' );
		}
		
		foxfpl.AppendFormat( "%s (%s)", pattern.GetName().GetString(), pattern.GetPattern().GetString() );
	}
}
