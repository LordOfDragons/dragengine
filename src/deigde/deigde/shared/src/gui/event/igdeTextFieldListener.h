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

#ifndef _IGDETEXTFIELDLISTENER_H_
#define _IGDETEXTFIELDLISTENER_H_


#include <dragengine/deObject.h>


class igdeTextField;


/**
 * \brief IGDE UI TextField Listener.
 */
class DE_DLL_EXPORT igdeTextFieldListener : public deObject{
protected:
	/** \text Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	igdeTextFieldListener();
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeTextFieldListener();
	/*@}*/
	
	
	
public:
	/** \text Events */
	/*@{*/
	/**
	 * \brief TextField text changed.
	 * 
	 * User entered text and accepted it.
	 */
	virtual void OnTextChanged( igdeTextField *textField );
	
	/**
	 * \brief TextField text is changing.
	 * 
	 * User is entering text but has not yet accepted the input. This event is send each
	 * time the user changes the text. Once accepted OnTextChanged() is called.
	 */
	virtual void OnTextChanging( igdeTextField *textField );
	
	/**
	 * \brief Enter key has been pressed.
	 * 
	 * Send after OnTextChanged() has been send.
	 */
	virtual void OnEnterKey( igdeTextField *textField );
	/*@}*/
};

#endif
