/* 
 * Drag[en]gine IGDE Project Editor
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

#ifndef _PROJTRPROFILELIST_H_
#define _PROJTRPROFILELIST_H_

#include <dragengine/common/collection/decObjectList.h>

class projTRProfile;
class deEngine;



/**
 * \brief Profiles
 */
class projTRProfileList{
private:
	decObjectList pProfiles;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create profiles. */
	projTRProfileList();
	
	/** \brief Clean up profiles. */
	~projTRProfileList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of profiles. */
	int GetCount() const;
	
	/** \brief Profile at index. */
	projTRProfile *GetAt( int index ) const;
	
	/** \brief Named profile or \em NULL if absent. */
	projTRProfile *GetNamed( const char *name ) const;
	
	/** \brief Profile is present. */
	bool Has( projTRProfile *profile ) const;
	
	/** \brief Named profile is present. */
	bool HasNamed( const char *name ) const;
	
	/** \brief Index of profile or -1 if absent. */
	int IndexOf( projTRProfile *profile ) const;
	
	/** \brief Index of named profile or -1 if absent. */
	int IndexOfNamed( const char *name ) const;
	
	/** \brief Add profile. */
	void Add( projTRProfile *profile );
	
	/** \brief Remove profile. */
	void Remove( projTRProfile *profile );
	
	/** \brief Remove all profiles. */
	void RemoveAll();
	
	
	
	/** \brief Validate profiles. */
	void ValidateProfiles( deEngine &engine );
	/*@}*/
};

#endif
