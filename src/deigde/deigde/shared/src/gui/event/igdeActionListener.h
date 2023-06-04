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

#ifndef _IGDEACTIONLISTENER_H_
#define _IGDEACTIONLISTENER_H_

#include <dragengine/dragengine_export.h>

class igdeAction;


/**
 * \brief IGDE UI Action Listener.
 * 
 * Listener used by UI elements connected to an igdeAction instance to be notified if
 * action parameters change. Action listeners are stored as pointer so the subclass
 * has to keep a reference to the action instance.
 */
class DE_DLL_EXPORT igdeActionListener{
protected:
	/** \text Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	igdeActionListener();
	
	/** \brief Clean up action listener. */
	virtual ~igdeActionListener();
	/*@}*/
	
	
	
public:
	/** \text Management */
	/*@{*/
	/** \brief Action parameters changed. */
	virtual void OnParameterChanged( igdeAction *action ) = 0;
	
	/** \brief Action has been destroyed. */
	virtual void OnDestroyed( igdeAction *action ) = 0;
	/*@}*/
};

#endif
