/* 
 * Drag[en]gine Live Launcher
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

#ifndef _DELLLAUNCHER_H_
#define _DELLLAUNCHER_H_

#include "game/dellGameManager.h"
#include "engine/dellEngine.h"

#include <dragengine/common/file/decPath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#include <dragengine/filesystem/deVirtualFileSystemReference.h>
#include <dragengine/logger/deLoggerReference.h>

class dellEngine;



/**
 * \brief Launcher.
 */
class dellLauncher{
private:
	decUnicodeArgumentList pArguments;
	decPath pWorkingDir;
	decPath pPathConfigUser;
	decStringList pEnvParamsStore;
	deVirtualFileSystemReference pFileSystem;
	deLoggerReference pLogger;
	dellEngine pEngine;
	dellGameManager pGameManager;
	
	decString pDelgaFile;
	decUnicodeArgumentList pGameArgs;
	decString pWindowTitle;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create launcher. */
	dellLauncher();
	
	/** \brief Clean up launcher. */
	~dellLauncher();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Command line arguments. */
	inline const decUnicodeArgumentList &GetArguments() const{ return pArguments; }
	
	/** \brief Add argument to the command line argument list. */
	void AddArgument( const decUnicodeString &argument );
	
	/** \brief File system. */
	inline deVirtualFileSystem *GetFileSystem() const{ return pFileSystem; }
	
	/** \brief Engine. */
	inline dellEngine &GetEngine(){ return pEngine; }
	inline const dellEngine &GetEngine() const{ return pEngine; }
	
	/** \brief Game manager. */
	inline dellGameManager &GetGameManager(){ return pGameManager; }
	inline const dellGameManager &GetGameManager() const{ return pGameManager; }
	
	/** \brief Logger. */
	inline deLogger *GetLogger() const{ return pLogger; }
	
	/** \brief Working directory. */
	inline const decPath GetWorkingDirectory() const{ return pWorkingDir; }
	
	/** \brief Init. */
	void Init();
	
	/** \brief Run. */
	void Run();
	/*@}*/
	
	
	
private:
	void pInitVFS();
	void pInitLogger();
	void pUpdateEnvironment();
};

#endif
