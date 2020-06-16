/* 
 * Drag[en]gine IGDE Rig Editor
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

// include only once
#ifndef _RELOADSAVESYSTEM_H_
#define _RELOADSAVESYSTEM_H_

// includes
#include <deigde/gui/filedialog/igdeFilePatternList.h>

// predefinitions
class reLSRig;
class reWindowMain;
class reRig;
class igdeGameDefinition;



/**
 * Load/Save System for the editor.
 */
class reLoadSaveSystem{
private:
	reWindowMain &pWindowMain;
	
	reLSRig **pLSRigs;
	int pLSRigCount;
	int pLSRigSize;
	
public:
	// constructor, destructor
	reLoadSaveSystem( reWindowMain &windowMain );
	~reLoadSaveSystem();
	
	// rigs management
	/** Retrieves the main window. */
	inline reWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** Retrieves the number of load save rigs. */
	inline int GetLSRigCount() const{ return pLSRigCount; }
	/** Retrieves the load save rig at the given index. */
	reLSRig *GetLSRigAt( int index ) const;
	/** Retrieves the index of the load save rig. */
	int IndexOfLSRig( reLSRig *lsRig ) const;
	/** Determines if the save rig exists. */
	bool HasLSRig( reLSRig *lsRig ) const;
	/** Retrieves the index of the load save rig matching the given filename. */
	int IndexOfLSRigMatching( const char *filename );
	/** Adds a load save rig. */
	void AddLSRig( reLSRig *lsRig );
	/** Removes a load save rig. */
	void RemoveLSRig( reLSRig *lsRig );
	/** Removes all load save rigs. */
	void RemoveAllLSRigs();
	/** Updates the load save rig list from the engine. */
	void UpdateLSRigs();
	
	/** Loads the rig from file if possible. */
	reRig *LoadRig( const char *filename );
	/** Saves the rig to file if possible. */
	void SaveRig( reRig *rig, const char *filename );
	
private:
	void pCleanUp();
};

// end of include only once
#endif
