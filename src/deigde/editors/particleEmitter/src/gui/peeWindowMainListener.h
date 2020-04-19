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

#ifndef _PEEWINDOWMAINLISTENER_H_
#define _PEEWINDOWMAINLISTENER_H_

#include "../emitter/peeEmitterListener.h"

class peeWindowMain;



/**
 * \brief Window Main Listener.
 */
class peeWindowMainListener : public peeEmitterListener{
private:
	peeWindowMain &pWindow;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	peeWindowMainListener( peeWindowMain &window );
	
	/** \brief Clean up listener. */
	~peeWindowMainListener();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Changed or saved state changed. */
	virtual void StateChanged( peeEmitter *emitter );
	
	/** \brief Undo changed. */
	virtual void UndoChanged( peeEmitter *emitter );
	
	/** \brief Emitter changed. */
	virtual void EmitterChanged( peeEmitter *emitter );
	
	/** \brief Controller count or order changed. */
	virtual void ControllerStructureChanged( peeEmitter *emitter );
	
	/** Active controller changed. */
	virtual void ActiveControllerChanged( peeEmitter *emitter );
	
	/** \brief Type count or order changed. */
	virtual void TypeStructureChanged( peeEmitter *emitter );
	
	/** \brief Active type active trail controller changed. */
	virtual void TypeActiveTrailControllerChanged( peeEmitter *emitter, peeType *type );
	
	/** \brief Active type active emit controller changed. */
	virtual void TypeActiveEmitControllerChanged( peeEmitter *emitter, peeType *type );
	
	/** \brief Active type parameter changed. */
	virtual void ActiveTypeParameterChanged( peeEmitter *emitter, peeType *type );
	
	/** \brief Active type changed. */
	virtual void ActiveTypeChanged( peeEmitter *emitter );
	/*@}*/
};

#endif
