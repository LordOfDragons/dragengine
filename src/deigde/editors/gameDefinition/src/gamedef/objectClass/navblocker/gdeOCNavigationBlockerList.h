/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#ifndef _GDEOCNAVIGATIONBLOCKERLIST_H_
#define _GDEOCNAVIGATIONBLOCKERLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class gdeOCNavigationBlocker;



/**
 * \brief Object class navigation blocker list.
 */
class gdeOCNavigationBlockerList{
private:
	decObjectOrderedSet pNavigationBlockers;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object class navigation blocker list. */
	gdeOCNavigationBlockerList();
	
	/** \brief Create copy of object class navigation blocker list. */
	gdeOCNavigationBlockerList( const gdeOCNavigationBlockerList &list );
	
	/** \brief Clean up object class navigation blocker list. */
	~gdeOCNavigationBlockerList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of object class navigation blockers. */
	int GetCount() const;
	
	/** \brief Object class navigation blocker at position. */
	gdeOCNavigationBlocker *GetAt( int index ) const;
	
	/** \brief Index of named object class navigation blocker or -1 if absent. */
	int IndexOf( gdeOCNavigationBlocker *blocker ) const;
	
	/** \brief Object class navigation blocker is present. */
	bool Has( gdeOCNavigationBlocker *blocker ) const;
	
	/** \brief Add object class navigation blocker. */
	void Add( gdeOCNavigationBlocker *blocker );
	
	/** \brief Remove object class navigation blocker. */
	void Remove( gdeOCNavigationBlocker *blocker );
	
	/** \brief Remove all object class navigation blockers. */
	void RemoveAll();
	
	/** \brief Set from another object class navigation blocker list. */
	gdeOCNavigationBlockerList &operator=( const gdeOCNavigationBlockerList &list );
	/*@}*/
};

#endif
