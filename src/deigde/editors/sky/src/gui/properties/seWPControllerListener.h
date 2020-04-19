/* 
 * Drag[en]gine IGDE Sky Editor
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

#ifndef _SEWPCONTROLLERLISTENER_H_
#define _SEWPCONTROLLERLISTENER_H_

#include "../../sky/seSkyListener.h"

class seWPController;


/**
 * \brief Controller properties panel listener.
 */
class seWPControllerListener : public seSkyListener{
private:
	seWPController &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	seWPControllerListener( seWPController &panel );
	
	/** \brief Clean up listener. */
	virtual ~seWPControllerListener();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Controller added or removed. */
	virtual void ControllerStructureChanged ( seSky *sky );
	
	/** \brief Controller changed. */
	virtual void ControllerChanged( seSky *sky, seController *controller );
	
	/** \brief Controller name changed. */
	virtual void ControllerNameChanged( seSky *sky, seController *controller );
	
	/** \brief Controller changed value. */
	virtual void ControllerValueChanged( seSky *sky, seController *controller );
	
	/** \brief Active controller changed. */
	virtual void ActiveControllerChanged( seSky *sky );
	/*@}*/
};

#endif
