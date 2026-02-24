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
#include <GroupView.h>
#include <TabView.h>
#include <MenuItem.h>
#include <SeparatorView.h>
#include <StringView.h>

#include "deglbDialogEngineProps.h"
#include "../games/deglbDialogProfileList.h"
#include "../deglbWindowMain.h"
#include "../../deglbLauncher.h"

#include <delauncher/engine/delEngine.h>
#include <delauncher/game/delGameManager.h>
#include <delauncher/game/profile/delGameProfile.h>

#include <dragengine/common/string/decString.h>
#include <dragengine/common/exceptions.h>


// Class deglbDialogEngineProps
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deglbDialogEngineProps::deglbDialogEngineProps(deglbWindowMain *windowMain,
	const BMessenger &resultTarget, int resultMessage) :
BWindow({}, "Engine Properties", B_TITLED_WINDOW, B_NOT_ZOOMABLE),
pWindowMain(windowMain),
pResultTarget(resultTarget),
pResultMessage(resultMessage),
pResultValue(false)
{
	SetFeel(B_MODAL_APP_WINDOW_FEEL);
	
	// Build UI with BTabView
	BTabView * const tabView = new BTabView("tabs", B_WIDTH_FROM_WIDEST);
	
	// Information tab
	BView * const infoTab = new BView("Information", 0);
	
	pEditPathConfig = new BTextControl("pathConfig", "Path Config:", "", nullptr);
	pEditPathConfig->SetEnabled(false);
	
	pEditPathShare = new BTextControl("pathShare", "Path Share:", "", nullptr);
	pEditPathShare->SetEnabled(false);
	
	pEditPathLib = new BTextControl("pathLib", "Path Libraries:", "", nullptr);
	pEditPathLib->SetEnabled(false);
	
	BLayoutBuilder::Group<>(infoTab, B_VERTICAL, B_USE_DEFAULT_SPACING)
		.SetInsets(B_USE_DEFAULT_SPACING)
		.Add(pEditPathConfig)
		.Add(pEditPathShare)
		.Add(pEditPathLib)
		.AddGlue()
	.End();
	tabView->AddTab(infoTab);
	
	// Settings tab
	BView * const settingsTab = new BView("Settings", 0);
	
	pPopupProfile = new BPopUpMenu("< Default Profile >");
	pMenuProfile = new BMenuField("profile", "Active Profile:", pPopupProfile);
	
	BButton * const btnEditProfiles = new BButton("editProfiles", "Edit Profiles...",
		new BMessage(MSG_EDIT_PROFILES));
	
	BLayoutBuilder::Group<>(settingsTab, B_VERTICAL, B_USE_DEFAULT_SPACING)
		.SetInsets(B_USE_DEFAULT_SPACING)
		.Add(pMenuProfile)
		.Add(btnEditProfiles)
		.AddGlue()
	.End();
	tabView->AddTab(settingsTab);
	
	// Close button
	BButton * const btnClose = new BButton("close", "Close",
		new BMessage(MSG_OK));
	
	BLayoutBuilder::Group<>(this, B_VERTICAL, B_USE_DEFAULT_SPACING)
		.SetInsets(B_USE_DEFAULT_SPACING)
		.Add(tabView)
		.AddGroup(B_HORIZONTAL)
			.AddGlue()
			.Add(btnClose)
			.AddGlue()
		.End()
	.End();
	
	SetDefaultButton(btnClose);
	
	SetFromEngine();
	
	ResizeToPreferred();
	
	font_height fh;
	be_plain_font->GetHeight(&fh);
	ResizeTo(be_plain_font->StringWidth("M") * 50, Size().Height());
	
	CenterOnScreen();
}

deglbDialogEngineProps::~deglbDialogEngineProps(){
}



// Management
///////////////

void deglbDialogEngineProps::SetFromEngine(){
	const delEngine &engine = pWindowMain->GetLauncher()->GetEngine();
	
	pEditPathConfig->SetText(engine.GetPathConfig());
	pEditPathShare->SetText(engine.GetPathShare());
	pEditPathLib->SetText(engine.GetPathLib());
	
	UpdateProfileList();
}

void deglbDialogEngineProps::UpdateProfileList(){
	const delGameManager &gameManager = pWindowMain->GetLauncher()->GetGameManager();
	const delGameProfile::List &profiles = gameManager.GetProfiles();
	
	// Clear existing items
	while(pPopupProfile->CountItems() > 0){
		delete pPopupProfile->RemoveItem((int32)0);
	}
	
	// Add default item
	BMessage *msg = new BMessage(MSG_PROFILE_CHANGED);
	msg->AddPointer("profile", nullptr);
	pPopupProfile->AddItem(new BMenuItem("< Default Profile >", msg));
	
	// Add profiles
	profiles.Visit([&](delGameProfile *p){
		msg = new BMessage(MSG_PROFILE_CHANGED);
		msg->AddPointer("profile", p);
		pPopupProfile->AddItem(new BMenuItem(p->GetName(), msg));
	});
	
	// Select active profile
	delGameProfile * const active = gameManager.GetActiveProfile();
	BMenuItem *found = nullptr;
	if(active){
		int i;
		for(i=0; i<pPopupProfile->CountItems(); i++){
			BMenuItem * const item = pPopupProfile->ItemAt(i);
			if(!item){
				continue;
			}
			
			BMessage * const imsg = item->Message();
			if(!imsg){
				continue;
			}
			
			void *ptr = nullptr;
			if(imsg->FindPointer("profile", &ptr) == B_OK && ptr == active){
				found = item;
				break;
			}
		}
	}
	
	if(found){
		found->SetMarked(true);
		
	}else if(pPopupProfile->CountItems() > 0){
		pPopupProfile->ItemAt(0)->SetMarked(true);
	}
}



// BWindow
///////////

void deglbDialogEngineProps::MessageReceived(BMessage *message){
	switch(message->what){
	case MSG_OK:
		pResultValue = true;
		Quit();
		break;
		
	case MSG_PROFILE_CHANGED:{
		void *ptr = nullptr;
		if(message->FindPointer("profile", &ptr) == B_OK){
			delGameManager &gameManager = pWindowMain->GetLauncher()->GetGameManager();
			gameManager.SetActiveProfile(reinterpret_cast<delGameProfile*>(ptr));
			pWindowMain->GetLauncher()->GetEngine().SaveConfig();
		}
		break;
	}
		
	case MSG_EDIT_PROFILES:
		(new deglbDialogProfileList(
			pWindowMain, pWindowMain->GetLauncher()->GetGameManager().GetActiveProfile(),
			BMessenger(this), MSG_EDIT_PROFILES_DONE))->Show();
		break;
		
	case MSG_EDIT_PROFILES_DONE:
		try{
			delGameManager &gameManager = pWindowMain->GetLauncher()->GetGameManager();
			pWindowMain->GetLauncher()->GetEngine().SaveConfig();
			gameManager.GetProfiles().Visit([&](delGameProfile &p){
				p.Verify(*pWindowMain->GetLauncher());
			});
			gameManager.ApplyProfileChanges();
			gameManager.SaveGameConfigs();
			UpdateProfileList();
			
		}catch(const deException &e){
			pWindowMain->DisplayException(e);
		}
		break;
		
	default:
		BWindow::MessageReceived(message);
	}
}

bool deglbDialogEngineProps::QuitRequested(){
	if(pResultTarget.IsValid()){
		BMessage reply(pResultMessage);
		reply.SetBool("result", pResultValue);
		pResultTarget.SendMessage(&reply);
	}
	return true;
}
