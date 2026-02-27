/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include <LayoutBuilder.h>
#include <PopUpMenu.h>
#include <MenuItem.h>
#include <MessageFilter.h>

#include "deglbPanelEngine.h"
#include "deglbWindowMain.h"
#include "engine/deglbDialogModuleProps.h"
#include "../deglbLauncher.h"

#include <delauncher/engine/delEngine.h>
#include <delauncher/engine/modules/delEngineModuleList.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/systems/deModuleSystem.h>


// Class deglbPanelEngine::cModuleListItem
//////////////////////////////////////////

deglbPanelEngine::cModuleListItem::cModuleListItem(delEngineModule *amodule) :
module(amodule)
{
	DEASSERT_NOTNULL(module)
	
	SetField(new BStringField(module->GetName()), 0);
	
	const char *typeString = "??";
	switch(amodule->GetType()){
	case deModuleSystem::emtGraphic:
		typeString = "Graphic";
		break;
		
	case deModuleSystem::emtAudio:
		typeString = "Audio";
		break;
		
	case deModuleSystem::emtInput:
		typeString = "Input";
		break;
		
	case deModuleSystem::emtNetwork:
		typeString = "Network";
		break;
		
	case deModuleSystem::emtPhysics:
		typeString = "Physics";
		break;
		
	case deModuleSystem::emtImage:
		typeString = "Image";
		break;
		
	case deModuleSystem::emtVideo:
		typeString = "Video";
		break;
		
	case deModuleSystem::emtScript:
		typeString = "Script";
		break;
		
	case deModuleSystem::emtModel:
		typeString = "Model";
		break;
		
	case deModuleSystem::emtRig:
		typeString = "Rig";
		break;
		
	case deModuleSystem::emtSkin:
		typeString = "Skin";
		break;
		
	case deModuleSystem::emtAnimation:
		typeString = "Animation";
		break;
		
	case deModuleSystem::emtFont:
		typeString = "Font";
		break;
		
	case deModuleSystem::emtCrashRecovery:
		typeString = "Crash Recovery";
		break;
		
	case deModuleSystem::emtLanguagePack:
		typeString = "Language Pack";
		break;
		
	case deModuleSystem::emtAnimator:
		typeString = "Animator";
		break;
		
	case deModuleSystem::emtSound:
		typeString = "Sound";
		break;
		
	case deModuleSystem::emtAI:
		typeString = "AI";
		break;
		
	case deModuleSystem::emtOcclusionMesh:
		typeString = "Occlusion Mesh";
		break;
		
	case deModuleSystem::emtSynthesizer:
		typeString = "Synthesizer";
		break;
		
	case deModuleSystem::emtArchive:
		typeString = "Archive";
		break;
		
	case deModuleSystem::emtService:
		typeString = "Service";
		break;
		
	case deModuleSystem::emtVR:
		typeString = "VR";
		break;
		
	case deModuleSystem::emtUnknown:
		typeString = "Unknown";
	}
	SetField(new BStringField(typeString), 1);
	
	SetField(new BStringField(module->GetVersion()), 2);
	
	const char *statusString = "??";
	switch(amodule->GetStatus()){
	case delEngineModule::emsReady:
		statusString = "Ready";
		break;
		
	case delEngineModule::emsNotTested:
		statusString = "Not Tested";
		break;
		
	case delEngineModule::emsBroken:
		statusString = "Broken";
		break;
	}
	SetField(new BStringField(statusString), 3);
}


// Class deglbPanelEngine
///////////////////////////

// Constructor, destructor
////////////////////////////

namespace{

class cMessageFilterListModules : public BMessageFilter{
	deglbPanelEngine &pPanel;
	BColumnListView &pListView;
	
public:
	cMessageFilterListModules(deglbPanelEngine &panel, BColumnListView &listView) :
	BMessageFilter(B_MOUSE_DOWN),
	pPanel(panel),
	pListView(listView){ 
	}
	
	filter_result Filter(BMessage *message, BHandler **target) override{
		switch(message->what){
		case B_MOUSE_DOWN:{
			int32 buttons = 0, clicks = 0;
			BPoint where;
			
			message->FindInt32("buttons", &buttons);
			message->FindInt32("clicks", &clicks);
			message->FindPoint("be:view_where", &where);
			
			auto view = dynamic_cast<BView*>(*target);
			auto item = dynamic_cast<deglbPanelEngine::cModuleListItem*>(pListView.RowAt(where));
			if(!item){
				break;
			}
			
			pListView.DeselectAll();
			pListView.AddToSelection(item);
			pListView.ScrollTo(item);
			
			if(buttons == B_SECONDARY_MOUSE_BUTTON){
				view->ConvertToScreen(&where);
				pPanel.OnListContextMenu(item, where);
				return B_SKIP_MESSAGE;
				
			}else if(buttons == B_PRIMARY_MOUSE_BUTTON && clicks == 2){
				pPanel.OnListInvoke(item);
				return B_SKIP_MESSAGE;
			}
			}break;
			
		default:
			break;
		};
		
		return B_DISPATCH_MESSAGE;
	}
};

}

deglbPanelEngine::deglbPanelEngine(deglbWindowMain *windowMain) :
BView("panelEngine", B_ASYNCHRONOUS_CONTROLS),
pWindowMain(windowMain)
{
	pListModules = new BColumnListView("modulesList", 0, B_NO_BORDER, false);
	pListModules->ScrollView()->AddFilter(new cMessageFilterListModules(*this, *pListModules));
	
	const float factor = be_plain_font->StringWidth("M");
	pListModules->AddColumn(new BStringColumn("Name", factor * 18, 10, 10000, B_TRUNCATE_END), 0);
	pListModules->AddColumn(new BStringColumn("Type", factor * 18, 10, 10000, B_TRUNCATE_END), 1);
	pListModules->AddColumn(new BStringColumn("Version", factor * 6, 10, 10000, B_TRUNCATE_END), 2);
	pListModules->AddColumn(new BStringColumn("Status", factor * 15, 10, 10000, B_TRUNCATE_END), 3);
	
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(pListModules)
	.End();
}

deglbPanelEngine::~deglbPanelEngine(){
}



// Management
///////////////

delEngineModule *deglbPanelEngine::GetSelectedModule() const{
	auto item = dynamic_cast<cModuleListItem*>(pListModules->CurrentSelection());
	return item ? item->module.Pointer() : nullptr;
}

void deglbPanelEngine::UpdateModuleList(){
	pListModules->Clear();
	
	pWindowMain->GetLauncher()->GetEngine().GetModules().Visit([&](delEngineModule *module){
		pListModules->AddRow(new cModuleListItem(module));
	});
}

void deglbPanelEngine::PrepareShutDown(){
	pListModules->Clear();
}


// BView
/////////

void deglbPanelEngine::MessageReceived(BMessage *message){
	switch(message->what){
	case MSG_CONTEXT_PROPS:{
		delEngineModule * const module = GetSelectedModule();
		if(module){
			(new deglbDialogModuleProps(pWindowMain, module, {}, 0))->Show();
		}
		}break;
		
	default:
		BView::MessageReceived(message);
	}
}

void deglbPanelEngine::OnListContextMenu(cModuleListItem *item, const BPoint &where){
	if(item){
		pListModules->DeselectAll();
		pListModules->AddToSelection(item);
		pListModules->ScrollTo(item);
	}
	if(GetSelectedModule()){
		pShowContextMenu(where);
	}
}

void deglbPanelEngine::OnListInvoke(cModuleListItem *item){
	(new deglbDialogModuleProps(pWindowMain, item->module, {}, 0))->Show();
}



// Private Functions
//////////////////////

void deglbPanelEngine::pShowContextMenu(BPoint screenWhere){
	BPopUpMenu *menu = new BPopUpMenu("context", false, false);
	
	BMenuItem * const item = new BMenuItem("Properties...", new BMessage(MSG_CONTEXT_PROPS));
	item->SetTarget(this);
	menu->AddItem(item);
	
	menu->Go(screenWhere, true, false, true);
}
