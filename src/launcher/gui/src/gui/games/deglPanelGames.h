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

#ifndef _DEGLPANELGAMES_H_
#define _DEGLPANELGAMES_H_

#include "../foxtoolkit.h"

class deglWindowMain;
class deglGame;



/**
 * @brief Games Panel.
 */
class deglPanelGames : public FXVerticalFrame{
	FXDECLARE( deglPanelGames )
protected:
	deglPanelGames();
	
public:
	/** \brief Icon list extended to send header click to enable sorting. */
	class ExtIconList : public FXIconList{
		FXDECLARE( ExtIconList )
	protected:
		ExtIconList();
		
	public:
		enum eFoxSelectors{
			SEL_HEADER_CLICKED = SEL_LAST
		};
		
		ExtIconList( FXComposite *p, FXObject *tgt = NULL, FXSelector sel = 0,
			FXuint opts = ICONLIST_NORMAL, FXint x = 0, FXint y = 0,
			FXint w = 0, FXint h = 0 );
		
		~ExtIconList();
		
		long onHeaderClicked( FXObject *sender, FXSelector selector, void *data );
	};
	
	
	
public:
	enum eFoxIDs{
		ID_CANVAS = FXVerticalFrame::ID_LAST,
		ID_LIST_GAMES,
		ID_PU_GAME_PROPS,
		ID_PU_GAME_RUN,
		ID_PU_GAME_RUNWITH,
		ID_PU_GAME_KILL,
		ID_PU_GAME_UNINSTALL,
		ID_PU_GAME_SHOWLOGS,
		ID_LAST
	};
	
	enum eListGamesSorting{
		elgsTitleAsc,
		elgsTitleDesc,
		elgsStatusAsc,
		elgsStatusDesc,
		elgsCreatorAsc,
		elgsCreatorDesc
	};
	
private:
	deglWindowMain *pWindowMain;
	
	FXIconList *pListGames;
	int pSortListGames;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new panel. */
	deglPanelGames( deglWindowMain *windowMain, FXComposite *container );
	/** Cleans up the panel. */
	virtual ~deglPanelGames();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the main window. */
	inline deglWindowMain *GetWindowMain() const{ return pWindowMain; }
	
	/** Retrieves the selected game if any. */
	deglGame *GetSelectedGame() const;
	/** Sets the selected game. */
	void SetSelectedGame( deglGame *game );
	
	/** Update the games list. */
	void UpdateGameList();
	
	/** Sorts game list by the title. */
	static FXint SortGamesByTitleAsc( const FXIconItem *item1, const FXIconItem *item2 );
	static FXint SortGamesByTitleDesc( const FXIconItem *item1, const FXIconItem *item2 );
	/** Sorts game list by the status. */
	static FXint SortGamesByStatusAsc( const FXIconItem *item1, const FXIconItem *item2 );
	static FXint SortGamesByStatusDesc( const FXIconItem *item1, const FXIconItem *item2 );
	/** Sorts game list by the creator. */
	static FXint SortGamesByCreatorAsc( const FXIconItem *item1, const FXIconItem *item2 );
	static FXint SortGamesByCreatorDesc( const FXIconItem *item1, const FXIconItem *item2 );
	/*@}*/
	
	/** @name Events */
	/*@{*/
	long onListGamesChanged( FXObject *sender, FXSelector selector, void *data );
	long onListGamesRDown( FXObject *sender, FXSelector selector, void *data );
	long onListGamesRUp( FXObject *sender, FXSelector selector, void *data );
	long onListGamesDblClick( FXObject *sender, FXSelector selector, void *data );
	long onListGamesHeaderClicked( FXObject *sender, FXSelector selector, void *data );
	
	long onPUGameProps( FXObject *sender, FXSelector selector, void *data );
	long updatePUGameProps( FXObject *sender, FXSelector selector, void *data );
	long onPUGameRun( FXObject *sender, FXSelector selector, void *data );
	long onPUGameRunWith( FXObject *sender, FXSelector selector, void *data );
	long updatePUGameRun( FXObject *sender, FXSelector selector, void *data );
	long onPUGameKill( FXObject *sender, FXSelector selector, void *data );
	long updatePUGameKill( FXObject *sender, FXSelector selector, void *data );
	long onPUGameUninstall( FXObject *sender, FXSelector selector, void *data );
	long updatePUGameUninstall( FXObject *sender, FXSelector selector, void *data );
	long onPUGameShowLogs( FXObject *sender, FXSelector selector, void *data );
	long updatePUGameShowLogs( FXObject *sender, FXSelector selector, void *data );
	/*@}*/
};

// end of include only once
#endif
