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

#ifndef _DECLGAMEPROFILELIST_H_
#define _DECLGAMEPROFILELIST_H_

#include <dragengine/common/collection/decObjectList.h>

class declGameProfile;
class declLauncher;



/**
 * @brief Game Profile List.
 */
class declGameProfileList{
private:
	decObjectList pProfiles;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new profile list. */
	declGameProfileList();
	/** Cleans up the profile list. */
	~declGameProfileList();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of profiles. */
	int GetCount() const;
	/** Retrieves the profile at the given position. */
	declGameProfile *GetAt( int index ) const;
	/** Retrieves the profile with the given name or NULL if not found. */
	declGameProfile *GetNamed( const char *name ) const;
	/** Determines if a profile exists. */
	bool Has( declGameProfile *profile ) const;
	/** Determines if a profile with the given name exists. */
	bool HasNamed( const char *name ) const;
	/** Retrieves the index of a profile or -1 if not found. */
	int IndexOf( declGameProfile *profile ) const;
	/** Retrieves the index of a profile with the given name or -1 if not found. */
	int IndexOfNamed( const char *name ) const;
	/** Adds a profile. */
	void Add( declGameProfile *profile );
	/** Removes a profile. */
	void Remove( declGameProfile *profile );
	/** Removes all profiles. */
	void RemoveAll();
	
	/** Validate profiles. */
	void ValidateProfiles( declLauncher &launcher );
	/*@}*/
};

#endif
