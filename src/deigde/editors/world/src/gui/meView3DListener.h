/* 
 * Drag[en]gine IGDE World Editor
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

#ifndef _MEVIEW3DLISTENER_H_
#define _MEVIEW3DLISTENER_H_

#include "../world/meWorldNotifier.h"

class meView3D;



/**
 * \brief View 3D Listener
 */
class meView3DListener : public meWorldNotifier{
private:
	meView3D &pView3D;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	meView3DListener( meView3D &view3D );
	
	/** \brief Clean up listener. */
	virtual ~meView3DListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Element or work mode changed. */
	virtual void ModeChanged( meWorld *world );
	
	/** \brief Active camera changed. */
	virtual void ActiveCameraChanged( meWorld *world );
	/*@}*/
};

#endif
