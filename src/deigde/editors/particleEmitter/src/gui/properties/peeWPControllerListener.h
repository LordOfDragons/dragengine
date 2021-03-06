/* 
 * Drag[en]gine IGDE Particle Emitter Editor
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

#ifndef _PEEWPCONTROLLERLISTENER_H_
#define _PEEWPCONTROLLERLISTENER_H_

#include "../../emitter/peeEmitterListener.h"

class peeWPController;



/**
 * \brief Controller Properties Panel Listener.
 */
class peeWPControllerListener : public peeEmitterListener{
private:
	peeWPController &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	peeWPControllerListener( peeWPController &panel );
	
	/** \brief Clean up listener. */
	virtual ~peeWPControllerListener();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Controller added or removed. */
	virtual void ControllerStructureChanged ( peeEmitter *emitter );
	
	/** \brief Controller changed. */
	virtual void ControllerChanged( peeEmitter *emitter, peeController *controller );
	
	/** \brief Controller name changed. */
	virtual void ControllerNameChanged( peeEmitter *emitter, peeController *controller );
	
	/** \brief Controller changed value. */
	virtual void ControllerValueChanged( peeEmitter *emitter, peeController *controller );
	
	/** \brief Active controller changed. */
	virtual void ActiveControllerChanged( peeEmitter *emitter );
	/*@}*/
};

#endif
