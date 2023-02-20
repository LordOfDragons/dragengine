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

#ifndef _IGDEEDITPROPERTYVALUELISTENER_H_
#define _IGDEEDITPROPERTYVALUELISTENER_H_


#include <dragengine/deObject.h>


class igdeEditPropertyValue;


/**
 * \brief IGDE UI EditPropertyValue Listener.
 */
class DE_DLL_EXPORT igdeEditPropertyValueListener : public deObject{
protected:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	igdeEditPropertyValueListener();
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeEditPropertyValueListener();
	/*@}*/
	
	
	
public:
	/** \name Events */
	/*@{*/
	/**
	 * \brief EditPropertyValue value changed.
	 * 
	 * User entered value and accepted it.
	 */
	virtual void OnPropertyValueChanged( igdeEditPropertyValue *editPropertyValue );
	
	/**
	 * \brief EditPropertyValue value is changing.
	 * 
	 * User is actively changing the value (for example slide changing) but has not
	 * yet accepted the value. OnPropertyValueChanged() will be called once finished.
	 */
	virtual void OnPropertyValueChanging( igdeEditPropertyValue *editPropertyValue );
	/*@}*/
};

#endif
