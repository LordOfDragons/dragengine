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

#ifndef _DECLLAUNCHER_H_
#define _DECLLAUNCHER_H_

#include "game/patch/declPatchManager.h"

#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/filesystem/deVirtualFileSystemReference.h>
#include <dragengine/logger/deLoggerReference.h>

class declConfiguration;
class declEngine;
class declGameManager;



/**
 * @brief Main Launcher Class.
 */
class declLauncher{
private:
	decUnicodeArgumentList pArgList;
	deVirtualFileSystemReference pFileSystem;
	declConfiguration *pConfiguration;
	declEngine *pEngine;
	declGameManager *pGameManager;
	declPatchManager pPatchManager;
	deLoggerReference pLogger;
	deLoggerReference pEngineLogger;
	deLoggerReference pEngineLoggerDebug;
	
	decString pXMLFile;
	decUnicodeArgumentList pGameArgs;
	decString pWindowTitle;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new launcher. */
	declLauncher();
	/** Cleans up the launcher. */
	~declLauncher();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the command line arguments. */
	inline const decUnicodeArgumentList &GetArgumentList() const{ return pArgList; }
	/** Add argument to the command line argument list. */
	void AddArgument( const decUnicodeString &argument );
	
	/** Retrieves the file system. */
	inline deVirtualFileSystem *GetFileSystem() const{ return pFileSystem; }
	/** Retrieves the configuration. */
	inline declConfiguration *GetConfiguration() const{ return pConfiguration; }
	/** Retrieves the engine. */
	inline declEngine *GetEngine() const{ return pEngine; }
	/** Retrieves the game manager. */
	inline declGameManager *GetGameManager() const{ return pGameManager; }
	
	/** \brief Game patch manager. */
	inline declPatchManager &GetPatchManager(){ return pPatchManager; }
	inline const declPatchManager &GetPatchManager() const{ return pPatchManager; }
	
	/** Retrieves the logger. */
	inline deLogger *GetLogger() const{ return pLogger; }
	/** Retrieves the engine logger. */
	inline deLogger *GetEngineLogger() const{ return pEngineLogger; }
	/** Retrieves the engine debug logger. */
	inline deLogger *GetEngineLoggerDebug() const{ return pEngineLoggerDebug; }
	
	/** Print syntax. */
	void PrintSyntax();
	
	/** Init. */
	void Init();
	/** Run. */
	int Run();
	/** Clean up. */
	void CleanUp();
	
	/** \brief Read input from user. */
	decString ReadInput() const;
	
	/** \brief Read input from user returning true if 'y' or 'Y'. */
	bool ReadInputConfirm() const;
	
	/** \brief Read input from user converting to integer number if valid else -1. */
	int ReadInputSelection() const;
	/*@}*/
	
private:
	void pInitLogger();
};

#endif
