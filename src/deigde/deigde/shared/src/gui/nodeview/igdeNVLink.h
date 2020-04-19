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

#ifndef _IGDENVLINK_H_
#define _IGDENVLINK_H_

#include "igdeNVSlotReference.h"

#include <dragengine/deObject.h>


class igdeNVBoard;


/**
 * \brief IGDE UI NodeView Link.
 */
class igdeNVLink : public deObject{
private:
	igdeNVSlotReference pSource;
	igdeNVSlotReference pTarget;
	
	igdeNVBoard *pOwnerBoard;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create link. */
	igdeNVLink( igdeNVSlot *source, igdeNVSlot *target );
	
	
	
protected:
	/**
	 * \brief Clean up link.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeNVLink();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Source. */
	inline igdeNVSlot *GetSource() const{ return pSource; }
	
	/** \brief Target. */
	inline igdeNVSlot *GetTarget() const{ return pTarget; }
	
	
	
	/** \brief Owner board or NULL. */
	inline igdeNVBoard *GetOwnerBoard() const{ return pOwnerBoard; }
	
	/**
	 * \brief Set owner board or NULL.
	 * \warning For use by \em igdeNVBoard only!
	 */
	void SetOwnerBoard( igdeNVBoard *board );
	/*@}*/
};

#endif
