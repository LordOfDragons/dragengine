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

#ifndef _GDEOCCAMERALIST_H_
#define _GDEOCCAMERALIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class gdeOCCamera;



/**
 * \brief Object class camera list.
 */
class gdeOCCameraList{
private:
	decObjectOrderedSet pCameras;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object class camera list. */
	gdeOCCameraList();
	
	/** \brief Create copy of object class camera list. */
	gdeOCCameraList( const gdeOCCameraList &list );
	
	/** \brief Clean up object class camera list. */
	~gdeOCCameraList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of object class cameras. */
	int GetCount() const;
	
	/** \brief Object class camera at position. */
	gdeOCCamera *GetAt( int index ) const;
	
	/** \brief Index of named object class camera or -1 if absent. */
	int IndexOf( gdeOCCamera *camera ) const;
	
	/** \brief Object class is present. */
	bool Has( gdeOCCamera *camera ) const;
	
	/** \brief Add object class camera. */
	void Add( gdeOCCamera *camera );
	
	/** \brief Remove object class camera. */
	void Remove( gdeOCCamera *camera );
	
	/** \brief Remove all object class cameras. */
	void RemoveAll();
	
	/** \brief Set from another object class camera list. */
	gdeOCCameraList &operator=( const gdeOCCameraList &list );
	/*@}*/
};

#endif
