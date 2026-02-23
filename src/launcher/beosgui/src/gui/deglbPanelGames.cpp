/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#include <LayoutBuilder.h>
#include <PopUpMenu.h>
#include <MenuItem.h>
#include <Alert.h>
#include <Roster.h>
#include <Entry.h>
#include <Path.h>

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
////////////////////////////////////////

deglbPanelGames::cGameListItem::cGameListItem(delGame *game) :
pGame(game),
pIcon(nullptr)
{
	// Try to get a game icon
	const delGameIconList &icons = game->GetIcons();
	const int iconCount = icons.GetCount();
	
	for(int i=0; i<iconCount; i++){
		deglbGameIcon * const icon = dynamic_cast<deglbGameIcon*>(icons.GetAt(i));
		if(icon && icon->GetBitmap()){
			pIcon = icon->CreateScaledBitmap(32);
			break;
		}
	}
}

deglbPanelGames::cGameListItem::~cGameListItem(){
	if(pIcon){
		delete pIcon;
	}
}

void deglbPanelGames::cGameListItem::DrawItem(BView *owner, BRect frame, bool complete){
	if(IsSelected() || complete){
		owner->SetHighColor(IsSelected() ? ui_color(B_LIST_SELECTED_BACKGROUND_COLOR)
			: owner->ViewColor());
		owner->FillRect(frame);
	}
	
	owner->SetHighColor(IsSelected() ? ui_color(B_LIST_SELECTED_ITEM_TEXT_COLOR)
		: ui_color(B_LIST_ITEM_TEXT_COLOR));
	
	const float iconSize = 32.0f;
	const float padding = 4.0f;
	
	// Draw icon if available
	if(pIcon){
		owner->DrawBitmap(pIcon, BPoint(frame.left + padding, frame.top + padding));
	}
	
	// Draw text
	font_height fh;
	owner->GetFontHeight(&fh);
	const float textX = frame.left + iconSize + padding * 2;
	const float textY = frame.top + iconSize * 0.35f + fh.ascent;
	
	const decString title(pGame->GetTitle().ToUTF8());
	owner->DrawString(title.GetString(), BPoint(textX, textY));
	
	// Status line
	const char *status = pGame->GetCanRun() ? "Ready" : "Not Ready";
	const float statusY = textY + fh.ascent + fh.descent + 2.0f;
	owner->DrawString(status, BPoint(textX, statusY));
}

void deglbPanelGames::cGameListItem::Update(BView *owner, const BFont *font){
	SetHeight(40.0f);
}


// Class deglbPanelGames
/////////////////////////

// Constructor, destructor
////////////////////////////

deglbPanelGames::deglbPanelGames(deglbWindowMain *windowMain) :
BView("panelGames", B_WILL_DRAW | B_FRAME_EVENTS),
pWindowMain(windowMain)
{
	pListGames = new BListView("gamesList");
	pListGames->SetSelectionMessage(new BMessage(MSG_LIST_CHANGED));
	pListGames->SetInvocationMessage(new BMessage(MSG_CONTEXT_RUN));
	
	BScrollView * const scrollView = new BScrollView("gamesScroll", pListGames,
		B_WILL_DRAW | B_FRAME_EVENTS, false, true);
	
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(scrollView)
	.End();
}

deglbPanelGames::~deglbPanelGames(){
}



// Management
///////////////

delGame *deglbPanelGames::GetSelectedGame() const{
	const int selection = pListGames->CurrentSelection();
	if(selection < 0){
		return nullptr;
	}
	
	const cGameListItem * const item = dynamic_cast<cGameListItem*>(pListGames->ItemAt(selection));
	return item ? item->pGame : nullptr;
}

void deglbPanelGames::SetSelectedGame(delGame *game){
	const int count = pListGames->CountItems();
	int i;
	
	for(i=0; i<count; i++){
		const cGameListItem * const item = dynamic_cast<cGameListItem*>(pListGames->ItemAt(i));
		if(item && item->pGame == game){
			pListGames->Select(i);
			return;
		}
	}
	
	if(count > 0){
		pListGames->Select(0);
	}
}

void deglbPanelGames::UpdateGameList(){
	delGame * const selectedGame = GetSelectedGame();
	
	pListGames->MakeEmpty();
	
	const delGame::List &gameList = pWindowMain->GetLauncher()->GetGameManager().GetGames();
	const int count = gameList.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		pListGames->AddItem(new cGameListItem(gameList.GetAt(i)));
	}
	
	SetSelectedGame(selectedGame);
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
		try{
			deglbDialogGameProperties *dlg = new deglbDialogGameProperties(pWindowMain, game);
			Window()->Unlock();
			const bool result = dlg->Go();
			Window()->Lock();
			if(result){
				game->SaveConfig();
			}
			pWindowMain->GetLauncher()->GetGameManager().Verify();
			UpdateGameList();
		}catch(const deException &e){
			pWindowMain->DisplayException(e);
		}
		break;
	}
		
	case MSG_CONTEXT_RUNWITH:{
		delGame * const game = GetSelectedGame();
		if(game){
			pRunGameWith(game);
		}
		break;
	}
		
	case MSG_CONTEXT_KILL:{
		delGame * const game = GetSelectedGame();
		if(!game){
			break;
		}
		if(!game->IsRunning()){
			Window()->Unlock();
			BAlert alert("Kill Game", "This game is not running.", "OK");
			alert.Go();
			Window()->Lock();
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
		break;
	}
		
	default:
		BView::MessageReceived(message);
	}
}

void deglbPanelGames::MouseDown(BPoint where){
	uint32 buttons;
	GetMouse(&where, &buttons);
	
	if(buttons & B_SECONDARY_MOUSE_BUTTON){
		const int index = pListGames->IndexOf(where);
		if(index >= 0){
			pListGames->Select(index);
		}
		
		delGame * const game = GetSelectedGame();
		if(game){
			ConvertToScreen(&where);
			pShowContextMenu(where, game);
		}
		return;
	}
	
	BView::MouseDown(where);
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
		Window()->Unlock();
		BAlert alert("Start Game", "This game is already running.", "OK");
		alert.Go();
		Window()->Lock();
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
				Window()->Unlock();
				BAlert alert("Can not run game", error.GetString(), "OK");
				alert.Go();
				Window()->Lock();
				return;
			}
			
			if(profile->GetReplaceRunArguments()){
				runParams.SetRunArguments(profile->GetRunArguments().GetString());
			}else{
				runParams.SetRunArguments(
					(game->GetRunArguments() + " " + profile->GetRunArguments()).GetString());
			}
			
			runParams.SetWidth(profile->GetWidth());
			runParams.SetHeight(profile->GetHeight());
			runParams.SetFullScreen(profile->GetFullScreen());
			
			const decPoint windowSize(game->GetDisplayScaledWindowSize());
			if(windowSize != decPoint()){
				runParams.SetWidth(windowSize.x);
				runParams.SetHeight(windowSize.y);
				runParams.SetFullScreen(false);
			}
			
			game->StartGame(runParams);
			break;
			
		}else if(!profile->GetValid()){
			Window()->Unlock();
			BAlert alert("Can not run game",
				"The Game Profile is not working. Please fix it in Game Properties.", "OK");
			alert.Go();
			Window()->Lock();
			break;
			
		}else if(!game->GetAllFormatsSupported()){
			Window()->Unlock();
			BAlert alert("Can not run game",
				"One or more File Formats required by the game are not working.", "OK");
			alert.Go();
			Window()->Lock();
			break;
			
		}else{
			Window()->Unlock();
			BAlert alert("Can not run game", "Game related properties are incorrect.", "OK");
			alert.Go();
			Window()->Lock();
			break;
		}
	}
	
	UpdateGameList();
}

void deglbPanelGames::pRunGameWith(delGame *game){
	if(game->IsRunning()){
		Window()->Unlock();
		BAlert alert("Start Game", "This game is already running.", "OK");
		alert.Go();
		Window()->Lock();
		return;
	}
	
	if(!game->GetCanRun()){
		const char *msg = game->GetAllFormatsSupported()
			? "Game related properties are incorrect."
			: "One or more File Formats required by the game are not working.";
		Window()->Unlock();
		BAlert alert("Can not run game", msg, "OK");
		alert.Go();
		Window()->Lock();
		return;
	}
	
	deglbDialogRunGameWith *dlg = new deglbDialogRunGameWith(pWindowMain, game);
	dlg->SetProfile(game->GetActiveProfile());
	
	Window()->Unlock();
	const bool result = dlg->Go();
	Window()->Lock();
	
	if(!result){
		UpdateGameList();
		return;
	}
	
	const delGameManager &gameManager = pWindowMain->GetLauncher()->GetGameManager();
	delGameRunParams runParams;
	runParams.SetGameProfile(dlg->GetProfile());
	if(!runParams.GetGameProfile()){
		runParams.SetGameProfile(gameManager.GetActiveProfile());
		if(!runParams.GetGameProfile()){
			runParams.SetGameProfile(gameManager.GetDefaultProfile());
		}
	}
	
	decString error;
	if(!runParams.FindPatches(*game, game->GetUseLatestPatch(),
	game->GetUseCustomPatch(), error)){
		Window()->Unlock();
		BAlert alert("Can not run game", error.GetString(), "OK");
		alert.Go();
		Window()->Lock();
		UpdateGameList();
		return;
	}
	
	if(runParams.GetGameProfile() && !runParams.GetGameProfile()->GetValid()){
		Window()->Unlock();
		BAlert alert("Can not run game", "The selected profile is not valid.", "OK");
		alert.Go();
		Window()->Lock();
		UpdateGameList();
		return;
	}
	
	if(runParams.GetGameProfile()){
		if(runParams.GetGameProfile()->GetReplaceRunArguments()){
			runParams.SetRunArguments(runParams.GetGameProfile()->GetRunArguments().GetString());
		}else{
			runParams.SetRunArguments((game->GetRunArguments() + " "
				+ runParams.GetGameProfile()->GetRunArguments()).GetString());
		}
		runParams.SetWidth(runParams.GetGameProfile()->GetWidth());
		runParams.SetHeight(runParams.GetGameProfile()->GetHeight());
		runParams.SetFullScreen(runParams.GetGameProfile()->GetFullScreen());
	}
	
	const decPoint windowSize(game->GetDisplayScaledWindowSize());
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
		BAlert * const alert = new BAlert("Show Logs",
			"There are no logs for this game.\nLogs will be present after running the game.",
			"OK");
		alert->Go(nullptr); // async
		return;
	}
	
	path.AddComponent("logs");
	
	// On Haiku, open folder in Tracker using BRoster
	BEntry entry(path.GetPathNative().GetString());
	entry_ref ref;
	if(entry.GetRef(&ref) == B_OK){
		BMessage msg(B_REFS_RECEIVED);
		msg.AddRef("refs", &ref);
		be_roster->Launch("application/x-vnd.Be-TRAK", &msg);
	}
}
