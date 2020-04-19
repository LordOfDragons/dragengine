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

#ifndef _DELLGAMEPROFILELIST_H_
#define _DELLGAMEPROFILELIST_H_

#include <dragengine/common/collection/decObjectList.h>

class dellGameProfile;
class dellLauncher;



/**
 * @brief Game Profile List.
 */
class dellGameProfileList{
private:
	decObjectList pProfiles;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new profile list. */
	dellGameProfileList();
	/** Cleans up the profile list. */
	~dellGameProfileList();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of profiles. */
	int GetCount() const;
	/** Retrieves the profile at the given position. */
	dellGameProfile *GetAt( int index ) const;
	/** Retrieves the profile with the given name or NULL if not found. */
	dellGameProfile *GetNamed( const char *name ) const;
	/** Determines if a profile exists. */
	bool Has( dellGameProfile *profile ) const;
	/** Determines if a profile with the given name exists. */
	bool HasNamed( const char *name ) const;
	/** Retrieves the index of a profile or -1 if not found. */
	int IndexOf( dellGameProfile *profile ) const;
	/** Retrieves the index of a profile with the given name or -1 if not found. */
	int IndexOfNamed( const char *name ) const;
	/** Adds a profile. */
	void Add( dellGameProfile *profile );
	/** Removes a profile. */
	void Remove( dellGameProfile *profile );
	/** Removes all profiles. */
	void RemoveAll();
	
	/** Validate profiles. */
	void ValidateProfiles( dellLauncher &launcher );
	/*@}*/
};

#endif
