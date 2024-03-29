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

#include <delauncher/delLauncher.h>

#include <dragengine/common/collection/decObjectSet.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#include <dragengine/filesystem/deVirtualFileSystemReference.h>
#include <dragengine/logger/deLoggerReference.h>

#ifdef OS_BEOS
#include <delauncher/game/delGame.h>
#endif



/**
 * Live launcher.
 */
class dellLauncher{
public:
	class Launcher : public delLauncher{
	public:
		Launcher();
		virtual ~Launcher();
	};
	
#ifdef OS_UNIX
	class PreloadLibrary : public deObject{
		void * const pHandle;
	public:
		PreloadLibrary( const decPath &basePath, const char *filename );
		virtual ~PreloadLibrary();
	};
#endif
	
	
	
private:
	decUnicodeArgumentList pArguments;
	decPath pWorkingDir;
	decPath pPathConfigUser;
	decStringList pEnvParamsStore;
	
#ifdef OS_UNIX
	decObjectSet pPreloadLibraries;
#endif
	
	Launcher *pLauncher;
	
	decString pDelgaFile;
	decUnicodeArgumentList pGameArgs;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create launcher. */
	dellLauncher();
	
	/** Clean up launcher. */
	~dellLauncher();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Command line arguments. */
	inline const decUnicodeArgumentList &GetArguments() const{ return pArguments; }
	
	/** Add argument to the command line argument list. */
	void AddArgument( const decUnicodeString &argument );
	
	/** Working directory. */
	inline const decPath GetWorkingDirectory() const{ return pWorkingDir; }
	
	/** Launcher. */
	inline Launcher &GetLauncher() const{ return *pLauncher; }
	
	/** Run. */
	void Run();

#ifdef OS_BEOS
	delGame::Ref runningGame;
#endif
	/*@}*/
	
	
	
private:
	void pUpdateEnvironment();
};

#endif
