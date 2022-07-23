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

#ifndef _DEGLWINDOWMAIN_H_
#define _DEGLWINDOWMAIN_H_

#include "foxtoolkit.h"

class deException;
class decUnicodeString;
class deglLauncher;
class deglPanelGames;
class deglPanelEngine;
class deglGuiBuilder;
class deglWindowLogger;



/**
 * Main Window.
 */
class deglWindowMain : public FXMainWindow{
	FXDECLARE( deglWindowMain )
	
protected:
	deglWindowMain();
	
public:
	enum eFoxIDs{
		ID_LAST = FXMainWindow::ID_LAST,
		ID_FILE_QUIT,
		ID_FILE_INSTALL,
		ID_VIEW_GAMES,
		ID_VIEW_ENGINE,
		ID_VIEW_LOGGING,
		ID_SETTINGS_ENGINE,
		ID_TIMER_PULSE
	};
	
	
	
private:
	deglLauncher *pLauncher;
	
	FXMenuBar *pMenuBar;
	
	FXSwitcher *pSwitcherPanels;
	deglPanelGames *pPanelGames;
	deglPanelEngine *pPanelEngine;
	deglGuiBuilder *pGuiBuilder;
	deglWindowLogger *pWindowLogger;
	
	FXStatusBar *pStatusBar;
	FXProgressBar *pSBProgress;
	
	FXToolTip *pToolTip;
	
	FXIcon *pIconApp;
	FXIcon *pIconValidSmall;
	FXIcon *pIconInvalidSmall;
	FXIcon *pIconButtonInfo;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create main window. */
	deglWindowMain( FXApp* app, int argc, char **argv );
	
	/** Clean up main window. */
	virtual ~deglWindowMain();
	
	/** Fox create window. */
	virtual void create();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Launcher. */
	
	inline deglLauncher *GetLauncher() const{ return pLauncher; }
	/** GUI builder. */
	inline deglGuiBuilder *GetGuiBuilder() const{ return pGuiBuilder; }
	
	/** Run command line actions. Returns true to start the application loop or false to exit. */
	bool RunCommandLineActions();
	
	/** Display exception error. */
	void DisplayException( const deException &exception );
	
	/** Menu bar. */
	inline FXMenuBar *GetMenuBar() const{ return pMenuBar; }
	
	/** Games panel. */
	inline deglPanelGames *GetPanelGames() const{ return pPanelGames; }
	
	/** Logger window or nullptr if not visible. */
	inline deglWindowLogger *GetWindowLogger() const{ return pWindowLogger; }
	
	/**
	 * Show logger window. If not existing yet it is created otherwise it is
	 * focused and raised ontop of all other windows.
	 */
	void ShowWindowLogger();
	
	/** Set visibility of the progress bar in the status bar. */
	void SetProgressVisible( bool visible );
	
	/** Set progress bar progress. */
	void SetProgress( float progress );
	
	/** Set progress text. */
	void SetProgressText( const decUnicodeString &text );
	
	/** Ask user to quit application. */
	bool QuitRequest();
	
	/** Reload games and patches. */
	void ReloadGamesAndPatches();
	
	/** Small valid icon. */
	inline FXIcon *GetIconValidSmall() const{ return pIconValidSmall; }
	
	/** Small invalid icon. */
	inline FXIcon *GetIconInvalidSmall() const{ return pIconInvalidSmall; }
	
	/** Info button icon. */
	inline FXIcon *GetIconButtonInfo() const{ return pIconButtonInfo; }
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onResize( FXObject *sender, FXSelector selector, void *data );
	
	long onFileQuit( FXObject *sender, FXSelector selector, void *data );
	long onFileInstall( FXObject *sender, FXSelector selector, void *data );
	
	long onViewGames( FXObject *sender, FXSelector selector, void *data );
	long onViewEngine( FXObject *sender, FXSelector selector, void *data );
	long onViewLogging( FXObject *sender, FXSelector selector, void *data );
	
	long onSettingsEngine( FXObject *sender, FXSelector selector, void *data );
	
	long onTimerPulse( FXObject *sender, FXSelector selector, void *data );
	/*@}*/
};

#endif
