/* 
 * Drag[en]gine GUI Launcher
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
#include <string.h>
#include <stdint.h>

#include "deglDialogGameProblems.h"
#include "deglDGPPanelFileFormats.h"
#include "../../deglWindowMain.h"
#include "../../deglGuiBuilder.h"
#include "../../../deglLauncher.h"
#include "../../../engine/deglEngine.h"
#include "../../../engine/modules/deglEngineModule.h"
#include "../../../game/deglGame.h"
#include "../../../game/fileformat/deglFileFormat.h"
#include "../../../game/profile/deglGameProfile.h"

#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/common/exceptions.h>



// Events
///////////

FXDEFMAP( deglDGPPanelFileFormats ) deglDGPPanelFileFormatsMap[]={
};



// Class deglDGPPanelFileFormats
/////////////////////////////////

FXIMPLEMENT( deglDGPPanelFileFormats, FXVerticalFrame, deglDGPPanelFileFormatsMap, ARRAYNUMBER( deglDGPPanelFileFormatsMap ) )

// Constructor, destructor
////////////////////////////

deglDGPPanelFileFormats::deglDGPPanelFileFormats(){ }

deglDGPPanelFileFormats::deglDGPPanelFileFormats( deglDialogGameProblems *parentDialog, FXComposite *container ) :
FXVerticalFrame( container, FRAME_RAISED | LAYOUT_FILL_Y | LAYOUT_FILL_X, 0, 0, 0, 0, 10, 10, 10, 10, 0, 0 ){
	if( ! parentDialog ) DETHROW( deeInvalidParam );
	
	FXScrollWindow *scrollWindow;
	FXVerticalFrame *frameContent;
	
	pParentDialog = parentDialog;
	pStatusWorking = false;
	
	// create content
	scrollWindow = new FXScrollWindow( this, LAYOUT_FILL_X | LAYOUT_FILL_Y | SCROLLERS_NORMAL | HSCROLLING_OFF | SCROLLERS_TRACK );
	if( ! scrollWindow ) DETHROW( deeOutOfMemory );
	
	frameContent = new FXVerticalFrame( scrollWindow, LAYOUT_FILL_Y | LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 );
	if( ! frameContent ) DETHROW( deeOutOfMemory );
	
	pListFormats = new FXIconList( frameContent, this, ID_LIST_FORMATS, ICONLIST_BROWSESELECT | ICONLIST_DETAILED |
		LAYOUT_FILL_X | LAYOUT_FILL_Y );
	if( ! pListFormats ) DETHROW( deeOutOfMemory );
	pListFormats->appendHeader( "Resource Type", NULL, 110 );
	pListFormats->appendHeader( "File Format", NULL, 130 );
	pListFormats->appendHeader( "Supported by", NULL, 130 );
	pListFormats->appendHeader( "Problem", NULL, 150 );
	
	RebuildFormatList();
}

deglDGPPanelFileFormats::~deglDGPPanelFileFormats(){
}



// Management
///////////////

void deglDGPPanelFileFormats::RebuildFormatList(){
	deglGame &game = *pParentDialog->GetGame();
	const deglEngine &engine = *pParentDialog->GetWindowMain()->GetLauncher()->GetEngine();
	const deglEngineModuleList &moduleList = engine.GetModuleList();
	deglFileFormatList &fileFormatList = game.GetFileFormatList();
	int f, formatCount = fileFormatList.GetFormatCount();
	int m, moduleCount = moduleList.GetModuleCount();
	deglEngineModule *matchingModule;
	deglEngineModule *module = NULL;
	deModuleSystem::eModuleTypes formatType;
	FXString text;
	
	pListFormats->clearItems();
	
	for( f=0; f<formatCount; f++ ){
		deglFileFormat &format = *fileFormatList.GetFormatAt( f );
		const decString &formatPattern = format.GetPattern();
		formatType = format.GetType();
		
		// type string
		if( formatType == deModuleSystem::emtAnimation ){
			text = "Animation\t";
			
		}else if( formatType == deModuleSystem::emtFont ){
			text = "Font\t";
			
		}else if( formatType == deModuleSystem::emtImage ){
			text = "Image\t";
			
		}else if( formatType == deModuleSystem::emtLanguagePack ){
			text = "Language Pack\t";
			
		}else if( formatType == deModuleSystem::emtModel ){
			text = "Model\t";
			
		}else if( formatType == deModuleSystem::emtRig ){
			text = "Rig\t";
			
		}else if( formatType == deModuleSystem::emtSkin ){
			text = "Skin\t";
			
		}else if( formatType == deModuleSystem::emtSound ){
			text = "Sound\t";
			
		}else if( formatType == deModuleSystem::emtVideo ){
			text = "Video\t";
			
		}else if( formatType == deModuleSystem::emtOcclusionMesh ){
			text = "Occlusion Mesh\t";
			
		}else{
			text = "???\t";
		}
		
		// add file format
		text.append( formatPattern.GetString() );
		text.append( '\t' );
		
		// add module supporting this file format
		matchingModule = NULL;
		
		if( ! deModuleSystem::IsSingleType( formatType ) ){
			for( m=0; m<moduleCount; m++ ){
				module = moduleList.GetModuleAt( m );
				
				if( module->GetType() == formatType ){
					if( formatPattern.MatchesPattern( module->GetPattern() ) ){
						matchingModule = module;
						break;
					}
				}
			}
		}
		
		if( matchingModule ){
			text.append( matchingModule->GetName().GetString() );
			text.append( '\t' );
			
		}else{
			text.append( "-\t" );
		}
		
		// add status
		if( matchingModule ){
			if( module->GetStatus() == deglEngineModule::emsReady ){
				text.append( "-" );
				
			}else{
				text.append( "Module broken" );
				pStatusWorking = false;
			}
			
		}else{
			text.append( "Missing module" );
			pStatusWorking = false;
		}
		
		pListFormats->appendItem( text, NULL, NULL, NULL );
	}
}

void deglDGPPanelFileFormats::UpdatePanel(){
	pStatusWorking = true;
	
	RebuildFormatList();
}



// Events
///////////




// Private Functions
//////////////////////

