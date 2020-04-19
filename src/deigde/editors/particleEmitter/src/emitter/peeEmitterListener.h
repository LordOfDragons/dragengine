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

#ifndef _PEEPARTICLEEMITTERLISTENER_H_
#define _PEEPARTICLEEMITTERLISTENER_H_

#include <dragengine/deObject.h>

class peeEmitter;
class peeController;
class peeType;
class peeParameter;



/**
 * \brief Particle Emitter Listener.
 */
class peeEmitterListener : public deObject{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new listener. */
	peeEmitterListener();
	/** \brief Clean up the listener. */
	virtual ~peeEmitterListener();
	/*@}*/
	
	/** \name Notifications */
	/*@{*/
	/** Changed or saved state changed. */
	virtual void StateChanged( peeEmitter *emitter );
	/** Undo changed. */
	virtual void UndoChanged( peeEmitter *emitter );
	/** Emitter changed. */
	virtual void EmitterChanged( peeEmitter *emitter );
	/** The sky changed. */
	virtual void SkyChanged( peeEmitter *emitter );
	/** The environment object changed. */
	virtual void EnvObjectChanged( peeEmitter *emitter );
	/** The view changed. */
	virtual void ViewChanged( peeEmitter *emitter );
	/** The camera changed. */
	virtual void CameraChanged( peeEmitter *emitter );
	
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
