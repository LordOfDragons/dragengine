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

#ifndef _IGDECOLORBOXLISTENER_H_
#define _IGDECOLORBOXLISTENER_H_


#include <dragengine/deObject.h>


class igdeColorBox;


/**
 * \brief IGDE UI ColorBox Listener.
 */
class DE_DLL_EXPORT igdeColorBoxListener : public deObject{
protected:
	/** \text Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	igdeColorBoxListener();
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeColorBoxListener();
	/*@}*/
	
	
	
public:
	/** \text Events */
	/*@{*/
	/**
	 * \brief ColorBox color changed.
	 * 
	 * User changed color by interaction.
	 */
	virtual void OnColorChanged( igdeColorBox *colorBox );
	/*@}*/
};

#endif
