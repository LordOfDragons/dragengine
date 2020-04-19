/* 
 * Drag[en]gine Console Launcher
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

#ifndef _DECLCONFIGURATION_H_
#define _DECLCONFIGURATION_H_

#include <dragengine/common/string/decString.h>

class declLauncher;



/**
 * \brief Launcher Configuration.
 */
class declConfiguration{
private:
	declLauncher *pLauncher;
	
	decString pPathConfigSystem;
	decString pPathConfigUser;
	decString pPathShares;
	decString pPathGames;
	decString pPathLogs;
	
	bool pCanSave;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create configuration. */
	declConfiguration( declLauncher *launcher );
	
	/** \brief Clean up configuration. */
	~declConfiguration();
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
	
	/** \brief Games path. */
	inline const decString &GetPathGames() const{ return pPathGames; }
	
	/** \brief Set games path. */
	void SetPathGames( const char *path );
	
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
	 * 
	 * Tries to locate the configuration file in the known places reading first the system
	 * wide configuration and then the user configuration if found. For all not found
	 * configuration values the default value is used.
	 */
	void LoadConfiguration();
	
	/**
	 * \brief Saves the configuration to the user configuration directory. If the
	 * directory or files do not exist they are created.
	 */
	void SaveConfiguration();
	/*@}*/
};

#endif
