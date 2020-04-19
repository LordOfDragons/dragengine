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

#ifndef _PEEWPTYPELISTENER_H_
#define _PEEWPTYPELISTENER_H_

#include "../../emitter/peeEmitterListener.h"

class peeWPType;



/**
 * \brief Type Properties Panel Listener.
 */
class peeWPTypeListener : public peeEmitterListener{
private:
	peeWPType &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new listener. */
	peeWPTypeListener( peeWPType &panel );
	
	/** \brief Clean up the listener. */
	virtual ~peeWPTypeListener();
	/*@}*/
	
	/** \name Management */
	/** Controller count or order changed. */
	virtual void ControllerStructureChanged( peeEmitter *emitter );
	
	/** Controller changed. */
	virtual void ControllerChanged( peeEmitter *emitter, peeController *controller );
	
	/** Type count or order changed. */
	virtual void TypeStructureChanged( peeEmitter *emitter );
	
	/** Type changed. */
	virtual void TypeChanged( peeEmitter *emitter, peeType *type );
	
	/** Active type active trail controller changed. */
	virtual void TypeActiveTrailControllerChanged( peeEmitter *emitter, peeType *type );
	
	/** Active type active emit controller changed. */
	virtual void TypeActiveEmitControllerChanged( peeEmitter *emitter, peeType *type );
	
	/** A type parameter changed. */
	virtual void TypeParameterChanged( peeEmitter *emitter, peeType *type, peeParameter *parameter );
	
	/** Active type parameter changed. */
	virtual void ActiveTypeParameterChanged( peeEmitter *emitter, peeType *type );
	
	/** Active type changed. */
	virtual void ActiveTypeChanged( peeEmitter *emitter );
	/*@}*/
};

#endif
