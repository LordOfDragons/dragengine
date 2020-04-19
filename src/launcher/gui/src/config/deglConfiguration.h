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

#ifndef _DEGLCONFIGURATION_H_
#define _DEGLCONFIGURATION_H_

#include "deglConfigWindow.h"
#include "../gui/foxtoolkit.h"

#include <dragengine/common/string/decString.h>

class deglLauncher;



/**
 * \brief Launcher Configuration.
 */
class deglConfiguration{
private:
	deglLauncher *pLauncher;
	
	decString pPathConfigSystem;
	decString pPathConfigUser;
	decString pPathShares;
	decString pPathGames;
	decString pPathLogs;
	
	deglConfigWindow pWindowMain;
	
	FXColor pClrValidBack;
	FXColor pClrValidText;
	FXColor pClrProblemBack;
	FXColor pClrProblemText;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new configuration. */
	deglConfiguration( deglLauncher *launcher );
	/** Cleans up the configuration. */
	~deglConfiguration();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the system config path. */
	inline const decString &GetPathConfigSystem() const{ return pPathConfigSystem; }
	/** Sets the system config path. */
	void SetPathConfigSystem( const char *path );
	/** Retrieves the user config path. */
	inline const decString &GetPathConfigUser() const{ return pPathConfigUser; }
	/** Sets the user config path. */
	void SetPathConfigUser( const char *path );
	
	/** \brief Shares path. */
	inline const decString &GetPathShares() const{ return pPathShares; }
	
	/** \brief Set shares path. */
	void SetPathShares( const char *path );
	
	/** \brief Games path. */
	inline const decString &GetPathGames() const{ return pPathGames; }
	
	/** \brief Set games path. */
	void SetPathGames( const char *path );
	
	/** Retrieves the logs path. */
	inline const decString &GetPathLogs() const{ return pPathLogs; }
	/** Sets the logs path. */
	void SetPathLogs( const char *path );
	
	/** Locate path. */
	void LocatePath();
	/** Initialize virtual file system. */
	void InitVirtualFileSystem();
	/** Log some important value. */
	void LogImportantValues();
	
	/**
	 * Load the configuration. Tries to locate the configuration file in
	 * the known places reading first the system wide configuration and
	 * then the user configuration if found. For all not found configuration
	 * values the default value is used.
	 */
	void LoadConfiguration();
	/**
	 * Saves the configuration to the user configuration directory. If the
	 * directory or files do not exist they are created.
	 */
	void SaveConfiguration();
	
	/** Retrieves the main window configuration. */
	inline deglConfigWindow &GetWindowMain(){ return pWindowMain; }
	inline const deglConfigWindow &GetWindowMain() const{ return pWindowMain; }
	
	/** Retrieves the background color for valid elements. */
	inline FXColor GetBackColorValid() const{ return pClrValidBack; }
	/** Retrieves the background color for problem elements. */
	inline FXColor GetBackColorProblem() const{ return pClrProblemBack; }
	/** Retrieves the text color for valid elements. */
	inline FXColor GetTextColorValid() const{ return pClrValidText; }
	/** Retrieves the text color for problem elements. */
	inline FXColor GetTextColorProblem() const{ return pClrProblemText; }
	/*@}*/
	
private:
};

#endif // _DEGLGAME_H_
