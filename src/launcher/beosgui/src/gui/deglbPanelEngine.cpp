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

#include "deglbPanelEngine.h"
#include "deglbWindowMain.h"
#include "engine/deglbDialogModuleProps.h"
#include "../deglbLauncher.h"

#include <delauncher/engine/delEngine.h>
#include <delauncher/engine/modules/delEngineModuleList.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/systems/deModuleSystem.h>


static const char *pModuleTypeName(int type){
	switch(type){
	case deModuleSystem::emtGraphic:
		return "Graphic";
		
	case deModuleSystem::emtAudio:
		return "Audio";
		
	case deModuleSystem::emtInput:
		return "Input";
		
	case deModuleSystem::emtNetwork:
		return "Network";
		
	case deModuleSystem::emtPhysics:
		return "Physics";
		
	case deModuleSystem::emtAnimator:
		return "Animator";
		
	case deModuleSystem::emtAI:
		return "AI";
		
	case deModuleSystem::emtCrashRecovery:
		return "Crash Recovery";
		
	case deModuleSystem::emtSynthesizer:	
		return "Synthesizer";
		
	case deModuleSystem::emtVR:
		return "VR";
		
	default:
		return "Unknown";
	}
}


// Class deglbPanelEngine::cModuleListItem
//////////////////////////////////////////

deglbPanelEngine::cModuleListItem::cModuleListItem(delEngineModule *module) : pModule(module){
}

void deglbPanelEngine::cModuleListItem::DrawItem(BView *owner, BRect frame, bool complete){
	if(IsSelected() || complete){
		owner->SetHighColor(IsSelected()
			? ui_color(B_LIST_SELECTED_BACKGROUND_COLOR)
			: owner->ViewColor());
		owner->FillRect(frame);
	}
	
	const bool ready = pModule->GetStatus() == delEngineModule::emsReady;
	rgb_color textColor = IsSelected()
		? ui_color(B_LIST_SELECTED_ITEM_TEXT_COLOR)
		: (ready ?
			ui_color(B_LIST_ITEM_TEXT_COLOR)
			: rgb_color{200, 0, 0, 255});
	
	owner->SetHighColor(textColor);
	
	font_height fh;
	owner->GetFontHeight(&fh);
	const float textY = frame.top + fh.ascent + 2.0f;
	
	decString text;
	const char *statusStr = "Ready";
	if(pModule->GetStatus() == delEngineModule::emsNotTested){
		statusStr = "Not Tested";
		
	}else if(pModule->GetStatus() == delEngineModule::emsBroken){
		statusStr = "Broken";
	}
	
	text.Format("%s  [%s]  v%s  %s",
		pModule->GetName().GetString(),
		pModuleTypeName(pModule->GetType()),
		pModule->GetVersion().GetString(),
		statusStr);
	
	owner->DrawString(text, BPoint(frame.left + 4, textY));
}

void deglbPanelEngine::cModuleListItem::Update(BView *owner, const BFont *font){
	SetHeight(24.0f);
}


// Class deglbPanelEngine
/////////////////////////

// Constructor, destructor
////////////////////////////

deglbPanelEngine::deglbPanelEngine(deglbWindowMain *windowMain) :
BView("panelEngine", B_WILL_DRAW | B_FRAME_EVENTS),
pWindowMain(windowMain)
{
	pListModules = new BListView("modulesList");
	pListModules->SetSelectionMessage(new BMessage(MSG_LIST_CHANGED));
	
	BScrollView * const scrollView = new BScrollView("modulesScroll", pListModules,
		B_WILL_DRAW | B_FRAME_EVENTS, false, true);
	
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(scrollView)
	.End();
}

deglbPanelEngine::~deglbPanelEngine(){
}



// Management
///////////////

delEngineModule *deglbPanelEngine::GetSelectedModule() const{
	const int selection = pListModules->CurrentSelection();
	if(selection < 0){
		return nullptr;
	}
	auto item = dynamic_cast<cModuleListItem*>(pListModules->ItemAt(selection));
	return item ? item->pModule : nullptr;
}

void deglbPanelEngine::UpdateModuleList(){
	pListModules->MakeEmpty();
	
	pWindowMain->GetLauncher()->GetEngine().GetModules().Visit([&](delEngineModule *module){
		pListModules->AddItem(new cModuleListItem(module));
	});
}



// BView
/////////

void deglbPanelEngine::MessageReceived(BMessage *message){
	switch(message->what){
	case MSG_LIST_CHANGED:
		break;
		
	case MSG_CONTEXT_PROPS:{
		delEngineModule * const module = GetSelectedModule();
		if(module){
			try{
				deglbDialogModuleProps *dlg = new deglbDialogModuleProps(pWindowMain, module);
				Window()->Unlock();
				dlg->Go();
				Window()->Lock();
				
			}catch(const deException &e){
				pWindowMain->DisplayException(e);
			}
		}
		}break;
		
	default:
		BView::MessageReceived(message);
	}
}

void deglbPanelEngine::MouseDown(BPoint where){
	uint32 buttons;
	GetMouse(&where, &buttons);
	
	if(buttons & B_SECONDARY_MOUSE_BUTTON){
		const int index = pListModules->IndexOf(where);
		if(index >= 0){
			pListModules->Select(index);
		}
		delEngineModule * const module = GetSelectedModule();
		if(module){
			ConvertToScreen(&where);
			pShowContextMenu(where, module);
		}
		return;
	}
	
	BView::MouseDown(where);
}



// Private Functions
//////////////////////

void deglbPanelEngine::pShowContextMenu(BPoint screenWhere, delEngineModule *module){
	BPopUpMenu *menu = new BPopUpMenu("context", false, false);
	
	BMenuItem * const item = new BMenuItem("Properties...", new BMessage(MSG_CONTEXT_PROPS));
	item->SetTarget(this);
	menu->AddItem(item);
	
	menu->Go(screenWhere, true, false, true);
	delete menu;
}
