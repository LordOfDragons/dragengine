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

#ifndef _DEGLPATCHMANAGER_H_
#define _DEGLPATCHMANAGER_H_

#include "deglPatchList.h"

#include <dragengine/common/string/decString.h>

class deglLauncher;
class deVFSDiskDirectory;
class deVirtualFileSystem;
class decPath;
class deglEngineInstance;


/**
 * \brief Game patch manager.
 */
class deglPatchManager{
private:
	deglLauncher &pLauncher;
	deglPatchList pPatches;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game patch manager. */
	deglPatchManager( deglLauncher &launcher );
	
	/** \brief Clean up game patch manager. */
	~deglPatchManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Game list. */
	inline deglPatchList &GetPatches(){ return pPatches; }
	inline const deglPatchList &GetPatches() const{ return pPatches; }
	
	/** \brief Load list of game patches from game patch directory. */
	void LoadPatchList( deglEngineInstance &instance );
	
	/** \brief Load a game patch from a disk file. */
	void LoadPatchFromDisk( deglEngineInstance &instance, const decString &path, deglPatchList &list );
	/*@}*/
	
	
private:
	void pScanPatchDefFiles( deglEngineInstance &instance, deVirtualFileSystem &vfs,
		const decPath &baseDir, const decPath &directory );
	void pProcessFoundFiles( deglEngineInstance &instance, const decPath &path );
};

#endif
