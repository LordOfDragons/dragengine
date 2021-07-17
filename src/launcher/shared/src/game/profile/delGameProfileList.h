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

#ifndef _DELGAMEPROFILELIST_H_
#define _DELGAMEPROFILELIST_H_

#include <dragengine/common/collection/decObjectList.h>

class delGameProfile;
class delLauncherSupport;


/**
 * \brief Game profile list.
 */
class delGameProfileList{
private:
	decObjectList pProfiles;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	delGameProfileList();
	
	/** \brief Clean up list. */
	~delGameProfileList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of profiles. */
	int GetProfileCount() const;
	
	/** \brief Profile at index. */
	delGameProfile *GetProfileAt( int index ) const;
	
	/** \brief Named profile or nullptr if absent. */
	delGameProfile *GetProfileNamed( const char *name ) const;
	
	/** \brief Profile is present. */
	bool HasProfile( delGameProfile *profile ) const;
	
	/** \brief Named profile is present. */
	bool HasProfileNamed( const char *name ) const;
	
	/** \brief Index of profile or -1 if absent. */
	int IndexOfProfile( delGameProfile *profile ) const;
	
	/** \brief Index of named profile or -1 if absent. */
	int IndexOfProfileNamed( const char *name ) const;
	
	/** \brief Add profile. */
	void AddProfile( delGameProfile *profile );
	
	/** \brief Remove profile. */
	void RemoveProfile( delGameProfile *profile );
	
	/** \brief Remove all profiles. */
	void RemoveAllProfiles();
	
	/** \brief Validate profiles. */
	void ValidateProfiles( delLauncherSupport &launcher );
	/*@}*/
};

#endif
