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
 * @brief Main Window.
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
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new main window. */
	deglWindowMain( FXApp* app, int argc, char **argv );
	/** Cleans up the main window prototype. */
	virtual ~deglWindowMain();
	/** Fox request for creating the window. */
	virtual void create();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the launcher. */
	inline deglLauncher *GetLauncher() const{ return pLauncher; }
	/** Retrieves the gui builder. */
	inline deglGuiBuilder *GetGuiBuilder() const{ return pGuiBuilder; }
	
	/**
	 * \brief Run command line actions.
	 * \returns True to start the application loop or false to exit
	 */
	bool RunCommandLineActions();
	
	/** Displays an exception error. */
	void DisplayException( const deException &exception );
	
	/** Retrieves the menu bar. */
	inline FXMenuBar *GetMenuBar() const{ return pMenuBar; }
	
	/** Retrieves the games panel. */
	inline deglPanelGames *GetPanelGames() const{ return pPanelGames; }
	
	/** Retrieves the logger window or NULL if not visible. */
	inline deglWindowLogger *GetWindowLogger() const{ return pWindowLogger; }
	/**
	 * Shows the logger window. If not existing yet it is created otherwise it is
	 * focused and raised ontop of all other windows.
	 */
	void ShowWindowLogger();
	
	/** Sets the visibility of the progress bar in the status bar. */
	void SetProgressVisible( bool visible );
	/** Sets the progress bar progress. */
	void SetProgress( float progress );
	/** Sets the progress text. */
	void SetProgressText( const decUnicodeString &text );
	
	/** Asks the user if it is okay to quit the application. */
	bool QuitRequest();
	
	/** \brief Reload games and patches. */
	void ReloadGamesAndPatches();
	
	/** Retrieves the small valid icon. */
	inline FXIcon *GetIconValidSmall() const{ return pIconValidSmall; }
	/** Retrieves the small invalid icon. */
	inline FXIcon *GetIconInvalidSmall() const{ return pIconInvalidSmall; }
	/** Retrieves the info button icon. */
	inline FXIcon *GetIconButtonInfo() const{ return pIconButtonInfo; }
	/*@}*/
	
	/** @name Events */
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

// end of include only once
#endif
