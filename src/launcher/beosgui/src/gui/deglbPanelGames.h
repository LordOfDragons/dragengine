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

#ifndef _DEGLB_PANELGAMES_H_
#define _DEGLB_PANELGAMES_H_

#include <View.h>
#include <ListView.h>
#include <ScrollView.h>
#include <ColumnListView.h>
#include <ColumnTypes.h>
#include <Bitmap.h>

#include <delauncher/game/delGame.h>

class deglbWindowMain;


/**
 * \brief Games Panel.
 */
class deglbPanelGames : public BView{
public:
	/** \brief Game list item. */
	class cGameListItem : public BRow{
	public:
		delGame::Ref game;
		BBitmap *icon;
		
		cGameListItem(delGame *game);
		~cGameListItem() override;
	};
	
	enum eMessages{
		MSG_LIST_CHANGED = 'pglc',
		MSG_CONTEXT_PROPS = 'pgcp',
		MSG_CONTEXT_PROPS_DONE = 'pgcP',
		MSG_CONTEXT_RUN = 'pgcr',
		MSG_CONTEXT_RUNWITH = 'pgcw',
		MSG_CONTEXT_RUNWITH_DONE = 'pgcW',
		MSG_CONTEXT_KILL = 'pgck',
		MSG_CONTEXT_SHOWLOGS = 'pgcs',
		MSG_UPDATE_GAMELIST = 'pgUG'
	};
	
	
private:
	deglbWindowMain *pWindowMain;
	BColumnListView *pListGames;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	deglbPanelGames(deglbWindowMain *windowMain);
	
	/** \brief Clean up panel. */
	~deglbPanelGames() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline deglbWindowMain *GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Selected game or nullptr. */
	delGame *GetSelectedGame() const;
	
	/** \brief Set selected game. */
	void SetSelectedGame(delGame *game);
	
	/** \brief Update the games list. */
	void UpdateGameList();
	/*@}*/
	
	
	
	/** \name BView */
	/*@{*/
	void MessageReceived(BMessage *message) override;
	void MouseDown(BPoint where) override;
	/*@}*/
	
	
	
private:
	void pShowContextMenu(BPoint screenWhere, delGame *game);
	void pRunGame(delGame *game);
	void pRunGameWith(delGame *game);
	void pRunGameWithDone(delGame *game, bool result, const delGameProfile::Ref &profile);
	void pShowLogs(delGame *game);
};

#endif
