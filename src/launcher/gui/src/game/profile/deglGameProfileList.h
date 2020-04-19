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

#ifndef _DEGLGAMEPROFILELIST_H_
#define _DEGLGAMEPROFILELIST_H_

#include <dragengine/common/collection/decObjectList.h>

class deglGameProfile;
class deglLauncher;



/**
 * @brief Game Profile List.
 */
class deglGameProfileList{
private:
	decObjectList pProfiles;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new profile list. */
	deglGameProfileList();
	/** Cleans up the profile list. */
	~deglGameProfileList();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of profiles. */
	int GetProfileCount() const;
	/** Retrieves the profile at the given position. */
	deglGameProfile *GetProfileAt( int index ) const;
	/** Retrieves the profile with the given name or NULL if not found. */
	deglGameProfile *GetProfileNamed( const char *name ) const;
	/** Determines if a profile exists. */
	bool HasProfile( deglGameProfile *profile ) const;
	/** Determines if a profile with the given name exists. */
	bool HasProfileNamed( const char *name ) const;
	/** Retrieves the index of a profile or -1 if not found. */
	int IndexOfProfile( deglGameProfile *profile ) const;
	/** Retrieves the index of a profile with the given name or -1 if not found. */
	int IndexOfProfileNamed( const char *name ) const;
	/** Adds a profile. */
	void AddProfile( deglGameProfile *profile );
	/** Removes a profile. */
	void RemoveProfile( deglGameProfile *profile );
	/** Removes all profiles. */
	void RemoveAllProfiles();
	
	/** Validate profiles. */
	void ValidateProfiles( deglLauncher &launcher );
	/*@}*/
};

#endif // _DEGLGAME_H_
