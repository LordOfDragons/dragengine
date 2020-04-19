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

#ifndef _IGDESLIDERLISTENER_H_
#define _IGDESLIDERLISTENER_H_


#include <dragengine/deObject.h>


class igdeSlider;


/**
 * \brief IGDE UI Slider Listener.
 */
class igdeSliderListener : public deObject{
protected:
	/** \text Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	igdeSliderListener();
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeSliderListener();
	/*@}*/
	
	
	
public:
	/** \text Events */
	/*@{*/
	/**
	 * \brief Slider value changed and has been accepted by user.
	 */
	virtual void OnValueChanged( igdeSlider *slider );
	
	/**
	 * \brief Slider value is changing but user has not yet accepted the value.
	 * 
	 * Used while slider is being dragged to allow tracking. If the user finishe
	 * dragging OnValueChanged() will be called.
	 */
	virtual void OnValueChanging( igdeSlider *slider );
	/*@}*/
};

#endif
