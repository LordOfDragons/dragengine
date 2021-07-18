/* 
 * Drag[en]gine Launcher Shared
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DELPATCHMANAGER_H_
#define _DELPATCHMANAGER_H_

#include "delPatchList.h"

#include <dragengine/common/string/decString.h>

class delLauncher;
class deVirtualFileSystem;
class decPath;
class delEngineInstance;


/**
 * \brief Game patch manager.
 */
class delPatchManager{
private:
	delLauncher &pLauncher;
	delPatchList pPatches;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game patch manager. */
	delPatchManager( delLauncher &launcher );
	
	/** \brief Clean up game patch manager. */
	~delPatchManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Launcher. */
	inline delLauncher &GetLauncher() const{ return pLauncher; }
	
	/** \brief Game list. */
	inline delPatchList &GetPatches(){ return pPatches; }
	inline const delPatchList &GetPatches() const{ return pPatches; }
	
	/** \brief Load list of game patches from game patch directory. */
	void LoadPatchList( delEngineInstance &instance );
	
	/** \brief Load a game patch from a disk file. */
	void LoadPatchFromDisk( delEngineInstance &instance, const decString &path, delPatchList &list );
	
	/** \brief Clear. */
	void Clear();
	/*@}*/
	
	
	
protected:
	void pScanPatchDefFiles( delEngineInstance &instance, deVirtualFileSystem &vfs,
		const decPath &baseDir, const decPath &directory );
	
	void pProcessFoundFiles( delEngineInstance &instance, const decPath &path );
};

#endif
