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

#ifndef _PEEWPVIEWLISTENER_H_
#define _PEEWPVIEWLISTENER_H_

#include "../../emitter/peeEmitterListener.h"

class peeWPView;



/**
 * \brief View panel listener.
 */
class peeWPViewListener : public peeEmitterListener{
private:
	peeWPView &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	peeWPViewListener( peeWPView &panel );
	
	/** \brief Clean up listener. */
	virtual ~peeWPViewListener();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Emitter changed. */
	virtual void EmitterChanged( peeEmitter *emitter );
	
	/** \brief The sky changed. */
	virtual void SkyChanged( peeEmitter *emitter );
	
	/** \brief The environment component changed. */
	virtual void EnvObjectChanged( peeEmitter *emitter );
	
	/** \brief The view changed. */
	virtual void ViewChanged( peeEmitter *emitter );
	
	/** \brief The camera changed. */
	virtual void CameraChanged( peeEmitter *emitter );
	/*@}*/
};

#endif
