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

#include <delauncher/engine/modules/delEngineModule.h>
#include <delauncher/engine/modules/delEngineModuleList.h>
#include <delauncher/game/fileformat/delFileFormat.h>

#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/common/exceptions.h>



// Events
///////////

FXDEFMAP( deglDGPPanelFileFormats ) deglDGPPanelFileFormatsMap[] = {};



// Class deglDGPPanelFileFormats
/////////////////////////////////

FXIMPLEMENT( deglDGPPanelFileFormats, FXVerticalFrame, deglDGPPanelFileFormatsMap, ARRAYNUMBER( deglDGPPanelFileFormatsMap ) )

// Constructor, destructor
////////////////////////////

deglDGPPanelFileFormats::deglDGPPanelFileFormats(){ }

deglDGPPanelFileFormats::deglDGPPanelFileFormats( deglDialogGameProblems *parentDialog, FXComposite *container ) :
FXVerticalFrame( container, FRAME_RAISED | LAYOUT_FILL_Y | LAYOUT_FILL_X, 0, 0, 0, 0, 10, 10, 10, 10, 0, 0 ),
pParentDialog( parentDialog ),
pStatusWorking( false )
{
	if( ! parentDialog ) DETHROW( deeInvalidParam );
	
	FXScrollWindow *scrollWindow;
	FXVerticalFrame *frameContent;
	
	// create content
	scrollWindow = new FXScrollWindow( this, LAYOUT_FILL_X | LAYOUT_FILL_Y
		| SCROLLERS_NORMAL | HSCROLLING_OFF | SCROLLERS_TRACK );
	frameContent = new FXVerticalFrame( scrollWindow, LAYOUT_FILL_Y | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 3 );
	
	pListFormats = new FXIconList( frameContent, this, ID_LIST_FORMATS,
		ICONLIST_BROWSESELECT | ICONLIST_DETAILED | LAYOUT_FILL_X | LAYOUT_FILL_Y );
	pListFormats->appendHeader( "Resource Type", nullptr, 110 );
	pListFormats->appendHeader( "File Format", nullptr, 130 );
	pListFormats->appendHeader( "Supported by", nullptr, 130 );
	pListFormats->appendHeader( "Problem", nullptr, 150 );
	
	RebuildFormatList();
}

deglDGPPanelFileFormats::~deglDGPPanelFileFormats(){
}



// Management
///////////////

void deglDGPPanelFileFormats::RebuildFormatList(){
	delGame &game = *pParentDialog->GetGame();
	const delEngine &engine = pParentDialog->GetWindowMain()->GetLauncher()->GetEngine();
	const delEngineModuleList &modules = engine.GetModules();
	delFileFormatList &fileFormats = game.GetFileFormats();
	int f, formatCount = fileFormats.GetCount();
	int m, moduleCount = modules.GetCount();
	delEngineModule *matchingModule;
	delEngineModule *module = nullptr;
	deModuleSystem::eModuleTypes formatType;
	FXString text;
	
	pListFormats->clearItems();
	
	for( f=0; f<formatCount; f++ ){
		delFileFormat &format = *fileFormats.GetAt( f );
		const decString &formatPattern = format.GetPattern();
		formatType = format.GetType();
		
		// type string
		switch( formatType ){
		case deModuleSystem::emtAnimation:
			text = "Animation\t";
			break;
			
		case deModuleSystem::emtFont:
			text = "Font\t";
			break;
			
		case deModuleSystem::emtImage:
			text = "Image\t";
			break;
			
		case deModuleSystem::emtLanguagePack:
			text = "Language Pack\t";
			break;
			
		case deModuleSystem::emtModel:
			text = "Model\t";
			break;
			
		case deModuleSystem::emtRig:
			text = "Rig\t";
			break;
			
		case deModuleSystem::emtSkin:
			text = "Skin\t";
			break;
			
		case deModuleSystem::emtSound:
			text = "Sound\t";
			break;
			
		case deModuleSystem::emtVideo:
			text = "Video\t";
			break;
			
		case deModuleSystem::emtOcclusionMesh:
			text = "Occlusion Mesh\t";
			break;
			
		default:
			text = "???\t";
		}
		
		// add file format
		text.append( formatPattern.GetString() );
		text.append( '\t' );
		
		// add module supporting this file format
		matchingModule = nullptr;
		
		if( ! deModuleSystem::IsSingleType( formatType ) ){
			for( m=0; m<moduleCount; m++ ){
				module = modules.GetAt( m );
				
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
			if( module->GetStatus() == delEngineModule::emsReady ){
				text.append( "-" );
				
			}else{
				text.append( "Module broken" );
				pStatusWorking = false;
			}
			
		}else{
			text.append( "Missing module" );
			pStatusWorking = false;
		}
		
		pListFormats->appendItem( text, nullptr, nullptr, nullptr );
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

