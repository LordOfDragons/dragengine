/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDEWDEBUGDRAWERSHAPELIST_H_
#define _IGDEWDEBUGDRAWERSHAPELIST_H_

#include <dragengine/common/collection/decPointerSet.h>

class deDebugDrawer;
class igdeWDebugDrawerShape;


/**
 * \brief Debug drawer shape wrapper List.
 */
class igdeWDebugDrawerShapeList{
private:
	decPointerSet pList;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	igdeWDebugDrawerShapeList();
	
	/** \brief Clean up list. */
	~igdeWDebugDrawerShapeList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of wrappers. */
	int GetCount() const;
	
	/** \brief Wrapper by index. */
	igdeWDebugDrawerShape *GetAt( int index ) const;
	
	/** \brief Determines if an wrapper exists. */
	bool Has( igdeWDebugDrawerShape *wrapper ) const;
	
	/** \brief Adds a wrapper. */
	void Add( igdeWDebugDrawerShape *wrapper );
	
	/** \brief Removes a wrapper. */
	void Remove( igdeWDebugDrawerShape *wrapper );
	
	/** \brief Removes all wrappers. */
	void RemoveAll();
	
	/** \brief Set parent debug drawer or NULL for all wrappers. */
	void SetParentDebugDrawerAll( deDebugDrawer *debugDrawer );
	
	/** \brief Set visible for all wrappers. */
	void SetVisibleAll( bool visible );
	/*@}*/
};

#endif
