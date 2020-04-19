/* 
 * Drag[en]gine Android Launcher
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

#ifndef _DEALCONFIGURATION_H_
#define _DEALCONFIGURATION_H_

#include "dealConfigWindow.h"

#include "../common/string/decString.h"

class dealLauncher;



/**
 * \brief Launcher configuration.
 */
class dealConfiguration{
private:
	dealLauncher &pLauncher;
	
	decString pPathConfigSystem;
	decString pPathConfigUser;
	decString pPathShares;
	decString pPathLogs;
	
	dealConfigWindow pWindowMain;
	
	int pClrValidBack;
	int pClrValidText;
	int pClrProblemBack;
	int pClrProblemText;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create configuration. */
	dealConfiguration( dealLauncher &launcher );
	
	/** \brief Clean up configuration. */
	~dealConfiguration();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief System config path. */
	inline const decString &GetPathConfigSystem() const{ return pPathConfigSystem; }
	
	/** \brief Set system config path. */
	void SetPathConfigSystem( const char *path );
	
	/** \brief User config path. */
	inline const decString &GetPathConfigUser() const{ return pPathConfigUser; }
	
	/** \brief Set user config path. */
	void SetPathConfigUser( const char *path );
	
	/** \brief Shares path. */
	inline const decString &GetPathShares() const{ return pPathShares; }
	
	/** \brief Set shares path. */
	void SetPathShares( const char *path );
	
	/** \brief Logs path. */
	inline const decString &GetPathLogs() const{ return pPathLogs; }
	
	/** \brief Set logs path. */
	void SetPathLogs( const char *path );
	
	
	
	/** \brief Locate path. */
	void LocatePath();
	
	/** \brief Initialize virtual file system. */
	void InitVirtualFileSystem();
	
	/** \brief Log some important value. */
	void LogImportantValues();
	
	/**
	 * \brief Load the configuration.
	 * \details Tries to locate the configuration file in the known places reading first
	 *          the system wide configuration and then the user configuration if found.
	 *          For all not found configuration values the default value is used.
	 */
	void LoadConfiguration();
	
	/**
	 * \brief Save configuration to the user configuration directory.
	 * \details If the directory or files do not exist they are created.
	 */
	void SaveConfiguration();
	
	/** \brief Main window configuration. */
	inline dealConfigWindow &GetWindowMain(){ return pWindowMain; }
	inline const dealConfigWindow &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Background color for valid elements. */
	inline int GetBackColorValid() const{ return pClrValidBack; }
	
	/** \brief Background color for problem elements. */
	inline int GetBackColorProblem() const{ return pClrProblemBack; }
	
	/** \brief Text color for valid elements. */
	inline int GetTextColorValid() const{ return pClrValidText; }
	
	/** \brief Text color for problem elements. */
	inline int GetTextColorProblem() const{ return pClrProblemText; }
	/*@}*/
	
private:
};

#endif
