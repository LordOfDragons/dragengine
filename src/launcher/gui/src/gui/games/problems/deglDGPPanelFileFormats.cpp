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

//FXDEFMAP( deglDGPPanelFileFormats ) deglDGPPanelFileFormatsMap[] = {};



// Class deglDGPPanelFileFormats
/////////////////////////////////

//FXIMPLEMENT( deglDGPPanelFileFormats, FXVerticalFrame, deglDGPPanelFileFormatsMap, ARRAYNUMBER( deglDGPPanelFileFormatsMap ) )
FXIMPLEMENT(deglDGPPanelFileFormats, FXVerticalFrame, nullptr, 0)

// Constructor, destructor
////////////////////////////

deglDGPPanelFileFormats::deglDGPPanelFileFormats(){}

deglDGPPanelFileFormats::deglDGPPanelFileFormats(deglDialogGameProblems *parentDialog, FXComposite *container) :
FXVerticalFrame(container, FRAME_RAISED | LAYOUT_FILL_Y | LAYOUT_FILL_X, 0, 0, 0, 0, 10, 10, 10, 10, 0, 0),
pParentDialog(parentDialog),
pStatusWorking(false)
{
	if(! parentDialog) DETHROW(deeInvalidParam);
	
	FXScrollWindow *scrollWindow;
	FXVerticalFrame *frameContent;
	
	// create content
	scrollWindow = new FXScrollWindow(this, LAYOUT_FILL_X | LAYOUT_FILL_Y
		| SCROLLERS_NORMAL | HSCROLLING_OFF | SCROLLERS_TRACK);
	frameContent = new FXVerticalFrame(scrollWindow, LAYOUT_FILL_Y | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 3);
	
	pListFormats = new FXIconList(frameContent, this, ID_LIST_FORMATS,
		ICONLIST_BROWSESELECT | ICONLIST_DETAILED | LAYOUT_FILL_X | LAYOUT_FILL_Y);
	pListFormats->appendHeader("Resource Type", nullptr, 110);
	pListFormats->appendHeader("File Format", nullptr, 130);
	pListFormats->appendHeader("Supported by", nullptr, 130);
	pListFormats->appendHeader("Problem", nullptr, 150);
	
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
	
	for(f=0; f<formatCount; f++){
		delFileFormat &format = *fileFormats.GetAt(f);
		const decString &formatPattern = format.GetPattern();
		formatType = format.GetType();
		
		// type string
		switch(formatType){
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
		text.append(formatPattern.GetString());
		text.append('\t');
		
		// add module supporting this file format
		matchingModule = nullptr;
		
		if(! deModuleSystem::IsSingleType(formatType)){
			for(m=0; m<moduleCount; m++){
				module = modules.GetAt(m);
				
				if(module->GetType() == formatType){
					if(formatPattern.MatchesPattern(module->GetPattern())){
						matchingModule = module;
						break;
					}
				}
			}
		}
		
		if(matchingModule){
			text.append(matchingModule->GetName().GetString());
			text.append('\t');
			
		}else{
			text.append("-\t");
		}
		
		// add status
		if(matchingModule){
			if(module->GetStatus() == delEngineModule::emsReady){
				text.append("-");
				
			}else{
				text.append("Module broken");
				pStatusWorking = false;
			}
			
		}else{
			text.append("Missing module");
			pStatusWorking = false;
		}
		
		pListFormats->appendItem(text, nullptr, nullptr, nullptr);
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

