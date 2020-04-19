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

#ifndef _DECLPATCHMANAGER_H_
#define _DECLPATCHMANAGER_H_

#include "declPatchList.h"

#include <dragengine/common/string/decString.h>

class declLauncher;
class deVFSDiskDirectory;
class deVirtualFileSystem;
class decPath;


/**
 * \brief Game patch manager.
 */
class declPatchManager{
private:
	declLauncher &pLauncher;
	declPatchList pPatches;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game patch manager. */
	declPatchManager( declLauncher &launcher );
	
	/** \brief Clean up game patch manager. */
	~declPatchManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Game list. */
	inline declPatchList &GetPatches(){ return pPatches; }
	inline const declPatchList &GetPatches() const{ return pPatches; }
	
	/** \brief Load list of game patches from game patch directory. */
	void LoadPatchList();
	
	/** \brief Load a game patch from a disk file. */
	void LoadPatchFromDisk( const decString &path, declPatchList &list );
	/*@}*/
	
	
private:
	void pScanPatchDefFiles( deVirtualFileSystem &vfs, const decPath &baseDir, const decPath &directory );
	void pProcessFoundFiles( const decPath &path );
};

#endif
