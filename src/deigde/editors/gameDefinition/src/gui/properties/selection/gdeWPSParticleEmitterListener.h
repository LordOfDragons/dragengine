/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#ifndef _GDEWPSPARTICLEEMITTERLISTENER_H_
#define _GDEWPSPARTICLEEMITTERLISTENER_H_

#include "../../../gamedef/gdeGameDefinitionListener.h"

class gdeWPSParticleEmitter;



/**
 * \brief Particle emitter panel listener.
 */
class gdeWPSParticleEmitterListener : public gdeGameDefinitionListener{
private:
	gdeWPSParticleEmitter &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	gdeWPSParticleEmitterListener( gdeWPSParticleEmitter &panel );
	
	/** \brief Clean up listener. */
	virtual ~gdeWPSParticleEmitterListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Particle emitter categories changed. */
	virtual void ParticleEmitterCategoriesChanged( gdeGameDefinition *gameDefinition );
	
	
	
	/** \brief Particle emitter changed. */
	virtual void ParticleEmitterChanged( gdeGameDefinition *gameDefinition,
		gdeParticleEmitter *particleEmitter );
	
	/** \brief Active object class changed. */
	virtual void ActiveParticleEmitterChanged( gdeGameDefinition *gameDefinition );
	/*@}*/
};

#endif
