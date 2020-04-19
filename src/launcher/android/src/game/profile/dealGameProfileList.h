/* 
 * Drag[en]gine Android Launcher
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

#ifndef _DEALGAMEPROFILELIST_H_
#define _DEALGAMEPROFILELIST_H_

#include "../../common/collection/decObjectList.h"

class dealGameProfile;
class dealLauncher;



/**
 * \brief Game Profile List.
 */
class dealGameProfileList{
private:
	decObjectList pProfiles;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create profile list. */
	dealGameProfileList();
	
	/** \brief Clean up profile list. */
	~dealGameProfileList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of profiles. */
	int GetCount() const;
	
	/** \brief Profile at the given position. */
	dealGameProfile *GetAt( int index ) const;
	
	/** \brief Profile with the given name or \em NULL if not found. */
	dealGameProfile *GetNamed( const char *name ) const;
	
	/** \brief Profile exists. */
	bool Has( dealGameProfile *profile ) const;
	
	/** \brief Profile with the given name exists. */
	bool HasNamed( const char *name ) const;
	
	/** \breif Index of a profile or -1 if not found. */
	int IndexOf( dealGameProfile *profile ) const;
	
	/** \brief Index of a profile with the given name or -1 if not found. */
	int IndexOfNamed( const char *name ) const;
	
	/** \brief Add profile. */
	void Add( dealGameProfile *profile );
	
	/** \brief Remove profile. */
	void Remove( dealGameProfile *profile );
	
	/** \brief Remove all profiles. */
	void RemoveAll();
	
	/** \brief Validate profiles. */
	void ValidateProfiles( dealLauncher &launcher );
	/*@}*/
};

#endif
