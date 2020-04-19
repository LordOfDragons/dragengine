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

#ifndef _IGDELISTHEADER_H_
#define _IGDELISTHEADER_H_

#include "../resources/igdeIconReference.h"

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringList.h>



/**
 * \brief IGDE UI List Header.
 * 
 * Stores information about a column header.
 */
class igdeListHeader : public deObject{
private:
	decString pTitle;
	igdeIconReference pIcon;
	int pSize;
	
	
	
public:
	/** \title Constructors and Destructors */
	/*@{*/
	/** \brief Create list header. */
	igdeListHeader( const char *title, int size );
	
	igdeListHeader( const char *title, igdeIcon *icon, int size );
	
	/** \brief Create copy of list header. */
	igdeListHeader( const igdeListHeader &header );
	
	
	
protected:
	/**
	 * \brief Clean up list header.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeListHeader();
	/*@}*/
	
	
	
public:
	/** \title Management */
	/*@{*/
	/** \brief Title. */
	inline const decString &GetTitle() const{ return pTitle; }
	
	/** \brief Set title. */
	void SetTitle( const char *title );
	
	/** \brief Icon or NULL. */
	inline igdeIcon *GetIcon() const{ return pIcon; }
	
	/** \brief Set icon or NULL. */
	void SetIcon( igdeIcon *icon );
	
	/** \brief Size in pixels. */
	inline int GetSize() const{ return pSize; }
	
	/** \brief Set size in pixels. */
	void SetSize( int size );
	
	
	
	/** \brief Set from another header. */
	virtual igdeListHeader &operator=( const igdeListHeader &header );
	/*@}*/
};

#endif
