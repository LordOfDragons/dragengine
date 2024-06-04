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

#ifndef _DEGLPANELGAMES_H_
#define _DEGLPANELGAMES_H_

#include "../foxtoolkit.h"

class deglWindowMain;
class delGame;



/**
 * Games Panel.
 */
class deglPanelGames : public FXVerticalFrame{
	FXDECLARE( deglPanelGames )
protected:
	deglPanelGames();
	
public:
	/** Icon list extended to send header click to enable sorting. */
	class ExtIconList : public FXIconList{
		FXDECLARE( ExtIconList )
	protected:
		ExtIconList();
		
	public:
		enum eFoxSelectors{
			SEL_HEADER_CLICKED = SEL_LAST
		};
		
		ExtIconList( FXComposite *p, FXObject *tgt = nullptr, FXSelector sel = 0,
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
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create panel. */
	deglPanelGames( deglWindowMain *windowMain, FXComposite *container );
	
	/** Clean up panel. */
	virtual ~deglPanelGames();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Main window. */
	inline deglWindowMain *GetWindowMain() const{ return pWindowMain; }
	
	/** Selected game or nullptr. */
	delGame *GetSelectedGame() const;
	
	/** Set selected game. */
	void SetSelectedGame( delGame *game );
	
	/** Update the games list. */
	void UpdateGameList();
	
	/** Sort game list by the title. */
	static FXint SortGamesByTitleAsc( const FXIconItem *item1, const FXIconItem *item2 );
	static FXint SortGamesByTitleDesc( const FXIconItem *item1, const FXIconItem *item2 );
	
	/** Sort game list by the status. */
	static FXint SortGamesByStatusAsc( const FXIconItem *item1, const FXIconItem *item2 );
	static FXint SortGamesByStatusDesc( const FXIconItem *item1, const FXIconItem *item2 );
	
	/** Sort game list by the creator. */
	static FXint SortGamesByCreatorAsc( const FXIconItem *item1, const FXIconItem *item2 );
	static FXint SortGamesByCreatorDesc( const FXIconItem *item1, const FXIconItem *item2 );
	/*@}*/
	
	
	
	/** \name Events */
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

#endif
