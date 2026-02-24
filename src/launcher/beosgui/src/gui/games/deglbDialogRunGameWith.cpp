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
#include <MenuItem.h>

#include "deglbDialogRunGameWith.h"
#include "deglbDialogProfileList.h"
#include "../deglbWindowMain.h"
#include "../../deglbLauncher.h"
#include "../../config/deglbConfiguration.h"

#include <delauncher/game/delGameManager.h>
#include <delauncher/game/profile/delGameProfile.h>

#include <dragengine/common/exceptions.h>


// Class deglbDialogRunGameWith
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deglbDialogRunGameWith::deglbDialogRunGameWith(deglbWindowMain *windowMain, delGame *game,
	const BMessenger &resultTarget, int resultMessage) :
BWindow({}, "Select Profile", B_TITLED_WINDOW, B_NOT_ZOOMABLE | B_ASYNCHRONOUS_CONTROLS),
pWindowMain(windowMain),
pResultTarget(resultTarget),
pResultMessage(resultMessage),
pResultValue(false),
pGame(game),
pProfile(game->GetActiveProfile())
{
	SetFeel(B_MODAL_APP_WINDOW_FEEL);
	
	pPopupProfile = new BPopUpMenu("< Default Profile >");
	pMenuProfile = new BMenuField("profile", "Profile:", pPopupProfile);
	
	pLabProblem = new BStringView("problem", "");
	pLabProblem->SetHighColor({200, 0, 0, 255});
	
	BButton * const btnRun = new BButton("run", "Run Game",
		new BMessage(MSG_RUN));
	BButton * const btnCancel = new BButton("cancel", "Cancel",
		new BMessage(MSG_CANCEL));
	BButton * const btnEdit = new BButton("edit", "Edit Profiles...",
		new BMessage(MSG_EDIT_PROFILES));
	
	BLayoutBuilder::Group<>(this, B_VERTICAL, B_USE_DEFAULT_SPACING)
		.SetInsets(B_USE_DEFAULT_SPACING)
		.AddGroup(B_HORIZONTAL)
			.Add(pMenuProfile)
			.Add(btnEdit)
		.End()
		.Add(pLabProblem)
		.AddGlue()
		.AddGroup(B_HORIZONTAL)
			.AddGlue()
			.Add(btnRun)
			.Add(btnCancel)
		.End()
	.End();
	
	SetDefaultButton(btnRun);
	
	UpdateGame();
	
	ResizeToPreferred();
	
	CenterOnScreen();
}

deglbDialogRunGameWith::~deglbDialogRunGameWith() = default;



// Management
///////////////

void deglbDialogRunGameWith::SetProfile(delGameProfile *profile){
	pProfile = profile;
	UpdateGame();
}

void deglbDialogRunGameWith::UpdateGame(){
	const delGameManager &gameManager = pWindowMain->GetLauncher()->GetGameManager();
	
	while(pPopupProfile->CountItems() > 0){
		delete pPopupProfile->RemoveItem((int32)0);
	}
	
	BMessage *msg = new BMessage(MSG_PROFILE_CHANGED);
	msg->AddPointer("profile", nullptr);
	pPopupProfile->AddItem(new BMenuItem("< Default Profile >", msg));
	
	if(pGame->GetCustomProfile()){
		msg = new BMessage(MSG_PROFILE_CHANGED);
		msg->AddPointer("profile", pGame->GetCustomProfile());
		pPopupProfile->AddItem(new BMenuItem("< Custom Profile >", msg));
	}
	
	gameManager.GetProfiles().Visit([&](delGameProfile *profile){
		msg = new BMessage(MSG_PROFILE_CHANGED);
		msg->AddPointer("profile", profile);
		pPopupProfile->AddItem(new BMenuItem(profile->GetName(), msg));
	});
	
	// Select current profile
	delGameProfile *sel = pProfile;
	BMenuItem *found = nullptr;
	if(sel){
		int i;
		for(i=0; i<pPopupProfile->CountItems(); i++){
			BMenuItem * const item = pPopupProfile->ItemAt(i);
			BMessage * const imsg = item ? item->Message() : nullptr;
			if(!imsg){
				continue;
			}
			
			void *ptr = nullptr;
			if(imsg->FindPointer("profile", &ptr) == B_OK && sel == ptr){
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
	
	// Check validity
	delGameProfile *validateProfile = pProfile;
	if(!validateProfile){
		validateProfile = gameManager.GetActiveProfile();
		if(!validateProfile){
			validateProfile = gameManager.GetDefaultProfile();
		}
	}
	
	if(validateProfile && validateProfile->GetValid()){
		pLabProblem->SetText("");
		
	}else{
		pLabProblem->SetText("Profile has problems. The game will not run with it.");
	}
}



// BWindow
///////////

void deglbDialogRunGameWith::MessageReceived(BMessage *message){
	switch(message->what){
	case MSG_RUN:
		pResultValue = true;
		Quit();
		break;
		
	case MSG_CANCEL:
		Quit();
		break;
		
	case MSG_PROFILE_CHANGED:{
		void *ptr = nullptr;
		if(message->FindPointer("profile", &ptr) == B_OK){
			pProfile = reinterpret_cast<delGameProfile*>(ptr);
			UpdateGame();
		}
		}break;
		
	case MSG_EDIT_PROFILES:{
		delGameManager &gameManager = pWindowMain->GetLauncher()->GetGameManager();
		delGameProfile *profile = pProfile;
		if(!profile){
			profile = gameManager.GetActiveProfile();
			if(!profile){
				profile = gameManager.GetDefaultProfile();
			}
		}
		
		(new deglbDialogProfileList(pWindowMain, profile,
			BMessenger(this), MSG_EDIT_PROFILES_DONE))->Show();
		}break;
		
	case MSG_EDIT_PROFILES_DONE:
		try{
			delGameManager &gameManager = pWindowMain->GetLauncher()->GetGameManager();
			pWindowMain->GetLauncher()->GetEngine().SaveConfig();
			gameManager.GetProfiles().Visit([&](delGameProfile &p){
				p.Verify(*pWindowMain->GetLauncher());
			});
			gameManager.ApplyProfileChanges();
			gameManager.SaveGameConfigs();
			UpdateGame();
			
		}catch(const deException &e){
			pWindowMain->DisplayException(e);
		}
		break;
		
	default:
		BWindow::MessageReceived(message);
	}
}

bool deglbDialogRunGameWith::QuitRequested(){
	if(pResultTarget.IsValid()){
		BMessage reply(pResultMessage);
		reply.SetBool("result", pResultValue);
		reply.SetString("profile", pProfile->GetName());
		pResultTarget.SendMessage(&reply);
	}
	return true;
}
