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
#include <Alert.h>
#include <Roster.h>
#include <Entry.h>
#include <Path.h>
#include <MessageFilter.h>

#include "deglbPanelGames.h"
#include "deglbWindowMain.h"
#include "games/deglbDialogGameProperties.h"
#include "games/deglbDialogRunGameWith.h"
#include "../deglbLauncher.h"
#include "../game/deglbGameIcon.h"

#include <delauncher/game/delGameRunParams.h>
#include <delauncher/game/profile/delGameProfile.h>
#include <delauncher/game/icon/delGameIconList.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>


// Class deglbPanelGames::cGameListItem
/////////////////////////////////////////

deglbPanelGames::cGameListItem::cGameListItem(delGame *agame) :
game(agame),
icon(nullptr)
{
	agame->GetIcons().VisitWhile([&](const delGameIcon::Ref &i){
		auto glbIcon = i.DynamicCast<deglbGameIcon>();
		if(glbIcon && glbIcon->GetBitmap()){
			icon = glbIcon->CreateScaledBitmap(32);
			return false;
		}
		return true;
	});
	
	if(icon){
		SetField(new BBitmapField(icon), 0);
	}
	SetField(new BStringField(agame->GetTitle().ToUTF8()), 1);
	
	delGameProfile * const profile = agame->GetProfileToUse();
	const char *status;
	if(agame->GetCanRun() && profile->GetValid()){
		status = "Ready";
		
	}else if(!profile->GetValid()){
		status = "Profile Broken";
		
	}else if(!agame->GetAllFormatsSupported()){
		status = "Missing Modules";
		
	}else{
		status = "Installation Broken";
	}
	SetField(new BStringField(status), 2);
	
	SetField(new BStringField(agame->GetCreator().ToUTF8()), 3);
}

deglbPanelGames::cGameListItem::~cGameListItem(){
	if(icon){
		delete icon;
	}
}


// Class deglbPanelGames
/////////////////////////

// Constructor, destructor
////////////////////////////

namespace{

class cMessageFilterListGames : public BMessageFilter{
	deglbPanelGames &pPanel;
	BColumnListView &pListView;
	
public:
	cMessageFilterListGames(deglbPanelGames &panel, BColumnListView &listView) :
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
			auto item = dynamic_cast<deglbPanelGames::cGameListItem*>(pListView.RowAt(where));
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

deglbPanelGames::deglbPanelGames(deglbWindowMain *windowMain) :
BView("panelGames", B_ASYNCHRONOUS_CONTROLS),
pWindowMain(windowMain)
{
	pListGames = new BColumnListView("gamesList", 0, B_NO_BORDER, false);
	pListGames->ScrollView()->AddFilter(new cMessageFilterListGames(*this, *pListGames));
	
	const float factor = be_plain_font->StringWidth("M");
	pListGames->AddColumn(new BBitmapColumn(" ", 32, 32, 32, B_ALIGN_CENTER), 0);
	pListGames->AddColumn(new BStringColumn("Game", factor * 25, 10, 10000, B_TRUNCATE_END), 1);
	pListGames->AddColumn(new BStringColumn("Status", factor * 12, 10, 10000, B_TRUNCATE_END), 2);
	pListGames->AddColumn(new BStringColumn("Creator", factor * 15, 10, 10000, B_TRUNCATE_END), 3);
	
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(pListGames)
	.End();
}

deglbPanelGames::~deglbPanelGames(){
}



// Management
///////////////

delGame *deglbPanelGames::GetSelectedGame() const{
	auto item = dynamic_cast<cGameListItem*>(pListGames->CurrentSelection());
	return item ? item->game.Pointer() : nullptr;
}

void deglbPanelGames::SetSelectedGame(delGame *game){
	const int count = pListGames->CountRows();
	int i;
	
	pListGames->DeselectAll();
	
	for(i=0; i<count; i++){
		auto item = dynamic_cast<cGameListItem*>(pListGames->RowAt(i));
		if(item && item->game == game){
			pListGames->AddToSelection(item);
			pListGames->ScrollTo(item);
			return;
		}
	}
	
	if(count > 0){
		pListGames->AddToSelection(pListGames->RowAt(0));
		pListGames->ScrollTo(pListGames->RowAt(0));
	}
}

void deglbPanelGames::UpdateGameList(){
	delGame * const selectedGame = GetSelectedGame();
	
	pListGames->Clear();
	
	pWindowMain->GetLauncher()->GetGameManager().GetGames().Visit([&](delGame *game){
		pListGames->AddRow(new cGameListItem(game));
	});
	
	SetSelectedGame(selectedGame);
}

void deglbPanelGames::PrepareShutDown(){
	pListGames->Clear();
}



// BView
/////////

void deglbPanelGames::MessageReceived(BMessage *message){
	switch(message->what){
	case MSG_LIST_CHANGED:
		// Selection changed - nothing to do
		break;
		
	case MSG_CONTEXT_RUN:{
		delGame * const game = GetSelectedGame();
		if(game){
			pRunGame(game);
		}
		break;
	}
		
	case MSG_CONTEXT_PROPS:{
		delGame * const game = GetSelectedGame();
		if(!game){
			break;
		}
		
		auto dialog = new deglbDialogGameProperties(pWindowMain, game,
			BMessenger(this), MSG_CONTEXT_PROPS_DONE);
		//dialog->AddToSubset(pWindowMain);
		dialog->Show();
		}break;
		
	case MSG_CONTEXT_PROPS_DONE:{
		delGame * const game = GetSelectedGame();
		if(!game){
			break;
		}
		
		bool result = false;
		message->FindBool("result", &result);
		
		try{
			if(result){
				game->SaveConfig();
			}
			pWindowMain->GetLauncher()->GetGameManager().Verify();
			UpdateGameList();
			
		}catch(const deException &e){
			pWindowMain->DisplayException(e);
		}
		}break;
		
	case MSG_CONTEXT_RUNWITH:{
		delGame * const game = GetSelectedGame();
		if(game){
			pRunGameWith(game);
		}
		}break;
		
	case MSG_CONTEXT_RUNWITH_DONE:{
		delGame * const game = GetSelectedGame();
		if(game){
			bool result = false;
			message->FindBool("result", &result);
			
			delGameProfile::Ref profile;
			BString profileName;
			if(result && message->FindString("profile", &profileName) == B_OK){
				profile = pWindowMain->GetLauncher()->GetGameManager().GetProfiles().FindNamed(profileName);
			}
			
			pRunGameWithDone(game, result, profile);
		}
		}break;
		
	case MSG_CONTEXT_KILL:{
		delGame * const game = GetSelectedGame();
		if(!game){
			break;
		}
		if(!game->IsRunning()){
			(new BAlert(
				"Kill Game",
				"This game is not running.",
				"OK"))->Go(nullptr);
			
		}else{
			try{
				game->KillGame();
			}catch(const deException &e){
				pWindowMain->DisplayException(e);
			}
		}
		break;
	}
		
	case MSG_CONTEXT_SHOWLOGS:{
		delGame * const game = GetSelectedGame();
		if(game){
			pShowLogs(game);
		}
		}break;
		
	case MSG_UPDATE_GAMELIST:
		UpdateGameList();
		break;
		
	default:
		BView::MessageReceived(message);
	}
}

void deglbPanelGames::OnListContextMenu(cGameListItem *item, const BPoint &where){
	if(item){
		pListGames->DeselectAll();
		pListGames->AddToSelection(item);
		pListGames->ScrollTo(item);
	}
	
	delGame * const game = GetSelectedGame();
	if(game){
		pShowContextMenu(where, game);
	}
}

void deglbPanelGames::OnListInvoke(cGameListItem *item){
	pRunGame(item->game);
}



// Private Functions
//////////////////////

void deglbPanelGames::pShowContextMenu(BPoint screenWhere, delGame *game){
	BPopUpMenu *menu = new BPopUpMenu("context", false, false);
	
	BMenuItem *item = new BMenuItem("Properties...", new BMessage(MSG_CONTEXT_PROPS));
	item->SetTarget(this);
	menu->AddItem(item);
	
	menu->AddSeparatorItem();
	
	item = new BMenuItem("Run Game", new BMessage(MSG_CONTEXT_RUN));
	item->SetTarget(this);
	if(game->IsRunning()){
		item->SetEnabled(false);
	}
	menu->AddItem(item);
	
	item = new BMenuItem("Run Game With Profile...", new BMessage(MSG_CONTEXT_RUNWITH));
	item->SetTarget(this);
	if(game->IsRunning()){
		item->SetEnabled(false);
	}
	menu->AddItem(item);
	
	item = new BMenuItem("Kill Game", new BMessage(MSG_CONTEXT_KILL));
	item->SetTarget(this);
	if(!game->IsRunning()){
		item->SetEnabled(false);
	}
	menu->AddItem(item);
	
	item = new BMenuItem("Show Logs", new BMessage(MSG_CONTEXT_SHOWLOGS));
	item->SetTarget(this);
	menu->AddItem(item);
	
	menu->Go(screenWhere, true, false, true);
	delete menu;
}

void deglbPanelGames::pRunGame(delGame *game){
	if(game->IsRunning()){
		(new BAlert(
			"Start Game",
			"This game is already running.",
			"OK"))->Go(nullptr);
		return;
	}
	
	while(true){
		delGameProfile * const profile = game->GetProfileToUse();
		
		if(game->GetCanRun() && profile->GetValid()){
			delGameRunParams runParams;
			runParams.SetGameProfile(profile);
			
			decString error;
			if(!runParams.FindPatches(*game, game->GetUseLatestPatch(),
			game->GetUseCustomPatch(), error)){
				(new BAlert(
					"Can not run game",
					error,
					"OK"))->Go(nullptr);
				return;
			}
			
			if(profile->GetReplaceRunArguments()){
				runParams.SetRunArguments(profile->GetRunArguments());
				
			}else{
				runParams.SetRunArguments(game->GetRunArguments() + " " + profile->GetRunArguments());
			}
			
			runParams.SetWidth(profile->GetWidth());
			runParams.SetHeight(profile->GetHeight());
			runParams.SetFullScreen(profile->GetFullScreen());
			
			const auto windowSize = game->GetDisplayScaledWindowSize();
			if(windowSize != decPoint()){
				runParams.SetWidth(windowSize.x);
				runParams.SetHeight(windowSize.y);
				runParams.SetFullScreen(false);
			}
			
			game->StartGame(runParams);
			break;
			
		}else if(!profile->GetValid()){
			(new BAlert(
				"Can not run game",
				"The Game Profile is not working. Please fix it in Game Properties.",
				"OK"))->Go(nullptr);
			break;
			
		}else if(!game->GetAllFormatsSupported()){
			(new BAlert(
				"Can not run game",
				"One or more File Formats required by the game are not working.",
				"OK"))->Go(nullptr);
			break;
			
		}else{
			(new BAlert(
				"Can not run game",
				"Game related properties are incorrect.",
				"OK"))->Go(nullptr);
			break;
		}
	}
	
	UpdateGameList();
}

void deglbPanelGames::pRunGameWith(delGame *game){
	if(game->IsRunning()){
		(new BAlert(
			"Start Game",
			"This game is already running.",
			"OK"))->Go(nullptr);
		return;
	}
	
	if(!game->GetCanRun()){
		const char *msg = game->GetAllFormatsSupported()
			? "Game related properties are incorrect."
			: "One or more File Formats required by the game are not working.";
		(new BAlert(
			"Can not run game",
			msg,
			"OK"))->Go(nullptr);
		return;
	}
	
	deglbDialogRunGameWith *dlg = new deglbDialogRunGameWith(
		pWindowMain, game, BMessenger(this), MSG_CONTEXT_RUNWITH_DONE);
	dlg->SetProfile(game->GetActiveProfile());
	dlg->Show();
}

void deglbPanelGames::pRunGameWithDone(delGame *game, bool result, const delGameProfile::Ref &profile){
	if(!result){
		UpdateGameList();
		return;
	}
	
	const delGameManager &gameManager = pWindowMain->GetLauncher()->GetGameManager();
	delGameRunParams runParams;
	runParams.SetGameProfile(profile);
	if(!runParams.GetGameProfile()){
		runParams.SetGameProfile(gameManager.GetActiveProfile());
		if(!runParams.GetGameProfile()){
			runParams.SetGameProfile(gameManager.GetDefaultProfile());
		}
	}
	
	decString error;
	if(!runParams.FindPatches(*game, game->GetUseLatestPatch(),
	game->GetUseCustomPatch(), error)){
		(new BAlert(
			"Can not run game",
			error,
			"OK"))->Go(new BInvoker(new BMessage(MSG_UPDATE_GAMELIST), BMessenger(this)));
		return;
	}
	
	if(runParams.GetGameProfile() && !runParams.GetGameProfile()->GetValid()){
		(new BAlert(
			"Can not run game",
			"The selected profile is not valid.",
			"OK"))->Go(new BInvoker(new BMessage(MSG_UPDATE_GAMELIST), BMessenger(this)));
		return;
	}
	
	if(runParams.GetGameProfile()){
		if(runParams.GetGameProfile()->GetReplaceRunArguments()){
			runParams.SetRunArguments(runParams.GetGameProfile()->GetRunArguments());
			
		}else{
			runParams.SetRunArguments(game->GetRunArguments() + " "
				+ runParams.GetGameProfile()->GetRunArguments());
		}
		runParams.SetWidth(runParams.GetGameProfile()->GetWidth());
		runParams.SetHeight(runParams.GetGameProfile()->GetHeight());
		runParams.SetFullScreen(runParams.GetGameProfile()->GetFullScreen());
	}
	
	const auto windowSize = game->GetDisplayScaledWindowSize();
	if(windowSize != decPoint()){
		runParams.SetWidth(windowSize.x);
		runParams.SetHeight(windowSize.y);
		runParams.SetFullScreen(false);
	}
	
	game->StartGame(runParams);
	UpdateGameList();
}

void deglbPanelGames::pShowLogs(delGame *game){
	decPath path;
	path.SetFromNative(pWindowMain->GetLauncher()->GetPathLogs());
	path.AddComponent("games");
	path.AddComponent(game->GetIdentifier().ToHexString(false));
	
	const deVFSDiskDirectory::Ref container(deVFSDiskDirectory::Ref::New(path));
	if(!container->ExistsFile(decPath::CreatePathUnix("/logs"))){
		(new BAlert("Show Logs",
			"There are no logs for this game.\nLogs will be present after running the game.",
			"OK"))->Go(nullptr);
		return;
	}
	
	path.AddComponent("logs");
	
	// On Haiku, open folder in Tracker using BRoster
	BEntry entry(path.GetPathNative());
	entry_ref ref;
	if(entry.GetRef(&ref) == B_OK){
		BMessage msg(B_REFS_RECEIVED);
		msg.AddRef("refs", &ref);
		be_roster->Launch("application/x-vnd.Be-TRAK", &msg);
	}
}
